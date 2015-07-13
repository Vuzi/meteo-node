/**
  * Vuzi
  *
  * Test for async callbacks in NodeJS
  * Based on : https://gist.github.com/dmh2000/9519489, and adapted to the newest version of Node.
  *
  * This will pretty much do what setInterval does already. The cancel methods allow to cancel the
  * task. The callback system of libuv allow us to safely call javascript functions.
  *
  * The std mutex and condition_variable are only here because I found it more easy-to-use that keeping
  * a reference of every thread and cancelling it. Plus it allow us to stop the thread waiting fort nothing 
  */

#include <unistd.h>
#include <node.h>
#include <string.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <uv.h>
#include <v8.h>

using namespace v8;

bool canceled = false;                // If threads should stop
std::condition_variable stop_threads; // Stop condition for all threads
std::mutex m;                         // Protects 'stop_threads'

// Native blocking/compute intensive function
void delay(int s) {
    // Wait for the specified time or a signal
    std::unique_lock<std::mutex> l(m);
    stop_threads.wait_for(l, std::chrono::seconds(s));
}

// The 'baton' is the carrier for data between functions
struct DelayBaton {
    // Required
    uv_work_t request;                  // libuv
    Persistent<Function> callback;      // javascript callback
 
    // Optional : data goes here.
    // data that doesn't go back to javascript can be any typedef
    // data that goes back to javascript needs to be a supported type
    int         seconds;
    char        greeting[256];
};

// Called by libuv worker in separate thread
static void DelayAsync(uv_work_t *req) {
    DelayBaton *baton = static_cast<DelayBaton *>(req->data);
    delay(baton->seconds);
}
 
// Called by libuv in event loop when async function completes
static void DelayAsyncAfter(uv_work_t *req, int status) {
    DelayBaton *baton = static_cast<DelayBaton *>(req->data);
    
    if(canceled) {
        // If thread is canceled, stop and return
        std::cout << "Canceled !" << std::endl;
        
        // Delete the baton & the callback
        baton->callback.Reset();
        delete baton;
        
        return;
    }
    
    auto isolate = Isolate::GetCurrent();
    
    // Get the reference to the baton from the request
    Local<Function> callback = Local<Function>::New(isolate, baton->callback);
    
    // Set up return arguments
    Handle<Value> argv[] = {
        Handle<Value>(Number::New(isolate, baton->seconds)),
        Handle<Value>(String::NewFromUtf8(isolate, baton->greeting))
    };
 
    // Execute the callback
    callback->Call(isolate->GetCurrentContext()->Global(), 2, argv);
    
    // Restart thread with same values
    uv_queue_work(uv_default_loop(),&baton->request,DelayAsync,DelayAsyncAfter);
}

void Cancel(const FunctionCallbackInfo<Value>& args) {
    canceled = true;             // Cancel
    stop_threads.notify_all();   // Notify all threads
}

// javascript callable function
void Delay(const FunctionCallbackInfo<Value>& args) {
    DelayBaton *baton = new DelayBaton;
    canceled = false;
 
    // get callback argument
    Local<Function> cb = Local<Function>::Cast(args[2]);
 
    // attach baton to uv work request
    baton->request.data = baton;
 
    // assign incoming arguments to baton
    baton->seconds = args[0]->Int32Value();
 
    // point at the argument as a string, then copy it to the baton
    v8::String::Utf8Value str(args[1]);
    strncpy(baton->greeting, *str, sizeof(baton->greeting));
 
    // assign callback to baton
    baton->callback.Reset(Isolate::GetCurrent(), cb);
 
    // queue the async function to the event loop
    // the uv default loop is the node.js event loop
    uv_queue_work(uv_default_loop(),&baton->request,DelayAsync,DelayAsyncAfter);
}

// Node export + binding
void init(Handle<Object> exports, Handle<Object> module) {
    NODE_SET_METHOD(exports, "delay", Delay);
    NODE_SET_METHOD(exports, "cancel", Cancel);
}

NODE_MODULE(delay, init);
