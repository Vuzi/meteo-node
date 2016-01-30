
#ifndef SENSOR_WRAPPER_H
#define SENSOR_WRAPPER_H

#include <node.h>
#include <node_object_wrap.h>

#include "format.h"

#include "sensor.h"
#include "scheduler.h"
#include "sensor_result.h"

using namespace v8;

class SensorWrapper : public node::ObjectWrap {

	public:
		static void Init(Local<Object> exports);

	private:
		explicit SensorWrapper(sensor::sensor* s);
		~SensorWrapper();

		// Private method used to generate a sensor and a result
		static sensor::sensor* InitSensor(const Local<String>& sensorName, const Local<Object>& sensorConfig);
		static void SendResult(sensor::sensor* s, sensor::result& r, Isolate* isolate, Local<Function>& cb);
		static void SendError(sensor::sensor* s, sensor::sensorException& r, Isolate* isolate, Local<Function>& cb);
		
		void fetch(const FunctionCallbackInfo<Value>& args, bool = false);
		void fetchClear();

		// Node exported methods
		static void New(const FunctionCallbackInfo<Value>& args);
		static void Fetch(const FunctionCallbackInfo<Value>& args);
		static void FetchInterval(const FunctionCallbackInfo<Value>& args);
		static void FetchClear(const FunctionCallbackInfo<Value>& args);

		static Persistent<Function> constructor;
		sensor::sensor* _s;
		std::list<sensor::scheduler<sensor::sensor*, sensor::resultsOrError>*> schedulers;
};


#endif // SENSOR_WRAPPER_H


