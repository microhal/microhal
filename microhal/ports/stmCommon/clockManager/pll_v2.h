/*
 * pll_v2.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_PLL_V2_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_PLL_V2_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "apbClock.h"
#include "clockTypes.h"
#include "hse.h"
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

struct PLL {
    enum class ClockSource : uint32_t { HSI = 0, HSE = 1 };

    static ClockSource clockSource(ClockSource source) noexcept {
        auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
        pllcfgr.PLLSRC =
            static_cast<typename std::underlying_type<ClockSource>::type>(source);  // fixme (pokas) maybe bit banding will be faster solution
        registers::rcc->pllcfgr.volatileStore(pllcfgr);
        return source;
    }

    static ClockSource clockSource() noexcept { return static_cast<ClockSource>(registers::rcc->pllcfgr.volatileLoad().PLLSRC.get()); }

    static void MDivider(uint32_t m) {
        if (m < 2 || m > 63) {
            while (1) {
            }
        }
        auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
        pllcfgr.PLLM = m;
        registers::rcc->pllcfgr.volatileStore(pllcfgr);
    }

    static uint32_t MDivider() { return registers::rcc->pllcfgr.volatileLoad().PLLM; }

    static float inputFrequency() {
        const float inputFrequency = clockSource() == ClockSource::HSI ? HSI::frequency() : HSE::frequency();
        return inputFrequency / MDivider();
    }

    struct Main {
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
        static bool isReady() noexcept { return registers::rcc->pllcfgr.volatileLoad().PLLRDY; }
        static bool isEnabled() noexcept { return registers::rcc->pllcfgr.volatileLoad().PLLON; }

        static float PFrequency() { return VCOOutputFrequency() / P(); }
        static float QFrequency() { return VCOOutputFrequency() / Q(); }

        static bool N(uint32_t n) noexcept {
            if (n < 2 || n > 432) return false;
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLN = n;
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
            return true;
        }

        static bool P(uint32_t p) noexcept {
            if ((p > 8) || (p & 0b1)) return false;
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLP = p / 2 - 1;
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
            return true;
        }

        static bool Q(uint32_t q) noexcept {
            if (q < 3 || q > 15) return false;
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLQ = q;
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
            return true;
        }

     private:
        static float VCOOutputFrequency() noexcept { return inputFrequency() * N(); }
        static uint32_t N() noexcept { return registers::rcc->pllcfgr.volatileLoad().PLLN; }
        static uint32_t P() noexcept {
            const uint32_t tab[] = {2, 4, 6, 8};
            return tab[registers::rcc->pllcfgr.volatileLoad().PLLP];
        }
        static uint32_t Q() noexcept { return registers::rcc->pllcfgr.volatileLoad().PLLQ; }
    };

#if defined(RCC_CR_PLLI2SON)
    struct I2S {
        static void enable() noexcept {
            auto cr = registers::rcc->cr.volatileLoad();
            cr.PLLI2SON.set();
            registers::rcc->cr.volatileStore(cr);
        }
        static void disable() noexcept {
            auto cr = registers::rcc->cr.volatileLoad();
            cr.PLLI2SON.clear();
            registers::rcc->cr.volatileStore(cr);
        }
        static bool isReady() noexcept { return registers::rcc->pllcfgr.volatileLoad().PLLI2SRDY; }
        static bool isEnabled() noexcept { return registers::rcc->pllcfgr.volatileLoad().PLLI2SON; }
    };
#endif
#if defined(RCC_CR_PLLSAION)
    struct SAI {
        static void enable() noexcept {
            auto cr = registers::rcc->cr.volatileLoad();
            cr.PLLSAION.set();
            registers::rcc->cr.volatileStore(cr);
        }
        static void disable() noexcept {
            auto cr = registers::rcc->cr.volatileLoad();
            cr.PLLSAION.clear();
            registers::rcc->cr.volatileStore(cr);
        }
        static bool isReady() noexcept { return registers::rcc->pllcfgr.volatileLoad().PLLSAIRDY; }
        static bool isEnabled() noexcept { return registers::rcc->pllcfgr.volatileLoad().PLLSAION; }
    };
#endif
};

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_PLL_V2_H_ */
