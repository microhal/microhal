/*
 * crypClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CRYPCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CRYPCLOCK_H_

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

#if defined(_MICROHAL_CRYP_BASE_ADDRESS)
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
static void enableCRYP(PowerMode mode) {
    auto ahb2enr = registers::rcc->ahb2enr.volatileLoad();
    auto ahb2lpenr = registers::rcc->ahb2lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) ahb2enr.CRYPEN.set();
    if (isEnabled(mode, PowerMode::Sleep)) ahb2lpenr.CRYPLPEN.set();
    registers::rcc->ahb2lpenr.volatileStore(ahb2lpenr);
    registers::rcc->ahb2enr.volatileStore(ahb2enr);
}
static void disableCRYP(PowerMode mode) {
    auto ahb2enr = registers::rcc->ahb2enr.volatileLoad();
    auto ahb2lpenr = registers::rcc->ahb2lpenr.volatileLoad();
    if (isEnabled(mode, PowerMode::Normal)) ahb2enr.CRYPEN.clear();
    if (isEnabled(mode, PowerMode::Sleep)) ahb2lpenr.CRYPLPEN.clear();
    registers::rcc->ahb2lpenr.volatileStore(ahb2lpenr);
    registers::rcc->ahb2enr.volatileStore(ahb2enr);
}

#else
#endif
#endif

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CRYPCLOCK_H_ */
