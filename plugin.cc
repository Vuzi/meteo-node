// addon.cc
#include <node.h>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <list>

// Test
#include <chrono>
#include <thread>

//#include "sensor.h"
//#include "i2c_sensor.h"
#include "gpio_sensor.h"
#include "DHT22.h"
#include "sensor_result.h"

#include <wiringPi.h>
#define MAXTIMINGS 85

using namespace v8;

/**
 * Init a sensor based of the V8 object given. If no sensor could be created, nullptr is returned
 * @param  sensorName   The name of the sensor
 * @param  sensorConfig The sensor configuration. Must contains the type of sensor
 * @return              The created sensor, or nullptr
 */
sensor::sensor* InitSensor(const Local<String>& sensorName, const Local<Object>& sensorConfig) {
	Isolate* isolate = Isolate::GetCurrent();
	
	// Get the type of sensor
	const Local<Value> jsvalue = sensorConfig->Get(String::NewFromUtf8(isolate, "type"));
	String::Utf8Value value(jsvalue->ToString());
	const std::string type = std::string(*value);
	
	// Only type is required
	if (type == "DHT22") {  // TODO new sensor
		std::cout << "Sensor of type DHT22 created" << std::endl;
		return (sensor::sensor*) new sensor::DHT22_sensor(0x7); // TODO get from configuration
	}
	else if (type == "TSL2561") { // TODO new sensor
		std::cout << "Sensor fo type TSL2561 created" << std::endl;
		return nullptr;
	}
	else {
		std::cout << "Error '" << type << "is not a valid sensor type" << std::endl;
		return nullptr;
	}
}

/**
 * For now, main method of the NodeJS plugin
 * @param args The arguments used to call the method. Must contains a callbacks and a V8 object
 */
void RunCallback(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);
	
	// Check that all the parameters are good
	if (args.Length() < 2) {
		isolate->ThrowException(Exception::TypeError(
			String::NewFromUtf8(isolate, "Wrong number of arguments : two are required")));
		return;
	}
	
	if (!args[0]->IsFunction() || !args[1]->IsObject()) {
		isolate->ThrowException(Exception::TypeError(
			String::NewFromUtf8(isolate, "Wrong argument(s) : first must be a callback, and the second must be the configuration")));
		return;
	}
	
	// Get elements
	Local<Function> cb = Local<Function>::Cast(args[0]);
	Local<Object> options = Local<Object>::Cast(args[1]);
	
	// Iter on each property
	const Local<Array> props = options->GetPropertyNames();
	const uint32_t length = props->Length();
	
	// Init the list of sensors
	std::list<sensor::sensor*> sensors;
	
	for (uint32_t i = 0 ; i < length ; i++) {
		const Local<Value> key = props->Get(i);
		const Local<Value> value = options->Get(key);
	
		// Init the sensor
		sensor::sensor* s = InitSensor(key->ToString(), value->ToObject());
		
		if (s == nullptr) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Unknown type : only DHT22 and TSL2561 are currently supported")));

			// TODO : free sensor already created
			
			return;
		} else {
			s->initialize();
			sensors.push_back(s);
		}
	}
	
	// Test : show address of all the elements + launch result retreiving
	for(int i = 0; i < 5; i++) {
		for(sensor::sensor* s : sensors) {
			for(auto result : s->getResults()) {
				
				// Get the type of value
				Local<String> type;
				
				if(result.getType() == sensor_result::type::TEMPERATURE)
					type = String::NewFromUtf8(isolate, "Temperature");
				else if(result.getType() == sensor_result::type::HUMIDITY)
					type = String::NewFromUtf8(isolate, "Humidity");
				else
					type = String::NewFromUtf8(isolate, "Other");
				
				// Get the value (always float for now)
				Local<Number> val = Number::New(isolate, result.getValue().f);
				
				// Call the callback with the values
				Local<Value> argv[2] = { type, val };
				cb->Call(isolate->GetCurrentContext()->Global(), 2, argv);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
	      
	// TODO call handler thread
}

void Init(Handle<Object> exports, Handle<Object> module) {
	NODE_SET_METHOD(module, "exports", RunCallback);
}

// TODO handle destructor

NODE_MODULE(meteonetwork, Init)


