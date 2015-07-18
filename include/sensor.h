#ifndef H_SENSOR
#define H_SENSOR

/**
 * @file sensor.h
 * @brief Base class for all sensors
 * @author Vuzi
 * @version 0.1
 */

#include <string>
#include <list>

#include "sensor_result.h"

/**
 * @namespace sensor
 *
 * Name space used to store every class and functions related to the sensors
 */
namespace sensor {

    /**
     * @class sensor
     * @brief Base class for all the sensors
     *
     *  Base class for all the sensors. The class has three default function,
     *  that should be handled by any implementation
     */
    class sensor {

        public:
            /**
             *  @brief Constructor
             *  @param frequence : The sensor frequence
             */
            sensor(int, std::string);

            /**
             *  @brief Empty constructor
             */
            sensor();

            /**
             *  @brief Destructor
             */
            virtual ~sensor() {};

            /**
             *  @brief Return the sensor frequence
             *  @return The sensor frequence
             */
            int getFrequence();

            /**
             *  @brief Set the result frequence. The frequence is indirectly used by the sensor
             *  @param frequence : The sensor frequence
             */
            void setFrequence(int);

            /**
             *  @brief Return the name of the sensor
             *  @return The name of the sensor
             */
            const std::string getName();

            /**
             *  @brief Initialize the sensor. This method must be called before any result
             */
            virtual void initialize() = 0;

            /**
             *  @brief Get the results of the sensor. Will throw exception if the ready fails
             *  @return The results of the sensor
             */
            virtual std::list<result> getResults() = 0;

        protected:
            int frequence;   // The capture frequence, in ms
            const std::string name // Name of the sensor

    };

}

#endif // H_SENSOR

