/*
 * timer_stm32f3xx.cpp
 *
 *  Created on: Aug 9, 2019
 *      Author: pokas
 */

#include "timer_stmCommon.h"
#include <array>
#include "ports/stmCommon/clockManager/timerClock.h"
#include _MICROHAL_INCLUDE_PORT_INTERRUPT_CONTROLLER

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

Timer::Timer(registers::TIM *addr) : timer(*addr) {
    if (tim[getNumber() - 1] != nullptr) std::terminate();
    tim[getNumber() - 1] = this;
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::enableTimer(getNumber(), ClockManager::PowerMode::Normal);
#else
    ClockManager::enableTimer(getNumber());
#endif
}

Timer::~Timer() {
    disableInterrupt();
    tim[getNumber() - 1] = nullptr;
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::disableTimer(getNumber(), ClockManager::PowerMode::Normal);
#else
    ClockManager::disableTimer(getNumber());
#endif
}

void Timer::enableInterupt(uint32_t priority) {
    // NVIC_ClearPendingIRQ(irq);
    enableTimerInterrupt(getNumber(), priority);
}

void Timer::disableInterrupt() {
    disableTimerInterrupt(getNumber());
}

uint32_t Timer::getTimerClockSourceFrequency() const {
    return ClockManager::TimerFrequency(getNumber());
}
uint32_t Timer::getTimerCounterFrequency() const {
    return ClockManager::TimerFrequency(getNumber()) / getPrescaler();
}

uint32_t Timer::getTickPeriod() const {
    uint32_t timerFrequency = ClockManager::TimerFrequency(getNumber());
    return (uint64_t{1000'000'000} * getPrescaler()) / timerFrequency;
}

uint8_t Timer::getNumber() const {
    for (size_t i = 0; i < timers.size(); i++) {
        if (timers[i] == &timer) return i + 1;
    }
    std::terminate();
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
    auto sr = registers::tim1->sr.volatileLoad();
    auto dier = registers::tim1->dier.volatileLoad();
    uint32_t interruptEnabledAndActive = dier & sr;

    if (interruptEnabledAndActive & Timer::Interrupt::Break) {
#if MICROHAL_USE_TIMER_SIGNAL > 0
        if (static_cast<uint32_t>(Timer::Interrupt::Break) & Timer::tim[0]->signalInterrupt1) Timer::tim[0]->signal1.emit();
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
        if (static_cast<uint32_t>(Timer::Interrupt::Break) & Timer::tim[0]->signalInterrupt2) Timer::tim[0]->signal2.emit();
#endif
        sr.BIF.clear();
        registers::tim1->sr.volatileStore(sr);
    }
}

void TIM1_UP_IRQHandler(void) {  // TIM1 Update
    auto sr = registers::tim1->sr.volatileLoad();
    auto dier = registers::tim1->dier.volatileLoad();
    uint32_t interruptEnabledAndActive = dier & sr;
    if (interruptEnabledAndActive & Timer::Interrupt::Update) {
#if MICROHAL_USE_TIMER_SIGNAL > 0
        if (static_cast<uint32_t>(interruptEnabledAndActive) & Timer::tim[0]->signalInterrupt1) Timer::tim[0]->signal1.emit();
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
        if (static_cast<uint32_t>(interruptEnabledAndActive) & Timer::tim[0]->signalInterrupt2) Timer::tim[0]->signal2.emit();
#endif
        sr.UIF.clear();
        registers::tim1->sr.volatileStore(sr);
    }
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
