#ifndef H_SENSOR_RESULT
#define H_SENSOR_RESULT

#include <chrono>
#include <iostream>

namespace sensor {

    // Type of result
    enum resultType {
        TEMPERATURE, // Temperature, in Celcius
        PRESSURE,    // Pressure, in hPa
        HUMIDITY,    // Humidity, in %
        LIGHT,       // Light, in lux
        PHOTO,       // Photo
        OTHER
    };
    
    typedef enum resultType resultType;

    // Result itself
    union resultValue {
        int i;
        float f;
        double d;
        // TODO : photo
    };
    
    typedef union resultValue resultValue;

    typedef std::chrono::time_point<std::chrono::system_clock> resultTimestamp;

    // Result class
    class result {
    public:
        result(resultType, resultValue);
        ~result();
        
        resultValue getValue();
        resultType getType();
        resultTimestamp getTime();
        long long getTimestamp();

    private:
        resultValue r;     // Data of the result
        resultType t;      // Type of the data
        resultTimestamp date;  // Date of creation
    };
}

#endif // H_SENSOR_RESULT
