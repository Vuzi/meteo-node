
#include "scheduler.h"

#include <iostream>

namespace sensor {

	// Called by libuv worker in separate thread
	void DelayAsync(uv_work_t *req) {
	    schedulerBaton *b = static_cast<schedulerBaton *>(req->data);
			
		sensor *s = b->s;
		scheduler *handle = b->h;
		
		// Lock and wait for the time/cancellation
	    std::unique_lock<std::mutex> l(handle->m);
	    handle->stop_threads.wait_for(l, std::chrono::seconds(s->getFrequence()));
	}
	 
	// Called by libuv in event loop when async function completes
	void DelayAsyncAfter(uv_work_t *req, int status) {
	    schedulerBaton *b = static_cast<schedulerBaton *>(req->data);

		sensor *s = b->s;
		scheduler *handle = b->h;
		
		// If canceled
		if(handle->cancelled) {
			// TODO : clean list of sensors ?
			delete b;
			return;
		}
		
        // Get all the results
		for(auto result : s->getResults()) {
			// Call the callback with the values
			handle->callback(s, &result);
		}
		
		// Re-launch the thread
		uv_queue_work(uv_default_loop(), &b->request, DelayAsync, DelayAsyncAfter);
	}
	
	scheduler::scheduler(std::list<sensor*> _sensors, schedulerCallback _callback) {
		sensors = _sensors;
		callback = _callback;
		
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
		
		for(auto s : sensors) {
    		// Create and set the baton
			schedulerBaton *baton = new schedulerBaton;
			
			baton->request.data = baton;
			baton->h = this;
			baton->s = s;
			
    		uv_queue_work(uv_default_loop(), &baton->request, DelayAsync, DelayAsyncAfter);
		}
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
