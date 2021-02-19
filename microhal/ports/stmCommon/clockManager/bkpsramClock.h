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
#include "rcc_register_select.h"

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
