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

//#include "i2c.h"

namespace bsp {

namespace rfm70 {

extern microhal::SPI &spi;
extern microhal::GPIO::IOPin csn;
extern microhal::GPIO::IOPin ce;
}

extern microhal::SerialPort &debugPort;

extern microhal::GPIO::IOPin Led3;
extern microhal::GPIO::IOPin Led4;
extern microhal::GPIO::IOPin Led5;
extern microhal::GPIO::IOPin Led6;

extern microhal::GPIO::IOPin Sw1;

extern microhal::GPIO::IOPin lis_ce;

extern microhal::GPIO::IOPin GreenLed;
extern microhal::GPIO::IOPin RedLed;
}
#endif /* STM32F4DISCOVERY_H_ */
