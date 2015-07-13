#include <node.h>
#include <iostream>
#include <stdlib.h>

#include <wiringPi.h>

using namespace v8;

void RunCallback(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = Isolate::GetCurrent();
        HandleScope scope(isolate);

        int ret = wiringPiSetup();

        Local<Number> num =  Number::New(isolate, ret);
        args.GetReturnValue().Set(num);
}

void Init(Handle<Object> exports, Handle<Object> module) {
        NODE_SET_METHOD(module, "exports", RunCallback);
}

NODE_MODULE(demo, Init)

