// addon.cc
#include <list>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <functional>

#include <node.h>

#include "wrapper_sensor.h"

using namespace v8;

void InitAll(Local<Object> exports) {
	// Export sensor wrapper
	SensorWrapper::Init(exports);
}

NODE_MODULE(meteonetwork, InitAll)
