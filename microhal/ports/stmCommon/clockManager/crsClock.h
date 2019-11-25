/*
 * usbClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CRSCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CRSCLOCK_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "apbClock.h"
#include "clockTypes.h"
#include "hsi.h"

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

#if defined(_MICROHAL_CRS_BASE_ADDRESS)
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
#else
static void enableUSB() {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    apb1enr.CRSEN.set();
    registers::rcc->apb1enr.volatileStore(apb1enr);
}

static void disableUSB() {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    apb1enr.CRSEN.clear();
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
#endif
#endif
}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CRSCLOCK_H_ */
