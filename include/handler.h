#ifndef H_HANDLER
#define H_HANDLER

/**
 * @file handler.h
 * @brief Class for the sensors handler. This object will contains all the informations
 *        about the different threads (mutex, conditions, ...) and sensors 
 * @author Vuzi
 * @version 0.1
 */
 
#include "sensor.h"
 
namespace sensor {
	
	typedef void (*handlerCallback)(*sensor, *result);

	/**
	 * Structure used with libuv as the data
	 */
	struct handlerBaton {
	    uv_work_t request;   // libuv
	    handler *h;          // javascript callback
		
		sensor* s;           // Sensor
	};
	
	/**
	 * Class that handle the lifecycle and threading of provided sensors. On each individual timeout,
	 * results will be retreived from each sensor and send individualy to the provided callback
	 */
	class handler {
		
		public:
			handler(std::list<sensor::sensor*>, handlerCallback);
			~handler();
			
			/**
			 * Launch all the sensors' workers. If already launched, this method has no effect
			 */
			void launch();

			/**
			 * Cancel the workers. If already canceled or never launched, this method has no effect
			 */
			void cancel();
		
			friend void DelayAsync(uv_work_t*);
			friend void DelayAsyncAfter(uv_work_t*, int);
			
			
		private:
			std::list<sensor::sensor*> sensors;   // List of sensors
			const handlerCallback callback;       // Callback
			bool canceled;                        // If threads should stop
			bool launched;                        // If already launched
			std::condition_variable stop_threads; // Stop condition for all threads
			std::mutex m;                         // Protects 'stop_threads'
	};
	
}
 
 
#endif // H_HANDLER