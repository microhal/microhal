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

struct PLL {
    enum class ClockSource : uint32_t {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
        HSI = 0,
#else
        HSIDiv2 = 0,
#endif
        HSE = 1,
#ifdef MCU_TYPE_STM32F1XX
        HSEDiv2 = 0b11
#endif
    };

    static ClockSource clockSource(ClockSource source) noexcept {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
        auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
        pllcfgr.PLLSRC =
            static_cast<typename std::underlying_type<ClockSource>::type>(source);  // fixme (pokas) maybe bit banding will be faster solution
        registers::rcc->pllcfgr.volatileStore(pllcfgr);
        return source;
#else
        auto cfgr = registers::rcc->cfgr.volatileLoad();
        cfgr.PLLSRC =
            static_cast<typename std::underlying_type<ClockSource>::type>(source);  // fixme (pokas) maybe bit banding will be faster solution
#ifdef MCU_TYPE_STM32F1XX
        cfgr.PLLXTPRE = source == ClockSource::HSEDiv2 ? 1 : 0;
#endif
        registers::rcc->cfgr.volatileStore(cfgr);
        return source;
#endif
    }

    static ClockSource clockSource() noexcept {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
        return static_cast<ClockSource>(registers::rcc->pllcfgr.volatileLoad().PLLSRC.get());
#else
#ifdef MCU_TYPE_STM32F1XX
        auto cfgr = registers::rcc->cfgr.volatileLoad();
        uint32_t clkSource = cfgr.PLLSRC | (cfgr.PLLXTPRE << 1);
        return static_cast<ClockSource>(clkSource);
#else
        return static_cast<ClockSource>(registers::rcc->cfgr.volatileLoad().PLLSRC.get());
#endif
#endif
    }

#ifndef MCU_TYPE_STM32F1XX
    static void divider(uint16_t m) {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
        if (m < 2 || m > 63)
            while (1) {
            }
        auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
        pllcfgr.PLLM = m;
        registers::rcc->pllcfgr.volatileStore(pllcfgr);
#else
        if (m < 1 || m > 17)
            while (1) {
            }
        auto cfgr2 = registers::rcc->cfgr2.volatileLoad();
        cfgr2.PREDIV = m - 1;
        registers::rcc->cfgr2.volatileStore(cfgr2);
#endif
    }

    static uint32_t divider() {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
        return registers::rcc->pllcfgr.volatileLoad().PLLM;
#else
        return registers::rcc->cfgr2.volatileLoad().PREDIV + 1;
#endif
    }
#else
    static constexpr uint32_t divider() { return 1; }
#endif
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

 private:
    static uint32_t PLLMUL() noexcept;
    static bool PLLMUL(uint32_t mul) noexcept;
};

}  // namespace ClockManager
}  // namespace microhal
#endif /* MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_PLL_H_ */
