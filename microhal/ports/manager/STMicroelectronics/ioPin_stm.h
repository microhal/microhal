/*
 * ioPin_stm.h
 *
 *  Created on: Oct 5, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_MANAGER_STMICROELECTRONICS_IOPIN_STM_H_
#define _MICROHAL_PORTS_MANAGER_STMICROELECTRONICS_IOPIN_STM_H_

#ifdef MCU_TYPE_STM32F0XX
#include "ports/stm32f0xx/ioPin.h"
namespace microhal {
namespace activePort = stm32f0xx;
}
#elif defined(MCU_TYPE_STM32F3XX)
#include "ports/stm32f3xx/ioPin.h"
namespace microhal {
using IOPin = stm32f3xx::IOPin;
// namespace activePort = stm32f3xx;
}  // namespace microhal
#elif defined(MCU_TYPE_STM32F4XX)
#include "ports/stm32f4xx/ioPin.h"
namespace microhal {
namespace activePort = stm32f4xx;
}

#else
#error "MCU type must be specified."
#endif

#endif /* _MICROHAL_PORTS_MANAGER_STMICROELECTRONICS_IOPIN_STM_H_ */
