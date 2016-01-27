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
#include <thread>
#include <functional>
#include <uv.h>

#include "sensor.h"
#include "sensor_result.h"
 
namespace sensor {
    
    typedef std::function<void* ()> schedulerTask;
    typedef std::function<void(void*)> schedulerCallback;
    
    /**
     * Class that handle the lifecycle and threading of provided sensors. On each individual timeout,
     * results will be retreived from each sensor and send individualy to the provided callback
     */
    class scheduler {
        
        public:
            scheduler(schedulerTask, schedulerCallback, unsigned, bool = false);
            ~scheduler();
            
            /**
             * Launch all the sensors' workers. If already launched, this method has no effect
             */
            void launch();

            /**
             * Cancel the workers. If already canceled or never launched, this method has no effect
             */
            void cancel();
        
            friend void AsyncAction(uv_work_t*);
            friend void AsyncActionAfter(uv_work_t*, int);
            
            
        private:
            schedulerTask task;                   // Callback
            schedulerCallback callback;           // Callback
        
            unsigned frequence;

            bool cancelled;                       // If threads should stop
            bool launched;                        // If already launched
            bool repeat;                          // If should be repeted

            std::condition_variable stop_threads; // Stop condition for all threads
            std::mutex m;                         // Protects 'stop_threads'
    };
    
    /**
     * Structure used with libuv as the data
     */
    struct schedulerBaton {
        uv_work_t request;  // libuv
        scheduler *handle;  // scheduler itself
        void* value;        // Sensor
    };
    
}
 
 
#endif // H_SCHEDULER
