/**
 * @file DHT22.cc
 * @brief Class implementation for all DHT22 sensors
 * @author Vuzi
 * @version 0.1
 * Based on https://github.com/adafruit/DHT-sensor-library and on https://github.com/Seeed-Studio/Grove-RaspberryPi
 */

#include "DHT22.h"

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
    DHT22_sensor::DHT22_sensor(unsigned _pin, int _freq, std::string _name):gpio_sensor(_pin, _freq, _name) {}
    
    DHT22_sensor::~DHT22_sensor() {}

    const std::string DHT22_sensor::getType() {
        return "DHT22";
    }

    void DHT22_sensor::initialize() {
        
        int iErr = wiringPiSetup();
        if (iErr == -1) {
            std::cout << "ERROR : Failed to init WiringPi " << iErr << std::endl;
        }
        
        // TODO handle init error
    }

    int DHT22_sensor::readData(int* piHumidity, int* piTemp) {
        uint8_t laststate = HIGH;
        uint8_t counter = 0;
        uint8_t j = 0, i;

        dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

        // Pull pin down for 18 milliseconds
        pinMode(getPin(), OUTPUT);
        digitalWrite(getPin(), LOW);
        delay(18);

        // Then pull it up for 40 microseconds
        digitalWrite(getPin(), HIGH);
        delayMicroseconds(40);

        // Prepare to read the pin
        pinMode(getPin(), INPUT);

        getTime(&laststate);
        getTime(&laststate);
        getTime(&laststate);

        // Detect change and read data
        for (i = 0; i < 40; i++) {
            int timer = getTime(&laststate);

            if(timer < 0)
                    break;

            timer = getTime(&laststate);
            if(timer < 0)
               break;

            // Ignore first 3 transitions
            // Shove each bit into the storage bytes
            dht22_dat[i/8] <<= 1;
            if (timer > 28)
                dht22_dat[i/8] |= 1;
            j++;
        }

        // Check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
        if ((dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF))) {
            *piHumidity = ((int)dht22_dat[0] << 8) + (int)dht22_dat[1];
            *piTemp = ((int)dht22_dat[2] << 8 ) + dht22_dat[3];

            if ((dht22_dat[2] & 0x80) != 0)
                *piTemp *= -1;

            return 1;
        } else
            return 0;
    }

    std::list<result> DHT22_sensor::getResults() {
        std::list<result> results;
        
        int iHumidity = -1;
        int iTemp = -1;
        
        for(int i = 0; i < 10; i++) {
            if (readData(7, &iHumidity, &iTemp) == 1) {
                // Humidity
                resultValue humidityValue;
                humidityValue.f = (float)(iHumidity/10.0);
                
                result humidity(resultType::HUMIDITY, humidityValue);
                
                // Temperature
                resultValue tempValue;
                tempValue.f = (float)(iTemp/10.0);
                
                result temp(resultType::TEMPERATURE, tempValue);
                
                // Add to the list and return
                results.push_back(humidity);
                results.push_back(temp);
                
                return results;
            } else {
                // In some case, no could will be read. Waiting a few seconds usually allow
                // to read ne data from the sensor
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
        
        // TODO throw exception
        std::cout << "Error :( " << std::endl;
        return results; // Return empty list of results
    }
    
}

