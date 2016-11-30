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

static microhal::SerialPort &serialPort = microhal::stm32f4xx::SerialPort::Serial3;

// ---------------------------- Sensors ----------------------------------------
static microhal::SPI &at45db_spi = microhal::stm32f4xx::SPI::spi1;
constexpr microhal::GPIO::IOPin at45db_CEpin(microhal::stm32f4xx::GPIO::Port::PortE, 3);
constexpr microhal::GPIO::IOPin at45db_RESETpin(microhal::stm32f4xx::GPIO::Port::PortE, 3);
constexpr microhal::GPIO::IOPin at45db_WPpin(microhal::stm32f4xx::GPIO::Port::PortE, 3);

// ----------------------------- LEDs -------------------------------------------
constexpr microhal::GPIO::IOPin Led3(microhal::stm32f4xx::GPIO::Port::PortD, 13);
constexpr microhal::GPIO::IOPin Led4(microhal::stm32f4xx::GPIO::Port::PortD, 12);
constexpr microhal::GPIO::IOPin Led5(microhal::stm32f4xx::GPIO::Port::PortD, 14);
constexpr microhal::GPIO::IOPin Led6(microhal::stm32f4xx::GPIO::Port::PortD, 15);

constexpr microhal::GPIO::IOPin Sw1(microhal::stm32f4xx::GPIO::Port::PortA, 0);

constexpr microhal::GPIO::IOPin GreenLed = Led4;
constexpr microhal::GPIO::IOPin RedLed = Led3;

#endif /* STM32F4DISCOVERY_H_ */
