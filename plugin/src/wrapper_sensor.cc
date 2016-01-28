#include "wrapper_sensor.h"

#include "DHT22.h"
#include "PIR.h"
#include "TSL2561.h"
#include "BMP180.h"

using namespace v8;

const struct sensor::sensorConf conf[] = {
    {
        type    : "DHT22",
        bus     : sensor::GPIO,
        factory : sensor::DHT22_sensor::create
    },
    {
        type    : "PIR",
        bus     : sensor::GPIO,
        factory : sensor::PIR_sensor::create
    },
    {
        type    : "TSL2561",
        bus     : sensor::I2C,
        factory : sensor::TSL2561_sensor::create
    }/*,
    {
        type    : "BMP180",
        bus     : sensor::I2C,
        factory : sensor::BMP180_sensor::create
    }*/
};

Persistent<Function> SensorWrapper::constructor;

SensorWrapper::SensorWrapper(sensor::sensor* s) : _s(s) {}

SensorWrapper::~SensorWrapper() {
    // Free the sensor ?
    // TODO
    // del _s; ??
}

sensor::sensor* SensorWrapper::InitSensor(const Local<String>& sensorName, const Local<Object>& sensorConfig) {
    Isolate* isolate = Isolate::GetCurrent();

    const Local<String> t = String::NewFromUtf8(isolate, "type");

    // Check for type
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

    const size_t size = std::extent<decltype(conf)>::value;
    
    for(size_t i = 0; i < size; i++) {
        if(conf[i].type == type) {
            const Local<String> prop = String::NewFromUtf8(isolate, (conf[i].bus == sensor::GPIO ? "pin" : "address"));

            // Get the required property
            if(!sensorConfig->Has(prop) || !sensorConfig->Get(prop)->IsNumber()) {
                throw Exception::TypeError(
                    String::NewFromUtf8(isolate, 
                        fmt::format("Error : a valid {0} property is required (number >= 0x0)",
                            (conf[i].bus == sensor::GPIO ? "pin" : "address")).c_str()));
            }

            Local<Number> propValue = Local<Number>::Cast(sensorConfig->Get(prop));
            sensor::sensor *s = conf[i].factory((int)propValue->NumberValue(), name);
            s->initialize(); // TODO : bind to the "init" method of the module

            return s;
        }
    }

    throw Exception::TypeError(
        String::NewFromUtf8(isolate, fmt::format("Error : {0} is not a valid sensor type", type).c_str()));

    return nullptr;
}

void SensorWrapper::Init(Local<Object> exports) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "Sensor"));
    tpl->InstanceTemplate()->SetInternalFieldCount(3);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "fetch", Fetch);
    NODE_SET_PROTOTYPE_METHOD(tpl, "FetchInterval", FetchInterval);
    NODE_SET_PROTOTYPE_METHOD(tpl, "fetchClear", FetchClear);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "Sensor"), tpl->GetFunction());
}

void SensorWrapper::New(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        // Check the provided values
        // TODO...
        const Local<Object> conf = args[0]->ToObject();
        const Local<String> name = args[1]->ToString();

        try {
            // Create a sensor
            SensorWrapper* obj = new SensorWrapper(InitSensor(name, conf));
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());

        } catch(Local<Value> &e) {
            // Transfer the exception to node
            isolate->ThrowException(e);
        }
    } else {
        // Invoked as plain function, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void SensorWrapper::Fetch(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    // TODO check args
    Local<Function> cb = Local<Function>::Cast(args[0]);

    Persistent<Function, CopyablePersistentTraits<Function>> callback(isolate, cb);

    SensorWrapper* sensorWrapper = ObjectWrap::Unwrap<SensorWrapper>(args.Holder());
    sensor::sensor* s = sensorWrapper->_s;

    sensor::scheduler* handler = new sensor::scheduler(s,
    [callback, isolate](sensor::sensor* s, std::list<sensor::result> results) {
        for(sensor::result r : results) {
            // Get the type of value & the value
            Local<Object> result = Object::New(isolate); 
            
            Local<String> type;
            Local<String> unit;
            Local<String> unit_display;
            Local<Number> value;
            
            // Local reference of the callback
            Local<Function> cb = Local<Function>::New(isolate, callback);

            if(r.getType() == sensor::resultType::TEMPERATURE) {
                type = String::NewFromUtf8(isolate, "Temperature");
                unit = String::NewFromUtf8(isolate, "Degree Celsius");
                unit_display = String::NewFromUtf8(isolate, "°C");
                value = Number::New(isolate, r.getValue().f);
            }
            else if(r.getType() == sensor::resultType::HUMIDITY) {
                type = String::NewFromUtf8(isolate, "Humidity");
                unit = String::NewFromUtf8(isolate, "Percent");
                unit_display = String::NewFromUtf8(isolate, "%");
                value = Number::New(isolate, r.getValue().f);
            }
            else if(r.getType() == sensor::resultType::LIGHT) {
                type = String::NewFromUtf8(isolate, "Light");
                unit = String::NewFromUtf8(isolate, "Lux");
                unit_display = String::NewFromUtf8(isolate, "Lux");
                value = Number::New(isolate, r.getValue().i);
            }
            else if(r.getType() == sensor::resultType::PRESSURE) {
                type = String::NewFromUtf8(isolate, "Pressure");
                unit = String::NewFromUtf8(isolate, "Pascal");
                unit_display = String::NewFromUtf8(isolate, "Pa");
                value = Number::New(isolate, r.getValue().f);
            }
            else if(r.getType() == sensor::resultType::DETECTION) {
                type = String::NewFromUtf8(isolate, "Detection");
                unit = String::NewFromUtf8(isolate, "Boolean");
                unit_display = String::NewFromUtf8(isolate, "Boolean");
                value = Number::New(isolate, r.getValue().i);
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
            result->Set(String::NewFromUtf8(isolate, "date"), Date::New(isolate, r.getTimestamp()));
            result->Set(String::NewFromUtf8(isolate, "timestamp"), Number::New(isolate, r.getTimestamp()));
            //result->Set(String::NewFromUtf8(isolate, "sensor_name"), String::NewFromUtf8(isolate, sensorWrapper->_s->getName().c_str()));
            //result->Set(String::NewFromUtf8(isolate, "sensor_type"), String::NewFromUtf8(isolate, sensorWrapper->_s->getType().c_str()));

            // Call the callback with the values
            Local<Value> argv[2] = { Undefined(isolate), result };
            cb->Call(isolate->GetCurrentContext()->Global(), 2, argv);
        }
    });
    handler->launch();
}

void SensorWrapper::FetchInterval(const FunctionCallbackInfo<Value>& args) {
  /*Isolate* isolate = args.GetIsolate();

  SensorWrapper* sensorWrapper = ObjectWrap::Unwrap<SensorWrapper>(args.Holder());
  // Todo call async sensor get value

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));*/
}

void SensorWrapper::FetchClear(const FunctionCallbackInfo<Value>& args) {
  /*Isolate* isolate = args.GetIsolate();

  SensorWrapper* sensorWrapper = ObjectWrap::Unwrap<SensorWrapper>(args.Holder());
  // Todo clear all fetchs

  args.GetReturnValue().Set(Number::New(isolate, obj->value_));*/
}
