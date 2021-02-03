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
