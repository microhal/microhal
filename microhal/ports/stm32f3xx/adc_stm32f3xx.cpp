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

bool Adc::configureSamplingSequence(gsl::span<Adc::Channel> sequence) {
    if ((sequence.length() == 0) || (sequence.length() > 16)) return false;
    if (adc.CR & ADC_CR_ADSTART) return false;

    for (int i = 0; i < sequence.length(); i++) {
        if (sequence.at(i) == Channel::Channel0) return false;
    }

    for (int i = 0; i < sequence.length(); i++) {
        setSamplingSequence(sequence.length(), i + 1, sequence.at(i));
    }

    enableInterrupts(Interrupt::EndOfRegularSequence | /* Interrupt::EndOfRegularConversion |*/ Interrupt::Overrun);
    return true;
}

void Adc::setSamplingSequence(uint_fast8_t sequenceLength, uint_fast8_t sequencePosition, Channel channel) {
    uint32_t sqr1 = adc.SQR1;
    sqr1 &= ~0b1111;
    sqr1 |= sequenceLength - 1;

    if (sequencePosition <= 4) {
        sqr1 &= ~(0b11111 << (sequencePosition * 6));
        sqr1 |= (channel << (sequencePosition * 6));
    } else if (sequencePosition <= 9) {
        uint32_t sqr = adc.SQR2;
        sqr &= ~(0b11111 << ((sequencePosition - 5) * 6));
        sqr |= (channel << ((sequencePosition - 5) * 6));
        adc.SQR2 = sqr;
    } else if (sequencePosition <= 14) {
        uint32_t sqr = adc.SQR3;
        sqr &= ~(0b11111 << ((sequencePosition - 10) * 6));
        sqr |= (channel << ((sequencePosition - 10) * 6));
        adc.SQR3 = sqr;
    } else if (sequencePosition <= 16) {
        uint32_t sqr = adc.SQR4;
        sqr &= ~(0b11111 << ((sequencePosition - 15) * 6));
        sqr |= (channel << ((sequencePosition - 15) * 6));
        adc.SQR4 = sqr;
    }

    adc.SQR1 = sqr1;
}

void Adc::configureDualDMA(Resolution resolution, uint32_t *data, size_t dataSize) {
    DMA::dma1->clockEnable();
    auto &stream = DMA::dma1->stream[0];
    stream.peripheralAddress(&ADC12_COMMON->CDR);
    stream.memoryAddress(data);
    stream.memoryIncrement(DMA::Channel::MemoryIncrementMode::PointerIncremented);
    stream.numberOfItemsToTransfer(dataSize);
    stream.init(DMA::Channel::MemoryDataSize::Word, DMA::Channel::PeripheralDataSize::Word, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::PerToMem);
    stream.enableCircularMode();
    stream.enable();

    ADC12_COMMON->CCR |= (resolution == Resolution_8Bit || resolution == Resolution_12Bit) ? ADC_CCR_MDMA_1 : ADC_CCR_MDMA;
    ADC12_COMMON->CCR |= ADC_CCR_DMACFG;  // select circular mode
}

void Adc::interruptFunction() {
    uint32_t isr = adc.ISR;
    uint32_t ier = adc.IER;
    uint32_t activeAndEnabledInterruptFlags = isr & ier;
    uint32_t interruptFlagToClear = 0;

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
