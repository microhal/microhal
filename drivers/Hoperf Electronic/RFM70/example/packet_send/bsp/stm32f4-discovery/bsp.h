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

//#include "i2c.h"

namespace bsp {

namespace con1 {
namespace a {
constexpr microhal::GPIO::IOPin io1 (microhal::stm32f4xx::GPIO::Port::PortE, 5);
constexpr microhal::GPIO::IOPin io2 (microhal::stm32f4xx::GPIO::Port::PortC, 15);
constexpr microhal::GPIO::IOPin io3 (microhal::stm32f4xx::GPIO::Port::PortC, 13);
constexpr microhal::GPIO::IOPin io4 (microhal::stm32f4xx::GPIO::Port::PortE, 6);
constexpr microhal::GPIO::IOPin io5 (microhal::stm32f4xx::GPIO::Port::PortE, 4);
constexpr microhal::GPIO::IOPin io6 (microhal::stm32f4xx::GPIO::Port::PortE, 2);
constexpr microhal::GPIO::IOPin ss (microhal::stm32f4xx::GPIO::Port::PortC, 14);
}
namespace b {
constexpr microhal::GPIO::IOPin io1 (microhal::stm32f4xx::GPIO::Port::PortC, 5);
constexpr microhal::GPIO::IOPin io2 (microhal::stm32f4xx::GPIO::Port::PortB, 1);
constexpr microhal::GPIO::IOPin io3 (microhal::stm32f4xx::GPIO::Port::PortE, 9);
constexpr microhal::GPIO::IOPin io4 (microhal::stm32f4xx::GPIO::Port::PortE, 11);
constexpr microhal::GPIO::IOPin io5 (microhal::stm32f4xx::GPIO::Port::PortE, 4);
constexpr microhal::GPIO::IOPin ss (microhal::stm32f4xx::GPIO::Port::PortE, 7);
}
}

namespace rfm70 {
static microhal::SPI &spi = microhal::stm32f4xx::SPI::spi1;
constexpr microhal::GPIO::IOPin csn = con1::a::io2;
constexpr microhal::GPIO::IOPin ce = con1::a::io1;
constexpr microhal::GPIO::IOPin irq = con1::a::io4;
}

static microhal::SerialPort &debugPort = microhal::stm32f4xx::SerialPort::Serial3;

constexpr microhal::GPIO::IOPin Led3(microhal::stm32f4xx::GPIO::Port::PortD, 13);
constexpr microhal::GPIO::IOPin Led4(microhal::stm32f4xx::GPIO::Port::PortD, 12);
constexpr microhal::GPIO::IOPin Led5(microhal::stm32f4xx::GPIO::Port::PortD, 14);
constexpr microhal::GPIO::IOPin Led6(microhal::stm32f4xx::GPIO::Port::PortD, 15);

constexpr microhal::GPIO::IOPin Sw1(microhal::stm32f4xx::GPIO::Port::PortA, 0);

constexpr microhal::GPIO::IOPin lis_ce(microhal::stm32f4xx::GPIO::Port::PortA, 3);

constexpr microhal::GPIO::IOPin GreenLed = Led4;
constexpr microhal::GPIO::IOPin RedLed = Led3;
}
#endif /* STM32F4DISCOVERY_H_ */
