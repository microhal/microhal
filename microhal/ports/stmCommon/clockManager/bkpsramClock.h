/*
 * bkpsramClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_BKPSRAMCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_BKPSRAMCLOCK_H_

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

[[maybe_unused]] static void enableBKPSRAM(PowerMode mode) {
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    auto ahb1lpenr = registers::rcc->ahb1lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) ahb1enr.BKPSRAMEN.set();
    if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.BKPSRAMLPEN.set();
    registers::rcc->ahb1lpenr.volatileStore(ahb1lpenr);
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
}
[[maybe_unused]] static void disableBKSPRAM(PowerMode mode) {
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    auto ahb1lpenr = registers::rcc->ahb1lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) ahb1enr.BKPSRAMEN.clear();
    if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.BKPSRAMLPEN.clear();
    registers::rcc->ahb1lpenr.volatileStore(ahb1lpenr);
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
}
#else
#endif

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_BKPSRAMCLOCK_H_ */
