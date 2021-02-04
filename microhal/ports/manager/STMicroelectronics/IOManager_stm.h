/*
 * IOManager_stm.h
 *
 *  Created on: 9 kwi 2014
 *      Author: pawel
 */

#ifndef IOMANAGER_STM_H_
#define IOMANAGER_STM_H_

#ifdef MCU_TYPE_STM32F0XX
#include "ports/stm32f0xx/IOManager_stm32f0xx.h"
namespace microhal {
namespace activePort = stm32f0xx;
}
#elif defined(MCU_TYPE_STM32F1XX)
#include "ports/stm32f1xx/IOManager.h"
// namespace microhal {
// namespace activePort = stm32f1xx;
//}
#elif defined(MCU_TYPE_STM32F3XX)
#include "ports/stm32f3xx/IOManager_stm32f3xx.h"
namespace microhal {
namespace activePort = stm32f3xx;
}
#elif defined(MCU_TYPE_STM32F4XX)
#include "ports/stm32f4xx/IOManager_stm32f4xx.h"
namespace microhal {
namespace activePort = stm32f4xx;
}
#elif defined(MCU_TYPE_STM32G0XX)
#include "ports/stm32g0xx/IOManager_stm32g0xx.h"
namespace microhal {
namespace activePort = stm32g0xx;
}
#else
#error "MCU type must be specified."
#endif

#endif /* IOMANAGER_STM_H_ */
