/*
 * gpioSet.cpp
 *
 *  Created on: Oct 26, 2019
 *      Author: pokas
 */

#include "bsp.h"

microhal::stm32f4xx::GPIO nativeRedLed(bsp::led3_pin, microhal::stm32f4xx::GPIO::Direction::Output);

void nativeInlineSetSizeTest() {
    nativeRedLed.set();
}
