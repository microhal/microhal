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
    std::array<registers::TIM *, 14> timers = {registers::tim1,  registers::tim2,  registers::tim3,  registers::tim4, registers::tim5,
                                               registers::tim6,  registers::tim7,  registers::tim8,  registers::tim9, registers::tim10,
                                               registers::tim11, registers::tim12, registers::tim13, registers::tim14};

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
