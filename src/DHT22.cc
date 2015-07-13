/**
 * @file DHT22.cc
 * @brief Class implementation for all DHT22 sensors
 * @author Vuzi
 * @version 0.1
 */

#include "DHT22.h"

// Only for test purpose
#include <string>
#include <iostream>

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
    DHT22_sensor::DHT22_sensor(unsigned _pin):gpio_sensor(_pin) {}
    
    DHT22_sensor::~DHT22_sensor() {}
    
    void DHT22_sensor::initialize() {
        
    	int iErr = wiringPiSetup();
        if (iErr == -1) {
    		std::cout << "ERROR : Failed to init WiringPi " << iErr << std::endl;
    	}
        
        // TODO handle init error
    }
    
    int DHT22_sensor::readData(int iPin, int* piHumidity, int* piTemp) {
        uint8_t laststate = HIGH;
        uint8_t counter = 0;
        uint8_t j = 0, i;

        int dht22_dat[5] = {0,0,0,0,0};
    
        // pull pin down for 18 milliseconds
        pinMode(iPin, OUTPUT);
        digitalWrite(iPin, LOW);
        delay(18);
    
        // then pull it up for 40 microseconds
        digitalWrite(iPin, HIGH);
        delayMicroseconds(40);
    
    	// prepare to read the pin
        pinMode(iPin, INPUT);
    
        // detect change and read data
        for ( i=0; i< MAXTIMINGS; i++) {
            counter = 0;
            while (digitalRead(iPin) == laststate) {
                counter++;
                delayMicroseconds(1);
                if (counter == 255) {
                    break;
                }
            }
            laststate = digitalRead(iPin);
    
            if (counter == 255) break;
    
            // ignore first 3 transitions
            if ((i >= 4) && (i%2 == 0)) {
                // shove each bit into the storage bytes
                dht22_dat[j/8] <<= 1;
                if (counter > 16)
                    dht22_dat[j/8] |= 1;
                j++;
            }
        }
    
        // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
        if ((j >= 40) && (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {
    		*piHumidity = dht22_dat[0] * 256 + dht22_dat[1];
    		*piTemp = (dht22_dat[2] & 0x7F)* 256 + dht22_dat[3];
            if ((dht22_dat[2] & 0x80) != 0)
    			*piTemp *= -1;
    
    		return 1;
        } else {
            return 0;
        }
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

