/*
 * gpio_interface.cpp
 *
 *  Created on: Dec 5, 2020
 *      Author: pokas
 */

#include "gpio_interface.h"

namespace microhal {

GPIO::Direction GPIO::getDirection() const {
    Direction dir;
    OutputType otype;
    PullType pull;
    getConfiguration(dir, otype, pull);
    return dir;
}

GPIO::OutputType GPIO::getOutputType() const {
    Direction dir;
    OutputType otype;
    PullType pull;
    getConfiguration(dir, otype, pull);
    return otype;
}

GPIO::PullType GPIO::getPullType() const {
    Direction dir;
    OutputType otype;
    PullType pull;
    getConfiguration(dir, otype, pull);
    return pull;
}

}  // namespace microhal
