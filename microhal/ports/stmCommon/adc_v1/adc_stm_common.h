/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 02-08-2018
 * last modification: 02-08-2018
 *
 * @copyright Copyright (c) 2018, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_PORTS_ADC_STMCOMMON_H_
#define _MICROHAL_PORTS_ADC_STMCOMMON_H_
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 */
#include <ports/stmCommon/driverConfiguration_stmCommon.h>
#if _MICROHAL_PORT_STM_ADC_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <thread>
#include "../registers/adc_common_registers_v1.h"
#include "../registers/adc_registers_v1.h"
#include "../stmCommonDefines.h"
#include "gsl/gsl"
#include "microhal_semaphore.h"
#include "ports/stmCommon/clockManager/adcClock.h"
#include "signalSlot/signalSlot.h"

#include _MICROHAL_INCLUDE_PORT_DEVICE  // stmCommonDefines.h have to be included before this
#include _MICROHAL_INCLUDE_PORT_CONFIG

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

/* ************************************************************************************************
 * EXTERN DECLARATION
 */
extern "C" {
void ADC1_2_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
}
/* **************************************************************************************************************************************************
 * CLASS
 */
namespace adc_detail {
enum class Interrupt {
    ADCReady = 0b0000'0000'0001,                     //
    EndOfSampling = 0b0000'0000'0010,                //
    EndOfRegularConversion = 0b0000'0000'0100,       //
    EndOfRegularSequence = 0b0000'0000'1000,         //
    Overrun = 0b0000'0001'0000,                      //
    EndOfInjectedConversion = 0b0000'0010'0000,      //
    EndOfInjectedSequence = 0b0000'0100'0000,        //
    AnalogWatchdog1 = 0b0000'1000'0000,              //
    AnalogWatchdog2 = 0b0001'0000'0000,              //
    AnalogWatchdog3 = 0b0010'0000'0000,              //
    InjectedContextQueueOverflow = 0b0100'0000'0000  //
};

constexpr Interrupt operator|(Interrupt a, Interrupt b) {
    return static_cast<Interrupt>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

constexpr Interrupt operator&(Interrupt a, Interrupt b) {
    return static_cast<Interrupt>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}
constexpr uint32_t operator&(uint32_t a, adc_detail::Interrupt b) {
    return a & static_cast<uint32_t>(b);
}

}  // namespace adc_detail

using adc_detail::operator|;
using adc_detail::operator&;

class Adc final {
 public:
    using Interrupt = adc_detail::Interrupt;

    enum class CalibrationType {
        SingleEnded = 0,
#ifdef _MICROHAL_REGISTERS_ADC_HAS_DIFFERENTIAL_MODE
        Differential
#endif
    };
    enum class TriggerSource { Software, HardwareOnRisingEdge, HardwareOnFallingEdge, HardwareOnBothEdges };
    /**
     * @brief Possible ADC channels
     */
    typedef enum : uint32_t {
        Channel0 = 0x00000000,
        Channel1,
        Channel2,
        Channel3,
        Channel4,
        Channel5,
        Channel6,
        Channel7,
        Channel8,
        Channel9,
        Channel10,
        Channel11,
        Channel12,
        Channel13,
        Channel14,
        Channel15,
        Channel16,
        Channel17,
        Channel18,
    } Channel;
    /**
     * @brief Possible ADC resolution
     */
    typedef enum {
        Resolution_12Bit = 0b00,
        Resolution_10Bit = 0b01,
        Resolution_8Bit = 0b10,
        Resolution_6Bit = 0b11,
    } Resolution;

    typedef enum {
        ADCCLK = 0x00,
#ifdef _MICROHAL_STM32G0XX_STM32G0xx
        PCLK_Half = 0b01,
        PCLK_Quater = 0b10,
        PCLK = 0b11,  //!< This option can be used only when AHB clock prescaler is set to 1
#else
        PCLK = 0b01,  //!< This option can be used only when AHB clock prescaler is set to 1
        PCLK_Half = 0b10,
        PCLK_Quater = 0b11
#endif
    } ClkMode;

    enum class ExternalTriggerSource {
        TIM1_CC1,
        TIM1_CC2,
        TIM1_CC3,
        TIM2_CC2,
        TIM3_TRGO,
        EXTI11 = 0b0110,
        HRTIM_ADCTRG1,
        HRTIM_ADCTRG3,
        TIM1_TRGO,
        TIM1_TRGO2,
        TIM2_TRGO,
        TIM6_TRGO = 0b1101,
        TIM15_TRGO = 0b1110,
        TIM3_CC4
    };

    enum class DualADCMode {
        Independent = 0,
        CombinedRegularSimultaneousAndIngectedSimultaneous,
        CombinedRegularSimultaneousAndAlternateTrigger,
        CombinedInterleavedAndInjectedSimultaneous,
        InjectedSimultaneousOnly = 0b00101,
        RegularSimultaneousOnly,
        InterleavedOnly,
        AlternateTriggerOnly = 0b01001
    };

    //=================== ADC On Off functions ===================
    bool enable();
    bool disable();

    bool isEnabled() { return adc.cr.volatileLoad().ADEN; }

    bool waitForADCready(uint32_t ms = 1000) {
        while (ms--) {
            if (adc.isr.volatileLoad().ADRDY) {
                return true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds{1});
        }
        return false;
    }

    void enableAutoDelayedConversionMode() {
        auto cfgr = adc.cfgr1.volatileLoad();
        cfgr.AUTDLY = 1;
        adc.cfgr1.volatileStore(cfgr);
    }
    void disableAutoDelayedConversionMode() {
        auto cfgr = adc.cfgr1.volatileLoad();
        cfgr.AUTDLY = 0;
        adc.cfgr1.volatileStore(cfgr);
    }

    bool setResolution(Resolution resolution) {
        if (isEnabled() == false) {
            auto cfgr = adc.cfgr1.volatileLoad();
            cfgr.RES = resolution;
            adc.cfgr1.volatileStore(cfgr);
            return true;
        }
        return false;
    }

#ifdef _MICROHAL_REGISTERS_ADC_HAS_OFR
    bool configureChannelOffset(Channel channel, uint16_t offset);
    bool enableChannelOffset(Channel channel);
    bool disableChannelOffset(Channel channel);
#endif

    //=================== Regular conversion functions ===================
    /**
     *
     * @param sequenceLength
     * @param sequencePosition
     * @param channel number form 1 to 18
     * @return
     */
    bool configureSamplingSequence(gsl::span<Adc::Channel> sequence);
    /**
     *
     * @param triggerSource - @ref TriggerSource
     * @param eventNumber - Select hardware trigger source, ignored in Software trigger mode
     */
    void configureTriggerSource(TriggerSource triggerSource, ExternalTriggerSource externalTrigger);
    /**
     *
     * @retval true if conversion was started
     * @retval false when conversion start is impossible. This may be caused by disabled ADC or ADC conversion is ongoing or stop conversion
     * request is ongoing.
     */
    bool startConversion() {
        // Software is allowed to set ADSTART only when ADEN=1 and ADDIS=0 (ADC is enabled and there is no pending request to disable the ADC)
        auto cr = adc.cr.volatileLoad();
        if (cr.ADEN == 1 && cr.ADDIS == 0 && cr.ADSTART == 0) {
            cr.ADSTART = 1;
            adc.cr.volatileStore(cr);
            return true;
        }
        return false;
    }
    /**
     * Stop ongoing conversion.
     */
    bool stopConversion() {
        // Setting ADSTP to 1 is only effective when ADSTART=1 and ADDIS=0 (ADC is enabled and may be converting and there is no pending request
        // to disable the ADC)
        auto cr = adc.cr.volatileLoad();
        if (cr.ADSTART == 1 && cr.ADDIS == 0) {
            cr.ADSTP = 1;
            adc.cr.volatileStore(cr);
            return true;
        }
        return false;
    }
    bool waitForConversionEnd(uint32_t ms = 10000) {
        while (ms--) {
            if (adc.isr.volatileLoad().EOC) {
                return true;
            }
        }
        return false;
    }
    bool waitForRegularSequenceEnd(std::chrono::milliseconds timeout) { return regularSequenceFinishSemaphore.wait(timeout); }
    uint16_t readSamples() { return adc.dr.volatileLoad().RDATA; }

    //=================== Injected conversion functions ===================

    //=================== Analog watchdog functions ===================

    //==================== ADC callibration functions ====================
    /**
     * @note Calibrate function can be called only when ADC is disabled.
     */
    bool calibrate(CalibrationType calibrationType);

    //=================== Temperature sensor functions ===================
    bool enableTemperatureSensor() {
        auto ccr = registers::adc12Common->ccr.volatileLoad();
        ccr.TSEN.set();
        registers::adc12Common->ccr.volatileStore(ccr);
        return true;
    }
    bool disableTemperatureSensor() {
        auto ccr = registers::adc12Common->ccr.volatileLoad();
        ccr.TSEN.clear();
        registers::adc12Common->ccr.volatileStore(ccr);
        return true;
    }
    constexpr Channel getTemperatureSensorChannel() const { return Channel12; }
    constexpr Channel getInteranlReferenceChannel() const { return Channel13; }
    static float voltageToTemperatureInCelsius(float voltage) {
        constexpr const float v25 = 1.43f;
        constexpr const float avgSlope = 4.3f / 1000.0f;
        return (v25 - voltage) / avgSlope + 25.0;
    }

    void initDMA(uint16_t *data, size_t len);

    Interrupt getInterrupFlags() const {
        uint32_t isr = adc.isr.volatileLoad();
        return static_cast<Interrupt>(isr);
    }
    void clearInterruptFlags(Interrupt interrupt) {
        auto isr = adc.isr.volatileLoad();
        isr &= ~static_cast<uint32_t>(interrupt);
        adc.isr.volatileStore(isr);
    }

    bool setClockMode(ClkMode mode) {
        // Software is allowed to write these bits only when the ADC is disabled (ADCAL=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).
        if (isEnabled() == false) {
#ifdef _MICROHAL_REGISTERS_ADC_CFGR2_HAS_CKMODE
            auto cfgr2 = adc.cfgr2.volatileLoad();
            cfgr2.CKMODE = mode;
            adc.cfgr2.volatileStore(cfgr2);
            return true;
#else
            auto ccr = registers::adc12Common->ccr.volatileLoad();
            ccr.CKMODE = mode;
            registers::adc12Common->ccr.volatileStore(ccr);
            return true;
#endif
        }
        return false;
    }

    bool registerIsrFunction(void (*interruptFunction)(void), uint32_t interruptPriority) {
        if (interruptFunction == nullptr) {
            //   DMA::dma1->stream[0].disableInterrupt(DMA::Channel::Interrupt::TransferComplete);
            //  DMA::dma1->disableInterrupt(DMA::dma1->stream[0]);
        }
        if (signal.connect(interruptFunction)) {
            //  DMA::dma1->stream[0].enableInterrupt(DMA::Channel::Interrupt::TransferComplete);
            //   DMA::dma1->enableInterrupt(DMA::dma1->stream[0], interruptPriority);
            return true;
        }
        return false;
    }

    template <typename T>
    bool registerIsrFunction(const T &slot, const typename T::type &object) {
        if (signal.connect(slot, object)) {
            return true;
        }
        return false;
    }
    /**
     * Note: This function is enabling interrupt for both ADC1 and ADC2 devices
     * @param priority
     */
    void enableInterrupt(uint32_t priority) {
        NVIC_ClearPendingIRQ(ADC1_2_IRQn);
        NVIC_SetPriority(ADC1_2_IRQn, priority);
        NVIC_EnableIRQ(ADC1_2_IRQn);
    }
    /**
     * Note: This function is disabling interrupt for both ADC1 and ADC2 devices
     */
    void disableInterrupt() { NVIC_DisableIRQ(ADC1_2_IRQn); }

    static void setDualADCMode(DualADCMode dualMode) {
        auto ccr = registers::adc12Common->ccr.volatileLoad();
        ccr.DUAL = static_cast<uint32_t>(dualMode);
        registers::adc12Common->ccr.volatileStore(ccr);
    }

    static void configureDualDMA(Resolution resolution, uint32_t *data, size_t dataSize);

 public:
    Adc(registers::ADC *adc) : adc(*adc) {
        ClockManager::enableADC(1);
        // RCC->AHBENR |= RCC_AHBENR_ADC12EN;
        // RCC->CFGR2 |= RCC_CFGR2_ADCPRE12_4 | RCC_CFGR2_ADCPRE12_3;
        // auto cfgr2 = registers::rcc->cfgr2.volatileLoad();
        // cfgr2.ADC12PRES = 0b11000;
        // registers::rcc->cfgr2.volatileStore(cfgr2);

        enableVoltageRegulator();

        auto ccr = registers::adc12Common->ccr.volatileLoad();
        ccr.VREFEN = 1;
        registers::adc12Common->ccr.volatileStore(ccr);

        if ((int)adc == _MICROHAL_ADC1_BASE_ADDRESS) adc1 = this;
#ifdef _MICROHAL_ADC2_BASE_ADDRESS
        if ((int)adc == _MICROHAL_ADC2_BASE_ADDRESS) adc2 = this;
#endif
    }
    ~Adc();

 private:
    static Adc *adc1;
#ifdef _MICROHAL_ADC2_BASE_ADDRESS
    static Adc *adc2;
#endif
    microhal::registers::ADC &adc;
    microhal::os::Semaphore regularSequenceFinishSemaphore = {};
    Signal<void> signal = {};

    void enableInterrupts(Interrupt interrupts) {
        auto ier = adc.ier.volatileLoad();
        ier |= static_cast<uint32_t>(interrupts);
        adc.ier.volatileStore(ier);
    }

    void setSamplingSequence(uint_fast8_t sequenceLength, uint_fast8_t sequencePosition, Channel channel);

    void enableVoltageRegulator();
    void disableVoltageRegulator();
    void interruptFunction();

    friend void ADC1_2_IRQHandler(void);
    friend void DMA1_Channel1_IRQHandler(void);
};
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
#endif  // _MICROHAL_PORT_STM_ADC_DRIVER_VERSION == 1
#endif  // _MICROHAL_PORTS_ADC_STMCOMMON_H_
