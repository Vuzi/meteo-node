
#include "sensor_result.h"


namespace sensor {
	
	result::result(resultType _t, resultValue _r) {
		t = _t;
		r  = _r;
		date = std::time(nullptr);
	}
	
	result::~result() {}
	
	resultValue result::getValue() {
		return r;
	}
	
	resultType result::getType() {
		return t;
	}
	
	std::time_t result::getTime() {
		return date;
	}
	
}