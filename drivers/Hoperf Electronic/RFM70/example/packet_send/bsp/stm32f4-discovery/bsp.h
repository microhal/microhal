/* =========================================================================================== */ /**
 @file        STM32F4Discovery.h
 @authors     Michal Karwatowski
 @version     $Id$
 @package
 @brief       hardware definitions
 @hardware    STM32F4Discovery
 @copyright   $Copyright$
 @details

 */ /* ============================================================================================
                                                                                                         */
/* Created on: 20 sty 2014 */

#ifndef STM32F4DISCOVERY_H_
#define STM32F4DISCOVERY_H_

#include "microhal.h"

namespace bsp {

namespace con1 {
namespace a {
constexpr microhal::IOPin io1(microhal::stm32f4xx::GPIO::Port::PortE, 5);
constexpr microhal::IOPin io2(microhal::stm32f4xx::GPIO::Port::PortC, 15);
constexpr microhal::IOPin io3(microhal::stm32f4xx::GPIO::Port::PortC, 13);
constexpr microhal::IOPin io4(microhal::stm32f4xx::GPIO::Port::PortE, 6);
constexpr microhal::IOPin io5(microhal::stm32f4xx::GPIO::Port::PortE, 4);
constexpr microhal::IOPin io6(microhal::stm32f4xx::GPIO::Port::PortE, 2);
constexpr microhal::IOPin ss(microhal::stm32f4xx::GPIO::Port::PortC, 14);
}  // namespace a
namespace b {
constexpr microhal::IOPin io1(microhal::stm32f4xx::GPIO::Port::PortC, 5);
constexpr microhal::IOPin io2(microhal::stm32f4xx::GPIO::Port::PortB, 1);
constexpr microhal::IOPin io3(microhal::stm32f4xx::GPIO::Port::PortE, 9);
constexpr microhal::IOPin io4(microhal::stm32f4xx::GPIO::Port::PortE, 11);
constexpr microhal::IOPin io5(microhal::stm32f4xx::GPIO::Port::PortE, 4);
constexpr microhal::IOPin ss(microhal::stm32f4xx::GPIO::Port::PortE, 7);
}  // namespace b
}  // namespace con1

namespace rfm70 {
static microhal::SPI &spi = microhal::stm32f4xx::SPI::spi1;
extern microhal::GPIO &csn;
extern microhal::GPIO &ce;
constexpr microhal::IOPin irq = con1::a::io4;
}  // namespace rfm70

extern microhal::GPIO &led;
extern microhal::GPIO &button;
static microhal::SerialPort &debugPort = microhal::stm32f4xx::SerialPort::Serial3;

constexpr microhal::IOPin Led3(microhal::stm32f4xx::GPIO::Port::PortD, 13);
constexpr microhal::IOPin Led4(microhal::stm32f4xx::GPIO::Port::PortD, 12);
constexpr microhal::IOPin Led5(microhal::stm32f4xx::GPIO::Port::PortD, 14);
constexpr microhal::IOPin Led6(microhal::stm32f4xx::GPIO::Port::PortD, 15);

constexpr microhal::IOPin Sw1(microhal::stm32f4xx::GPIO::Port::PortA, 0);

constexpr microhal::IOPin lis_ce(microhal::stm32f4xx::GPIO::Port::PortA, 3);

constexpr microhal::IOPin GreenLed = Led4;
constexpr microhal::IOPin RedLed = Led3;
}  // namespace bsp
#endif /* STM32F4DISCOVERY_H_ */
