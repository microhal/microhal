/*
 * rngClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RNGCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RNGCLOCK_H_

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
#if defined(_MICROHAL_RNG_BASE_ADDRESS)
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1

[[maybe_unused]] static void enableRNG(PowerMode mode) {
#if defined(RCC_AHB1ENR_RNGEN)
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    auto ahb1lpenr = registers::rcc->ahb1lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) ahb1enr.RNGEN.set();
    if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.RNGLPEN.set();
    registers::rcc->ahb1lpenr.volatileStore(ahb1lpenr);
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
#else
    auto ahb2enr = registers::rcc->ahb2enr.volatileLoad();
    auto ahb2lpenr = registers::rcc->ahb2lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) ahb2enr.RNGEN.set();
    if (isEnabled(mode, PowerMode::Sleep)) ahb2lpenr.RNGLPEN.set();
    registers::rcc->ahb2lpenr.volatileStore(ahb2lpenr);
    registers::rcc->ahb2enr.volatileStore(ahb2enr);
#endif
}

[[maybe_unused]] static void disableRNG(PowerMode mode) {
#if defined(RCC_AHB1ENR_RNGEN)
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    auto ahb1lpenr = registers::rcc->ahb1lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) Rahb1enr.RNGEN.clear();
    if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.RNGLPEN.clear();
    registers::rcc->ahb1lpenr.volatileStore(ahb1lpenr);
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
#else
    auto ahb2enr = registers::rcc->ahb2enr.volatileLoad();
    auto ahb2lpenr = registers::rcc->ahb2lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) ahb2enr.RNGEN.clear();
    if (isEnabled(mode, PowerMode::Sleep)) ahb2lpenr.RNGLPEN.clear();
    registers::rcc->ahb2lpenr.volatileStore(ahb2lpenr);
    registers::rcc->ahb2enr.volatileStore(ahb2enr);
#endif
}

#else
#endif

#endif

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RNGCLOCK_H_ */
