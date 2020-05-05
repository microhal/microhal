/*
 * adc_stm32f3xx.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: pokas
 */

#include "adc_v2_stm_common.h"
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 */
#if _MICROHAL_PORT_STM_ADC_DRIVER_VERSION == 2  // Check if driver is compatible with selected MCU
namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

Adc *Adc::adc1 = nullptr;
Adc *Adc::adc2 = nullptr;

Adc::~Adc() {
    // disableInterrupt();
    disable();

    if ((int)&adc == _MICROHAL_ADC1_BASE_ADDRESS) {
        ClockManager::disableADC(1);
        adc1 = nullptr;
    }
    if ((int)&adc == _MICROHAL_ADC2_BASE_ADDRESS) {
        ClockManager::disableADC(2);
        adc2 = nullptr;
    }
    if ((int)&adc == _MICROHAL_ADC2_BASE_ADDRESS) {
        ClockManager::disableADC(3);
    }
}

bool Adc::configureSamplingSequence(gsl::span<Adc::Channel> sequence) {
    if ((sequence.length() == 0) || (sequence.length() > 16)) return false;
    //    if (adc.cr.volatileLoad().ADSTART) return false;

    for (int i = 0; i < sequence.length(); i++) {
        if (sequence.at(i) == Channel::Channel18) return false;
    }

    for (int i = 0; i < sequence.length(); i++) {
        setSamplingSequence(sequence.length(), i + 1, sequence.at(i));
    }
    //    auto cr1 = adc.cr1.volatileLoad();
    //    cr1.SCAN.set();  // enable scan mode
    //    adc.cr1.volatileStore(cr1);

    // enableInterrupts(Interrupt::EndOfRegularSequence /*| /* Interrupt::EndOfRegularConversion | Interrupt::Overrun*/);
    return true;
}

/**
 *
 * @param sequenceLength
 * @param sequencePosition from 1 to 16
 * @param channel - from 0 to 17
 */
void Adc::setSamplingSequence(uint_fast8_t sequenceLength, uint_fast8_t sequencePosition, Channel channel) {
    auto sqr1 = adc.sqr1.volatileLoad();
    sqr1.L = sequenceLength - 1;

    if (sequencePosition <= 6) {
        auto sqr3 = adc.sqr3.volatileLoad();
        sqr3 &= ~(0b11111 << ((sequencePosition - 1) * 5));
        sqr3 |= (channel << ((sequencePosition - 1) * 5));
        adc.sqr3.volatileStore(sqr3);
    } else if (sequencePosition <= 12) {
        auto sqr2 = adc.sqr2.volatileLoad();
        sqr2 &= ~(0b11111 << ((sequencePosition - 7) * 5));
        sqr2 |= (channel << ((sequencePosition - 7) * 5));
        adc.sqr2.volatileStore(sqr2);
    } else if (sequencePosition <= 16) {
        sqr1 &= ~(0b11111 << ((sequencePosition - 13) * 5));
        sqr1 |= (channel << ((sequencePosition - 13) * 5));
    }

    adc.sqr1.volatileStore(sqr1);
}

bool Adc::configureInjectedSamplingSequence(gsl::span<Adc::Channel> sequence) {
    if ((sequence.length() == 0) || (sequence.length() > 4)) return false;

    for (int i = 0; i < sequence.length(); i++) {
        if (sequence.at(i) == Channel::Channel18) return false;
    }

    uint8_t tmp_sequence[4] = {0, 0, 0, 0};
    for (int i = 0; i < sequence.length(); i++) {
        tmp_sequence[4 - sequence.length() + i] = sequence.at(i);
    }

    registers::ADC::JSQR jsqr = {};
    jsqr.JL = sequence.length() - 1;
    jsqr.JSQ1 = tmp_sequence[0];
    jsqr.JSQ2 = tmp_sequence[1];
    jsqr.JSQ3 = tmp_sequence[2];
    jsqr.JSQ4 = tmp_sequence[3];
    adc.jsqr.volatileStore(jsqr);

    return true;
}

bool Adc::configureInjectedChannelOffset(Channel channel, uint16_t offset) {
    auto jsqr = adc.jsqr.volatileLoad();

    if (jsqr.JSQ1 == channel) {
        adc.jofr[0].volatileStore(offset);
        return true;
    }
    if (jsqr.JSQ2 == channel) {
        adc.jofr[1].volatileStore(offset);
        return true;
    }
    if (jsqr.JSQ3 == channel) {
        adc.jofr[2].volatileStore(offset);
        return true;
    }
    if (jsqr.JSQ4 == channel) {
        adc.jofr[3].volatileStore(offset);
        return true;
    }

    return false;
}

bool Adc::configureAnalogWatchdog(Channel channel, uint16_t lowThreshold, uint16_t highThreshold) {
    auto cr1 = adc.cr1.volatileLoad();
    if (cr1.AWDEN == 0 && cr1.JAWDEN == 0) {  // change settings only when analog watchdog is disabled
        cr1.AWDCH = channel;
        adc.cr1.volatileStore(cr1);
        adc.htr.volatileStore(highThreshold);
        adc.ltr.volatileStore(lowThreshold);
    }
    return false;
}

bool Adc::calibrate() {
    if (isEnabled()) {
        auto cr2 = adc.cr2.volatileLoad();
        cr2.CAL.set();
        adc.cr2.volatileStore(cr2);
        return true;
    }
    return false;
}

void Adc::initDMA(uint16_t *data, size_t len) {
    //    DMA::dma1->clockEnable();
    //    auto &stream = DMA::dma1->stream[0];
    //    stream.setPeripheralAddress(&adc.dr);
    //    stream.setMemoryAddress(data);
    //    stream.memoryIncrement(DMA::Channel::MemoryIncrementMode::PointerIncremented);
    //    stream.setNumberOfItemsToTransfer(len);
    //    stream.init(DMA::Channel::MemoryDataSize::HalfWord, DMA::Channel::PeripheralDataSize::HalfWord,
    //                DMA::Channel::MemoryIncrementMode::PointerIncremented, DMA::Channel::PeripheralIncrementMode::PointerFixed,
    //                DMA::Channel::TransmisionDirection::PerToMem);
    //    stream.enableCircularMode();
    //    stream.enable();
    //
    //    auto cfgr = adc.cfgr.volatileLoad();
    //    cfgr.DMACFG = 1;
    //    cfgr.DMAEN = 1;
    //    adc.cfgr.volatileStore(cfgr);
}

void Adc::configureTriggerSource(TriggerSource externalTrigger) {
    auto cr2 = adc.cr2.volatileLoad();
    cr2.EXTSEL = static_cast<uint32_t>(externalTrigger);
    adc.cr2.volatileStore(cr2);
}

// void Adc::configureDualDMA(Resolution resolution, uint32_t *data, size_t dataSize) {
//    DMA::dma1->clockEnable();
//    auto &stream = DMA::dma1->stream[0];
//    stream.setPeripheralAddress(&ADC12_COMMON->CDR);
//    stream.setMemoryAddress(data);
//    stream.memoryIncrement(DMA::Channel::MemoryIncrementMode::PointerIncremented);
//    stream.setNumberOfItemsToTransfer(dataSize);
//    stream.init(DMA::Channel::MemoryDataSize::Word, DMA::Channel::PeripheralDataSize::Word,
//    DMA::Channel::MemoryIncrementMode::PointerIncremented,
//                DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::PerToMem);
//    stream.enableCircularMode();
//    stream.enable();
//
//    auto ccr = registers::adc12Common->ccr.volatileLoad();
//    ccr.MDMA = (resolution == Resolution_8Bit || resolution == Resolution_12Bit) ? 0b10 : 0b11;
//    ccr.DMACFG = 1;
//    registers::adc12Common->ccr.volatileStore(ccr);
//}

void Adc::interruptFunction() {
    auto sr = adc.sr.volatileLoad();
    auto cr1 = adc.cr1.volatileLoad();

    if (sr.EOC && cr1.EOCIE) {
        signal.emit();
    }
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
#endif  // _MICROHAL_PORT_STM_ADC_DRIVER_VERSION == 2
