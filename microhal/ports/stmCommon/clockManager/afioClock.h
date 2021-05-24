/*
 * afioClock.h
 *
 *  Created on: Dec 11, 2020
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_AFIOCLOCK_H_
#define SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_AFIOCLOCK_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "apbClock.h"
#include "clockTypes.h"
#include "rcc_register_select.h"

namespace microhal {
namespace ClockManager {

inline void enableAFIO() {
    auto apb2 = registers::rcc->apb2enr.volatileLoad();
    apb2.AFIOEN.set();
    registers::rcc->apb2enr.volatileStore(apb2);
}

inline void disableAFIO() {
    auto apb2 = registers::rcc->apb2enr.volatileLoad();
    apb2.AFIOEN.clear();
    registers::rcc->apb2enr.volatileStore(apb2);
}

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_AFIOCLOCK_H_ */
