/*
 * serialPort_stm.h
 *
 *  Created on: 9 kwi 2014
 *      Author: pawel
 */

#ifndef SERIALPORT_STM_H_
#define SERIALPORT_STM_H_

#if defined(MCU_TYPE_STM32F0XX)
#include "STM32F0xx/serialPort_stm32f0xx.h"
namespace microhal {
    namespace activePort = stm32f0xx;
}
#elif defined(MCU_TYPE_STM32F4XX)
#include "ports/stm32f4xx/serialPort_stm32f4xx.h"
namespace microhal {
    namespace activePort = stm32f4xx;
}
#else
#error "MCU type must be specified."
#endif

#endif /* SERIALPORT_STM_H_ */
