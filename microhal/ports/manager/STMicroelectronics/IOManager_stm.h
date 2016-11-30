/*
 * IOManager_stm.h
 *
 *  Created on: 9 kwi 2014
 *      Author: pawel
 */

#ifndef IOMANAGER_STM_H_
#define IOMANAGER_STM_H_

#ifdef MCU_TYPE_STM32F0XX
#include "STM32F0xx/IOManager_STM32F0xx.h"
namespace microhal {
    namespace activePort = stm32f0xx;
}
#elif defined(MCU_TYPE_STM32F4XX)
#include "ports/stm32f4xx/IOManager_stm32f4xx.h"
namespace microhal {
    namespace activePort = stm32f4xx;
}

#else
#error "MCU type must be specified."
#endif

#endif /* IOMANAGER_STM_H_ */
