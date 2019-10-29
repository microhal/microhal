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

    enableInterrupts(Interrupt::EndOfRegularSequence /*| /* Interrupt::EndOfRegularConversion | Interrupt::Overrun*/);
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

bool Adc::configureChannelOffset(Channel channel, uint16_t offset) {
    if (auto offsetRegister = findOffsetRegisterForChannel(channel); offsetRegister != nullptr) {
        offsetRegister->OFFSET = offset;
        return true;
    } else if (auto offsetRegister = findEmptyOffsetRegister(); offsetRegister != nullptr) {
        offsetRegister->OFFSET_CH = static_cast<uint32_t>(channel);
        offsetRegister->OFFSET = offset;
        return true;
    }
    return false;
}

bool Adc::enableChannelOffset(Channel channel) {
    if (auto offsetRegister = findOffsetRegisterForChannel(channel); offsetRegister != nullptr) {
        offsetRegister->OFFSET_EN = 1;
        return true;
    }
    return false;
}

bool Adc::disableChannelOffset(Channel channel) {
    if (auto offsetRegister = findOffsetRegisterForChannel(channel); offsetRegister != nullptr) {
        offsetRegister->OFFSET_EN = 0;
        return true;
    }
    return false;
}

registers::ADC::OFR_t *Adc::findOffsetRegisterForChannel(Channel channel) {
    for (size_t i = 0; i < 4; i++) {
        if (adc_.OFR[i].OFFSET_CH == static_cast<uint32_t>(channel)) return const_cast<registers::ADC::OFR_t *>(&adc_.OFR[i]);
    }
    return nullptr;
}

registers::ADC::OFR_t *Adc::findEmptyOffsetRegister() {
    for (size_t i = 0; i < 4; i++) {
        if (adc_.OFR[i].OFFSET_CH == 0) return const_cast<registers::ADC::OFR_t *>(&adc_.OFR[i]);
    }
    return nullptr;
}

bool Adc::calibrate(CalibrationType calibrationType) {
    // Software is allowed to set ADCAL only when the ADC is disabled (ADCAL=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).
    uint32_t cr = adc.CR;
    if ((cr & (ADC_CR_ADCAL | ADC_CR_ADSTART | ADC_CR_ADSTP | ADC_CR_ADDIS | ADC_CR_ADEN)) == 0) {
        adc_.CR.ADCALDIF = static_cast<uint32_t>(calibrationType);
        adc_.CR.ADCAL = 1;
        do {
            // wait until calibration is ready
        } while (adc_.CR.ADCAL == 1);
        return true;
    }
    return false;
}

void Adc::initDMA(uint16_t *data, size_t len) {
    DMA::dma1->clockEnable();
    auto &stream = DMA::dma1->stream[0];
    stream.peripheralAddress(&adc.DR);
    stream.memoryAddress(data);
    stream.memoryIncrement(DMA::Channel::MemoryIncrementMode::PointerIncremented);
    stream.setNumberOfItemsToTransfer(len);
    stream.init(DMA::Channel::MemoryDataSize::HalfWord, DMA::Channel::PeripheralDataSize::HalfWord,
                DMA::Channel::MemoryIncrementMode::PointerIncremented, DMA::Channel::PeripheralIncrementMode::PointerFixed,
                DMA::Channel::TransmisionDirection::PerToMem);
    stream.enableCircularMode();
    stream.enable();

    adc_.CFGR.DMACFG = 1;
    adc_.CFGR.DMAEN = 1;
}

void Adc::configureTriggerSource(TriggerSource triggerSource, ExternalTriggerSource externalTrigger) {
    adc_.CFGR.EXTEN = static_cast<uint32_t>(triggerSource);
    adc_.CFGR.EXTSEL = static_cast<uint32_t>(externalTrigger);
}

void Adc::configureDualDMA(Resolution resolution, uint32_t *data, size_t dataSize) {
    DMA::dma1->clockEnable();
    auto &stream = DMA::dma1->stream[0];
    stream.peripheralAddress(&ADC12_COMMON->CDR);
    stream.memoryAddress(data);
    stream.memoryIncrement(DMA::Channel::MemoryIncrementMode::PointerIncremented);
    stream.setNumberOfItemsToTransfer(dataSize);
    stream.init(DMA::Channel::MemoryDataSize::Word, DMA::Channel::PeripheralDataSize::Word, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::PerToMem);
    stream.enableCircularMode();
    stream.enable();

    ADC12_COMMON->CCR |= (resolution == Resolution_8Bit || resolution == Resolution_12Bit) ? ADC_CCR_MDMA_1 : ADC_CCR_MDMA;
    ADC12_COMMON->CCR |= ADC_CCR_DMACFG;  // select circular mode
}

void Adc::enableVoltageRegulator() {
    // check if already enabled, if not the enable Voltage Regulator
    if (adc_.CR.ADVREGEN != 0b01) {
        // this have to be done in two steps, Do not optimize!
        adc_.CR.ADVREGEN = 0;
        adc_.CR.ADVREGEN = 0b01;

        // wait for ADC voltage regulator enable
        while (adc_.CR.ADVREGEN == 0) {
        }
    }
}

void Adc::interruptFunction() {
    uint32_t isr = adc.ISR;
    uint32_t ier = adc.IER;
    uint32_t activeAndEnabledInterruptFlags = isr & ier;
    uint32_t interruptFlagToClear = 0;

    if (activeAndEnabledInterruptFlags & Interrupt::EndOfRegularSequence) {
        interruptFlagToClear |= static_cast<uint32_t>(Interrupt::EndOfRegularSequence);
        signal.emit();
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

#ifdef MICROHAL_USE_ADC_ISR_DMA
void DMA1_Channel1_IRQHandler(void) {
    DMA::dma1->clearInterruptFlag(DMA::dma1->stream[0], DMA::Channel::Interrupt::TransferComplete);
    DMA::dma1->stream[0].disable();

    Adc::adc1->signal.emit();
}
#endif
}  // namespace stm32f3xx
}  // namespace microhal
