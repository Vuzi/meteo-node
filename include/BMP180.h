#ifndef H_BMP180
#define H_BMP180

/**
 * @file BMP180.h
 * @brief Class for BMP180 sensors. A BMP180 use the I2C bus on the raspberry
 * @author Vuzi
 * @version 0.1
 */

#include "i2c_sensor.h"
#include "sensor_result.h"

#define OSS 0

/**
 * @namespace sensor
 *
 * Name space used to store every class and functions related to the sensors
 */
namespace sensor {

    /**
     * @class TSL2561_sensor
     * @brief Class for TSL2561 sensors
     */
    class BMP180_sensor : public i2c_sensor {

        public:
            /**
             *  @brief Constructor
             *  @param address : Address of the sensor
             */
            BMP180_sensor(uint16_t, int);

            /**
             *  @brief Destructor
             */
            virtual ~BMP180_sensor();
    
            void initialize();
            
            std::list<result> getResults();
            
         private:
            int16_t ac1;
            int16_t ac2;
            int16_t ac3;
            uint16_t ac4;
            uint16_t ac5;
            uint16_t ac6;
            int16_t b1;
            int16_t b2;
            int16_t mb;
            int16_t mc;
            int16_t md;
        
            uint16_t readUT();
            unsigned long readUP();
            
            float convertTemperature(uint16_t ut);
            long convertPressure(unsigned long up);
    
            /**
             * Read raw data from the I2C sensor
             * @param channel0 Data read in the first channel
             * @param channel1 Data read in the second channel
             */
            //void readRawData(uint32_t*, uint32_t*);

            /**
             * Read raw data and convert it to a readable lux value
             * @return The lux value read from the sensor
             */
            //uint32_t readData(bool iGain, bool tInt, bool iType);
    };

}

#endif // H_TSL2561