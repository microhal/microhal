/*
 * ahbClock.h
 *
 *  Created on: Nov 15, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_AHBCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_AHBCLOCK_H_
#include <cstdint>
#include <exception>
#include <type_traits>

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
#include "ports/stm32f4xx/rcc_411.h"
#endif

namespace microhal {
namespace ClockManager {
namespace ClockManagerDetail {}

static uint32_t sysclkFrequency() {
    return 8'000'000;
}

static uint32_t ahbFrequency() {
    uint32_t hpre = registers::rcc->cfgr.volatileLoad().HPRE;
    if (hpre & 0b1000) {
        static constexpr const uint16_t dividers[] = {2, 4, 8, 16, 64, 128, 256, 512};
        return sysclkFrequency() / dividers[hpre & 0b111];
    }
    return sysclkFrequency();
}

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_AHBCLOCK_H_ */
