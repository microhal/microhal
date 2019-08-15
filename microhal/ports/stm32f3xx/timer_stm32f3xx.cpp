/*
 * timer_stm32f3xx.cpp
 *
 *  Created on: Aug 9, 2019
 *      Author: pokas
 */

#include "timer_stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {

Timer *Timer::tim3 = nullptr;

extern "C" void TIM1_CC_IRQHandler(void) {
    static volatile int i;
    i++;
    uint32_t sr = TIM1->SR;
    uint32_t interruptEnabledAndActive = TIM1->DIER & sr;

    if (interruptEnabledAndActive & Timer::Interrupt::CaptureCompare1) {
        // sr = ~static_cast<uint32_t>(Timer::Interrupt::CaptureCompare1);
    }
    TIM1->SR = sr;
}

extern "C" void TIM3_IRQHandler(void) {
    uint32_t sr = TIM3->SR;
    if (sr & TIM_SR_UIF) {
        TIM3->SR &= ~TIM_SR_UIF;
    }
    if (sr & TIM_SR_CC2IF) {
        TIM3->SR &= ~TIM_SR_CC2IF;
    }
    Timer::tim3->signal.emit();
}
}  // namespace stm32f3xx
}  // namespace microhal
