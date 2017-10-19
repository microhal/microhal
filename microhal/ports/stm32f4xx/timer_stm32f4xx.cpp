/*
 * timer_stm32f0xx.cpp
 *
 *  Created on: 19 cze 2014
 *      Author: Dell
 */

#include "timer_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

Timer Timer::timer1(*TIM1);
Timer Timer::timer2(*TIM2);
Timer Timer::timer3(*TIM3);
//Timer Timer::timer4(*TIM4);
//Timer Timer::timer5(*TIM5);


//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
extern "C" void DMA1_Stream1_IRQHandler(void) {
    DMA1->LIFCR = DMA_LIFCR_CTCIF1;
    DMA1_Stream1->CR &= ~DMA_SxCR_EN;
    TIM2->CR1 &= ~TIM_CR1_CEN;
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void){

}
void TIM1_CC_IRQHandler(void){
	Timer::timer1.onCompare.emit();
}
void TIM2_IRQHandler(void){
	//uint16_t read = TIM2->SR;
	//read = 0;
	TIM2->SR = 0;
	Timer::timer2.onCompare.emit();
}
//void TIM3_IRQHandler(void){
//
//}
//void TIM6_DAC_IRQHandler(void);
//void TIM7_IRQHandler(void);
//void TIM14_IRQHandler(void);
//void TIM15_IRQHandler(void);
//void TIM16_IRQHandler(void);
//void TIM17_IRQHandler(void);
} //namespace stm32f4xx
} // namespace microhal
