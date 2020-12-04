/*
 * adcClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_ADCCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_ADCCLOCK_H_

#include <cstdint>
#include <exception>
#include "apbClock.h"
#include "clockTypes.h"

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
#endif

namespace microhal {
namespace ClockManager {

#if defined(_MICROHAL_ADC1_BASE_ADDRESS) || defined(_MICROHAL_ADC2_BASE_ADDRESS) || defined(_MICROHAL_ADC3_BASE_ADDRESS)
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1

static void enableADC(uint16_t adcNumber, PowerMode mode) {
    auto apb2enr = registers::rcc->apb2enr.volatileLoad();
    auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
    if (adcNumber == 1) {
        if (isEnabled(mode, PowerMode::Normal)) apb2enr.ADC1EN.set();
        if (isEnabled(mode, PowerMode::Sleep)) apb2lpenr.ADC1LPEN.set();
#if defined(ADC2)
    } else if (adcNumber == 2) {
        if (isEnabled(mode, PowerMode::Normal)) apb2enr.ADC2EN.set();
        if (isEnabled(mode, PowerMode::Sleep)) apb2lpenr.ADC2LPEN.set();
#endif
#if defined(ADC3)
    } else if (adcNumber == 3) {
        if (isEnabled(mode, PowerMode::Normal)) apb2enr.ADC3EN.set();
        if (isEnabled(mode, PowerMode::Sleep)) apb2lpenr.ADC3LPEN.set();
#endif
    } else {
        std::terminate();
    }
    registers::rcc->apb2lpenr.volatileStore(apb2lpenr);
    registers::rcc->apb2enr.volatileStore(apb2enr);
}
static void disableADC(uint16_t adcNumber, PowerMode mode) {
    auto apb2enr = registers::rcc->apb2enr.volatileLoad();
    auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
    if (adcNumber == 1) {
        if (isEnabled(mode, PowerMode::Normal)) apb2enr.ADC1EN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) apb2lpenr.ADC1LPEN.clear();
#if defined(ADC2)
    } else if (adcNumber == 2) {
        if (isEnabled(mode, PowerMode::Normal)) apb2enr.ADC2EN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) apb2lpenr.ADC2LPEN.clear();
#endif
#if defined(ADC3)
    } else if (adcNumber == 3) {
        if (isEnabled(mode, PowerMode::Normal)) apb2enr.ADC3EN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) apb2lpenr.ADC3LPEN.clear();
#endif
    } else {
        std::terminate();
    }
    registers::rcc->apb2lpenr.volatileStore(apb2lpenr);
    registers::rcc->apb2enr.volatileStore(apb2enr);
}

#else

static void enableADC(uint16_t adcNumber) {
#if defined(_MICROHAL_REGISTERS_RCC_AHB1ENR_HAS_ADC12EN) || defined(_MICROHAL_REGISTERS_RCC_AHB1ENR_HAS_ADC34EN)
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();

    if (adcNumber == 1) {
        ahb1enr.ADC12EN.set();
#if defined(ADC2)
    } else if (adcNumber == 2) {
        ahb1enr.ADC12EN.set();
#endif  // defined(ADC2)
#if defined(ADC3)
    } else if (adcNumber == 3) {
        ahb1enr.ADC34EN.set();
#endif  // defined(ADC3)
    } else {
        std::terminate();
    }

    registers::rcc->ahb1enr.volatileStore(ahb1enr);
#endif  // defined(_MICROHAL_REGISTERS_RCC_AHB1ENR_HAS_ADC12EN) || defined(_MICROHAL_REGISTERS_RCC_AHB1ENR_HAS_ADC34EN)

#if defined(_MICROHAL_REGISTERS_RCC_APB2ENR_HAS_ADC1EN) || defined(_MICROHAL_REGISTERS_RCC_APB2ENR_HAS_ADC2EN) || \
    defined(_MICROHAL_REGISTERS_RCC_APB2ENR_HAS_ADC3EN)
    auto apb2enr = registers::rcc->apb2enr.volatileLoad();
    if (adcNumber == 1) {
        apb2enr.ADC1EN.set();
    } else if (adcNumber == 2) {
        apb2enr.ADC2EN.set();
    } else if (adcNumber == 3) {
        apb2enr.ADC3EN.set();
    } else {
        std::terminate();
    }
    registers::rcc->apb2enr.volatileStore(apb2enr);
#endif
}
static void disableADC(uint16_t adcNumber) {
#if defined(_MICROHAL_REGISTERS_RCC_AHB1ENR_HAS_ADC12EN) || defined(_MICROHAL_REGISTERS_RCC_AHB1ENR_HAS_ADC34EN)
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();

    if (adcNumber == 1) {
        ahb1enr.ADC12EN.clear();
#if defined(ADC2)
    } else if (adcNumber == 2) {
        ahb1enr.ADC12EN.clear();
#endif
#if defined(ADC3)
    } else if (adcNumber == 3) {
        ahb1enr.ADC34EN.clear();
#endif
    } else {
        std::terminate();
    }

    registers::rcc->ahb1enr.volatileStore(ahb1enr);
#endif

#if defined(_MICROHAL_REGISTERS_RCC_APB2ENR_HAS_ADC1EN) || defined(_MICROHAL_REGISTERS_RCC_APB2ENR_HAS_ADC2EN) || \
    defined(_MICROHAL_REGISTERS_RCC_APB2ENR_HAS_ADC3EN)
    auto apb2enr = registers::rcc->apb2enr.volatileLoad();
    if (adcNumber == 1) {
        apb2enr.ADC1EN.clear();
    } else if (adcNumber == 2) {
        apb2enr.ADC2EN.clear();
    } else if (adcNumber == 3) {
        apb2enr.ADC3EN.clear();
    } else {
        std::terminate();
    }
    registers::rcc->apb2enr.volatileStore(apb2enr);
#endif
}

[[maybe_unused]] inline bool ADCprescaler(uint_fast8_t prescaler) {
    auto cfgr = registers::rcc->cfgr.volatileLoad();
    switch (prescaler) {
        case 2:
            cfgr.ADCPRE = 0b00;
            break;
        case 4:
            cfgr.ADCPRE = 0b01;
            break;
        case 6:
            cfgr.ADCPRE = 0b10;
            break;
        case 8:
            cfgr.ADCPRE = 0b11;
            break;
        default:
            return false;
    }
    registers::rcc->cfgr.volatileStore(cfgr);
    return true;
}

[[maybe_unused]] inline uint_fast8_t ADCprescaler() {
    auto cfgr = registers::rcc->cfgr.volatileLoad();
    switch (cfgr.ADCPRE) {
        case 0b00:
            return 2;
        case 0b01:
            return 4;
        case 0b10:
            return 6;
        case 0b11:
            return 8;
    }
    std::terminate();
}

[[maybe_unused]] inline uint32_t ADCFrequency(uint8_t number) {
    return APB2::frequency() / ADCprescaler();
}

#endif  // defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
#endif  // defined(_MICROHAL_ADC1_BASE_ADDRESS) || defined(_MICROHAL_ADC2_BASE_ADDRESS) || defined(_MICROHAL_ADC3_BASE_ADDRESS)

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_ADCCLOCK_H_ */
