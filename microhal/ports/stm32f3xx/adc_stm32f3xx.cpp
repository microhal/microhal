/*
 * adc_stm32f3xx.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: pokas
 */

#include "adc_stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {

Adc *Adc::adc1 = nullptr;
Signal<void> Adc::signal;

void ADC1_2_IRQHandler(void) {
    uint32_t isr = ADC1->ISR;
    uint32_t ier = ADC1->IER;
    uint32_t activeAndEnabledInterruptFlags = isr & ier;
    uint32_t interruptFlagToClear = 0;
    if (activeAndEnabledInterruptFlags & Adc::Interrupt::EndOfRegularConversion) {
        if (Adc::adc1->dataBegin != Adc::adc1->dataEnd) {
            *(Adc::adc1->dataBegin) = ADC1->DR;
            Adc::adc1->dataBegin++;
        }
        interruptFlagToClear |= static_cast<uint32_t>(Adc::Interrupt::EndOfRegularConversion);  // always clear interrupt flag
    }
    if (activeAndEnabledInterruptFlags & Adc::Interrupt::EndOfRegularSequence) {
        interruptFlagToClear |= static_cast<uint32_t>(Adc::Interrupt::EndOfRegularSequence);
        bool shouldYeld = Adc::adc1->regularSequenceFinishSemaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
        portYIELD_FROM_ISR(shouldYeld);
#else
        (void)shouldYeld;
#endif
    }

    if (activeAndEnabledInterruptFlags & Adc::Interrupt::Overrun) {
        interruptFlagToClear |= static_cast<uint32_t>(Adc::Interrupt::Overrun);
    }

    // clear flags of interrupt that have been served
    ADC1->ISR = interruptFlagToClear;
}
}  // namespace stm32f3xx
}  // namespace microhal
