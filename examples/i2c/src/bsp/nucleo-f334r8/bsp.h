#ifndef STM32F3DISCOVERY_H_
#define STM32F3DISCOVERY_H_

#include "microhal.h"

#define MCU_HAVE_TWO_I2C 0

extern microhal::GPIO& greenLed;
extern microhal::GPIO& redLed;
extern microhal::GPIO& button;

namespace bsp {
static microhal::SerialPort& debugPort = microhal::stm32f3xx::SerialPort::Serial2;
static microhal::I2C& i2c = microhal::stm32f3xx::I2C::i2c1;

void init();
}  // namespace bsp

#endif  // STM32F3DISCOVERY_H_
