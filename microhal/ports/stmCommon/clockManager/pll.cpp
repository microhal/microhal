/*
 * pll.cpp
 *
 *  Created on: Nov 22, 2019
 *      Author: pokas
 */

#include "pll.h"
#include <cmath>  // for round
#include "hse.h"
#include "hsi.h"

namespace microhal {
namespace ClockManager {
float PLL::inputFrequency() {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
    if (clockSource() == ClockSource::HSI) {
        return HSI::frequency();
#else
    if (clockSource() == ClockSource::HSIDiv2) {
        return HSI::frequency() / 2;
#endif
    } else {
        return HSE::frequency() / divider();
    }
}

uint32_t PLL::frequency(uint32_t frequency_Hz) {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
#else
    if (clockSource() == ClockSource::HSIDiv2) {
        // we only can change pll mul
        uint32_t mul = round(frequency_Hz / inputFrequency());
        if (mul > 16) mul = 16;
        if (mul < 2) mul = 2;
        PLLMUL(mul);
        while (HSI::isReady() == false)
            ;
    } else {
        uint32_t mul = round(frequency_Hz / inputFrequency());
        if (mul > 16) mul = 16;
        if (mul < 2) mul = 2;
        PLLMUL(mul);
        while (HSE::isReady() == false) {
        }
    }

    enable();
    while (isReady() == false)
        ;
#endif
    return VCOOutputFrequency();
}  // namespace ClockManager

float PLL::VCOOutputFrequency() noexcept {
    return inputFrequency() * PLLMUL();
}

uint32_t PLL::PLLMUL() noexcept {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_PLLCFGR
    return registers::rcc->pllcfgr.volatileLoad().PLLN;
#else
    uint32_t mul = registers::rcc->cfgr.volatileLoad().PLLMUL;
    if (mul == 0b1111) return 16;
    return mul + 2;
#endif
}

bool PLL::PLLMUL(uint32_t mul) noexcept {
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
}  // namespace ClockManager
}  // namespace microhal
