/*
 * mcu_description.h
 *
 *  Created on: Dec 9, 2020
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_PORTS_STM32F1XX_MCUCAPABILITIES_H_
#define SRC_THIRD_PARTY_MICROHAL_PORTS_STM32F1XX_MCUCAPABILITIES_H_

#include <cstddef>
#include <cstdint>
#include "mcuDefines.h"

#ifdef _MICROHAL_STM32F1XX_STM32F100xx
constexpr const uint32_t cpuMaxFrequency = 24000000;
#elif defined(_MICROHAL_STM32F1XX_STM32F101xx)
constexpr const uint32_t cpuMaxFrequency = 36000000;
#elif defined(_MICROHAL_STM32F1XX_STM32F102xx)
constexpr const uint32_t cpuMaxFrequency = 48000000;
#elif defined(_MICROHAL_STM32F1XX_STM32F103xx) || defined(_MICROHAL_STM32F1XX_STM32F105xx) || defined(_MICROHAL_STM32F1XX_STM32F107xx)
constexpr const uint32_t cpuMaxFrequency = 72000000;
#else
#error MCU type has to be defined
#endif

constexpr size_t flashSize = 768;

#if defined(_MICROHAL_STM32F1XX_MEDIUM_DENSITY_PERFORMANCE_LINE)  // STM32F103x8 STM32F103xB

constexpr size_t ramSize = 20 * 1024;

#define _MICROHAL_STM32F1XX_HAS_ADC1 1
#define _MICROHAL_STM32F1XX_HAS_ADC2 1
#define _MICROHAL_STM32F1XX_HAS_ADC3 0

#define _MICROHAL_STM32F1XX_HAS_CAN1 1
#define _MICROHAL_STM32F1XX_HAS_CAN2 0

#define _MICROHAL_STM32F1XX_HAS_I2C1 1
#if defined(_MICROHAL_STM32F1XX_STM32F103Cx) || defined(_MICROHAL_STM32F1XX_STM32F103Rx) || defined(_MICROHAL_STM32F1XX_STM32F103Vx)
#define _MICROHAL_STM32F1XX_HAS_I2C2 1
#else
#define _MICROHAL_STM32F1XX_HAS_I2C2 0
#endif

#define _MICROHAL_STM32F1XX_HAS_USART1 1
#define _MICROHAL_STM32F1XX_HAS_USART2 1
#if defined(_MICROHAL_STM32F1XX_STM32F103Cx) || defined(_MICROHAL_STM32F1XX_STM32F103Rx) || defined(_MICROHAL_STM32F1XX_STM32F103Vx)
#define _MICROHAL_STM32F1XX_HAS_USART3 1
#else
#define _MICROHAL_STM32F1XX_HAS_USART3 0
#endif
#define _MICROHAL_STM32F1XX_HAS_USART4 0

#define _MICROHAL_STM32F1XX_HAS_USB1 1

#define _MICROHAL_STM32F1XX_HAS_SPI1 1
#if defined(_MICROHAL_STM32F1XX_STM32F103Cx) || defined(_MICROHAL_STM32F1XX_STM32F103Rx) || defined(_MICROHAL_STM32F1XX_STM32F103Vx)
#define _MICROHAL_STM32F1XX_HAS_SPI2 1
#else
#define _MICROHAL_STM32F1XX_HAS_SPI2 0
#endif

#define _MICROHAL_STM32F1XX_HAS_TIMER1 1  // Advanced-control
#define _MICROHAL_STM32F1XX_HAS_TIMER2 1
#define _MICROHAL_STM32F1XX_HAS_TIMER3 1
#define _MICROHAL_STM32F1XX_HAS_TIMER4 1

#endif

#if defined(_MICROHAL_STM32F1XX_HIGH_DENSITY_PERFORMANCE_LINE)  // STM32F103xC, STM32F103xD, STM32F103xE

constexpr const size_t ramSize = 64 * 1024;

#define _MICROHAL_STM32F1XX_HAS_ADC1 1
#define _MICROHAL_STM32F1XX_HAS_ADC2 1
#define _MICROHAL_STM32F1XX_HAS_ADC3 1

#define _MICROHAL_STM32F1XX_HAS_CAN1 1
#define _MICROHAL_STM32F1XX_HAS_CAN2 0

#define _MICROHAL_STM32F1XX_HAS_DAC1 1
#define _MICROHAL_STM32F1XX_HAS_DAC2 1

#if defined(_MICROHAL_STM32F1XX_STM32F103Vx) || defined(_MICROHAL_STM32F1XX_STM32F103Zx)
#define _MICROHAL_STM32F1XX_HAS_FSMC 1
#else
#define _MICROHAL_STM32F1XX_HAS_FSMC 0
#endif

#define _MICROHAL_STM32F1XX_HAS_I2C1 1
#define _MICROHAL_STM32F1XX_HAS_I2C2 1

#define _MICROHAL_STM32F1XX_HAS_USART1 1
#define _MICROHAL_STM32F1XX_HAS_USART2 1
#define _MICROHAL_STM32F1XX_HAS_USART3 1
#define _MICROHAL_STM32F1XX_HAS_USART4 1
#define _MICROHAL_STM32F1XX_HAS_USART5 1

#define _MICROHAL_STM32F1XX_HAS_USB1 1

#define _MICROHAL_STM32F1XX_HAS_SDIO1 1

#define _MICROHAL_STM32F1XX_HAS_SPI1 1
#define _MICROHAL_STM32F1XX_HAS_SPI2 1
#define _MICROHAL_STM32F1XX_HAS_SPI3 1

#define _MICROHAL_STM32F1XX_HAS_TIMER1 1  // Advanced-control
#define _MICROHAL_STM32F1XX_HAS_TIMER2 1
#define _MICROHAL_STM32F1XX_HAS_TIMER3 1
#define _MICROHAL_STM32F1XX_HAS_TIMER4 1
#define _MICROHAL_STM32F1XX_HAS_TIMER5 1
#define _MICROHAL_STM32F1XX_HAS_TIMER6 1  // Basic
#define _MICROHAL_STM32F1XX_HAS_TIMER7 1  // Basic
#define _MICROHAL_STM32F1XX_HAS_TIMER8 1  // Advanced-control
#define _MICROHAL_STM32F1XX_HAS_TIMER9 0
#define _MICROHAL_STM32F1XX_HAS_TIMER10 0
#define _MICROHAL_STM32F1XX_HAS_TIMER11 0
#define _MICROHAL_STM32F1XX_HAS_TIMER12 0
#define _MICROHAL_STM32F1XX_HAS_TIMER13 0
#define _MICROHAL_STM32F1XX_HAS_TIMER14 0

#endif

#if defined(_MICROHAL_STM32F1XX_XL_DENSITY_PERFORMANCE_LINE)  // STM32F103xF STM32F103xG

constexpr size_t ramSize = 96 * 1024;

#define _MICROHAL_STM32F1XX_HAS_ADC1 1
#define _MICROHAL_STM32F1XX_HAS_ADC2 1
#define _MICROHAL_STM32F1XX_HAS_ADC3 1

#define _MICROHAL_STM32F1XX_HAS_CAN1 1

#define _MICROHAL_STM32F1XX_HAS_DAC1 1
#define _MICROHAL_STM32F1XX_HAS_DAC2 1

#if defined(_MICROHAL_STM32F1XX_STM32F103Vx) || defined(_MICROHAL_STM32F1XX_STM32F103Zx)
#define _MICROHAL_STM32F1XX_HAS_FSMC 1
#else
#define _MICROHAL_STM32F1XX_HAS_FSMC 0
#endif

#define _MICROHAL_STM32F1XX_HAS_I2C1 1
#define _MICROHAL_STM32F1XX_HAS_I2C2 1

#define _MICROHAL_STM32F1XX_HAS_USART1 1
#define _MICROHAL_STM32F1XX_HAS_USART2 1
#define _MICROHAL_STM32F1XX_HAS_USART3 1
#define _MICROHAL_STM32F1XX_HAS_USART4 1
#define _MICROHAL_STM32F1XX_HAS_USART5 1

#define _MICROHAL_STM32F1XX_HAS_USB1 1
#define _MICROHAL_STM32F1XX_HAS_USB_OTG1 0

#define _MICROHAL_STM32F1XX_HAS_SDIO1 1

#define _MICROHAL_STM32F1XX_HAS_SPI1 1
#define _MICROHAL_STM32F1XX_HAS_SPI2 1
#define _MICROHAL_STM32F1XX_HAS_SPI3 1

#define _MICROHAL_STM32F1XX_HAS_TIMER1 1  // Advanced-control
#define _MICROHAL_STM32F1XX_HAS_TIMER2 1
#define _MICROHAL_STM32F1XX_HAS_TIMER3 1
#define _MICROHAL_STM32F1XX_HAS_TIMER4 1
#define _MICROHAL_STM32F1XX_HAS_TIMER5 1
#define _MICROHAL_STM32F1XX_HAS_TIMER6 1  // Basic
#define _MICROHAL_STM32F1XX_HAS_TIMER7 1  // Basic
#define _MICROHAL_STM32F1XX_HAS_TIMER8 1  // Advanced-control
#define _MICROHAL_STM32F1XX_HAS_TIMER9 1
#define _MICROHAL_STM32F1XX_HAS_TIMER10 1
#define _MICROHAL_STM32F1XX_HAS_TIMER11 1
#define _MICROHAL_STM32F1XX_HAS_TIMER12 1
#define _MICROHAL_STM32F1XX_HAS_TIMER13 1
#define _MICROHAL_STM32F1XX_HAS_TIMER14 1

#endif

#if defined(_MICROHAL_STM32F1XX_CONNECTIVITY_LINE)  // STM32F105xx STM32F107xx

constexpr size_t ramSize = 64 * 1024;

#define _MICROHAL_STM32F1XX_HAS_ADC1 1
#define _MICROHAL_STM32F1XX_HAS_ADC2 1
#define _MICROHAL_STM32F1XX_HAS_ADC3 0

#define _MICROHAL_STM32F1XX_HAS_CAN1 1
#define _MICROHAL_STM32F1XX_HAS_CAN2 1

#define _MICROHAL_STM32F1XX_HAS_DAC1 1
#define _MICROHAL_STM32F1XX_HAS_DAC2 1

#define _MICROHAL_STM32F1XX_HAS_FSMC 0

#define _MICROHAL_STM32F1XX_HAS_I2C1 1
#if defined(_MICROHAL_STM32F1XX_STM32F105Rx) || defined(_MICROHAL_STM32F1XX_STM32F105Vx)
#define _MICROHAL_STM32F1XX_HAS_I2C2 1
#else
#define _MICROHAL_STM32F1XX_HAS_I2C2 0
#endif

#define _MICROHAL_STM32F1XX_HAS_USART1 1
#define _MICROHAL_STM32F1XX_HAS_USART2 1
#define _MICROHAL_STM32F1XX_HAS_USART3 1
#define _MICROHAL_STM32F1XX_HAS_USART4 1
#define _MICROHAL_STM32F1XX_HAS_USART5 1

#define _MICROHAL_STM32F1XX_HAS_USB1 0
#define _MICROHAL_STM32F1XX_HAS_USB_OTG1 1

#define _MICROHAL_STM32F1XX_HAS_SDIO1 0

#define _MICROHAL_STM32F1XX_HAS_SPI1 1
#define _MICROHAL_STM32F1XX_HAS_SPI2 1
#define _MICROHAL_STM32F1XX_HAS_SPI3 1

#define _MICROHAL_STM32F1XX_HAS_TIMER1 1  // Advanced-control
#define _MICROHAL_STM32F1XX_HAS_TIMER2 1
#define _MICROHAL_STM32F1XX_HAS_TIMER3 1
#define _MICROHAL_STM32F1XX_HAS_TIMER4 1
#define _MICROHAL_STM32F1XX_HAS_TIMER5 1
#define _MICROHAL_STM32F1XX_HAS_TIMER6 1  // Basic
#define _MICROHAL_STM32F1XX_HAS_TIMER7 1  // Basic
#define _MICROHAL_STM32F1XX_HAS_TIMER8 0  // Advanced-control
#define _MICROHAL_STM32F1XX_HAS_TIMER9 0
#define _MICROHAL_STM32F1XX_HAS_TIMER10 0
#define _MICROHAL_STM32F1XX_HAS_TIMER11 0
#define _MICROHAL_STM32F1XX_HAS_TIMER12 0
#define _MICROHAL_STM32F1XX_HAS_TIMER13 0
#define _MICROHAL_STM32F1XX_HAS_TIMER14 0

#endif

#endif /* SRC_THIRD_PARTY_MICROHAL_PORTS_STM32F1XX_MCUCAPABILITIES_H_ */
