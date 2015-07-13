#ifndef H_GPIO_SENSOR
#define H_GPIO_SENSOR

/**
 * @file gpio_sensor.h
 * @brief Base class for all GPIO sensors. A GPIO sensor is using the GPIO bus on the raspberry
 * @author Vuzi
 * @version 0.1
 */

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
            gpio_sensor(unsigned);

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
    };

}

#endif // H_GPIO_SENSOR

