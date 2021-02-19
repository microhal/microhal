#ifndef _MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_BACKUPREGISTERS_H_
#define _MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_BACKUPREGISTERS_H_

#include <cstdint>
#include <exception>
#include "apbClock.h"
#include "clockTypes.h"
#include "rcc_register_select.h"

namespace microhal {
namespace ClockManager {

#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1

#else
inline void enableBackupRegisters() {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    apb1enr.BKPEN.set();
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
inline void disableBackupRegisters() {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    apb1enr.BKPEN.clear();
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
#endif  // defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1

}  // namespace ClockManager
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_BACKUPREGISTERS_H_ */
