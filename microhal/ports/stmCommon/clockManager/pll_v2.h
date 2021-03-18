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
#include "rcc_register_select.h"

namespace microhal {
namespace ClockManager {

struct PLL {
#ifdef MCU_TYPE_STM32G0XX
    enum class ClockSource : uint32_t { NoClock = 0b00, HSI = 0b10, HSE = 0b11 };

    static constexpr const uint32_t mDividerMinValue = 1;
    static constexpr const uint32_t mDividerMaxValue = 8;
    static constexpr const uint32_t mDividerRegisterCorrectionValue = 1;
    static constexpr const uint32_t nDividerMinValue = 8;
    static constexpr const uint32_t nDividerMaxValue = 86;
    static constexpr const uint32_t pDividerMinValue = 2;
    static constexpr const uint32_t pDividerMaxValue = 32;
    static constexpr const uint32_t pDividerRegisterCorrectionValue = 1;
    static constexpr const uint32_t rDividerMinValue = 2;
    static constexpr const uint32_t rDividerMaxValue = 8;
    static constexpr const uint32_t rDividerRegisterCorrectionValue = 1;
    static constexpr const uint32_t qDividerMinValue = 2;
    static constexpr const uint32_t qDividerMaxValue = 8;
    static constexpr const uint32_t qDividerRegisterCorrectionValue = 1;
#else
#define P_DIVIDER_EVEN_VALUES_ONLY
    enum class ClockSource : uint32_t { HSI = 0, HSE = 1 };

    static constexpr const uint32_t mDividerMinValue = 2;
    static constexpr const uint32_t mDividerMaxValue = 63;
    static constexpr const uint32_t mDividerRegisterCorrectionValue = 0;
    static constexpr const uint32_t nDividerMinValue = 8;
    static constexpr const uint32_t nDividerMaxValue = 432;
    static constexpr const uint32_t pDividerMaxValue = 8;
    static constexpr const uint32_t pDividerRegisterCorrectionValue = 0;
    static constexpr const uint32_t rDividerMinValue = 2;
    static constexpr const uint32_t rDividerMaxValue = 8;
    static constexpr const uint32_t rDividerRegisterCorrectionValue = 0;
    static constexpr const uint32_t qDividerMinValue = 3;
    static constexpr const uint32_t qDividerMaxValue = 15;
    static constexpr const uint32_t qDividerRegisterCorrectionValue = 0;
#endif

    static ClockSource clockSource(ClockSource source) noexcept {
        auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
        pllcfgr.PLLSRC =
            static_cast<typename std::underlying_type<ClockSource>::type>(source);  // fixme (pokas) maybe bit banding will be faster solution
        registers::rcc->pllcfgr.volatileStore(pllcfgr);
        return source;
    }

    static ClockSource clockSource() noexcept { return static_cast<ClockSource>(registers::rcc->pllcfgr.volatileLoad().PLLSRC.get()); }

    static void MDivider(uint32_t m) {
        if (m < mDividerMinValue || m > mDividerMaxValue) {
            while (1) {
            }
        }
        auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
        pllcfgr.PLLM = m - mDividerRegisterCorrectionValue;
        registers::rcc->pllcfgr.volatileStore(pllcfgr);
    }

    static uint32_t MDivider() { return registers::rcc->pllcfgr.volatileLoad().PLLM + mDividerRegisterCorrectionValue; }

    static float inputFrequency() {
        const float inputFrequency = clockSource() == ClockSource::HSI ? HSI::frequency() : HSE::frequency();
        return inputFrequency / MDivider();
    }

    static float PLLCLKFrequency() {
#ifdef MCU_TYPE_STM32G0XX
        return Main::RFrequency();
#else
        return Main::PFrequency();
#endif
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
        static bool isReady() noexcept { return registers::rcc->cr.volatileLoad().PLLRDY; }
        static bool isEnabled() noexcept { return registers::rcc->cr.volatileLoad().PLLON; }

        static float PFrequency() { return VCOOutputFrequency() / P(); }
        static float QFrequency() { return VCOOutputFrequency() / Q(); }
        static float RFrequency() { return VCOOutputFrequency() / R(); }

        static void enableROutput() {
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLREN.set();
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
        }
        static void disableROutput() {
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLREN.clear();
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
        }

        static void enablePOutput() {
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLPEN.set();
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
        }
        static void disablePOutput() {
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLPEN.clear();
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
        }

        static void enableQOutput() {
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLQEN.set();
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
        }
        static void disableQOutput() {
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLQEN.clear();
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
        }

        static bool N(uint32_t n) noexcept {
            if (n < nDividerMinValue || n > nDividerMaxValue) return false;
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLN = n;
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
            return true;
        }

        static bool P(uint32_t p) noexcept {
#ifdef P_DIVIDER_EVEN_VALUES_ONLY
            if ((p > pDividerMaxValue) || (p & 0b1)) return false;
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLP = p / 2 - 1;
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
            return true;
#else
            if ((p < pDividerMinValue) || (p > pDividerMaxValue)) return false;
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLP = p - pDividerRegisterCorrectionValue;
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
            return true;
#endif
        }

        static bool Q(uint32_t q) noexcept {
            if (q < qDividerMinValue || q > qDividerMaxValue) return false;
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLQ = q - qDividerRegisterCorrectionValue;
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
            return true;
        }

        static bool R(uint32_t r) noexcept {
            if (r < rDividerMinValue || r > rDividerMaxValue) return false;
            auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
            pllcfgr.PLLR = r - rDividerRegisterCorrectionValue;
            registers::rcc->pllcfgr.volatileStore(pllcfgr);
            return true;
        }

     private:
        static float VCOOutputFrequency() noexcept { return inputFrequency() * N(); }
        static uint32_t N() noexcept { return registers::rcc->pllcfgr.volatileLoad().PLLN; }
        static uint32_t P() noexcept {
#ifdef P_DIVIDER_EVEN_VALUES_ONLY
            const uint32_t tab[] = {2, 4, 6, 8};
            return tab[registers::rcc->pllcfgr.volatileLoad().PLLP];
#else
            return registers::rcc->pllcfgr.volatileLoad().PLLP + pDividerRegisterCorrectionValue;
#endif
        }
        static uint32_t Q() noexcept { return registers::rcc->pllcfgr.volatileLoad().PLLQ + qDividerRegisterCorrectionValue; }
        static uint32_t R() noexcept { return registers::rcc->pllcfgr.volatileLoad().PLLR + rDividerRegisterCorrectionValue; }
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
        static bool isReady() noexcept { return registers::rcc->cr.volatileLoad().PLLI2SRDY; }
        static bool isEnabled() noexcept { return registers::rcc->cr.volatileLoad().PLLI2SON; }
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
