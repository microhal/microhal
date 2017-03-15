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

#include "microhal.h"

namespace bsp {
static microhal::SerialPort &debugPort = microhal::stm32f4xx::SerialPort::Serial3;

// ---------------------------- Sensors ----------------------------------------
namespace at45db {
static microhal::SPI &spi = microhal::stm32f4xx::SPI::spi1;
constexpr microhal::GPIO::IOPin ce(microhal::stm32f4xx::GPIO::Port::PortE, 3);
constexpr microhal::GPIO::IOPin reset(microhal::stm32f4xx::GPIO::Port::PortE, 3);
constexpr microhal::GPIO::IOPin wp(microhal::stm32f4xx::GPIO::Port::PortE, 3);
}

// ----------------------------- LEDs -------------------------------------------
constexpr microhal::GPIO::IOPin Led3(microhal::stm32f4xx::GPIO::Port::PortD, 13);
constexpr microhal::GPIO::IOPin Led4(microhal::stm32f4xx::GPIO::Port::PortD, 12);
constexpr microhal::GPIO::IOPin Led5(microhal::stm32f4xx::GPIO::Port::PortD, 14);
constexpr microhal::GPIO::IOPin Led6(microhal::stm32f4xx::GPIO::Port::PortD, 15);

constexpr microhal::GPIO::IOPin Sw1(microhal::stm32f4xx::GPIO::Port::PortA, 0);

}  // namespace bsp
#endif /* STM32F4DISCOVERY_H_ */
