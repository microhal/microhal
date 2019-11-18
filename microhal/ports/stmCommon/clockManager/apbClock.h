/*
 * apbClock.h
 *
 *  Created on: Nov 15, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_APBCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_APBCLOCK_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "ahbClock.h"
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
#include "ports/stm32f4xx/rcc_411.h"
#endif

namespace microhal {
namespace ClockManager {
namespace ClockManagerDetail {
CREATE_SET_CLEAR_FUNCTION(PPRE2)
}  // namespace ClockManagerDetail

static uint32_t apb1Frequency() {
    uint32_t ppre1 = registers::rcc->cfgr.volatileLoad().PPRE1;
    if (ppre1 & 0b100) {
        static constexpr const uint8_t dividers[] = {2, 4, 8, 16};
        return ahbFrequency() / dividers[ppre1 & 0b11];
    }
    return ahbFrequency();
}
static uint32_t apb2Frequency() {
    auto cfgr = registers::rcc->cfgr.volatileLoad();
    uint32_t ppre2 = ClockManagerDetail::get_PPRE2_ifExist(cfgr);

    if (ppre2 & 0b100) {
        static constexpr const uint8_t dividers[] = {2, 4, 8, 16};
        return ahbFrequency() / dividers[ppre2 & 0b11];
    }
    return ahbFrequency();
}
}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_APBCLOCK_H_ */
