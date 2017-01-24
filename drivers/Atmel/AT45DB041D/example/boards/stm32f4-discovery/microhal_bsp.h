/* =========================================================================================== */ /**
 @file        STM32F4Discovery.h
 @authors     Michal Karwatowski
 @version     $Id$
 @package
 @brief       hardware definitions
 @hardware    STM32F4Discovery
 @copyright   $Copyright$
 @details

 */ /* ============================================================================================ */
/* Created on: 20 sty 2014 */

#ifndef STM32F4DISCOVERY_H_
#define STM32F4DISCOVERY_H_

extern microhal::SerialPort &serialPort;

// ---------------------------- Sensors ----------------------------------------
extern microhal::SPI &at45db_spi;
extern microhal::GPIO::IOPin at45db_CEpin;
extern microhal::GPIO::IOPin at45db_RESETpin;
extern microhal::GPIO::IOPin at45db_WPpin;

// ----------------------------- LEDs
// -------------------------------------------
extern microhal::GPIO::IOPin Led3;
extern microhal::GPIO::IOPin Led4;
extern microhal::GPIO::IOPin Led5;
extern microhal::GPIO::IOPin Led6;

extern microhal::GPIO::IOPin Sw1;

extern microhal::GPIO::IOPin GreenLed;
extern microhal::GPIO::IOPin RedLed;

#endif /* STM32F4DISCOVERY_H_ */
