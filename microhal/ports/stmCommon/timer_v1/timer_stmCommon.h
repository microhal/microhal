/*
 * timer_stm32f3xx.h
 *
 *  Created on: May 23, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_TIMER_STMCOMMON_H_
#define _MICROHAL_TIMER_STMCOMMON_H_

#include <cstdint>
#include "../registers/timerRegisters_v1.h"
#include "../stmCommonDefines.h"
#include "gsl/span"
#include "signalSlot/signalSlot.h"

#include _MICROHAL_INCLUDE_PORT_CONFIG

#if !defined(MICROHAL_USE_TIMER_SIGNAL) || MICROHAL_USE_TIMER_SIGNAL > 2
#error MICROHAL_USE_TIMER_SIGNAL have to be defined, possible values 0, 1, 2. 0 - timer signals disabled, 1 - one signal per timer, 2 - signals per timer
#endif

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

extern "C" {
void TIM1_BRK_IRQHandler(void);
void TIM1_UP_IRQHandler(void);
void TIM1_TRG_COM_IRQHandler(void);
void TIM1_CC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM6_IRQHandler(void);
void TIM7_IRQHandler(void);
void TIM8_BRK_IRQHandler(void);
void TIM8_UP_IRQHandler(void);
void TIM8_TRG_COM_IRQHandler(void);
void TIM8_CC_IRQHandler(void);
}

namespace timer_detail {
enum class Interrupt {
    Update = 0b0001,
    CaptureCompare1 = 0b0010,
    CaptureCompare2 = 0b0100,
    CaptureCompare3 = 0b1000,
    CaptureCompare4 = 0b0001'0000,
    COM = 0b0010'0000,
    Trigger = 0b0100'0000,
    Break = 0b1000'0000
};

enum class Event {
    Update = 0b0001,
    CaptureCompare1 = 0b0010,
    CaptureCompare2 = 0b0100,
    CaptureCompare3 = 0b1000,
    CaptureCompare4 = 0b0001'0000,
    COM = 0b0010'0000,
    Trigger = 0b0100'0000,
    Break = 0b1000'0000,
    Break2 = 0b0001'0000'0000
};

constexpr Interrupt operator&(Interrupt a, Interrupt b) {
    return static_cast<Interrupt>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}
constexpr Interrupt operator|(Interrupt a, Interrupt b) {
    return static_cast<Interrupt>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
constexpr Interrupt operator|=(Interrupt &a, Interrupt b) {
    a = a | b;
    return a;
}

constexpr uint32_t operator&(uint32_t a, Interrupt b) {
    return a & static_cast<uint32_t>(b);
}
}  // namespace timer_detail

using timer_detail::operator&;

class Timer {
 public:
    using Interrupt = timer_detail::Interrupt;
    using Event = timer_detail::Event;
    enum class Mode {
        EdgeAligned,    ///<! The counter counts up or down depending on the direction set by function @ref setDirection
        CenterAligned1, /** The counter counts up and down alternatively. Output compare interrupt flags of channels configured in output (CCxS=00
                         *  in TIMx_CCMRx register) are set only when the counter is counting down.
                         */
        CenterAligned2, /** The counter counts up and down alternatively. Output compare interrupt flags of channels configured in output (CCxS=00 in
                         *  TIMx_CCMRx register) are set only when the counter is counting up.
                         */
        CenterAligned3  /** The counter counts up and down alternatively. Output compare interrupt flags of channels configured in output (CCxS=00 in
                         *  TIMx_CCMRx register) are set both when the counter is counting up or down.
                         */
    };
    enum class Direction { UpCounter = 0x0, DownCounter = 0x1 };
    enum class ClockSource { Internal, ETRPin };
    enum class CommutationEventSource { COMEventOnly, COMEventOrTrigger };
    enum class Trigger {
        InternalTrigger0 = 0b000,
        InternalTrigger1,
        InternalTrigger2,
        InternalTrigger3,
        TI1EdgeDetector,
        FilteredTimerInput1,
        FilteredTimerInput2,
        ExternalTriggerInput
    };
    enum class MasterMode { Reset = 0b000, Enable, Update, ComparePulse, Compare1, Compare2, Compare3, Compare4 };
    enum class ADCSynchronizationEvent {
        Reset = 0b000, /* the UG bit from the TIMx_EGR register is used as trigger output (TRGO2). If the reset is generated by the trigger input
                        * (slave mode controller configured in reset mode), the signal on TRGO2 is delayed compared to the actual reset.
                        */
        Enable, /* the Counter Enable signal CNT_EN is used as trigger output (TRGO2). It is useful to start several timers at the same time or to
                 * control a window in which a slave timer is enabled. The Counter Enable signal is generated by a logic OR between the CEN
                 *control bit and the trigger input when configured in Gated mode. When the Counter Enable signal is controlled by the trigger
                 *input, there is a delay on TRGO2, except if the Master/Slave mode is selected (see the MSM bit description in TIMx_SMCR
                 *register).
                 **/
        Update, /* the update event is selected as trigger output (TRGO2). For instance, a master timer can then be used as a prescaler for a
                 * slave timer.
                 */
        ComparePulse,   /* the trigger output sends a positive pulse when the CC1IF flag is to be set (even if it was already high), as soon as a
                         * capture or compare match occurs (TRGO2).
                         */
        Compare1,       // OC1REF signal is used as trigger output (TRGO2)
        Compare2,       // OC2REF signal is used as trigger output (TRGO2)
        Compare3,       // OC3REF signal is used as trigger output (TRGO2)
        Compare4,       // OC4REF signal is used as trigger output (TRGO2)
        Compare5,       // OC5REF signal is used as trigger output (TRGO2)
        Compare6,       // OC6REF signal is used as trigger output (TRGO2)
        Compare4Pulse,  // OC4REF rising or falling edges generate pulses on TRGO2
        Compare6Pulse,  // OC6REF rising or falling edges generate pulses on TRGO2
        Compare4Or6RisingEdge,         // OC4REF or OC6REF rising edges generate pulses on TRGO2
        Compare4RisingOr6FallingEdge,  // OC4REF rising or OC6REF falling edges generate pulses on TRGO2
        Compare5Or6RisinggEdge,        // OC5REF or OC6REF rising edges generate pulses on TRGO2
        Compare5RisingOr6FallingEdge,  // OC5REF rising or OC6REF falling edges generate pulses on TRGO2
    };
    enum class SlaveMode {
        Disabled = 0b000,
        EncoderMode1,
        EncoderMode2,
        EncoderMode3,
        ResetMode,
        GatedMode,
        TriggerMode,
        ExternalClockMode1,
        CombinedResetAndTrigger
    };

#if MICROHAL_USE_TIMER_SIGNAL > 0
    Signal<void> signal1 = {};
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    Signal<void> signal2 = {};
#endif

    Timer(registers::TIM *addr);
    ~Timer();

    /**
     * @brief This function enable timer.
     */
    void enable() {
        auto cr1 = timer.cr1.volatileLoad();
        cr1.CEN.set();
        timer.cr1.volatileStore(cr1);
    }
    /**
     * @brief This function disable timer.
     */
    void disable() {
        auto cr1 = timer.cr1.volatileLoad();
        cr1.CEN.clear();
        timer.cr1.volatileStore(cr1);
    }

    void enableInterupt(uint32_t priority);
    void disableInterrupt();

    void enableInterrupts(Interrupt interrupt) {
        auto dier = timer.dier.volatileLoad();
        dier |= static_cast<uint32_t>(interrupt);
        timer.dier.volatileStore(dier);
    }

    void setMode(Mode mode) {
        auto cr1 = timer.cr1.volatileLoad();
        cr1.CMS = static_cast<uint32_t>(mode);
        timer.cr1.volatileStore(cr1);
    }

    /**
     * When one pulse mode is enabled counter stops counting at the next update event (clearing the bit CEN)
     */
    void enableOnePulseMode() {
        auto cr1 = timer.cr1.volatileLoad();
        cr1.OPM.set();
        timer.cr1.volatileStore(cr1);
    }
    /**
     * When one pulse mode is disabled counter is not stopped at update event
     */
    void disableOnePulseMode() {
        auto cr1 = timer.cr1.volatileLoad();
        cr1.OPM.set();
        timer.cr1.volatileStore(cr1);
    }

    /**
     * @brief This function set timer direction.
     *
     * @param direction - new timer direction.
     */
    void setDirection(Direction direction) {
        auto cr1 = timer.cr1.volatileLoad();
        cr1.DIR = static_cast<uint32_t>(direction);
        timer.cr1.volatileStore(cr1);
    }
    /**
     * This function return current direction setting
     * @return Direction
     */
    Direction getDirection() { return static_cast<Direction>(timer.cr1.volatileLoad().DIR.get()); }

    void enableAutoReload() {
        auto cr1 = timer.cr1.volatileLoad();
        cr1.ARPE.set();
        timer.cr1.volatileStore(cr1);
    }
    void disableAutoReload() {
        auto cr1 = timer.cr1.volatileLoad();
        cr1.ARPE.clear();
        timer.cr1.volatileStore(cr1);
    }

    void setAutoReloadRegister(uint32_t value) { timer.arr.volatileStore(value); }
    uint32_t getAutoReloadRegiser() const { return timer.arr.volatileLoad(); }

    void disableUpdateEventGeneration() {
        auto cr1 = timer.cr1.volatileLoad();
        cr1.UDIS.set();
        timer.cr1.volatileStore(cr1);
    }

    /**
     * @note if 0 is passed as a function parameter it will have the same effect as passing 1.
     * @param prescaler from 0 to 65536, if 0 or 1 is passed it will set prescaler to 1
     */
    void setPrescaler(uint32_t prescaler) {
        if (prescaler == 0) {
            timer.psc.volatileStore(0);
        } else {
            timer.psc.volatileStore(prescaler - 1);
        }
    }
    uint32_t getPrescaler() const { return (uint32_t)timer.psc.volatileLoad() + 1; }
    uint32_t getMaxPrescaler() const { return 65536; }

    void setClockSource(ClockSource clockSource) {
        switch (clockSource) {
            case ClockSource::Internal:
                break;
            case ClockSource::ETRPin: {
                auto smcr = timer.smcr.volatileLoad();
                smcr.ETF.clear();
                smcr.ETPS.clear();
                smcr.ETP.clear();
                smcr.ECE.set();
                timer.smcr.volatileStore(smcr);
            } break;
        }
    }

    void enableCaptureComparePreaload(CommutationEventSource commutationEventSource) {
        auto cr2 = timer.cr2.volatileLoad();
        cr2.CCUS = static_cast<uint32_t>(commutationEventSource);
        cr2.CCPC.set();
        timer.cr2.volatileStore(cr2);
    }

    void disableCaptureComparePreaload() {
        auto cr2 = timer.cr2.volatileLoad();
        cr2.CCPC.clear();
        timer.cr2.volatileStore(cr2);
    }

    uint32_t maxCounterValue() const { return 0xFFFF; }

    uint32_t getTimerClockSourceFrequency() const;
    uint32_t getTimerCounterFrequency() const;
    // one tick duration in [ns]
    uint32_t getTickPeriod() const;

    void configureADCSyncronizationSource(ADCSynchronizationEvent adcSync) {
        auto cr2 = timer.cr2.volatileLoad();
        cr2.MMS = static_cast<uint32_t>(adcSync);
        timer.cr2.volatileStore(cr2);
    }

    void clearInterruptFlag(Timer::Interrupt interrupts) { timer.sr.volatileStore(~static_cast<uint32_t>(interrupts)); }

    class OutputCompare {
     public:
        enum class Mode {
            Frozen = 0,                 //! The comparison between the output compare register TIMx_CCRx and the counter TIMx_CNT has no effect on the
                                        //! outputs.(this mode is used to generate a timing base).
            SetChannelToActiveOnMatch,  //! Set channel x to active level on match. OCxREF signal is forced high when the counter TIMx_CNT
                                        //! matches the capture/compare register
            SetChannelToInactiveOnMatch,  //! Set channel x to inactive level on match. OC1REF signal is forced low when the counter TIMx_CNT
                                          //! matches the capture/compare register x (TIMx_CCRx).
            Toggle,                       //! OCxREF toggles when TIMx_CNT=TIMx_CCRx
            ForceInactiveLevel,           //! OCxREF is forced low.
            ForceActiveLevel,             //! OCxREF is forced high.
            PWMMode1,  //! In upcounting, channel x is active as long as TIMx_CNT<TIMx_CCRx else inactive. In downcounting, channel x is
                       //! inactive (OCxREF=‘0’) as long as TIMx_CNT>TIMx_CCR1 else active (OCxREF=’1’).
            PWMMode2,  //! In upcounting, channel 1 is inactive as long as TIMx_CNT<TIMx_CCR1 else active. In downcounting, channel 1 is
                       //! active as long as TIMx_CNT>TIMx_CCR1 else inactive
            OnePulseMode1,
            OnePulseMode2,
            CombinedPWMMode1 = 0b1100,  //! OC1REF has the same behavior as in PWM mode 1. OC1REFC is the logical OR between OC1REF and OC2REF.
            CombinedPWMMode2,           //! OC1REF has the same behavior as in PWM mode 2. OC1REFC is the logical AND between OC1REF and OC2REF.
            AssymmetricPWMMode1,        //! OC1REF has the same behavior as in PWM mode 1. OC1REFC outputs OC1REF when the counter is counting up,
                                        //! OC2REF when it is counting down.
            AssymmetricPWMMode2         //! OC1REF has the same behavior as in PWM mode 2. OC1REFC outputs OC1REF when the counter is counting up,
                                        //! OC2REF when it is counting down.
        };

        enum class Polarity { ActiveHigh = 0, ActiveLow };
        enum class IdleState { Low, High };

        void enable() {
            if (compareChannelNumber < 2) {
                auto ccmr1 = timer.ccmr1.volatileLoad();
                switch (compareChannelNumber) {
                    case 0:
                        ccmr1.bitfieldOutput.CC1S.clear();
                        break;
                    case 1:
                        ccmr1.bitfieldOutput.CC2S.clear();
                        break;
                }
                timer.ccmr1.volatileStore(ccmr1);
            } else {
                auto ccmr2 = timer.ccmr2.volatileLoad();
                switch (compareChannelNumber) {
                    case 2:
                        ccmr2.bitfieldOutput.CC3S.clear();
                        break;
                    case 3:
                        ccmr2.bitfieldOutput.CC4S.clear();
                        break;
                }
                timer.ccmr2.volatileStore(ccmr2);
            }
        }

        void setValue(uint32_t value) { return timer.ccr[compareChannelNumber].volatileStore(value); }
        uint32_t getValue() { return timer.ccr[compareChannelNumber].volatileLoad(); }
        void enablePreload() {
            if (compareChannelNumber < 2) {
                auto ccmr1 = timer.ccmr1.volatileLoad();
                switch (compareChannelNumber) {
                    case 0:
                        ccmr1.bitfieldOutput.OC1PE.set();
                        break;
                    case 1:
                        ccmr1.bitfieldOutput.OC2PE.set();
                        break;
                }
                timer.ccmr1.volatileStore(ccmr1);
            } else {
                auto ccmr2 = timer.ccmr2.volatileLoad();
                switch (compareChannelNumber) {
                    case 2:
                        ccmr2.bitfieldOutput.OC3PE.set();
                        break;
                    case 3:
                        ccmr2.bitfieldOutput.OC4PE.set();
                        break;
                }
                timer.ccmr2.volatileStore(ccmr2);
            }
        }
        void disablePreload() {
            if (compareChannelNumber < 2) {
                auto ccmr1 = timer.ccmr1.volatileLoad();
                switch (compareChannelNumber) {
                    case 0:
                        ccmr1.bitfieldOutput.OC1PE.clear();
                        break;
                    case 1:
                        ccmr1.bitfieldOutput.OC2PE.clear();
                        break;
                }
                timer.ccmr1.volatileStore(ccmr1);
            } else {
                auto ccmr2 = timer.ccmr2.volatileLoad();
                switch (compareChannelNumber) {
                    case 2:
                        ccmr2.bitfieldOutput.OC3PE.clear();
                        break;
                    case 3:
                        ccmr2.bitfieldOutput.OC4PE.clear();
                        break;
                }
                timer.ccmr2.volatileStore(ccmr2);
            }
        }
        void setMode(Mode mode) {
            if (compareChannelNumber < 2) {
                auto ccmr1 = timer.ccmr1.volatileLoad();
                switch (compareChannelNumber) {
                    case 0:
                        ccmr1.setOC1M(static_cast<uint32_t>(mode));
                        break;
                    case 1:
                        ccmr1.setOC2M(static_cast<uint32_t>(mode));
                        break;
                }
                timer.ccmr1.volatileStore(ccmr1);
            } else {
                auto ccmr2 = timer.ccmr2.volatileLoad();
                switch (compareChannelNumber) {
                    case 2:
                        ccmr2.setOC3M(static_cast<uint32_t>(mode));
                        break;
                    case 3:
                        ccmr2.setOC4M(static_cast<uint32_t>(mode));
                        break;
                }
                timer.ccmr2.volatileStore(ccmr2);
            }
        }
        void configureOutput(Polarity polarity, IdleState idleState) {
            auto ccer = timer.ccer.volatileLoad();
            auto cr2 = timer.cr2.volatileLoad();
            switch (compareChannelNumber) {
                case 0:
                    ccer.CC1P = static_cast<uint32_t>(polarity);
                    cr2.OIS1 = static_cast<uint32_t>(idleState);
                    break;
                case 1:
                    ccer.CC2P = static_cast<uint32_t>(polarity);
                    cr2.OIS2 = static_cast<uint32_t>(idleState);
                    break;
                case 2:
                    ccer.CC3P = static_cast<uint32_t>(polarity);
                    cr2.OIS3 = static_cast<uint32_t>(idleState);
                    break;
                case 3:
                    ccer.CC4P = static_cast<uint32_t>(polarity);
                    cr2.OIS4 = static_cast<uint32_t>(idleState);
                    break;
            }
            timer.ccer.volatileStore(ccer);
            timer.cr2.volatileStore(cr2);
        }
        /**
         * Note: idleState is by default disabled. To enable idleState functionality call TODO function.
         * @param polarity
         * @param idleState
         */
        void configureComplementaryOutput(Polarity polarity, IdleState idleState) {
            auto ccer = timer.ccer.volatileLoad();
            auto cr2 = timer.cr2.volatileLoad();
            switch (compareChannelNumber) {
                case 0:
                    ccer.CC1NP = static_cast<uint32_t>(polarity);
                    cr2.OIS1N = static_cast<uint32_t>(idleState);
                    break;
                case 1:
                    ccer.CC2NP = static_cast<uint32_t>(polarity);
                    cr2.OIS2N = static_cast<uint32_t>(idleState);
                    break;
                case 2:
                    ccer.CC3NP = static_cast<uint32_t>(polarity);
                    cr2.OIS3N = static_cast<uint32_t>(idleState);
                    break;
            }
            timer.ccer.volatileStore(ccer);
            timer.cr2.volatileStore(cr2);
        }

        void outputEnable() {
            auto ccer = timer.ccer.volatileLoad();
            switch (compareChannelNumber) {
                case 0:
                    ccer.CC1E.set();
                    break;
                case 1:
                    ccer.CC2E.set();
                    break;
                case 2:
                    ccer.CC3E.set();
                    break;
                case 3:
                    ccer.CC4E.set();
                    break;
            }
            timer.ccer.volatileStore(ccer);
        }

        void complementaryOutputEnable() {
            auto ccer = timer.ccer.volatileLoad();
            switch (compareChannelNumber) {
                case 0:
                    ccer.CC1NE.set();
                    break;
                case 1:
                    ccer.CC2NE.set();
                    break;
                case 2:
                    ccer.CC3NE.set();
                    break;
            }
            timer.ccer.volatileStore(ccer);
        }

        void outputDisable() {
            auto ccer = timer.ccer.volatileLoad();
            switch (compareChannelNumber) {
                case 0:
                    ccer.CC1E.clear();
                    break;
                case 1:
                    ccer.CC2E.clear();
                    break;
                case 2:
                    ccer.CC3E.clear();
                    break;
                case 3:
                    ccer.CC4E.clear();
                    break;
            }
            timer.ccer.volatileStore(ccer);
        }
        void complementaryOutputDisable() {
            auto ccer = timer.ccer.volatileLoad();
            switch (compareChannelNumber) {
                case 0:
                    ccer.CC1NE.clear();
                    break;
                case 1:
                    ccer.CC2NE.clear();
                    break;
                case 2:
                    ccer.CC3NE.clear();
                    break;
            }
            timer.ccer.volatileStore(ccer);
        }

     private:
        registers::TIM &timer;
        uint_fast8_t compareChannelNumber;

        OutputCompare(registers::TIM &timer, uint_fast8_t channel) : timer(timer), compareChannelNumber(channel) {}
        friend Timer;
    };

    class InputCapture {
     public:
        enum class InputSource { TRC = 0b11, TI1 = 0b01, TI2 = 0b10, TI3 = 0b101, TI4 = 0b110 };
        enum class InputPrescaler { None = 0, Prescaler2, Prescaler4, Prescaler8 };
        enum class ActiveEdge { Rising = 0b00, Falling = 0b01, RisingAndFalling = 0b11 };
        enum class InputFilter { None = 0, Fck_int_N2, Fck_int_N4, Fck_int_N8, Fdts2_N6, Fdts2_N8, Fdts4_N6, Fdts4_N8 };

        void enable() {
            auto ccer = timer.ccer.volatileLoad();
            switch (captureChannelNumber) {
                case 0:
                    ccer.CC1E.set();
                    break;
                case 1:
                    ccer.CC2E.set();
                    break;
                case 2:
                    ccer.CC3E.set();
                    break;
                case 3:
                    ccer.CC4E.set();
                    break;
            }
            timer.ccer.volatileStore(ccer);
        }

        void disable() {
            auto ccer = timer.ccer.volatileLoad();
            switch (captureChannelNumber) {
                case 0:
                    ccer.CC1E.clear();
                    break;
                case 1:
                    ccer.CC2E.clear();
                    break;
                case 2:
                    ccer.CC3E.clear();
                    break;
                case 3:
                    ccer.CC4E.clear();
                    break;
            }
            timer.ccer.volatileStore(ccer);
        }

        bool setInputSource(InputSource inputSource, ActiveEdge activeEdge, InputFilter filter) {
            switch (captureChannelNumber) {
                case 0: {
                    if (static_cast<uint32_t>(inputSource) & 0b100) return false;  // Selected TI3 or TI4 witch is forbidden on this channel
                    auto ccmr1 = timer.ccmr1.volatileLoad();
                    ccmr1.bitfieldInput.CC1S = static_cast<uint32_t>(inputSource);
                    if (inputSource == InputSource::TI1) {
                        ccmr1.bitfieldInput.IC1F = static_cast<uint32_t>(filter);
                    } else if (inputSource == InputSource::TI2) {
                        ccmr1.bitfieldInput.IC2F = static_cast<uint32_t>(filter);
                    }
                    timer.ccmr1.volatileStore(ccmr1);
                    auto ccer = timer.ccer.volatileLoad();
                    ccer.CC1P = static_cast<uint32_t>(activeEdge);
                    timer.ccer.volatileStore(ccer);
                    return true;
                }
                case 1: {
                    if (static_cast<uint32_t>(inputSource) & 0b100) return false;  // Selected TI3 or TI4 witch is forbidden on this channel
                    static constexpr const uint8_t map[4] = {0b00, 0b10, 0b01, 0b11};
                    auto ccmr1 = timer.ccmr1.volatileLoad();
                    ccmr1.bitfieldInput.CC2S = map[static_cast<uint32_t>(inputSource)];
                    if (inputSource == InputSource::TI1) {
                        ccmr1.bitfieldInput.IC1F = static_cast<uint32_t>(filter);
                    } else if (inputSource == InputSource::TI2) {
                        ccmr1.bitfieldInput.IC2F = static_cast<uint32_t>(filter);
                    }
                    timer.ccmr1.volatileStore(ccmr1);
                    auto ccer = timer.ccer.volatileLoad();
                    ccer.CC2P = static_cast<uint32_t>(activeEdge);
                    timer.ccer.volatileStore(ccer);
                    return true;
                }
                case 2: {
                    if (!(static_cast<uint32_t>(inputSource) & 0b100)) return false;  // Selected TI1 or TI2 witch is forbidden on this channel
                    auto ccmr2 = timer.ccmr2.volatileLoad();
                    ccmr2.bitfieldInput.CC3S =
                        static_cast<uint32_t>(inputSource);  // no need to mask MSB bit, it will be truncated during field assignment
                    if (inputSource == InputSource::TI3) {
                        ccmr2.bitfieldInput.IC3F = static_cast<uint32_t>(filter);
                    } else if (inputSource == InputSource::TI4) {
                        ccmr2.bitfieldInput.IC4F = static_cast<uint32_t>(filter);
                    }
                    timer.ccmr2.volatileStore(ccmr2);
                    auto ccer = timer.ccer.volatileLoad();
                    ccer.CC3P = static_cast<uint32_t>(activeEdge);
                    timer.ccer.volatileStore(ccer);
                    return true;
                }
                case 3: {
                    if (!(static_cast<uint32_t>(inputSource) & 0b100)) return false;  // Selected TI1 or TI2 witch is forbidden on this channel
                    static constexpr const uint8_t map2[4] = {0b00, 0b10, 0b01, 0b11};
                    auto ccmr2 = timer.ccmr2.volatileLoad();
                    ccmr2.bitfieldInput.CC4S = map2[static_cast<uint32_t>(inputSource) & 0b11];
                    if (inputSource == InputSource::TI3) {
                        ccmr2.bitfieldInput.IC3F = static_cast<uint32_t>(filter);
                    } else if (inputSource == InputSource::TI4) {
                        ccmr2.bitfieldInput.IC4F = static_cast<uint32_t>(filter);
                    }
                    timer.ccmr2.volatileStore(ccmr2);
                    auto ccer = timer.ccer.volatileLoad();
                    ccer.CC4P = static_cast<uint32_t>(activeEdge);
                    timer.ccer.volatileStore(ccer);
                    return true;
                }
            }
            return false;
        }

        void setInputPrescaler(InputPrescaler inputPrescaler) {
            if (captureChannelNumber < 2) {
                auto ccmr1 = timer.ccmr1.volatileLoad();
                switch (captureChannelNumber) {
                    case 0:
                        ccmr1.bitfieldInput.IC1PSC = static_cast<uint32_t>(inputPrescaler);
                        break;
                    case 1:
                        ccmr1.bitfieldInput.IC2PSC = static_cast<uint32_t>(inputPrescaler);
                        break;
                }
                timer.ccmr1.volatileStore(ccmr1);
            } else {
                auto ccmr2 = timer.ccmr2.volatileLoad();
                switch (captureChannelNumber) {
                    case 2:
                        ccmr2.bitfieldInput.IC3PSC = static_cast<uint32_t>(inputPrescaler);
                        break;
                    case 3:
                        ccmr2.bitfieldInput.IC4PSC = static_cast<uint32_t>(inputPrescaler);
                        break;
                }
                timer.ccmr2.volatileStore(ccmr2);
            }
        }

        uint32_t getValue() { return timer.ccr[captureChannelNumber].volatileLoad(); }

     private:
        registers::TIM &timer;
        uint_fast8_t captureChannelNumber;

        InputCapture(registers::TIM &timer, uint_fast8_t channel) : timer(timer), captureChannelNumber(channel) {}
        friend Timer;
    };

    void enableXORInput() {
        auto cr2 = timer.cr2.volatileLoad();
        cr2.TI1S.set();
        timer.cr2.volatileStore(cr2);
    }
    void disableXORInput() {
        auto cr2 = timer.cr2.volatileLoad();
        cr2.TI1S.clear();
        timer.cr2.volatileStore(cr2);
    }
    void generateEvent(Event event) { timer.egr.volatileStore(static_cast<uint32_t>(event)); }

    void enableCompareOutputs() {
        auto bdtr = timer.bdtr.volatileLoad();
        bdtr.MOE.set();
        timer.bdtr.volatileStore(bdtr);
    }
    uint32_t getValue() { return timer.cnt.volatileLoad(); }
    void setValue(uint32_t value) { timer.cnt.volatileStore(value); }

    enum class OffStateForRunMode { HiZ_GPIO, InactiveState };
    enum class OffStateForIdleMode { HiZ_GPIO, IdleState };
    void configureOffState(OffStateForRunMode ossr, OffStateForIdleMode ossi) {
        auto bdtr = timer.bdtr.volatileLoad();
        bdtr.OSSR = static_cast<uint32_t>(ossr);
        bdtr.OSSI = static_cast<uint32_t>(ossi);
        timer.bdtr.volatileStore(bdtr);
    }

    void configureMasterMode(MasterMode masterMode) {
        auto cr2 = timer.cr2.volatileLoad();
        cr2.MMS = static_cast<uint32_t>(masterMode);
        timer.cr2.volatileStore(cr2);
    }
    void configureSlaveMode(SlaveMode slaveMode, Trigger triggerSource) {
        auto smcr = timer.smcr.volatileLoad();
        smcr.SMS = static_cast<uint32_t>(slaveMode);
        smcr.TS = static_cast<uint32_t>(triggerSource);
        timer.smcr.volatileStore(smcr);
    }

    OutputCompare compare(uint_fast8_t i) { return OutputCompare(timer, i - 1); }
    InputCapture capture(uint_fast8_t i) { return InputCapture(timer, i - 1); }

#if MICROHAL_USE_TIMER_SIGNAL > 0
    bool setSignal1Sources(Interrupt sources) {
        signalInterrupt1 = sources;
        return true;
    }
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    bool setSignal2Sources(Interrupt sources) {
        signalInterrupt2 = sources;
        return true;
    }
#endif

 private:
    registers::TIM &timer;
#if MICROHAL_USE_TIMER_SIGNAL > 0
    Interrupt signalInterrupt1{};
#endif
#if MICROHAL_USE_TIMER_SIGNAL == 2
    Interrupt signalInterrupt2{};
#endif

    static Timer *tim[8];

    /**
     *
     * @return timer number form 0, in example for tim1 function will return 0;
     */
    uint8_t getNumber() const;

    void interruptFunction();

    friend void TIM1_BRK_IRQHandler(void);
    friend void TIM1_UP_IRQHandler(void);
    friend void TIM1_TRG_COM_IRQHandler(void);
    friend void TIM1_CC_IRQHandler(void);
    friend void TIM2_IRQHandler(void);
    friend void TIM3_IRQHandler(void);
    friend void TIM4_IRQHandler(void);
    friend void TIM6_IRQHandler(void);
    friend void TIM7_IRQHandler(void);
    friend void TIM8_BRK_IRQHandler(void);
    friend void TIM8_UP_IRQHandler(void);
    friend void TIM8_TRG_COM_IRQHandler(void);
    friend void TIM8_CC_IRQHandler(void);
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* _MICROHAL_TIMER_STMCOMMON_H_ */
