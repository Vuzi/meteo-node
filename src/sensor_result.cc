
#include "sensor_result.h"


namespace sensor {
    
    /**
     * Sensor's result class constructor
     */
    result::result(resultType _t, resultValue _r) {
        t = _t;
        r  = _r;
        date = std::chrono::system_clock::now(); // Get current time
    }
    
    result::~result() {}
    
    /**
     * Return the result's raw value
     * @return The result value
     */
    resultValue result::getValue() {
        return r;
    }
    
    /**
     * Get the result's type
     * @return The result type
     */
    resultType result::getType() {
        return t;
    }
    
    /**
     * Get the creation time of the result
     * @return The creation date
     */
    resultTimestamp result::getTime() {
        return date;
    }
    
    long long result::getTimestamp() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(date.time_since_epoch()).count();
    }
}
