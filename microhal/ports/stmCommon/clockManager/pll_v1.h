/*
 * pll.h
 *
 *  Created on: Nov 22, 2019
 *      Author: pokas
 */

#ifndef MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_PLL_H_
#define MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_PLL_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "apbClock.h"
#include "clockTypes.h"
#include "rcc_register_select.h"

namespace microhal {
namespace ClockManager {

struct PLL {
    enum class ClockSource : uint32_t {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
        HSI = 0,
#else
        HSIDiv2 = 0,
#define _MICROHAL_PLL_CLOCKSOURCE_HAS_HSIDiv2
#endif
#ifdef _MICROHAL_REGISTERS_RCC_CRGR2_HAS_PREDIV1SRC
#define _MICROHAL_PLL_CLOCKSOURCE_HAS_HSEDiv
#define _MICROHAL_PLL_CLOCKSOURCE_HAS_PLL2Div
        HSEDiv = 0b01,
        PLL2Div = 0b11
#else
        HSE = 1,
#define _MICROHAL_PLL_CLOCKSOURCE_HAS_HSE
#ifdef MCU_TYPE_STM32F1XX
        HSEDiv2 = 0b11
#define _MICROHAL_PLL_CLOCKSOURCE_HAS_HSEDiv2
#endif
#endif
    };

    static ClockSource clockSource(ClockSource source) noexcept;
    static ClockSource clockSource() noexcept;

    static void divider(uint16_t m);
    static uint32_t divider();

    static float inputFrequency();
    static uint32_t frequency(uint32_t frequency_Hz);

    static void enable() noexcept {
        auto cr = registers::rcc->cr.volatileLoad();
        cr.PLLON.set();
        registers::rcc->cr.volatileStore(cr);
    }

    static void disable() noexcept {
        auto cr = registers::rcc->cr.volatileLoad();
        cr.PLLON.clear();
        registers::rcc->cr.volatileStore(cr);
    }

    static bool isReady() noexcept { return registers::rcc->cr.volatileLoad().PLLRDY; }

    static float VCOOutputFrequency() noexcept;

    static float PLLCLKFrequency() { return VCOOutputFrequency(); }

    static uint32_t multiplier() noexcept;
    static bool multiplier(uint32_t mul) noexcept;
};

#ifdef _MICROHAL_REGISTERS_RCC_HAS_CFGR2
struct PLL2 {
    static uint32_t inputFrequency();
    static uint32_t frequency(uint32_t frequency_Hz);
    static void enable() noexcept;
    static void disable() noexcept;
    static bool isEnabled() noexcept { return registers::rcc->cr.volatileLoad().PLL2ON; }

    static bool isReady() noexcept { return registers::rcc->cr.volatileLoad().PLL2RDY; }

    static float VCOOutputFrequency() noexcept;

    static float frequency() { return VCOOutputFrequency(); }

    static uint16_t divider();
    static void divider(uint16_t m);
    static uint32_t multiplier() noexcept;
    static bool multiplier(uint32_t mul) noexcept;
};

struct PLL3 {
    static uint32_t inputFrequency();
    static uint32_t frequency(uint32_t frequency_Hz);
    static void enable() noexcept;
    static void disable() noexcept;

    static bool isEnabled() noexcept { return registers::rcc->cr.volatileLoad().PLL3ON; }

    static bool isReady() noexcept { return registers::rcc->cr.volatileLoad().PLL3RDY; }

    static float VCOOutputFrequency() noexcept;
    static float PLLCLKFrequency() { return VCOOutputFrequency(); }

    static bool multiplier(uint32_t mul) noexcept;
};
#endif

}  // namespace ClockManager
}  // namespace microhal
#endif /* MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_PLL_H_ */
