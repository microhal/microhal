/*
 * hse.h
 *
 *  Created on: Nov 22, 2019
 *      Author: pokas
 */

#ifndef MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_HSE_H_
#define MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_HSE_H_

#include <cstdint>
#include <exception>
#include "../stmCommonDefines.h"
#include _MICROHAL_INCLUDE_PORT_CONFIG
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
struct HSE {
    /**
     * @brief This function return HSE frequency. This value is defined under HSE_FREQUENCY
     *
     * @return HSE frequency in [Hz]
     */
    static Frequency frequency() noexcept {
        static_assert(externalClockFrequency >= 4000000 && externalClockFrequency <= 26000000,
                      "External HSE frequency out of allowed range. HSE have to be grater than 4MHz and lower than 26MHz.");
        if constexpr (externalClockPresent == false) {
            std::terminate();
        }
        return externalClockFrequency;
    }

    static void enable() noexcept {
        auto cr = registers::rcc->cr.volatileLoad();
        cr.HSEON.set();
        registers::rcc->cr.volatileStore(cr);
    }
    static void disable() noexcept {
        auto cr = registers::rcc->cr.volatileLoad();
        cr.HSEON.clear();
        registers::rcc->cr.volatileStore(cr);
    }
    static bool isReady() noexcept { return registers::rcc->cr.volatileLoad().HSERDY; }
    static bool isEnabled() noexcept { return registers::rcc->cr.volatileLoad().HSEON; }
    static void enableBypas() noexcept {
        auto cr = registers::rcc->cr.volatileLoad();
        cr.HSEBYP.set();
        registers::rcc->cr.volatileStore(cr);
    }
    static void disableBypas() noexcept {
        auto cr = registers::rcc->cr.volatileLoad();
        cr.HSEBYP.clear();
        registers::rcc->cr.volatileStore(cr);
    }
};

}  // namespace ClockManager
}  // namespace microhal
#endif /* MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_HSE_H_ */
