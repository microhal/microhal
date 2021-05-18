/*
 * pll.cpp
 *
 *  Created on: Nov 22, 2019
 *      Author: pokas
 */

#include "pll_v1.h"
#include <cassert>
#include <cmath>  // for round
#include "hse.h"
#include "hsi.h"

namespace microhal {
namespace ClockManager {
//------------------------------------------------------------------------------
//                                 PLL
//------------------------------------------------------------------------------
PLL::ClockSource PLL::clockSource(ClockSource source) noexcept {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
    auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
    pllcfgr.PLLSRC =
        static_cast<typename std::underlying_type<ClockSource>::type>(source);  // fixme (pokas) maybe bit banding will be faster solution
    registers::rcc->pllcfgr.volatileStore(pllcfgr);
    return source;
#else  // _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
    auto cfgr = registers::rcc->cfgr.volatileLoad();
    cfgr.PLLSRC = static_cast<typename std::underlying_type<ClockSource>::type>(source);  // fixme (pokas) maybe bit banding will be faster solution
#ifdef _MICROHAL_REGISTERS_RCC_CRGR2_HAS_PREDIV1SRC
    auto cfgr2 = registers::rcc->cfgr2.volatileLoad();
    cfgr2.PREDIV1SRC = (source == ClockSource::PLL2Div) ? 1 : 0;
    registers::rcc->cfgr2.volatileStore(cfgr2);
#else  // _MICROHAL_REGISTERS_RCC_CRGR2_HAS_PREDIV1SRC
#ifdef MCU_TYPE_STM32F1XX
    cfgr.PLLXTPRE = source == ClockSource::HSEDiv2 ? 1 : 0;
#endif  // MCU_TYPE_STM32F1XX
#endif  // _MICROHAL_REGISTERS_RCC_CRGR2_HAS_PREDIV1SRC
    registers::rcc->cfgr.volatileStore(cfgr);
    return source;
#endif  // _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
}

PLL::ClockSource PLL::clockSource() noexcept {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
    return static_cast<ClockSource>(registers::rcc->pllcfgr.volatileLoad().PLLSRC.get());
#else  // _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
#ifdef _MICROHAL_REGISTERS_RCC_CRGR2_HAS_PREDIV1SRC
    if (registers::rcc->cfgr.volatileLoad().PLLSRC.get()) {
        const auto cfgr2 = registers::rcc->cfgr2.volatileLoad();
        return cfgr2.PREDIV1SRC.get() ? ClockSource::PLL2Div : ClockSource::HSEDiv;
    } else {
        return ClockSource::HSIDiv2;
    }

#else  // _MICROHAL_REGISTERS_RCC_CRGR2_HAS_PREDIV1SRC
#ifdef MCU_TYPE_STM32F1XX
    auto cfgr = registers::rcc->cfgr.volatileLoad();
    uint32_t clkSource = cfgr.PLLSRC | (cfgr.PLLXTPRE << 1);
    return static_cast<ClockSource>(clkSource);
#else
    return static_cast<ClockSource>(registers::rcc->cfgr.volatileLoad().PLLSRC.get());
#endif  // MCU_TYPE_STM32F1XX
#endif  // _MICROHAL_REGISTERS_RCC_CRGR2_HAS_PREDIV1SRC
#endif  // _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
}

#if defined(_MICROHAL_REGISTERS_RCC_HAS_PLLCFGR) || defined(_MICROHAL_REGISTERS_RCC_HAS_CFGR2)
void PLL::divider(uint16_t m) {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
    if (m < 2 || m > 63)
        while (1) {
        }
    auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
    pllcfgr.PLLM = m;
    registers::rcc->pllcfgr.volatileStore(pllcfgr);
#else   //_MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
    assert(m > 0 && m < 17);
    auto cfgr2 = registers::rcc->cfgr2.volatileLoad();
    cfgr2.PREDIV1 = m - 1;
    registers::rcc->cfgr2.volatileStore(cfgr2);
#endif  //_MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
}

uint32_t PLL::divider() {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
    return registers::rcc->pllcfgr.volatileLoad().PLLM;
#else   //_MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
    return registers::rcc->cfgr2.volatileLoad().PREDIV1 + 1;
#endif  //_MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
}
#endif

float PLL::inputFrequency() {
    switch (clockSource()) {
#ifdef _MICROHAL_PLL_CLOCKSOURCE_HAS_HSI
        case ClockSource::HSI:
            return HSI::frequency();
#endif
#ifdef _MICROHAL_PLL_CLOCKSOURCE_HAS_HSIDiv2
        case ClockSource::HSIDiv2:
            return HSI::frequency() / 2;
#endif
#ifdef _MICROHAL_PLL_CLOCKSOURCE_HAS_HSE
        case ClockSource::HSE:
            return HSE::frequency();
#endif
#ifdef _MICROHAL_PLL_CLOCKSOURCE_HAS_HSEDiv
        case ClockSource::HSEDiv:
            return HSE::frequency() / divider();
#endif
#ifdef _MICROHAL_PLL_CLOCKSOURCE_HAS_HSEDiv2
        case ClockSource::HSEDiv2:
            return HSE::frequency() / 2;
#endif
#ifdef _MICROHAL_PLL_CLOCKSOURCE_HAS_PLL2Div
        case ClockSource::PLL2Div:
            return PLL2::frequency() / divider();
#endif
    }
    std::terminate();
}

uint32_t PLL::frequency(uint32_t frequency_Hz) {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
#else
    if (clockSource() == ClockSource::HSIDiv2) {
        // we only can change pll mul
        uint32_t mul = round(frequency_Hz / inputFrequency());
        if (mul > 16) mul = 16;
        if (mul < 2) mul = 2;
        multiplier(mul);
        while (HSI::isReady() == false)
            ;
    } else {
        uint32_t mul = round(frequency_Hz / inputFrequency());
        if (mul > 16) mul = 16;
        if (mul < 2) mul = 2;
        multiplier(mul);
        while (HSE::isReady() == false) {
        }
    }

    enable();
    while (isReady() == false)
        ;
#endif
    return VCOOutputFrequency();
}

float PLL::VCOOutputFrequency() noexcept {
    return inputFrequency() * multiplier();
}

uint32_t PLL::multiplier() noexcept {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
    return registers::rcc->pllcfgr.volatileLoad().PLLN;
#else
    uint32_t mul = registers::rcc->cfgr.volatileLoad().PLLMUL;
    if (mul == 0b1111) return 16;
    return mul + 2;
#endif
}

bool PLL::multiplier(uint32_t mul) noexcept {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
    if (mul >= 50 && mul <= 432) {
        auto pllcfgr = registers::rcc->pllcfgr.volatileLoad();
        pllcfgr.PLLN = mul;
        registers::rcc->pllcfgr.volatileStore(pllcfgr);
    }
#else
    if (mul >= 2 && mul <= 16) {
        auto cfgr = registers::rcc->cfgr.volatileLoad();
        cfgr.PLLMUL = mul - 2;
        registers::rcc->cfgr.volatileStore(cfgr);
        return true;
    }
#endif
    return false;
}

//------------------------------------------------------------------------------
//                                 PLL2
//------------------------------------------------------------------------------
#ifdef _MICROHAL_REGISTERS_RCC_HAS_CFGR2
uint16_t PLL2::divider() {
    const auto cfgr2 = registers::rcc->cfgr2.volatileLoad();
    return cfgr2.PREDIV2.get() + 1;
}

void PLL2::divider(uint16_t m) {
    assert(m > 0);
    auto cfgr2 = registers::rcc->cfgr2.volatileLoad();
    cfgr2.PREDIV2 = m - 1;
    registers::rcc->cfgr2.volatileStore(cfgr2);
}

void PLL2::enable() noexcept {
    auto cr = registers::rcc->cr.volatileLoad();
    cr.PLL2ON.set();
    registers::rcc->cr.volatileStore(cr);
}

void PLL2::disable() noexcept {
    auto cr = registers::rcc->cr.volatileLoad();
    cr.PLL2ON.clear();
    registers::rcc->cr.volatileStore(cr);
}

uint32_t PLL2::multiplier() noexcept {
    const auto cfgr2 = registers::rcc->cfgr2.volatileLoad();
    if (cfgr2.PLL2MUL.get() == 0b1110) return 16;
    if (cfgr2.PLL2MUL.get() == 0b1111) return 20;
    return cfgr2.PLL2MUL.get() + 2;
}

bool PLL2::multiplier(uint32_t mul) noexcept {
    if (!isEnabled()) {
        auto cfgr = registers::rcc->cfgr2.volatileLoad();
        if (mul >= 8 && mul <= 14) {
            cfgr.PLL2MUL = mul - 2;
        } else if (mul == 16) {
            cfgr.PLL2MUL = 0b1110;
        } else if (mul == 20) {
            cfgr.PLL2MUL = 0b1111;
        } else {
            return false;
        }
        registers::rcc->cfgr2.volatileStore(cfgr);
        return true;
    }
    return false;
}

uint32_t PLL2::inputFrequency() {
    return HSE::frequency() / divider();  // for stm32f107
}

float PLL2::VCOOutputFrequency() noexcept {
    return inputFrequency() * multiplier();
}

//------------------------------------------------------------------------------
//                                 PLL3
//------------------------------------------------------------------------------
void PLL3::enable() noexcept {
    auto cr = registers::rcc->cr.volatileLoad();
    cr.PLL3ON.set();
    registers::rcc->cr.volatileStore(cr);
}

void PLL3::disable() noexcept {
    auto cr = registers::rcc->cr.volatileLoad();
    cr.PLL3ON.clear();
    registers::rcc->cr.volatileStore(cr);
}

bool PLL3::multiplier(uint32_t mul) noexcept {
    if (!isEnabled()) {
        auto cfgr = registers::rcc->cfgr2.volatileLoad();
        if (mul >= 8 && mul <= 14) {
            cfgr.PLL3MUL = mul - 2;
        } else if (mul == 16) {
            cfgr.PLL3MUL = 0b1110;
        } else if (mul == 20) {
            cfgr.PLL3MUL = 0b1111;
        } else {
            return false;
        }
        registers::rcc->cfgr2.volatileStore(cfgr);
        return true;
    }
    return false;
}
#endif

}  // namespace ClockManager
}  // namespace microhal
