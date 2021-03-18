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
#include "rcc_register_select.h"

namespace microhal {
namespace ClockManager {

struct HSI {
    /**
     * @brief This function return HSI frequency.
     *
     * @return HSI frequency in [Hz].
     */
    static constexpr Frequency frequency() noexcept {
#if defined(MCU_TYPE_STM32F4XX) || defined(MCU_TYPE_STM32G0XX)
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

#ifdef _MICROHAL_REGISTERS_RCC_HAS_HSITRIM
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
#endif

#ifdef _MICROHAL_REGISTERS_RCC_ICSCR_HAS_HSITRIM
    static bool trim(uint8_t trim) {
        if (trim <= 0b11111) {
            auto cr = registers::rcc->icscr.volatileLoad();
            cr.HSITRIM = trim;
            registers::rcc->icscr.volatileStore(cr);
            return true;
        }
        return false;
    }
    static uint8_t trim() noexcept { return registers::rcc->icscr.volatileLoad().HSITRIM; }
#endif
};

}  // namespace ClockManager
}  // namespace microhal

#endif /* MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_HSI_H_ */
