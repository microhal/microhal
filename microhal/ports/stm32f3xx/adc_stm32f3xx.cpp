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
Adc *Adc::adc2 = nullptr;
Signal<void> Adc::signal;

void Adc::interruptFunction() {
    uint32_t isr = adc.ISR;
    uint32_t ier = adc.IER;
    uint32_t activeAndEnabledInterruptFlags = isr & ier;
    uint32_t interruptFlagToClear = 0;

    if (activeAndEnabledInterruptFlags & Interrupt::EndOfRegularConversion) {
        if (dataBegin != dataEnd) {
            *(dataBegin) = adc.DR;
            dataBegin++;
        }
        interruptFlagToClear |= static_cast<uint32_t>(Interrupt::EndOfRegularConversion);  // always clear interrupt flag
    }
    if (activeAndEnabledInterruptFlags & Interrupt::EndOfRegularSequence) {
        interruptFlagToClear |= static_cast<uint32_t>(Interrupt::EndOfRegularSequence);
        bool shouldYeld = regularSequenceFinishSemaphore.giveFromISR();
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
    adc.ISR = interruptFlagToClear;
}

void ADC1_2_IRQHandler(void) {
    if (Adc::adc1) Adc::adc1->interruptFunction();
    if (Adc::adc2) Adc::adc2->interruptFunction();
}
}  // namespace stm32f3xx
}  // namespace microhal
