/*
 * hsi48.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_HSI48_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_HSI48_H_

#include <cstdint>
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
struct HSI48 {
    /**
     * @brief This function return HSI48 frequency.
     *
     * @return HSI frequency in [Hz].
     */
    static constexpr Frequency frequency() noexcept { return 48000000; }

    static void enable() noexcept {
        auto cr2 = registers::rcc->cr2.volatileLoad();
        cr2.HSI48ON.set();
        registers::rcc->cr2.volatileStore(cr2);
    }
    static void disable() noexcept {
        auto cr2 = registers::rcc->cr2.volatileLoad();
        cr2.HSI48ON.clear();
        registers::rcc->cr2.volatileStore(cr2);
    }
    static bool isReady() noexcept { return registers::rcc->cr2.volatileLoad().HSI48RDY; }
};

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_HSI48_H_ */
