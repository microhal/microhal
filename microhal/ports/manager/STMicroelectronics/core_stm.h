/*
 * core_stm.h
 *
 *  Created on: 9 kwi 2014
 *      Author: pawel
 */

#ifndef CORE_STM_H_
#define CORE_STM_H_

#ifdef MCU_TYPE_STM32F0XX
#include "STM32F0xx/core_STM32F0xx.h"
namespace microhal {
    namespace activePort = stm32f0xx;
}
#elif defined(MCU_TYPE_STM32F4XX)
#include "ports/stm32f4xx/core_stm32f4xx.h"
namespace microhal {
    namespace activePort = stm32f4xx;
}
#else
#error "MCU type must be specified."
#endif

#endif /* CORE_STM_H_ */
