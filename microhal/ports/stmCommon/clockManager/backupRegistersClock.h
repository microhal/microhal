#ifndef _MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_BACKUPREGISTERS_H_
#define _MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_BACKUPREGISTERS_H_

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
