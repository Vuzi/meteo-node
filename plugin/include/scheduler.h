#ifndef H_SCHEDULER
#define H_SCHEDULER

/**
 * @file handler.h
 * @brief Class for the sensors handler. This object will contains all the informations
 *        about the different threads (mutex, conditions, ...) and sensors 
 * @author Vuzi
 * @version 0.1
 */
 
#include <unistd.h>
#include <node.h>
#include <string.h>
#include <iostream>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <uv.h>

namespace sensor {

    void AsyncAction(uv_work_t* req);
    void AsyncActionAfter(uv_work_t* req, int status);

    template<typename T1, typename T2>
    class scheduler;
    
    /**
     * Structure used with libuv as the data
     */
    template<typename T1, typename T2>
    struct schedulerBaton {
        uv_work_t request;         // libuv
        scheduler<T1, T2> *handle; // scheduler itself
        T2 resultValue;            // Sensor
    };

    /**
     * @brief      Scheduler used to schedule action in time. This action may be repeatable or not.
     *  
     */
    template<typename T1, typename T2>
    class scheduler {

        using schedulerAction = std::function<T2(T1)>;
        using schedulerCallback = std::function<void(T1, T2)>;
        using schedulerCleanup = std::function<void()>;
        
        friend void AsyncAction(uv_work_t* req) {
            schedulerBaton<T1, T2>* baton = static_cast<schedulerBaton<T1, T2>*>(req->data);
            scheduler<T1, T2>* handle = baton->handle;

            // Lock and wait for the time/cancellation
            if(handle->frequence > 0) {
                std::unique_lock<std::mutex> l(handle->m);
                handle->stop_threads.wait_for(l, std::chrono::seconds(handle->frequence));
            }

            // Call the async action
            baton->resultValue = handle->action(handle->producer);
        }

        friend void AsyncActionAfter(uv_work_t* req, int status) {
            schedulerBaton<T1, T2>* baton = static_cast<schedulerBaton<T1, T2>*>(req->data);
            scheduler<T1, T2>* handle = baton->handle;
            
            // If canceled, quit now
            if(handle->cancelled)
                goto quit;

            // Call callback with result
            handle->callback(handle->producer, baton->resultValue);

            // Re-launch the thread
            if(handle->repeat)
                uv_queue_work(uv_default_loop(), &baton->request, AsyncAction, AsyncActionAfter);
            else
                goto quit; // Not to relaunch, quit

            return;

            quit:
            delete baton;
            if(handle->cleanup)
                handle->cleanup(); // If any cleanup method is defined, call it
            delete handle;
        }

        public:
            scheduler(T1 _producer, schedulerAction _action, schedulerCallback _callback, unsigned _frequence = 0, bool _repeat = false)
                      : scheduler(_producer, _action, _callback, schedulerCleanup(), _frequence, _repeat) {}
            
            scheduler(T1 _producer, schedulerAction _action, schedulerCallback _callback, schedulerCleanup _cleanup, unsigned _frequence = 0, bool _repeat = false) {
                action = _action;
                callback = _callback;
                cleanup = _cleanup;
                frequence = _frequence;
                producer = _producer;
                repeat = _repeat;
                
                launched = false; // Not launched 
            }

            ~scheduler() {}
            
            /**
             * Launch all the sensors' workers. If already launched, this method has no effect
             */
            void launch() {
                if(launched)
                    return;

                // Not canceled
                cancelled = false;
                launched = true;

                // Create and set the baton
                schedulerBaton<T1, T2> *baton = new schedulerBaton<T1, T2>;
                
                baton->request.data = baton;
                baton->handle = this;
                
                uv_queue_work(uv_default_loop(), &baton->request, AsyncAction, AsyncActionAfter);
            }

            /**
             * Cancel the workers. If already canceled or never launched, this method has no effect
             */
            void cancel() {
                if(!launched)
                    return;
                
                cancelled = true;
                stop_threads.notify_all();
            }

        private:
            schedulerAction action;               // Action
            schedulerCallback callback;           // Callback
            schedulerCleanup cleanup;             // Cleanup
        
            unsigned frequence;
            T1 producer;

            bool cancelled;                       // If threads should stop
            bool launched;                        // If already launched
            bool repeat;                          // If should be repeted

            std::condition_variable stop_threads; // Stop condition for all threads
            std::mutex m;                         // Protects 'stop_threads'

    };

}

#endif // H_SCHEDULER
