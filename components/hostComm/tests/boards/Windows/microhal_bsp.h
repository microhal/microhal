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

#ifndef WINDOWS_BSP_H_
#define WINDOWS_BSP_H_

bool BSP_Init(void);
bool BSP_Deinit(void);

extern  microhal::IODevice &debugPort;
extern  microhal::SerialPort &communicationPortA;
extern  microhal::SerialPort &communicationPortB;

#endif /* STM32F4DISCOVERY_H_ */
