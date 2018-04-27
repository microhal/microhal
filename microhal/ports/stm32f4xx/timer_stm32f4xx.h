/*
 * timer_stm32f0xx.h
 *
 *  Created on: 19 cze 2014
 *      Author: Dell
 */

#ifndef TIMER_STM32F4XX_H_
#define TIMER_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "clockManager.h"
#include "dma_stm32f4xx.h"
#include "signalSlot/signalSlot.h"

#include "device/stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
extern "C" {
void TIM1_BRK_UP_TRG_COM_IRQHandler(void);
void TIM1_CC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);
void TIM7_IRQHandler(void);
void TIM14_IRQHandler(void);
void TIM15_IRQHandler(void);
void TIM16_IRQHandler(void);
void TIM17_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
class Timer {
 public:
    /**
     * @brief Possible counter directions.
     */
    enum class Direction { UpCounter = 0x0000, DownCounter = 0x0010 };

    enum class Event {
        UpdateGeneration = 0x0001,
        Compare1 = 0x0002,
        Compare2 = 0x0004,
        Compare3 = 0x0008,
        Compare4 = 0x0010,
        TriggerGeneration = 0x0040,
    };
    //--------------------------------------- static variables --------------------------------------//
    static Timer timer1;
    static Timer timer2;
    static Timer timer3;
    static Timer timer4;
    static Timer timer5;
    //------------------------------------------- variables -----------------------------------------//
    Signal<void> onCompare;
    Signal<void> overflow;
    //------------------------------------------- functions -----------------------------------------//
    /**
     * @brief This function enable timer.
     */
    void enable() {
        // NVIC_EnableIRQ(TIM2_IRQn);
        // NVIC_SetPriority(TIM2_IRQn, 0);
        // timer.DIER |= 1;
        timer.CR1 |= TIM_CR1_CEN;
    }
    /**
     * @brief This function disable timer.
     */
    void disable() {
        timer.CR1 &= ~TIM_CR1_CEN;
        // NVIC_DisableIRQ(TIM2_IRQn);
    }
    /**
     * @brief This function set timer direction.
     *
     * @param direction - new timer direction.
     */
    void setDirection(Direction direction) {
        uint16_t cr1 = timer.CR1;
        // clear actual settings
        cr1 &= ~(static_cast<uint16_t>(Direction::UpCounter) | static_cast<uint16_t>(Direction::DownCounter));
        // set new settings
        cr1 |= static_cast<uint16_t>(direction);
        // update register
        timer.CR1 = cr1;
    }

    uint32_t getValue() { return timer.CNT; }

    void setValue(uint32_t value) { timer.CNT = value; }

    uint32_t getPrescalerValue() { return timer.PSC; }

    void setPrescalerValue(uint32_t value) { timer.PSC = value; }

    void setAutoReloadRegister(uint32_t value) { timer.ARR = value; }

    void enableAutoReload() { timer.CR1 |= TIM_CR1_ARPE; }
    void disableAutoReload() { timer.CR1 &= ~TIM_CR1_ARPE; }

    void setPeriod(uint32_t ms) {
        // todo
        ms = ms;
    }

    void enableEvent(Event event) { timer.EGR |= static_cast<uint16_t>(event); }

    void disableEvent(Event event) { timer.EGR &= ~static_cast<uint16_t>(event); }

    volatile void *getDMARegisterPtr() const { return &timer.DMAR; }

    void enableAutoReloadPreload() {
        timer.CR1 |= TIM_CR1_ARPE;  // | TIM_CR1_URS;
    }

    void magic() {
        timer.CCMR2 = TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
        timer.CCER |= TIM_CCER_CC3E;
        timer.DCR = 15;
        timer.CCMR2 |= TIM_CCMR2_OC3PE;
        timer.DIER |= TIM_DIER_CC3DE;
    }

    struct CaptureCompare {};

 private:
    //------------------------------------------- variables -----------------------------------------//
    TIM_TypeDef &timer;
    //------------------------------------------- constructors --------------------------------------//
    constexpr Timer(TIM_TypeDef &timer) : onCompare{}, overflow{}, timer(timer) { ClockManager::enable(timer, ClockManager::PowerMode::Normal); }
    // virtual ~timer_stm32f0xx();
    //------------------------------------------- friends -------------------------------------------//
    friend void TIM1_BRK_UP_TRG_COM_IRQHandler(void);
    friend void TIM1_CC_IRQHandler(void);
    friend void TIM2_IRQHandler(void);
    friend void TIM3_IRQHandler(void);
    friend void TIM6_DAC_IRQHandler(void);
    friend void TIM7_IRQHandler(void);
    friend void TIM14_IRQHandler(void);
    friend void TIM15_IRQHandler(void);
    friend void TIM16_IRQHandler(void);
    friend void TIM17_IRQHandler(void);
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif /* TIMER_STM32F4XX_H_ */
