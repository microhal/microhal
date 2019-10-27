/*
 * stmCommonDefines.h
 *
 *  Created on: Oct 26, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_STMCOMMON_STMCOMMONDEFINES_H_
#define _MICROHAL_PORTS_STMCOMMON_STMCOMMONDEFINES_H_

#ifdef MCU_TYPE_STM32F0XX
#define _MICROHAL_ACTIVE_PORT_NAMESPACE stm32f0xx
#define _MICROHAL_INCLUDE_PORT_clockManager "ports/stm32f0xx/clockManager.h"
#define _MICROHAL_INCLUDE_PORT_DEVICE "ports/stm32f0xx/device/stm32f0xx.h"
#elif defined(MCU_TYPE_STM32F3XX)
#define _MICROHAL_ACTIVE_PORT_NAMESPACE stm32f3xx
#define _MICROHAL_INCLUDE_PORT_clockManager "ports/stm32f3xx/clockManager.h"
#define _MICROHAL_INCLUDE_PORT_DEVICE "ports/stm32f3xx/device/stm32f3xx.h"
#define _MICROHAL_INCLUDE_PORT_CANREGISTERS "ports/stm32f3xx/can_registers.h"
#elif defined(MCU_TYPE_STM32F4XX)
#define _MICROHAL_ACTIVE_PORT_NAMESPACE stm32f4xx
#define _MICROHAL_INCLUDE_PORT_clockManager "ports/stm32f4xx/clockManager.h"
#define _MICROHAL_INCLUDE_PORT_DEVICE "ports/stm32f4xx/device/stm32f4xx.h"
#define _MICROHAL_INCLUDE_PORT_CANREGISTERS "ports/stm32f4xx/can_registers.h"
#else
#error "MCU type must be specified."
#endif

#endif /* _MICROHAL_PORTS_STMCOMMON_STMCOMMONDEFINES_H_ */
