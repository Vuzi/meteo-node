
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
    sensor::sensor(std::string _name):name(_name) {}

    /**
    *  @brief Empty constructor
    */
    sensor::sensor():name("no_name") {}

    /**
     * Return the name of the sensor
     * @return The sensor name
     */
    const std::string sensor::getName() {
        return name;
    }

    const std::string sensor::getType() {
        return "Generic";
    }

    resultsOrError sensor::getResultsOrError() {
        try {
            prepare(); // Prepare the sensor, then fetch results
            return resultsOrError(getResults());
        } catch (const sensorException& e) {
            return resultsOrError(e);
        } 
    }
    

    sensorException::sensorException(const std::string& msg, sensorErrorCode _errorCode) : runtime_error(msg) {
        errorCode = _errorCode;
    }

    sensorException::sensorException() : sensorException("", sensorErrorCode::NONE) {}

    const char* sensorException::what() {
        return fmt::format("{0} : code {1}", runtime_error::what(), errorCode).c_str();
    }

    sensorErrorCode sensorException::code() {
        return errorCode;
    }
}
