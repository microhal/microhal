/*
 * registersBaseAddressDefinitions.h
 *
 *  Created on: Oct 31, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_REGISTERS_REGISTERSBASEADDRESSDEFINITIONS_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_REGISTERS_REGISTERSBASEADDRESSDEFINITIONS_H_

#if defined(STM32F407xx)
#define STM32F407
#include "peripheralBaseAddressDefinitions/registersBaseAddressDefinitions_stm32f407.h"
//#define _MICROHAL_CAN1_BASE 0x40006400  // size 0x400
//#define _MICROHAL_CAN2_BASE 0x40006800  // size 0x400

//#define _MICROHAL_I2C1_BASE 1
//#define _MICROHAL_I2C2_BASE 2
#endif

#if defined(STM32F334x8)
#define STM32F3x4
#include "peripheralBaseAddressDefinitions/registersBaseAddressDefinitions_stm32f3x4.h"

//#define _MICROHAL_CAN_BASE 0x40006400  // size 0x400
//
//#define _MICROHAL_ADC1_BASE 0x50000000       // size 0xB9
//#define _MICROHAL_ADC2_BASE 0x50000100       // size 0xB9
//#define _MICROHAL_ADCCOMMON_BASE 0x50000300  // size 0x11
#endif

#if defined(STM32F103)
#include "peripheralBaseAddressDefinitions/registersBaseAddressDefinitions_stm32f103.h"
#endif

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_REGISTERS_REGISTERSBASEADDRESSDEFINITIONS_H_ */
