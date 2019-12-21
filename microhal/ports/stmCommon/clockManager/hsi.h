/*
 * hsi.h
 *
 *  Created on: Nov 22, 2019
 *      Author: pokas
 */

#ifndef MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_HSI_H_
#define MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_HSI_H_

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

struct HSI {
    /**
     * @brief This function return HSI frequency.
     *
     * @return HSI frequency in [Hz].
     */
    static constexpr Frequency frequency() noexcept {
#if defined(MCU_TYPE_STM32F4XX)
        return 16000000;
#else
        return 8000000;
#endif
    }

    static void enable() noexcept {
        auto cr = registers::rcc->cr.volatileLoad();
        cr.HSION.set();
        registers::rcc->cr.volatileStore(cr);
    }

    static void disable() noexcept {
        auto cr = registers::rcc->cr.volatileLoad();
        cr.HSION.clear();
        registers::rcc->cr.volatileStore(cr);
    }

    static bool isEnabled() noexcept { return registers::rcc->cr.volatileLoad().HSION; }
    static bool isReady() noexcept { return registers::rcc->cr.volatileLoad().HSIRDY; }

    static bool trim(uint8_t trim) {
        if (trim <= 0b11111) {
            auto cr = registers::rcc->cr.volatileLoad();
            cr.HSITRIM = trim;
            registers::rcc->cr.volatileStore(cr);
            return true;
        }
        return false;
    }
    static uint8_t trim() noexcept { return registers::rcc->cr.volatileLoad().HSITRIM; }
};

}  // namespace ClockManager
}  // namespace microhal

#endif /* MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_HSI_H_ */
