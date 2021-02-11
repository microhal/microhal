/*
 * syscfgClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_SYSCFGCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_SYSCFGCLOCK_H_

#include <cstdint>
#include <exception>
#include "apbClock.h"
#include "clockTypes.h"
#include "rcc_register_select.h"

namespace microhal {
namespace ClockManager {

#if defined(_MICROHAL_SYSCFG_BASE_ADDRESS) || defined(_MICROHAL_SYSCFG_COMP_OPAMP_BASE_ADDRESS) || defined(_MICROHAL_SYSCFG_COMP_BASE_ADDRESS) || \
    defined(_MICROHAL_SYSCFG_VREFBUF_BASE_ADDRESS)
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1

static void enableSYSCFG(PowerMode mode) {
    auto apb2enr = registers::rcc->apb2enr.volatileLoad();
    auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) apb2enr.SYSCFGEN.set();
    if (isEnabled(mode, PowerMode::Sleep)) apb2lpenr.SYSCFGLPEN.set();
    registers::rcc->apb2lpenr.volatileStore(apb2lpenr);
    registers::rcc->apb2enr.volatileStore(apb2enr);
}
static void disableSYSCFG(PowerMode mode) {
    auto apb2enr = registers::rcc->apb2enr.volatileLoad();
    auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) apb2enr.SYSCFGEN.clear();
    if (isEnabled(mode, PowerMode::Sleep)) apb2lpenr.SYSCFGLPEN.clear();
    registers::rcc->apb2lpenr.volatileStore(apb2lpenr);
    registers::rcc->apb2enr.volatileStore(apb2enr);
}

#else
inline void enableSYSCFG() {
    auto apb2enr = registers::rcc->apb2enr.volatileLoad();
    apb2enr.SYSCFGEN.set();
    registers::rcc->apb2enr.volatileStore(apb2enr);
}
inline void disableSYSCFG() {
    auto apb2enr = registers::rcc->apb2enr.volatileLoad();
    apb2enr.SYSCFGEN.clear();
    registers::rcc->apb2enr.volatileStore(apb2enr);
}
#endif  // defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
#endif  // defined(_MICROHAL_SYSCFG_BASE_ADDRESS)

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_SYSCFGCLOCK_H_ */
