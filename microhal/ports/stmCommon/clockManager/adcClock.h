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
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();

    if (adcNumber == 1) {
        ahb1enr.ADC12EN.set();
#if defined(ADC2)
    } else if (adcNumber == 2) {
        ahb1enr.ADC12EN.set();
#endif
#if defined(ADC3)
    } else if (adcNumber == 3) {
        ahb1enr.ADC34EN.set();
#endif
    } else {
        std::terminate();
    }

    registers::rcc->ahb1enr.volatileStore(ahb1enr);
}
static void disableADC(uint16_t adcNumber) {
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
}

#endif  // defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
#endif  // defined(_MICROHAL_ADC1_BASE_ADDRESS) || defined(_MICROHAL_ADC2_BASE_ADDRESS) || defined(_MICROHAL_ADC3_BASE_ADDRESS)

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_ADCCLOCK_H_ */
