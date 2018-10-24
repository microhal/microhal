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
extern microhal::SPI &spi;
extern microhal::GPIO &ce;
extern microhal::GPIO &reset;
extern microhal::GPIO &wp;
}  // namespace at45db

}  // namespace bsp
#endif /* STM32F4DISCOVERY_H_ */
