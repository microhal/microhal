/*
 * rcc_register_select.h
 *
 *  Created on: Feb 2, 2021
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RCC_REGISTER_SELECT_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RCC_REGISTER_SELECT_H_

#ifdef MCU_TYPE_STM32F0XX
#include "ports/stm32f0xx/RCC_2.h"
#endif
#ifdef MCU_TYPE_STM32F1XX
#include "ports/stm32f1xx/rcc_stm32f103.h"
#endif
#ifdef MCU_TYPE_STM32F3XX
#include "ports/stm32f3xx/rcc_3x4.h"
#endif
#ifdef MCU_TYPE_STM32F4XX
#ifdef STM32F411xE
#include "ports/stm32f4xx/rcc_411.h"
#else
#include "ports/stm32f4xx/rcc_407.h"
#endif
#ifdef MCU_TYPE_STM32G0XX
#include "ports/stm32g0xx/RCC_1.h"
#endif
#endif

#ifdef MCU_TYPE_STM32G0XX
#include "ports/stm32g0xx/RCC_4.h"
#endif

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RCC_REGISTER_SELECT_H_ */
