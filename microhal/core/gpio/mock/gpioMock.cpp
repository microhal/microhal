/*
 * GPIO.cpp
 *
 *  Created on: Dec 2, 2020
 *      Author: pokas
 */

#include "gpioMock.h"

namespace microhal {
namespace mock {

GPIO::GPIO() {}

GPIO::~GPIO() {}

GPIO::Error GPIO::configure(Direction dir, OutputType otype, PullType pull) {
    direction = dir;
    if (direction == Direction::Output) {
        outputType = otype;
    }
    pullType = pull;
    return Error::None;
}

// bool GPIO::getConfiguration(Direction &dir, OutputType &otype, PullType &pull) const {
//    dir = direction;
//    otype = outputType;
//    pull = pullType;
//    return true;
//}

}  // namespace mock
}  // namespace microhal
