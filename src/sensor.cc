
/**
 * @file sensor.cpp
 * @brief Base class for all sensors implementation.
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
    *  @brief Constructor
    *  @param frequence : The sensor frequence
    */
    sensor::sensor(int _frequence) {
        frequence = _frequence;
    }

    /**
    *  @brief Empty constructor
    */
    sensor::sensor() {
        frequence = 5000; // Every 5s (default)
    }

    /**
     *  @brief Return the sensor frequence
     *  @return The sensor frequence
     */
    int sensor::getFrequence() {
        return frequence;
    }

    /**
     *  @brief Set the result frequence. The frequence is indirectly used by the sensor
     *  @param frequence : The sensor frequence
     */
    void sensor::setFrequence(int _frequence) {
        frequence = _frequence;
    }
}
