/**
 * @file TSL2561.cc
 * @brief Class implementation for all TSL2561 sensors
 * @author Vuzi
 * @version 0.1
 */

#include "TSL2561.h"

/**
 * @namespace sensor
 *
 * Name space used to store every class and functions related to the sensors
 */
namespace sensor {

    TSL2561_sensor::TSL2561_sensor(uint16_t deviceAddress):i2c_sensor(deviceAddress) {}

    TSL2561_sensor::~TSL2561_sensor() {};

    void TSL2561_sensor::initialize() {
        writeRegister(TSL2561_CONTROL, 0x03);  // POWER UP
        writeRegister(TSL2561_TIMING, 0x11);   //High Gain (16x), integration time of 101ms
        writeRegister(TSL2561_INTERRUPT, 0x00);
    }

    /**
     * Read raw data from the I2C sensor
     * @param channel0 Data read in the first channel
     * @param channel1 Data read in the second channel
     */
    void TSL2561_sensor::readRawData(uint32_t* channel0, uint32_t* channel1) {
        uint16_t CH0_LOW = readRegister(TSL2561_CHANNAL0L);
        uint16_t CH0_HIGH = readRegister(TSL2561_CHANNAL0H);

        //read two bytes from registers 0x0E and 0x0F
        uint16_t CH1_LOW = readRegister(TSL2561_CHANNAL1L);
        uint16_t CH1_HIGH = readRegister(TSL2561_CHANNAL1H);

        *channel0 = CH0_HIGH * 256 + CH0_LOW;
        *channel1 = CH1_HIGH * 256 + CH1_LOW;
    }

    /**
     * Read raw data and convert it to a readable lux value
     * @return The lux value read from the sensor
     */
    uint32_t TSL2561_sensor::readData(bool iGain, bool tInt, bool iType) {
        uint32_t channel0 = 0x0, channel1 = 0x0;

        // Read raw data
        readRawData(&channel0, &channel1);

        uint32_t chScale;
        uint32_t ratio1;
        uint32_t b = 0x0;
        uint32_t m = 0x0;
        uint32_t temp;
        uint32_t lux;

        switch (tInt) {
            case 0:  // 13.7 msec
                chScale = CHSCALE_TINT0;
                break;
            case 1: // 101 msec
                chScale = CHSCALE_TINT1;
                break;
            default: // assume no scaling
                chScale = (1 << CH_SCALE);
                break;
        }

        if (!iGain)
            chScale = chScale << 4; // scale 1X to 16X

        // scale the channel values
        channel0 = (channel0 * chScale) >> CH_SCALE;
        channel1 = (channel1 * chScale) >> CH_SCALE;

        ratio1 = 0;

        if (channel0 != 0)
            ratio1 = (channel1 << (RATIO_SCALE+1))/channel0;

        // round the ratio value
        uint16_t ratio = (ratio1 + 1) >> 1;

        switch (iType) {
            case 0: // T package
                if (ratio <= K1T)
                {b=B1T; m=M1T;}
                else if (ratio <= K2T)
                {b=B2T; m=M2T;}
                else if (ratio <= K3T)
                {b=B3T; m=M3T;}
                else if (ratio <= K4T)
                {b=B4T; m=M4T;}
                else if (ratio <= K5T)
                {b=B5T; m=M5T;}
                else if (ratio <= K6T)
                {b=B6T; m=M6T;}
                else if (ratio <= K7T)
                {b=B7T; m=M7T;}
                else if (ratio > K8T)
                {b=B8T; m=M8T;}
                break;
        case 1:// CS package
                if (ratio <= K1C)
                {b=B1C; m=M1C;}
                else if (ratio <= K2C)
                {b=B2C; m=M2C;}
                else if (ratio <= K3C)
                {b=B3C; m=M3C;}
                else if (ratio <= K4C)
                {b=B4C; m=M4C;}
                else if (ratio <= K5C)
                {b=B5C; m=M5C;}
                else if (ratio <= K6C)
                {b=B6C; m=M6C;}
                else if (ratio <= K7C)
                {b=B7C; m=M7C;}
        }

        temp = ((channel0*b) - (channel1*m));
        temp += (1 << (LUX_SCALE-1));

        // strip off fractional portion
        lux=temp>>LUX_SCALE;

        return lux;
    }

    std::list<sensor_result::result> TSL2561_sensor::getResults() {
        std::list<sensor_result::result> results;

        // Read the result from the sensor
        uint32_t lux = readData(false, false, true);
        
        // Init the data
        sensor_result::resultValue luxValue;
        luxValue.i = lux;
                
        sensor_result::result luxResult(sensor_result::type::LIGHT, luxValue);
        
        // Return
        results.push_back(luxResult);
        return results;
    }
}

