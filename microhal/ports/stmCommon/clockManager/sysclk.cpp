/*
 * sysclk.cpp
 *
 *  Created on: Nov 22, 2019
 *      Author: pokas
 */

#include "sysclk.h"
#if defined(MCU_TYPE_STM32F3XX) || defined(MCU_TYPE_STM32F0XX) || defined(MCU_TYPE_STM32F1XX)
#include "../registers/flash_v1.h"
#else
#include "../registers/flash_v2.h"
#endif
#include "hse.h"
#include "hsi.h"
#include "pll.h"

namespace microhal {
namespace ClockManager {

void flashLatency(Frequency sysclkFrequency_Hz) {
    auto acr = registers::flash->acr.volatileLoad();
    if (sysclkFrequency_Hz <= 24'000'000) {
        acr.LATENCY = 0;
    } else if (sysclkFrequency_Hz <= 48'000'000) {
        acr.LATENCY = 0b01;
    } else {
        acr.LATENCY = 0b10;
    }
    registers::flash->acr.volatileStore(acr);
}

void SYSCLK::source(Source source) {
    switch (source) {
        case Source::HSI:
            flashLatency(HSI::frequency());
            break;
        case Source::HSE:
            flashLatency(HSE::frequency());
            break;
        case Source::PLL:
            flashLatency(PLL::PLLCLKFrequency());
            break;
#ifdef _MICROHAL_REGISTERS_RCC_HAS_HSI48
        case Source::HSI48:
            flashLatency(HSI48::frequency());
            break;
#endif
    }
    auto cfgr = registers::rcc->cfgr.volatileLoad();
    cfgr.SW = static_cast<typename std::underlying_type<Source>::type>(source);
    registers::rcc->cfgr.volatileStore(cfgr);
}

uint32_t SYSCLK::frequency() noexcept {
    uint32_t freq = 0;
    switch (registers::rcc->cfgr.volatileLoad().SWS) {
        case 0b00:
            freq = HSI::frequency();
            break;
        case 0b01:
            freq = HSE::frequency();
            break;
        case 0b10:
            freq = PLL::PLLCLKFrequency();
            break;
#ifdef _MICROHAL_REGISTERS_RCC_HAS_HSI48
        case 0b11:
            freq = HSI48::frequency();
            break;
#endif
    }
    return freq;
}

}  // namespace ClockManager
}  // namespace microhal
