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

static const std::array<const registers::TIM *, 14> timers = {
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
    nullptr,
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

Timer *Timer::tim[8] = {nullptr};

uint8_t Timer::getNumber() const {
    for (size_t i = 0; i < timers.size(); i++) {
        if (timers[i] == &timer) return i;
    }
    std::terminate();
}

IRQn_Type Timer::getIRQn() const {
    std::array<IRQn_Type, 14> irq = {TIM1_UP_IRQn, TIM2_IRQn,      TIM3_IRQn,      TIM4_IRQn,      TIM5_IRQn,      TIM6_IRQn,      TIM7_IRQn,
                                     TIM8_CC_IRQn, HardFault_IRQn, HardFault_IRQn, HardFault_IRQn, HardFault_IRQn, HardFault_IRQn, HardFault_IRQn};

    return irq[getNumber()];
}

void Timer::enableInterupt(uint32_t priority) {
    IRQn_Type irq = getIRQn();
    NVIC_SetPriority(irq, priority);
    NVIC_ClearPendingIRQ(irq);
    NVIC_EnableIRQ(irq);
}

void Timer::disableInterrupt() {
    NVIC_EnableIRQ(getIRQn());
}

void Timer::interruptFunction() {
    auto sr = timer.sr.volatileLoad();
    auto dier = timer.dier.volatileLoad();
    uint32_t interruptEnabledAndActive = dier & sr;
#if MICROHAL_USE_TIMER_SIGNAL > 0
    if (interruptEnabledAndActive & signalInterrupt1) signal1.emit();
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    if (interruptEnabledAndActive & signalInterrupt2) signal2.emit();
#endif
    timer.sr.volatileStore(~interruptEnabledAndActive);
}

#ifdef _MICROHAL_TIM1_BASE_ADDRESS
void TIM1_BRK_IRQHandler(void) {  // TIM1 Break
    const Timer::Interrupt interrupts = Timer::Interrupt::Break;
    auto sr = registers::tim1->sr.volatileLoad();
    sr.BIF.clear();
#if MICROHAL_USE_TIMER_SIGNAL > 0
    if (static_cast<uint32_t>(interrupts) & Timer::tim[0]->signalInterrupt1) Timer::tim[0]->signal1.emit();
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    if (static_cast<uint32_t>(interrupts) & Timer::tim[0]->signalInterrupt2) Timer::tim[0]->signal2.emit();
#endif
    registers::tim1->sr.volatileStore(sr);
}

void TIM1_UP_IRQHandler(void) {  // TIM1 Update
    const Timer::Interrupt interrupts = Timer::Interrupt::Update;
    auto sr = registers::tim1->sr.volatileLoad();
    sr.UIF.clear();
#if MICROHAL_USE_TIMER_SIGNAL > 0
    if (static_cast<uint32_t>(interrupts) & Timer::tim[0]->signalInterrupt1) Timer::tim[0]->signal1.emit();
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    if (static_cast<uint32_t>(interrupts) & Timer::tim[0]->signalInterrupt2) Timer::tim[0]->signal2.emit();
#endif
    registers::tim1->sr.volatileStore(sr);
}

void TIM1_TRG_COM_IRQHandler(void) {  // TIM1 Trigger and Commutation
    auto sr = registers::tim1->sr.volatileLoad();
    auto dier = registers::tim1->dier.volatileLoad();
    uint32_t interruptEnabledAndActive = dier & sr;

    if (interruptEnabledAndActive & Timer::Interrupt::Trigger) {
        sr.TIF.clear();
    }
    if (interruptEnabledAndActive & Timer::Interrupt::COM) {
        sr.COMIF.clear();
    }
#if MICROHAL_USE_TIMER_SIGNAL > 0
    if (interruptEnabledAndActive & Timer::tim[0]->signalInterrupt1) Timer::tim[0]->signal1.emit();
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    if (interruptEnabledAndActive & Timer::tim[0]->signalInterrupt2) Timer::tim[0]->signal2.emit();
#endif
    registers::tim1->sr.volatileStore(sr);
}

void TIM1_CC_IRQHandler(void) {
    Timer::Interrupt interrupts{};
    auto sr = registers::tim1->sr.volatileLoad();
    auto dier = registers::tim1->dier.volatileLoad();
    uint32_t interruptEnabledAndActive = dier & sr;
    if (interruptEnabledAndActive & Timer::Interrupt::CaptureCompare1) {
        interrupts |= Timer::Interrupt::CaptureCompare1;
    }
    if (interruptEnabledAndActive & Timer::Interrupt::CaptureCompare2) {
        interrupts |= Timer::Interrupt::CaptureCompare2;
    }
    if (interruptEnabledAndActive & Timer::Interrupt::CaptureCompare3) {
        interrupts |= Timer::Interrupt::CaptureCompare3;
    }
    if (interruptEnabledAndActive & Timer::Interrupt::CaptureCompare4) {
        interrupts |= Timer::Interrupt::CaptureCompare4;
    }
#if MICROHAL_USE_TIMER_SIGNAL > 0
    if (interruptEnabledAndActive & Timer::tim[0]->signalInterrupt1) Timer::tim[0]->signal1.emit();
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    if (interruptEnabledAndActive & Timer::tim[0]->signalInterrupt2) Timer::tim[0]->signal2.emit();
#endif
    registers::tim1->sr.volatileStore(~static_cast<uint32_t>(interrupts));
}
#endif

#ifdef _MICROHAL_TIM2_BASE_ADDRESS
void TIM2_IRQHandler(void) {
    Timer::tim[1]->interruptFunction();
}
#endif
#ifdef _MICROHAL_TIM3_BASE_ADDRESS
void TIM3_IRQHandler(void) {
    Timer::tim[2]->interruptFunction();
}
#endif
#ifdef _MICROHAL_TIM4_BASE_ADDRESS
void TIM4_IRQHandler(void) {
    Timer::tim[3]->interruptFunction();
}
#endif
#ifdef _MICROHAL_TIM6_BASE_ADDRESS
void TIM6_IRQHandler(void) {
    Timer::tim[5]->interruptFunction();
}
#endif
#ifdef _MICROHAL_TIM7_BASE_ADDRESS
void TIM7_IRQHandler(void) {
    Timer::tim[6]->interruptFunction();
}
#endif

#ifdef _MICROHAL_TIM8_BASE_ADDRESS
void TIM8_BRK_IRQHandler(void) {
    const Timer::Interrupt interrupts = Timer::Interrupt::Break;
    auto sr = registers::tim8->sr.volatileLoad();
    sr.BIF.clear();
#if MICROHAL_USE_TIMER_SIGNAL > 0
    if (static_cast<uint32_t>(interrupts) & Timer::tim[7]->signalInterrupt1) Timer::tim[7]->signal1.emit();
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    if (static_cast<uint32_t>(interrupts) & Timer::tim[7]->signalInterrupt2) Timer::tim[7]->signal2.emit();
#endif
    registers::tim8->sr.volatileStore(sr);
}
void TIM8_UP_IRQHandler(void) {
    const Timer::Interrupt interrupts = Timer::Interrupt::Update;
    auto sr = registers::tim8->sr.volatileLoad();
    sr.UIF.clear();
#if MICROHAL_USE_TIMER_SIGNAL > 0
    if (static_cast<uint32_t>(interrupts) & Timer::tim[7]->signalInterrupt1) Timer::tim[7]->signal1.emit();
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    if (static_cast<uint32_t>(interrupts) & Timer::tim[7]->signalInterrupt2) Timer::tim[7]->signal2.emit();
#endif
    registers::tim8->sr.volatileStore(sr);
}

void TIM8_TRG_COM_IRQHandler(void) {
    auto sr = registers::tim8->sr.volatileLoad();
    auto dier = registers::tim8->dier.volatileLoad();
    uint32_t interruptEnabledAndActive = dier & sr;

    if (interruptEnabledAndActive & Timer::Interrupt::Trigger) {
        sr.TIF.clear();
    }
    if (interruptEnabledAndActive & Timer::Interrupt::COM) {
        sr.COMIF.clear();
    }
#if MICROHAL_USE_TIMER_SIGNAL > 0
    if (interruptEnabledAndActive & Timer::tim[7]->signalInterrupt1) Timer::tim[7]->signal1.emit();
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    if (interruptEnabledAndActive & Timer::tim[7]->signalInterrupt2) Timer::tim[7]->signal2.emit();
#endif
    registers::tim8->sr.volatileStore(sr);
}

void TIM8_CC_IRQHandler(void) {
    Timer::Interrupt interrupts{};
    auto sr = registers::tim8->sr.volatileLoad();
    auto dier = registers::tim8->dier.volatileLoad();
    uint32_t interruptEnabledAndActive = dier & sr;

    if (interruptEnabledAndActive & Timer::Interrupt::CaptureCompare1) {
        interrupts |= Timer::Interrupt::CaptureCompare1;
    }
    if (interruptEnabledAndActive & Timer::Interrupt::CaptureCompare2) {
        interrupts |= Timer::Interrupt::CaptureCompare2;
    }
    if (interruptEnabledAndActive & Timer::Interrupt::CaptureCompare3) {
        interrupts |= Timer::Interrupt::CaptureCompare3;
    }
    if (interruptEnabledAndActive & Timer::Interrupt::CaptureCompare4) {
        interrupts |= Timer::Interrupt::CaptureCompare4;
    }
#if MICROHAL_USE_TIMER_SIGNAL > 0
    if (interruptEnabledAndActive & Timer::tim[7]->signalInterrupt1) Timer::tim[7]->signal1.emit();
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    if (interruptEnabledAndActive & Timer::tim[7]->signalInterrupt2) Timer::tim[7]->signal2.emit();
#endif
    registers::tim8->sr.volatileStore(~static_cast<uint32_t>(interrupts));
}
#endif

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
