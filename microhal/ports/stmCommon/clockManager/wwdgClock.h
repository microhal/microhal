/*
 * wwdgClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_WWDGCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_WWDGCLOCK_H_

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

#if defined(_MICROHAL_WWDG_BASE_ADDRESS)
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1

static void enableWWDG(PowerMode mode) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) apb1enr.WWDGEN.set();
    if (isEnabled(mode, PowerMode::Sleep)) apb1lpenr.WWDGLPEN.set();
    registers::rcc->apb1lpenr.volatileStore(apb1lpenr);
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
static void disableWWDG(PowerMode mode) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) apb1enr.WWDGEN.clear();
    if (isEnabled(mode, PowerMode::Sleep)) apb1lpenr.WWDGLPEN.clear();
    registers::rcc->apb1lpenr.volatileStore(apb1lpenr);
    registers::rcc->apb1enr.volatileStore(apb1enr);
}

#else

#endif  // defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
#endif  // defined(_MICROHAL_WWDG_BASE_ADDRESS)

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_WWDGCLOCK_H_ */
