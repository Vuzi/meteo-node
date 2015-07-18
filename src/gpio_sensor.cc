
/**
 * @file gpio_sensor.cpp
 * @brief Base class implementation for all GPIO sensors. A GPIO sensor is using the GPIO bus on the raspberry
 * @author Vuzi
 * @version 0.1
 */

#include "gpio_sensor.h"

/**
 * @namespace sensor
 *
 * Name space used to store every class and functions related to the sensors
 */
namespace sensor {

    /**
     *  @brief Constructor
     *  @param pin : Value of which pin to read data on
     */
    gpio_sensor::gpio_sensor(unsigned _pin, int _freq, std::string _name):sensor(_name, _freq) {
        pin = _pin;
    }

    /**
     *  @brief Destructor
     */
    gpio_sensor::~gpio_sensor() {}

    /**
     *  @brief Return the pin that the sensor is using
     *  @return The pin that the sensor is using
     */
    unsigned gpio_sensor::getPin() {
        return pin;
    }
    
}

