/*
 * registersBaseAddressDefinitions.h
 *
 *  Created on: Oct 31, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_REGISTERS_REGISTERSBASEADDRESSDEFINITIONS_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_REGISTERS_REGISTERSBASEADDRESSDEFINITIONS_H_

#if defined(STM32F051x8)
#define STM32F0x1
#include "peripheralBaseAddressDefinitions/registersBaseAddressDefinitions_stm32f0x1.h"
#elif defined(STM32F103)
#include "peripheralBaseAddressDefinitions/registersBaseAddressDefinitions_stm32f103.h"
#elif defined(STM32F334x8)
#define STM32F3x4
#include "peripheralBaseAddressDefinitions/registersBaseAddressDefinitions_stm32f3x4.h"
#elif defined(STM32F407xx)
#define STM32F407
#include "peripheralBaseAddressDefinitions/registersBaseAddressDefinitions_stm32f407.h"
#elif defined(STM32F411xE)
#define STM32F411
#include "peripheralBaseAddressDefinitions/registersBaseAddressDefinitions_stm32f411.h"
#else
#error "MCU type have to be specified."
#endif

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_REGISTERS_REGISTERSBASEADDRESSDEFINITIONS_H_ */
