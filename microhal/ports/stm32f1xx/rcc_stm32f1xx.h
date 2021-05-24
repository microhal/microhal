/*
 * rcc_stm32f1xx.h
 *
 *  Created on: May 21, 2021
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_PORTS_STM32F1XX_RCC_STM32F1XX_H_
#define SRC_THIRD_PARTY_MICROHAL_PORTS_STM32F1XX_RCC_STM32F1XX_H_

#include "mcuDefines.h"

#if defined(_MICROHAL_STM32F1XX_STM32F103xx)
#include "rcc/rcc_stm32f103.h"
#elif defined(_MICROHAL_STM32F1XX_STM32F107xx)
#include "rcc/rcc_stm32f107.h"
#endif

#endif /* SRC_THIRD_PARTY_MICROHAL_PORTS_STM32F1XX_RCC_STM32F1XX_H_ */
