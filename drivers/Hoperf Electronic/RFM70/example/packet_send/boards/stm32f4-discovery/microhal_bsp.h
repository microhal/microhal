/* =========================================================================================== *//**
 @file        STM32F4Discovery.h
 @authors     Michal Karwatowski
 @version     $Id$
 @package
 @brief       hardware definitions
 @hardware    STM32F4Discovery
 @copyright   $Copyright$
 @details

 *//* ============================================================================================ */
/* Created on: 20 sty 2014 */

#ifndef STM32F4DISCOVERY_H_
#define STM32F4DISCOVERY_H_

#include "i2c.h"

static microhal::SerialPort &debugPort = microhal::stm32f4xx::SerialPort::Serial3;

static microhal::SPI &lisSpi = microhal::stm32f4xx::SPI::spi1;

constexpr microhal::GPIO::IOPin Led3(microhal::GPIO::Port::PortD, 13);
constexpr microhal::GPIO::IOPin Led4(microhal::GPIO::Port::PortD, 12);
constexpr microhal::GPIO::IOPin Led5(microhal::GPIO::Port::PortD, 14);
constexpr microhal::GPIO::IOPin Led6(microhal::GPIO::Port::PortD, 15);

constexpr microhal::GPIO::IOPin Sw1(microhal::GPIO::Port::PortA, 0);

constexpr microhal::GPIO::IOPin GreenLed = Led4;
constexpr microhal::GPIO::IOPin RedLed = Led3;

#endif /* STM32F4DISCOVERY_H_ */
