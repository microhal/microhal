/*
 * adc_stm32f3xx.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: pokas
 */

#include "adc_stm_common.h"

#include _MICROHAL_INCLUDE_PORT_DMA

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

Adc *Adc::adc1 = nullptr;
Adc *Adc::adc2 = nullptr;

Adc::~Adc() {
    disableInterrupt();
    stopConversion();
    disable();
    RCC->AHBENR &= ~RCC_AHBENR_ADC12EN;
    RCC->CFGR2 &= (~RCC_CFGR2_ADCPRE12_4 | RCC_CFGR2_ADCPRE12_3 | RCC_CFGR2_ADCPRE12_2 | RCC_CFGR2_ADCPRE12_1 | RCC_CFGR2_ADCPRE12_0);
}

bool Adc::enable() {
    // Software is allowed to set ADEN only when all bits of ADC_CR registers are 0 (ADCAL=0, ADSTP=0, ADSTART=0, ADDIS=0 and ADEN=0)
    auto cr = adc.cr.volatileLoad();
    if (cr.ADCAL == 0 && cr.JADSTART == 0 && cr.ADSTP == 0 && cr.ADSTART == 0 && cr.ADDIS == 0 && cr.ADEN == 0) {
        cr.ADEN = 1;
        adc.cr.volatileStore(cr);
        return true;
    }
    return false;
}

bool Adc::disable() {
    // Setting ADDIS to �1� is only effective when ADEN=1 and ADSTART=0 (which ensures that no conversion is ongoing)
    auto cr = adc.cr.volatileLoad();
    if (cr.ADEN == 1 && cr.ADSTART == 0 && cr.JADSTART == 0) {
        cr.ADDIS = 1;
        adc.cr.volatileStore(cr);
        return true;
    }
    return false;
}

bool Adc::configureSamplingSequence(gsl::span<Adc::Channel> sequence) {
    if ((sequence.length() == 0) || (sequence.length() > 16)) return false;
    if (adc.cr.volatileLoad().ADSTART) return false;

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
    auto sqr1 = adc.sqr1.volatileLoad();
    sqr1.L = sequenceLength - 1;

    if (sequencePosition <= 4) {
        sqr1 &= ~(0b11111 << (sequencePosition * 6));
        sqr1 |= (channel << (sequencePosition * 6));
    } else if (sequencePosition <= 9) {
        auto sqr = adc.sqr2.volatileLoad();
        sqr &= ~(0b11111 << ((sequencePosition - 5) * 6));
        sqr |= (channel << ((sequencePosition - 5) * 6));
        adc.sqr2.volatileStore(sqr);
    } else if (sequencePosition <= 14) {
        auto sqr = adc.sqr3.volatileLoad();
        sqr &= ~(0b11111 << ((sequencePosition - 10) * 6));
        sqr |= (channel << ((sequencePosition - 10) * 6));
        adc.sqr3.volatileStore(sqr);
    } else if (sequencePosition <= 16) {
        auto sqr = adc.sqr4.volatileLoad();
        sqr &= ~(0b11111 << ((sequencePosition - 15) * 6));
        sqr |= (channel << ((sequencePosition - 15) * 6));
        adc.sqr4.volatileStore(sqr);
    }

    adc.sqr1.volatileStore(sqr1);
}

bool Adc::configureChannelOffset(Channel channel, uint16_t offset) {
    for (size_t i = 0; i < 4; i++) {
        auto ofr = adc.ofr[i].volatileLoad();
        if (ofr.OFFSET_CH == static_cast<uint32_t>(channel)) {
            ofr.OFFSET = offset;
            adc.ofr[i].volatileStore(ofr);
            return true;
        }
    }
    // unable to find offset for specific channel, try to find empty offset register and assign it with requested channel
    for (size_t i = 0; i < 4; i++) {
        auto ofr = adc.ofr[i].volatileLoad();
        if (ofr.OFFSET_CH == 0) {
            ofr.OFFSET_CH = static_cast<uint32_t>(channel);
            ofr.OFFSET = offset;
            adc.ofr[i].volatileStore(ofr);
            return true;
        }
    }
    return false;
}

bool Adc::enableChannelOffset(Channel channel) {
    for (size_t i = 0; i < 4; i++) {
        auto ofr = adc.ofr[i].volatileLoad();
        if (ofr.OFFSET_CH == static_cast<uint32_t>(channel)) {
            ofr.OFFSET_EN = 1;
            adc.ofr[i].volatileStore(ofr);
            return true;
        }
    }
    return false;
}

bool Adc::disableChannelOffset(Channel channel) {
    for (size_t i = 0; i < 4; i++) {
        auto ofr = adc.ofr[i].volatileLoad();
        if (ofr.OFFSET_CH == static_cast<uint32_t>(channel)) {
            ofr.OFFSET_EN = 0;
            adc.ofr[i].volatileStore(ofr);
            return true;
        }
    }
    return false;
}

bool Adc::calibrate(CalibrationType calibrationType) {
    // Software is allowed to set ADCAL only when the ADC is disabled (ADCAL=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).
    auto cr = adc.cr.volatileLoad();
    if (cr.ADCAL == 0 && cr.ADSTART == 0 && cr.ADSTP == 0 && cr.ADDIS == 0 && cr.ADEN == 0) {
        cr.ADCALDIF = static_cast<uint32_t>(calibrationType);
        cr.ADCAL = 1;
        do {
            // wait until calibration is ready
        } while (adc.cr.volatileLoad().ADCAL == 1);
        return true;
    }
    return false;
}

void Adc::initDMA(uint16_t *data, size_t len) {
    DMA::dma1->clockEnable();
    auto &stream = DMA::dma1->stream[0];
    stream.setPeripheralAddress(&adc.dr);
    stream.setMemoryAddress(data);
    stream.setMemoryIncrement(DMA::Channel::MemoryIncrementMode::PointerIncremented);
    stream.setNumberOfItemsToTransfer(len);
    stream.init(DMA::Channel::MemoryDataSize::HalfWord, DMA::Channel::PeripheralDataSize::HalfWord,
                DMA::Channel::MemoryIncrementMode::PointerIncremented, DMA::Channel::PeripheralIncrementMode::PointerFixed,
                DMA::Channel::TransmisionDirection::PerToMem);
    stream.enableCircularMode();
    stream.enable();

    auto cfgr = adc.cfgr.volatileLoad();
    cfgr.DMACFG = 1;
    cfgr.DMAEN = 1;
    adc.cfgr.volatileStore(cfgr);
}

void Adc::configureTriggerSource(TriggerSource triggerSource, ExternalTriggerSource externalTrigger) {
    auto cfgr = adc.cfgr.volatileLoad();
    cfgr.EXTEN = static_cast<uint32_t>(triggerSource);
    cfgr.EXTSEL = static_cast<uint32_t>(externalTrigger);
    adc.cfgr.volatileStore(cfgr);
}

void Adc::configureDualDMA(Resolution resolution, uint32_t *data, size_t dataSize) {
    DMA::dma1->clockEnable();
    auto &stream = DMA::dma1->stream[0];
    stream.setPeripheralAddress(&ADC12_COMMON->CDR);
    stream.setMemoryAddress(data);
    stream.setMemoryIncrement(DMA::Channel::MemoryIncrementMode::PointerIncremented);
    stream.setNumberOfItemsToTransfer(dataSize);
    stream.init(DMA::Channel::MemoryDataSize::Word, DMA::Channel::PeripheralDataSize::Word, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::PerToMem);
    stream.enableCircularMode();
    stream.enable();

    auto ccr = registers::adc12Common->ccr.volatileLoad();
    ccr.MDMA = (resolution == Resolution_8Bit || resolution == Resolution_12Bit) ? 0b10 : 0b11;
    ccr.DMACFG = 1;
    registers::adc12Common->ccr.volatileStore(ccr);
}

void Adc::enableVoltageRegulator() {
    // check if already enabled, if not the enable Voltage Regulator
    auto cr = adc.cr.volatileLoad();
    if (cr.ADVREGEN != 0b01) {
        // this have to be done in two steps, Do not optimize!
        cr.ADVREGEN = 0;
        adc.cr.volatileStore(cr);
        cr.ADVREGEN = 0b01;
        adc.cr.volatileStore(cr);

        // wait for ADC voltage regulator enable
        while (adc.cr.volatileLoad().ADVREGEN != 0b01) {
        }
    }
}

void Adc::disableVoltageRegulator() {
    auto cr = adc.cr.volatileLoad();
    cr.ADVREGEN = 0b00;
    adc.cr.volatileStore(cr);
    cr.ADVREGEN = 0b10;
    adc.cr.volatileStore(cr);
}

void Adc::interruptFunction() {
    auto isr = adc.isr.volatileLoad();
    auto ier = adc.ier.volatileLoad();
    uint32_t activeAndEnabledInterruptFlags = isr & ier;

    if (activeAndEnabledInterruptFlags & Interrupt::EndOfRegularSequence) {
        isr |= static_cast<uint32_t>(Interrupt::EndOfRegularSequence);
        signal.emit();
    }

    if (activeAndEnabledInterruptFlags & Adc::Interrupt::Overrun) {
        isr |= static_cast<uint32_t>(Adc::Interrupt::Overrun);
    }
    // clear flags of interrupt that have been served
    adc.isr.volatileStore(isr);
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
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
