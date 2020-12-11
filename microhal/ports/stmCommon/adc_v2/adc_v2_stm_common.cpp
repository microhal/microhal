/*
 * adc_stm32f3xx.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: pokas
 */

#include "adc_v2_stm_common.h"
#include "../stmCommonDefines.h"

#include _MICROHAL_INCLUDE_PORT_DEVICE
#if _MICROHAL_PORT_STM_DMA_DRIVER_VERSION == 1
#include <ports/stmCommon/dma/dma_v1/dma_stmCommon.h>
#endif
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 */
#if _MICROHAL_PORT_STM_ADC_DRIVER_VERSION == 2  // Check if driver is compatible with selected MCU
namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

Adc *Adc::adc1 = nullptr;
Adc *Adc::adc2 = nullptr;
#ifdef _MICROHAL_ADC3_BASE_ADDRESS
Adc *Adc::adc3 = nullptr;
#endif

Adc::~Adc() {
    disable();
    disableInterrupt();

    if ((int)&adc == _MICROHAL_ADC1_BASE_ADDRESS) {
        ClockManager::disableADC(1);
        adc1 = nullptr;
    }
    if ((int)&adc == _MICROHAL_ADC2_BASE_ADDRESS) {
        ClockManager::disableADC(2);
        adc2 = nullptr;
    }
#ifdef _MICROHAL_ADC3_BASE_ADDRESS
    if ((int)&adc == _MICROHAL_ADC2_BASE_ADDRESS) {
        ClockManager::disableADC(3);
        adc3 = nullptr;
    }
#endif
}

bool Adc::configureSamplingSequence(const gsl::span<const Adc::Channel> sequence) {
    if ((sequence.length() == 0) || (sequence.length() > 16)) return false;
    if (adc.cr2.volatileLoad().ADON) return false;

    for (int i = 0; i < sequence.length(); i++) {
        if (sequence.at(i) == Channel::Channel18) return false;
    }

    for (int i = 0; i < sequence.length(); i++) {
        setSamplingSequence(sequence.length(), i + 1, sequence.at(i));
    }
    auto cr1 = adc.cr1.volatileLoad();
    cr1.SCAN.set();  // enable scan mode
    adc.cr1.volatileStore(cr1);

    enableInterrupts(Interrupt::EndOfConversion);
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
    auto cr2 = adc.cr2.volatileLoad();
    if (cr2.ADON) {  // if enabled
        cr2.CAL.set();
        adc.cr2.volatileStore(cr2);
        return true;
    }
    return false;
}

bool Adc::waitForCallibrationFinish() {
    while (1) {
        if (!adc.cr2.volatileLoad().CAL) break;
    }
    return true;
}

bool Adc::resetCallibration() {
    auto cr2 = adc.cr2.volatileLoad();
    cr2.RSTCAL.set();
    adc.cr2.volatileStore(cr2);
    return true;
}

bool Adc::waitForConversionEnd(uint32_t ms) {
    while (ms--) {
        if (adc.sr.volatileLoad().EOC) {
            return true;
        }
    }
    return false;
}

#ifdef MICROHAL_USE_ADC_ISR_DMA
void Adc::initDMA(uint16_t *data, size_t len) {
    DMA::dma1->clockEnable();
    volatile int i = 300000;
    while (i--) {
    }
    auto &stream = DMA::dma1->channel[0];
    stream.setPeripheralAddress(&adc.dr);
    stream.setMemoryAddress(data);
    stream.setNumberOfItemsToTransfer(len);
    stream.init(DMA::Channel::MemoryDataSize::HalfWord, DMA::Channel::PeripheralDataSize::HalfWord,
                DMA::Channel::MemoryIncrementMode::PointerIncremented, DMA::Channel::PeripheralIncrementMode::PointerFixed,
                DMA::Channel::TransmisionDirection::PerToMem);
    stream.enableCircularMode();
    stream.enableInterrupt(DMA::Channel::Interrupt::TransferComplete);
    stream.enable();

    auto cr2 = adc.cr2.volatileLoad();
    cr2.DMA.set();
    adc.cr2.volatileStore(cr2);
}
#endif

void Adc::configureTriggerSource(TriggerSourceADC12 externalTrigger) {
    auto cr2 = adc.cr2.volatileLoad();
    cr2.EXTSEL = static_cast<uint32_t>(externalTrigger);
    adc.cr2.volatileStore(cr2);
}

#ifdef _MICROHAL_ADC3_BASE_ADDRESS
void Adc::configureTriggerSource(TriggerSourceADC3 externalTrigger) {
    auto cr2 = adc.cr2.volatileLoad();
    cr2.EXTSEL = static_cast<uint32_t>(externalTrigger);
    adc.cr2.volatileStore(cr2);
}
#endif

void Adc::enableInterrupts(Interrupt interrupts) {
    auto cr1 = adc.cr1.volatileLoad();
    if (static_cast<uint32_t>(interrupts) & Interrupt::AnalogWatchdog1) cr1.AWDIE.set();
    if (static_cast<uint32_t>(interrupts) & Interrupt::EndOfInjectedConversion) cr1.JEOCIE.set();
    if (static_cast<uint32_t>(interrupts) & Interrupt::EndOfConversion) cr1.EOCIE.set();
    adc.cr1.volatileStore(cr1);
}

void Adc::enableInterrupt(uint32_t priority) {
    NVIC_ClearPendingIRQ(ADC1_2_IRQn);
    NVIC_SetPriority(ADC1_2_IRQn, priority);
    NVIC_EnableIRQ(ADC1_2_IRQn);
#ifdef MICROHAL_USE_ADC_ISR_DMA
    DMA::dma1->enableInterrupt(DMA::dma1->channel[0], priority);
#endif
}

void Adc::disableInterrupt() {
    NVIC_DisableIRQ(ADC1_2_IRQn);
#ifdef MICROHAL_USE_ADC_ISR_DMA
    DMA::dma1->disableInterrupt(DMA::dma1->channel[0]);
#endif
}

void Adc::interruptFunction() {
    auto sr = adc.sr.volatileLoad();
    auto cr1 = adc.cr1.volatileLoad();

    if (sr.EOC && cr1.EOCIE) {
        sr.EOC.clear();
#ifdef MICROHAL_ADC_USE_SIGNAL
        signal.emit();
#endif
        auto shouldYeld = regularSequenceFinishSemaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
        portYIELD_FROM_ISR(shouldYeld);
#else
        (void)shouldYeld;
#endif
    }
    adc.sr.volatileStore(sr);
}

void ADC1_2_IRQHandler(void) {
    if (Adc::adc1) Adc::adc1->interruptFunction();
    if (Adc::adc2) Adc::adc2->interruptFunction();
}

#ifdef _MICROHAL_ADC3_BASE_ADDRESS
void ADC3_IRQHandler(void) {
    Adc::adc3->interruptFunction();
}
#endif

#ifdef MICROHAL_USE_ADC_ISR_DMA
void DMA1_Channel1_IRQHandler(void) {
    DMA::dma1->clearInterruptFlag(DMA::dma1->channel[0], DMA::Channel::Interrupt::TransferComplete);

    auto shouldYeld = Adc::adc1->regularSequenceFinishSemaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#else
    (void)shouldYeld;
#endif

#ifdef MICROHAL_ADC_USE_SIGNAL
    Adc::adc1->signal.emit();
#endif  // MICROHAL_ADC_USE_SIGNAL
}
#endif  // MICROHAL_USE_ADC_ISR_DMA

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
#endif  // _MICROHAL_PORT_STM_ADC_DRIVER_VERSION == 2
