/*
 * timer_stm32f3xx.cpp
 *
 *  Created on: Aug 9, 2019
 *      Author: pokas
 */

#include "timer_stmCommon.h"
#include <array>

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

Timer *Timer::tim1 = nullptr;
Timer *Timer::tim3 = nullptr;

uint8_t Timer::getNumber() const {
    std::array<registers::TIM *, 14> timers = {
#ifdef _MICROHAL_TIM1_BASE_ADDRESS
        registers::tim1,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM2_BASE_ADDRESS
        registers::tim2,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM3_BASE_ADDRESS
        registers::tim3,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM4_BASE_ADDRESS
        registers::tim4,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM5_BASE_ADDRESS
        registers::tim5,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM6_BASE_ADDRESS
        registers::tim6,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM7_BASE_ADDRESS
        registers::tim7,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM8_BASE_ADDRESS
        registers::tim8,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM9_BASE_ADDRESS
        registers::tim9,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM10_BASE_ADDRESS
        registers::tim10,
#else
        nullptr
#endif
#ifdef _MICROHAL_TIM11_BASE_ADDRESS
        registers::tim11,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM12_BASE_ADDRESS
        registers::tim12,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM13_BASE_ADDRESS
        registers::tim13,
#else
        nullptr,
#endif
#ifdef _MICROHAL_TIM14_BASE_ADDRESS
        registers::tim14
#else
        nullptr
#endif
    };

    for (size_t i = 0; i < timers.size(); i++) {
        if (timers[i] == &timer) return i + 1;
    }
    std::terminate();
}

IRQn_Type Timer::getIRQn() const {
    std::array<IRQn_Type, 14> irq = {TIM1_UP_IRQn, TIM2_IRQn,      TIM3_IRQn,      TIM4_IRQn,      TIM5_IRQn,      TIM6_IRQn,      TIM7_IRQn,
                                     TIM8_CC_IRQn, HardFault_IRQn, HardFault_IRQn, HardFault_IRQn, HardFault_IRQn, HardFault_IRQn, HardFault_IRQn};

    return irq[getNumber() - 1];
}

void TIM1_UP_IRQHandler() {
    auto sr = registers::tim1->sr.volatileLoad();
    if (sr.UIF) {
        sr.UIF.clear();
    }
    registers::tim1->sr.volatileStore(sr);
    Timer::tim1->signal.emit();
}

void TIM1_CC_IRQHandler(void) {
    static volatile int i;
    i++;
    auto sr = registers::tim1->sr.volatileLoad();
    auto dier = registers::tim1->dier.volatileLoad();
    uint32_t interruptEnabledAndActive = dier & sr;

    if (interruptEnabledAndActive & Timer::Interrupt::CaptureCompare1) {
        // sr = ~static_cast<uint32_t>(Timer::Interrupt::CaptureCompare1);
    }
    registers::tim3->sr.volatileStore(sr);
}

void TIM3_IRQHandler(void) {
    auto sr = registers::tim3->sr.volatileLoad();
    if (sr.UIF) {
        sr.UIF.clear();
    }
    if (sr.CC2IF) {
        sr.CC2IF.clear();
    }
    registers::tim3->sr.volatileStore(sr);
    Timer::tim3->signal.emit();
}
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
