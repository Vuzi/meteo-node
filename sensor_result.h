#ifndef H_SENSOR_RESULT
#define H_SENSOR_RESULT

#include <ctime>
#include <iostream>

namespace sensor_result {

	// Type of result
	enum type {
		TEMPERATURE, // Temperature, in Celcius
		PRESSURE,    // Pressure, in hPa
		HUMIDITY,    // Humidity, in %
		LIGHT,       // Light, in lux
		PHOTO,       // Photo
		OTHER
	};

	// Result itself
	union resultValue {
		int i;
		float f;
		double d;
		// TODO : photo
	};
	
	typedef union resultValue resultValue;

	// Result class
	class result {
	public:
		result(type, resultValue);
		~result();
		
		resultValue getValue();
		type getType();
		std::time_t getTime();

	private:
		resultValue r;     // Data of the result
		type t;            // Type of the data
		std::time_t date;  // Date of creation
	};
}

#endif // H_SENSOR_RESULT