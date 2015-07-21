#ifndef H_GPIO_SENSOR
#define H_GPIO_SENSOR

/**
 * @file gpio_sensor.h
 * @brief Base class for all GPIO sensors. A GPIO sensor is using the GPIO bus on the raspberry
 * @author Vuzi
 * @version 0.1
 */

#include <wiringPi.h>
#include <sys/time.h>

#include "sensor.h"

/**
 * @namespace sensor
 *
 * Name space used to store every class and functions related to the sensors
 */
namespace sensor {

    /**
     * @class gpio_sensor
     * @brief Base class for all the GPIO sensors
     *
     *  Base class for all the GPIO sensors. The class extends the default sensor function
     *  and adds the handling of the pin number, and default function to easily handle
     *  GPIO features (waiting for a state change, etc...)
     */
    class gpio_sensor : public sensor {

        public:
            /**
             *  @brief Constructor
             *  @param pin : Value of which pin to read data on
             */
            gpio_sensor(unsigned, int, std::string);

            /**
             *  @brief Destructor
             */
            virtual ~gpio_sensor();

            /**
             *  @brief Return the pin that the sensor is using
             *  @return The pin that the sensor is using
             */
            unsigned getPin();

        protected:
            unsigned pin; /** Pin number */

            /**
             * Get the time of change 
             * @param  laststate Last state of the value
             * @return           The time between the state change
             */
            int getTime(uint8_t*);

            /**
             * Get the time difference between two interval of time
             * @param  x The first interval
             * @param  y The second interval
             * @return   The difference in ms
             */
            double time_diff(struct timeval, struct timeval);
    };

}

#endif // H_GPIO_SENSOR

