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

static microhal::SerialPort *serialPorts[] = {
    &microhal::stm32f4xx::SerialPort::Serial1, &microhal::stm32f4xx::SerialPort::Serial2, &microhal::stm32f4xx::SerialPort::Serial3,
    &microhal::stm32f4xx::SerialPort::Serial4  //,
                                               // microhal::stm32f4xx::SerialPort::Serial5,
    // microhal::stm32f4xx::SerialPort::Serial6
};

#endif /* STM32F4DISCOVERY_H_ */
