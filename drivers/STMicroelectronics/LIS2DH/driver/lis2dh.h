
#ifndef LIS2DH_H_
#define LIS2DH_H_

#include <cstdint>
#include <experimental/optional>

#include "microhal.h"
#include "I2CDevice/I2CDevice.h"
#include "units/magnetic.h"
#include "units/temperature.h"
#include "math/vector.h"


class LIS2DH : protected microhal::I2CDevice {

};


#endif /* LIS2DH_H_ */
