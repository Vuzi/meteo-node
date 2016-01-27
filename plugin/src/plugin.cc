// addon.cc
#include <list>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <functional>

#include <node.h>

#include "DHT22.h"
#include "PIR.h"
#include "TSL2561.h"
#include "BMP180.h"
#include "sensor_result.h"
#include "scheduler.h"

using namespace v8;

enum SensorType { GPIO, I2C };

struct SensorConf {
    std::string model;
    SensorType type;
    //std::function<int (std::string&, int)> factory;
};

const struct SensorConf conf[] = {
    {
        type   : "DHT22",
        bus    : GPIO
        //factory : test
    },
    {
        type   : "PIR",
        bus    : GPIO
        //factory : test
    },
    {
        type   : "TSL2561",
        bus    : I2C
        //factory : test
    }
};

sensor::sensor* InitSensor2(const Local<String>& sensorName, const Local<Object>& sensorConfig) {
    Isolate* isolate = Isolate::GetCurrent();

    // String used in the method
    const Local<String> pin = String::NewFromUtf8(isolate, "pin");
    const Local<String> addr = String::NewFromUtf8(isolate, "address");
    const Local<String> t = String::NewFromUtf8(isolate, "type");

    // Check for type
    if(!sensorConfig->Has(t) || !sensorConfig->Get(t)->IsString())
        throw Exception::TypeError(
            String::NewFromUtf8(isolate, "Error : no type specified. All sensors require a valid 'type' property"));

    // Get the type of sensor
    const Local<Value> jsvalue = sensorConfig->Get(t);
    String::Utf8Value value(jsvalue->ToString());
    const std::string type = std::string(*value);

    const size_t size = std::extent<decltype(conf)>::value;
    
    for(size_t i = 0; i < size; i++) {
        if(conf[i].type == type) {
            std::cout << "Found " << conf[i].type << std::endl;
            break;
        }
    }
}

/**
 * Init a sensor based of the V8 object given. If no sensor could be created, nullptr is returned. Will throw
 * exception if any error occured during the creation of the sensor
 * @param  sensorName   The name of the sensor
 * @param  sensorConfig The sensor configuration. Must contains the type of sensor
 * @return              The created sensor, or nullptr
 */
sensor::sensor* InitSensor(const Local<String>& sensorName, const Local<Object>& sensorConfig) {
    Isolate* isolate = Isolate::GetCurrent();
    
    // String used in the method
    const Local<String> pin = String::NewFromUtf8(isolate, "pin");
    const Local<String> addr = String::NewFromUtf8(isolate, "address");
    const Local<String> t = String::NewFromUtf8(isolate, "type");
    const Local<String> freq = String::NewFromUtf8(isolate, "frequence");

    if(!sensorConfig->Has(freq) || !sensorConfig->Get(freq)->IsNumber())
        throw Exception::TypeError(
            String::NewFromUtf8(isolate, "Error : all sensors require a valid 'frequence' property (number >= 0)"));
            
    if(!sensorConfig->Has(t) || !sensorConfig->Get(t)->IsString())
        throw Exception::TypeError(
            String::NewFromUtf8(isolate, "Error : no type specified. All sensors require a valid 'type' property"));

    // Get the sensor name
    String::Utf8Value nameValue(sensorName->ToString());
    const std::string name = std::string(*nameValue);

    // Get the type of sensor
    const Local<Value> jsvalue = sensorConfig->Get(t);
    String::Utf8Value value(jsvalue->ToString());
    const std::string type = std::string(*value);
    
    // Get the frequence
    const Local<Value> jsFrequence = sensorConfig->Get(freq);
    int frequence = (int) jsFrequence->NumberValue();
    
    // Only type is required
    if (type == "DHT22") {
        // Get the pin
        if(!sensorConfig->Has(pin) || !sensorConfig->Get(pin)->IsNumber()) {
            throw Exception::TypeError(
                String::NewFromUtf8(isolate, "Error : DHT22 require a valid 'pin' property (number > 0)"));
        }

        Local<Number> pinValue = Local<Number>::Cast(sensorConfig->Get(pin));

        std::cout << "Sensor of type DHT22 created" << std::endl;
        return (sensor::sensor*) new sensor::DHT22_sensor((unsigned) pinValue->NumberValue(), frequence, name);
    } else if (type == "PIR") {
        // Get the pin
        if(!sensorConfig->Has(pin) || !sensorConfig->Get(pin)->IsNumber()) {
            throw Exception::TypeError(
                String::NewFromUtf8(isolate, "Error : PIR require a valid 'pin' property (number > 0)"));
        }

        Local<Number> pinValue = Local<Number>::Cast(sensorConfig->Get(pin));

        std::cout << "Sensor of type PIR created" << std::endl;
        return (sensor::sensor*) new sensor::PIR_sensor((unsigned) pinValue->NumberValue(), frequence, name);
    }
    else if (type == "TSL2561") {
        // Get the address
        if(!sensorConfig->Has(addr) || !sensorConfig->Get(addr)->IsNumber()) {
            throw Exception::TypeError(
                String::NewFromUtf8(isolate, "Error : TSL2561 require a valid 'address' property (number > 0)"));
        }

        Local<Number> addrValue = Local<Number>::Cast(sensorConfig->Get(addr));

        std::cout << "Sensor of type TSL2561 created" << std::endl;
        return (sensor::sensor*) new sensor::TSL2561_sensor((uint16_t) addrValue->NumberValue(), frequence, name);
    }
    else if (type == "BMP180") {
        // Get the address
        if(!sensorConfig->Has(addr) || !sensorConfig->Get(addr)->IsNumber()) {
            throw Exception::TypeError(
                String::NewFromUtf8(isolate, "Error : BMP180 require a valid 'address' property (number > 0)"));
        }

        Local<Number> addrValue = Local<Number>::Cast(sensorConfig->Get(addr));

        std::cout << "Sensor of type BMP180 created" << std::endl;
        return (sensor::sensor*) new sensor::BMP180_sensor((uint16_t) addrValue->NumberValue(), frequence, name);
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
            
        Persistent<Function, CopyablePersistentTraits<Function>> callback(isolate, cb);

        // Iter on each property
        const Local<Array> props = options->GetPropertyNames();
        const uint32_t length = props->Length();
        
        for (uint32_t i = 0 ; i < length ; i++) {
            const Local<Value> key = props->Get(i);
            const Local<Value> value = options->Get(key);
        
            // Init the sensor
            InitSensor2(key->ToString(), value->ToObject());
            sensor::sensor* s = InitSensor(key->ToString(), value->ToObject());

            if(s != nullptr) {
                s->initialize();
                sensors.push_back(s);
            }
        }
        
        // Create & launch the main scheduler
        sensor::scheduler* handler = new sensor::scheduler(sensors, [callback, isolate](sensor::sensor* s, sensor::result* r) {
            // Get the type of value & the value
            Local<Object> result = Object::New(isolate); 
            
            Local<String> type;
            Local<String> unit;
            Local<String> unit_display;
            Local<Number> value;
            
            // Local reference of the callback
            Local<Function> cb = Local<Function>::New(isolate, callback);

            if(r->getType() == sensor::resultType::TEMPERATURE) {
                type = String::NewFromUtf8(isolate, "Temperature");
                unit = String::NewFromUtf8(isolate, "Degree Celsius");
                unit_display = String::NewFromUtf8(isolate, "°C");
                value = Number::New(isolate, r->getValue().f);
            }
            else if(r->getType() == sensor::resultType::HUMIDITY) {
                type = String::NewFromUtf8(isolate, "Humidity");
                unit = String::NewFromUtf8(isolate, "Percent");
                unit_display = String::NewFromUtf8(isolate, "%");
                value = Number::New(isolate, r->getValue().f);
            }
            else if(r->getType() == sensor::resultType::LIGHT) {
                type = String::NewFromUtf8(isolate, "Light");
                unit = String::NewFromUtf8(isolate, "Lux");
                unit_display = String::NewFromUtf8(isolate, "Lux");
                value = Number::New(isolate, r->getValue().i);
            }
            else if(r->getType() == sensor::resultType::PRESSURE) {
                type = String::NewFromUtf8(isolate, "Pressure");
                unit = String::NewFromUtf8(isolate, "Pascal");
                unit_display = String::NewFromUtf8(isolate, "Pa");
                value = Number::New(isolate, r->getValue().f);
            }
            else if(r->getType() == sensor::resultType::DETECTION) {
                type = String::NewFromUtf8(isolate, "Detection");
                unit = String::NewFromUtf8(isolate, "Boolean");
                unit_display = String::NewFromUtf8(isolate, "Boolean");
                value = Number::New(isolate, r->getValue().i);
            }
            else {
                type = String::NewFromUtf8(isolate, "Other");
                unit = String::NewFromUtf8(isolate, "-");
                unit_display = String::NewFromUtf8(isolate, "");
                value = Number::New(isolate, 0);
            }
            
            result->Set(String::NewFromUtf8(isolate, "type"), type);
            result->Set(String::NewFromUtf8(isolate, "unit"), unit);
            result->Set(String::NewFromUtf8(isolate, "unit_display"), unit_display);
            result->Set(String::NewFromUtf8(isolate, "value"), value);
            result->Set(String::NewFromUtf8(isolate, "date"), Date::New(isolate, r->getTimestamp()));
            result->Set(String::NewFromUtf8(isolate, "timestamp"), Number::New(isolate, r->getTimestamp()));
            result->Set(String::NewFromUtf8(isolate, "sensor_name"), String::NewFromUtf8(isolate, s->getName().c_str()));
            result->Set(String::NewFromUtf8(isolate, "sensor_type"), String::NewFromUtf8(isolate, s->getType().c_str()));

            // Call the callback with the values
            Local<Value> argv[1] = { result };
            cb->Call(isolate->GetCurrentContext()->Global(), 1, argv);
        });
        
        handler->launch();
        
    } catch(Local<Value> &e) {
        // If we catch any error error here, then no thread are launched : everything can be free'd
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
