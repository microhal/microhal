/*
 * flashClock.h
 *
 *  Created on: Feb 27, 2021
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_FLASHCLOCK_H_
#define _MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_FLASHCLOCK_H_

#include "rcc_register_select.h"

namespace microhal {
namespace ClockManager {

inline void enableFlash() {
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    ahb1enr.FLASHEN.set();
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
}

inline void disableFlash() {
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    ahb1enr.FLASHEN.clear();
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
}

}  // namespace ClockManager
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_FLASHCLOCK_H_ */
