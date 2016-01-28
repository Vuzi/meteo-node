
#include "scheduler.h"

#include <iostream>

namespace sensor {

    // Called by libuv worker in separate thread
    void AsyncAction(uv_work_t *req) {
        schedulerBaton* baton = static_cast<schedulerBaton*>(req->data);
        scheduler* handle = baton->handle;
        
        // Lock and wait for the time/cancellation
        if(handle->frequence > 0) {
            std::unique_lock<std::mutex> l(handle->m);
            handle->stop_threads.wait_for(l, std::chrono::seconds(handle->frequence));
        }

        // Call the async callback
        baton->results = handle->s->getResults();
    }

    // Called by libuv in event loop when async function completes
    void AsyncActionAfter(uv_work_t* req, int status) {
        schedulerBaton* baton = static_cast<schedulerBaton*>(req->data);
        scheduler* handle = baton->handle;
        
        // If canceled
        if(handle->cancelled) {
            // TODO : clean ?
            // And clean scheduler ?
            delete baton;
            return;
        }
        
        // Call callback with result
        handle->callback(handle->s, baton->results);
        
        // Re-launch the thread
        if(handle->repeat)
            uv_queue_work(uv_default_loop(), &baton->request, AsyncAction, AsyncActionAfter);
        else {
            // TODO clean everything
        }
    }
    
    scheduler::scheduler(sensor* _s, schedulerCallback _callback, unsigned _frequence, bool _repeat) {
        callback = _callback;
        frequence = _frequence;
        s = _s;
        repeat = _repeat;
        
        launched = false; // Not launched 
    }
    
    scheduler::~scheduler() {}

    /**
     * Launch all the sensors' workers. If already launched, this method has no effect
     */
    void scheduler::launch() {
        if(launched)
            return;

        // Not canceled
        cancelled = false;

        // Create and set the baton
        schedulerBaton *baton = new schedulerBaton;
        
        baton->request.data = baton;
        baton->handle = this;
        
        uv_queue_work(uv_default_loop(), &baton->request, AsyncAction, AsyncActionAfter);
    }
    
    /**
     * Cancel the workers. If already canceled or never launched, this method has no effect
     */
    void scheduler::cancel() {
        if(!launched)
            return;
        
        cancelled = true;
        stop_threads.notify_all();
    }
}
