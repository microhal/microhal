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
#include <stdint.h>
#include "signalSlot/signalSlot.h"

#include "stm32f4xx.h"

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
	typedef enum {
		UP_COUNTER = 0x0000, DOWN_COUNTER = 0x0010
	} Direction;

	typedef enum {
		UPDATE_GENERATION = 0x0001,
		COMPARE1 = 0x0002,
		COMPARE2 = 0x0004,
		COMPARE3 = 0x0008,
		COMPARE4 = 0x0010,
		TRIGGER_GENERATION = 0x0040,

	} Events;
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
		NVIC_EnableIRQ(TIM2_IRQn);
		NVIC_SetPriority(TIM2_IRQn, 0);
		timer.DIER |= 1;
		timer.CR1 |= TIM_CR1_CEN;
	}
	/**
	 * @brief This function disable timer.
	 */
	void disable() {
		timer.CR1 &= ~TIM_CR1_CEN;
	}
	/**
	 * @brief This function set timer direction.
	 *
	 * @param direction - new timer direction.
	 */
	void setDirection(Direction direction) {
		uint16_t cr1 = timer.CR1;
		//clear actual settings
		cr1 &= ~(UP_COUNTER | DOWN_COUNTER);
		//set new settings
		cr1 |= direction;
		//update register
		timer.CR1 = cr1;
	}

	uint32_t getValue() {
		return timer.CNT;
	}

	void setValue(uint32_t value) {
		timer.CNT = value;
	}

	uint32_t getPrescalerValue() {
		return timer.PSC;
	}

	void setPrescalerValue(uint32_t value) {
		timer.PSC = value;
	}

	void setAutoReloadRegister(uint32_t value) {
		timer.ARR = value;
	}

	void enableAutoReload() {
		timer.CR1 |= TIM_CR1_ARPE;
	}
	void disableAutoReload() {
		timer.CR1 &= ~TIM_CR1_ARPE;
	}

	void setPeriod(uint32_t ms) {
		//todo
		ms = ms;
	}

	void enableEvent(Events event) {
		timer.EGR |= event;
	}

	void disableEvent(Events event) {
		timer.EGR &= ~event;
	}
private:
//------------------------------------------- variables -----------------------------------------//
	TIM_TypeDef &timer;
//------------------------------------------- constructors --------------------------------------//
	constexpr Timer(TIM_TypeDef &timer) :
			timer(timer) {
	}
	//virtual ~timer_stm32f0xx();
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

} //namespace stm32f4xx
} // namespace microhal

#endif /* TIMER_STM32F4XX_H_ */
