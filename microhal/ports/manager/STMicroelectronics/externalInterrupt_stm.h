/*
 * externalInterrupt_stm.h
 *
 *  Created on: 9 kwi 2014
 *      Author: pawel
 */

#ifndef EXTERNALINTERRUPT_STM_H_
#define EXTERNALINTERRUPT_STM_H_

#ifdef MCU_TYPE_STM32F0XX
#include "ports/stm32f0xx/externalInterrupt_stm32f0xx.h"
namespace microhal {
namespace activePort = stm32f0xx;
}
#elif defined(MCU_TYPE_STM32F3XX)
#include "ports/stm32f3xx/externalInterrupt_stm32f3xx.h"
namespace microhal {
namespace activePort = stm32f3xx;
}
#elif defined(MCU_TYPE_STM32F4XX)
#include "ports/stm32f4xx/externalInterrupt_stm32f4xx.h"
namespace microhal {
namespace activePort = stm32f4xx;
}
#else
#error "MCU type must be specified."
#endif

#endif /* EXTERNALINTERRUPT_STM_H_ */
