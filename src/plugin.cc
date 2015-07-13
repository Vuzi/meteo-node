// addon.cc
#include <node.h>
#include <string>
#include <iostream>
#include <list>
#include <chrono>
#include <thread>

#include "DHT22.h"
#include "TSL2561.h"
#include "sensor_result.h"

using namespace v8;

/**
 * Init a sensor based of the V8 object given. If no sensor could be created, nullptr is returned
 * @param  sensorName   The name of the sensor
 * @param  sensorConfig The sensor configuration. Must contains the type of sensor
 * @return              The created sensor, or nullptr
 */
sensor::sensor* InitSensor(const Local<String>& sensorName, const Local<Object>& sensorConfig) {
	Isolate* isolate = Isolate::GetCurrent();
	
	// String used in the method
	const Local<String> pin = String::NewFromUtf8(isolate, "pin");
	const Local<String> addr = String::NewFromUtf8(isolate, "address");

	// Get the type of sensor
	const Local<Value> jsvalue = sensorConfig->Get(String::NewFromUtf8(isolate, "type"));
	String::Utf8Value value(jsvalue->ToString());
	const std::string type = std::string(*value);
	
	// Only type is required
	if (type == "DHT22") {
		// Get the pin
		if(!sensorConfig->Has(pin) || !sensorConfig->Get(pin)->IsNumber()) {
			throw Exception::TypeError(
				String::NewFromUtf8(isolate, "Error : DHT22 require a valid pin property"));
		}

		Local<Number> pinValue = Local<Number>::Cast(sensorConfig->Get(pin));

		std::cout << "Sensor of type DHT22 created" << std::endl;
		return (sensor::sensor*) new sensor::DHT22_sensor((unsigned) pinValue->NumberValue());
	}
	else if (type == "TSL2561") {
		// Get the address
		if(!sensorConfig->Has(addr) || !sensorConfig->Get(addr)->IsNumber()) {
			throw Exception::TypeError(
				String::NewFromUtf8(isolate, "Error : TSL2561 require a valid address property"));
		}

		Local<Number> addrValue = Local<Number>::Cast(sensorConfig->Get(addr));

		std::cout << "Sensor fo type TSL2561 created" << std::endl;
		return (sensor::sensor*) new sensor::TSL2561_sensor((uint16_t) addrValue->NumberValue());
	}
	else {
		throw Exception::TypeError(
			String::NewFromUtf8(isolate, "Error : invalid sensor type"));
	}

	return nullptr;
}

/**
 * For now, main method of the NodeJS plugin
 * @param args The arguments used to call the method. Must contains a callbacks and a V8 object
 */
void RunCallback(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = Isolate::GetCurrent();
	HandleScope scope(isolate);

	std::list<sensor::sensor*> sensors;

	try {
		// Check that all the parameters are good
		if (args.Length() < 2)
			throw Exception::TypeError(
				String::NewFromUtf8(isolate, "Error : Wrong number of arguments : two are required"));
		
		if (!args[0]->IsFunction() || !args[1]->IsObject())
			throw Exception::TypeError(
				String::NewFromUtf8(isolate, "Error : Wrong argument(s) : first must be a callback, and the second must be the configuration"));
		
		// Get elements
		Local<Function> cb = Local<Function>::Cast(args[0]);
		Local<Object> options = Local<Object>::Cast(args[1]);
		
		// Iter on each property
		const Local<Array> props = options->GetPropertyNames();
		const uint32_t length = props->Length();
		
		for (uint32_t i = 0 ; i < length ; i++) {
			const Local<Value> key = props->Get(i);
			const Local<Value> value = options->Get(key);
		
			// Init the sensor
			sensor::sensor* s = InitSensor(key->ToString(), value->ToObject());

			if(s != nullptr) {
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
					Local<Number> val;

					if(result.getType() == sensor::resultType::TEMPERATURE) {
						type = String::NewFromUtf8(isolate, "Temperature");
						val = Number::New(isolate, result.getValue().f);
					}
					else if(result.getType() == sensor::resultType::HUMIDITY) {
						type = String::NewFromUtf8(isolate, "Humidity");
						val = Number::New(isolate, result.getValue().f);
					}
					else if(result.getType() == sensor::resultType::LIGHT) {
						type = String::NewFromUtf8(isolate, "Light");
						val = Number::New(isolate, result.getValue().i);
					}
					else
						type = String::NewFromUtf8(isolate, "Other");
					
					// Call the callback with the values
					Local<Value> argv[2] = { type, val };
					cb->Call(isolate->GetCurrentContext()->Global(), 2, argv);
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		}
	} catch(Local<Value> &e) {
		// TODO free every sensor

		// Transfer the exception to node
		isolate->ThrowException(e);
	}
}

void Init(Handle<Object> exports, Handle<Object> module) {
	NODE_SET_METHOD(module, "exports", RunCallback);
}

// TODO handle destructor ?

NODE_MODULE(meteonetwork, Init)


