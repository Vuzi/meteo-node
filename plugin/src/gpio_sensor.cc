
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

    bool gpio_sensor::isGPIOInitialized = false;

    void gpio_sensor::prepare() {
        if(!isGPIOInitialized) {
            if(wiringPiSetup() < 0) {
                throw sensorException("Failed to init WiringPi", sensorErrorCode::GPIO_ERROR);
            } else {
                isGPIOInitialized = true;
            }
        }
    }

    /**
     *  @brief Constructor
     *  @param pin : Value of which pin to read data on
     */
    gpio_sensor::gpio_sensor(unsigned _pin, std::string _name):sensor(_name) {
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

    /**
     * Get the time difference between two interval of time
     * @param  x The first interval
     * @param  y The second interval
     * @return   The difference in ms
     */
    double gpio_sensor::timeDiff(struct timeval x , struct timeval y) {
        double x_ms , y_ms , diff;

        x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
        y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;

        diff = (double)y_ms - (double)x_ms;

        return diff;
    }

    /**
     * Get the time of change
     * @param  laststate Last state of the value
     * @return           The time between the state change
     */
    int gpio_sensor::getTime(uint8_t* laststate) {
        struct timeval start, stop;
        int counter = 0;
        gettimeofday(&start, NULL);

        while (digitalRead(pin) == *laststate) {
            delayMicroseconds(1);
            counter++;
            if (counter == 255) {
                return -1;
            }
        }

        gettimeofday(&stop, NULL);
        *laststate = digitalRead(pin);

        return (int) timeDiff(start, stop);
    }

}

