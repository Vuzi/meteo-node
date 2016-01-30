#ifndef H_PIR
#define H_PIR

/**
 * @file PIR.h
 * @brief Class for PIR sensors. A PIR use the GPIO bus on the raspberry,
 * and set it to HIGH if anything is detected
 * @author Vuzi
 * @version 0.1
 */

#include <chrono>
#include <thread>
#include <list>

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
     * @class PIR_sensor
     * @brief Class for PIR sensors
     */
    class PIR_sensor : public gpio_sensor {

        public:
            /**
             *  @brief Constructor
             *  @param pin : Value of which pin to read data on
             */
            PIR_sensor(unsigned, std::string);

            /**
             *  @brief Destructor
             */
            virtual ~PIR_sensor();

            const std::string getType();

            std::list<result> getResults();

            static sensor* create(int, const std::string&);

         private:
            int readData(int* piDetection);
    };

}

#endif // H_PIR

