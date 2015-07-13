#ifndef H_DHT22
#define H_DHT22

/**
 * @file DHT22.h
 * @brief Class for DHT22 sensors. A DHT22 use the GPIO bus on the raspberry
 * @author Vuzi
 * @version 0.1
 */

#include <chrono>
#include <thread>
#include <list>

#include <wiringPi.h>

#include "gpio_sensor.h"
#include "sensor_result.h"

#define MAXTIMINGS 85

/**
 * @namespace sensor
 *
 * Name space used to store every class and functions related to the sensors
 */
namespace sensor {

    /**
     * @class DHT22_sensor
     * @brief Class for DHT22 sensors
     */
    class DHT22_sensor : public gpio_sensor {

        public:
            /**
             *  @brief Constructor
             *  @param pin : Value of which pin to read data on
             */
            DHT22_sensor(unsigned);

            /**
             *  @brief Destructor
             */
            virtual ~DHT22_sensor();
    
            void initialize();
            
            std::list<sensor_result::result> getResults();
            
         private:
            int readData(int iPin, int* piHumidity, int* piTemp);
    };

}

#endif // H_DHT22

