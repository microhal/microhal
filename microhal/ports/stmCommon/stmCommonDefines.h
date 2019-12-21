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
#define _MICROHAL_INCLUDE_PORT_DEVICE "ports/stm32f0xx/device/stm32f0xx.h"
//#define _MICROHAL_INCLUDE_PORT_DMA "ports/stm32f0xx/dma_stm32f0xx.h"
#define _MICROHAL_INCLUDE_PORT_CONFIG "microhalPortConfig_stm32f0xx.h"
#define _MICROHAL_INCLUDE_PORT_IOMANAGER "ports/stm32f0xx/IOManager_stm32f0xx.h"
#define _MICROHAL_INCLUDE_PORT_GPIO "ports/stm32f0xx/gpio_stm32f0xx.h"
#elif defined(MCU_TYPE_STM32F1XX)
#define _MICROHAL_ACTIVE_PORT_NAMESPACE stm32f1xx
#define _MICROHAL_INCLUDE_PORT_DEVICE "ports/stm32f1xx/nvic.h"
#define _MICROHAL_INCLUDE_PORT_DMA "ports/stm32f1xx/dma_stm32f1xx.h"
#define _MICROHAL_INCLUDE_PORT_CONFIG "microhalPortConfig_stm32f1xx.h"
#define _MICROHAL_INCLUDE_PORT_IOMANAGER "ports/stm32f1xx/IOManager.h"
#define _MICROHAL_INCLUDE_PORT_GPIO "ports/stm32f1xx/gpio_stm32f1xx.h"
#elif defined(MCU_TYPE_STM32F3XX)
#define _MICROHAL_ACTIVE_PORT_NAMESPACE stm32f3xx
#define _MICROHAL_INCLUDE_PORT_DEVICE "ports/stm32f3xx/device/stm32f3xx.h"
#define _MICROHAL_INCLUDE_PORT_DMA "ports/stm32f3xx/dma_stm32f3xx.h"
#define _MICROHAL_INCLUDE_PORT_CONFIG "microhalPortConfig_stm32f3xx.h"
#define _MICROHAL_INCLUDE_PORT_IOMANAGER "ports/stm32f3xx/IOManager_stm32f3xx.h"
#define _MICROHAL_INCLUDE_PORT_GPIO "ports/stm32f3xx/gpio_stm32f3xx.h"
#elif defined(MCU_TYPE_STM32F4XX)
#define _MICROHAL_ACTIVE_PORT_NAMESPACE stm32f4xx
#define _MICROHAL_INCLUDE_PORT_DEVICE "ports/stm32f4xx/device/stm32f4xx.h"
#define _MICROHAL_INCLUDE_PORT_DMA "ports/stm32f4xx/dma_stm32f4xx.h"
#define _MICROHAL_INCLUDE_PORT_CONFIG "microhalPortConfig_stm32f4xx.h"
#define _MICROHAL_INCLUDE_PORT_IOMANAGER "ports/stm32f4xx/IOManager_stm32f4xx.h"
#define _MICROHAL_INCLUDE_PORT_GPIO "ports/stm32f4xx/gpio_stm32f4xx.h"
#else
#error "MCU type must be specified."
#endif

#endif /* _MICROHAL_PORTS_STMCOMMON_STMCOMMONDEFINES_H_ */
