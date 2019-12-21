/*
 * fmcClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_FMCCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_FMCCLOCK_H_

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

#if defined(_MICROHAL_FMC_BASE_ADDRESS)
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
static void enableFMC(PowerMode mode) {
    auto ahb3enr = registers::rcc->ahb3enr.volatileLoad();
    auto ahb3lpenr = registers::rcc->ahb3lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) ahb3enr.FMCEN.set();
    if (isEnabled(mode, PowerMode::Sleep)) ahb3lpenr.FMCLPEN.set();
    registers::rcc->ahb3lpenr.volatileStore(ahb3lpenr);
    registers::rcc->ahb3enr.volatileStore(ahb3enr);
}

static void disableFMC(PowerMode mode) {
    auto ahb3enr = registers::rcc->ahb3enr.volatileLoad();
    auto ahb3lpenr = registers::rcc->ahb3lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) ahb3enr.FMCEN.clear();
    if (isEnabled(mode, PowerMode::Sleep)) ahb3lpenr.FMCLPEN.clear();
    registers::rcc->ahb3lpenr.volatileStore(ahb3lpenr);
    registers::rcc->ahb3enr.volatileStore(ahb3enr);
}
#else
#endif
#endif

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_FMCCLOCK_H_ */
