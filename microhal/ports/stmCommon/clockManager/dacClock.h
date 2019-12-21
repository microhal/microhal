/*
 * dacClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_DACCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_DACCLOCK_H_

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

#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1

#if defined(_MICROHAL_DAC_BASE_ADDRESS)
static void enableDAC(PowerMode mode) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) apb1enr.DACEN.set();
    if (isEnabled(mode, PowerMode::Sleep)) apb1lpenr.DACLPEN.set();
    registers::rcc->apb1lpenr.volatileStore(apb1lpenr);
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
static void disableDAC(PowerMode mode) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) apb1enr.DACEN.clear();
    if (isEnabled(mode, PowerMode::Sleep)) apb1lpenr.DACLPEN.clear();
    registers::rcc->apb1lpenr.volatileStore(apb1lpenr);
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
#endif

#else

[[maybe_unused]] static void enable(uint16_t dacNumber) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
#if defined(_MICROHAL_DAC1_BASE_ADDRESS)
    if (dacNumber == 1) {
        apb1enr.DAC1EN.set();
    } else
#endif
#if defined(_MICROHAL_DAC2_BASE_ADDRESS)
        if (dacNumber == 2) {
        apb1enr.DAC2EN.set();
    } else
#endif
    {
        std::terminate();  // critical error, should never go here
    }
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
#endif

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_DACCLOCK_H_ */
