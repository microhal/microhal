/*
 * timer_stm32f3xx.h
 *
 *  Created on: May 23, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_TIMER_STM32F3XX_H_
#define _MICROHAL_TIMER_STM32F3XX_H_

#include <cstdint>
#include "clockManager.h"
#include "device/stm32f3xx.h"
#include "gsl/span"

namespace microhal {
namespace stm32f3xx {

template <typename type, int begin, int end>
class myBitfield {
 public:
    myBitfield &operator=(type value) {
        uint32_t tmp = data;
        tmp &= ~bitfieldMask;
        tmp |= (value & valueMask) << begin;
        data = tmp;
        return *this;
    }

    myBitfield &operator|=(type value) {
        uint32_t tmp = data;
        tmp |= (value & valueMask) << begin;
        data = tmp;
        return *this;
    }

    myBitfield &operator&=(type value) {
        uint32_t tmp = data;
        tmp &= (value | ~valueMask) << begin;
        data = tmp;
        return *this;
    }

    operator type() { return get(); }

    void set() { data |= bitfieldMask; }
    void clear() { data &= bitfieldMask; }
    type get() { return (data >> begin) & valueMask; }

 private:
    type data;

    static constexpr uint32_t maskGen(int maskBegin, int maskEnd) {
        uint32_t mask = 0;
        for (int i = maskBegin; i <= maskEnd; i++) {
            mask |= 1 << i;
        }
        return mask;
    }
    static_assert(maskGen(0, 0) == 0b01, "");
    static_assert(maskGen(0, 1) == 0b11, "");

    enum { bitfieldMask = maskGen(begin, end), valueMask = maskGen(0, end - begin) };
};

struct TimerRegisterMap {
    union CR1_t {
        myBitfield<uint32_t, 0, 0> CEN;   // Counter enable
        myBitfield<uint32_t, 1, 1> UDIS;  // Update disable
        myBitfield<uint32_t, 2, 2> URS;   // Update request source
        myBitfield<uint32_t, 3, 3> OPM;   // One pulse mode
        myBitfield<uint32_t, 4, 4> DIR;   // Direction; 0 -> upcounter1, 1 -> downcounter
        myBitfield<uint32_t, 5, 6> CMS;   // Center-aligned mode selection
        myBitfield<uint32_t, 7, 7> ARPE;  // Auto-reload preload enable
        myBitfield<uint32_t, 8, 9> CKD;   // Clock division
        // bit 10 is reserved
        myBitfield<uint32_t, 11, 11> UIFREMAP;  // UIF status bit remapping

        uint32_t raw;
    };
    static_assert(sizeof(CR1_t) == sizeof(uint32_t), "Microhal internal error, probably incorrect toolchain configuration.");

    union CR2_t {
        myBitfield<uint32_t, 0, 0> CCPC;  // Capture/compare preload control
        // bit 1 is reserved, must be kept at reset value
        myBitfield<uint32_t, 2, 2> CCUS;     // Capture/compare control update selection
        myBitfield<uint32_t, 3, 3> CCDS;     // Capture/compare DMA selection
        myBitfield<uint32_t, 4, 6> MMS;      // Master mode selection
        myBitfield<uint32_t, 7, 7> TI1S;     // TI1 selection
        myBitfield<uint32_t, 8, 8> OIS1;     // Output Idle state 1 (OC1 output)
        myBitfield<uint32_t, 9, 9> OIS1N;    // Output Idle state 1 (OC1N output)
        myBitfield<uint32_t, 10, 10> OIS2;   // Output Idle state 2 (OC2 output)
        myBitfield<uint32_t, 11, 11> OIS2N;  // Output Idle state 2 (OC2N output)
        myBitfield<uint32_t, 12, 12> OIS3;   // Output Idle state 3 (OC3 output)
        myBitfield<uint32_t, 13, 13> OIS3N;  // Output Idle state 3 (OC3N output)
        myBitfield<uint32_t, 14, 14> OIS4;   // Output Idle state 4 (OC4 output)
        // bit 15 is reserved, must be kept at reset value.
        myBitfield<uint32_t, 16, 16> OIS5;  // Output Idle state 5 (OC5 output)
        // bit 17 Reserved, must be kept at reset value.
        myBitfield<uint32_t, 18, 18> OIS6;  // Output Idle state 6 (OC6 output)
        // bit 19 is reserved, must be kept at reset value.
        myBitfield<uint32_t, 20, 23> MMS2;  // Master mode selection 2

        uint32_t raw;
    };

    union SMCR_t {
        // myBitfield<uint32_t, 0, 2> SMSa;    // Slave mode selection
        myBitfield<uint32_t, 3, 3> OCCS;    // OCREF clear selection
        myBitfield<uint32_t, 4, 6> TS;      //  Trigger selection
        myBitfield<uint32_t, 7, 7> MSM;     // Master/slave mode
        myBitfield<uint32_t, 8, 11> ETF;    // External trigger filter
        myBitfield<uint32_t, 12, 13> ETPS;  // External trigger prescaler
        myBitfield<uint32_t, 14, 14> ECE;   // External clock enable
        myBitfield<uint32_t, 15, 15> ETP;   // External trigger polarity
        // myBitfield<uint32_t, 16, 16> SMSb ;  // Slave mode selection

        uint32_t raw;

        void setSMS(uint32_t sms) {
            uint32_t tmp = raw;
            tmp &= ~0x0001'0007;
            tmp |= (sms & 0b111) | ((sms & 0b1000) << 13);
            raw = tmp;
        }

        uint32_t getSMS() {
            uint32_t tmp = raw;
            uint32_t sms = tmp & 0b111;
            sms |= (tmp >> 13) & 0b1000;
            return sms;
        }
    };

    union CCMR1_t {
        union BitfieldInput {
            myBitfield<uint32_t, 0, 1> CC1S;      // Capture/Compare 1 Selection
            myBitfield<uint32_t, 2, 3> IC1PSC;    // Input capture 1 prescaler
            myBitfield<uint32_t, 4, 7> IC1F;      // Input capture 1 filte
            myBitfield<uint32_t, 8, 9> CC2S;      // Capture/Compare 2 selection
            myBitfield<uint32_t, 10, 11> IC2PSC;  // Input capture 2 prescaler
            myBitfield<uint32_t, 12, 15> IC2F;    // Input capture 2 filter
        };
        union BitfieldOutput {
            myBitfield<uint32_t, 0, 1> CC1S;     /* Capture/Compare 1 Selection
                                                  * 00: CC1 channel is configured as output
                                                  * 01: CC1 channel is configured as input, IC1 is mapped on TI1
                                                  * 10: CC1 channel is configured as input, IC1 is mapped on TI2
                                                  * 11: CC1 channel is configured as input, IC1 is mapped on TRC.
                                                  *     This mode is working only if an internal trigger input
                                                  *     is selected through TS bit (TIMx_SMCR register)
                                                  */
            myBitfield<uint32_t, 2, 2> OC1FE;    /* Output Compare 1 fast enable
                                                  * 0: CC1 behaves normally depending on counter and CCR1 values even
                                                  *    when the trigger is ON. The minimum delay to activate CC1 output
                                                  *    when an edge occurs on the trigger input is 5 clock cycles.
                                                  * 1: An active edge on the trigger input acts like a compare match
                                                  *    on CC1 output. Then, OC is set to the compare level independently
                                                  *    from the result of the comparison. Delay to sample the trigger
                                                  *    input and to activate CC1 output is reduced to 3 clock cycles.
                                                  *    OCFE acts only if the channel is configured in PWM1 or PWM2 mode.
                                                  */
            myBitfield<uint32_t, 3, 3> OC1PE;    /* Output Compare 1 preload enable
                                                  * 0: Preload register on TIMx_CCR1 disabled. TIMx_CCR1 can be written
                                                  *    at anytime, the new value is taken in account immediately.
                                                  * 1: Preload register on TIMx_CCR1 enabled. Read/Write operations
                                                  *    access the preload register. TIMx_CCR1 preload value is loaded
                                                  *    in the active register at each update event.
                                                  */
            myBitfield<uint32_t, 4, 6> OC1Ma;    /* Output Compare 1 mode
                                                  * 0000: Frozen - The comparison between the output compare register
                                                  *       TIMx_CCR1 and the counter TIMx_CNT has no effect on the outputs.
                                                  * 0001: Set channel 1 to active level on match. OC1REF signal is forced
                                                  *       high when the counter TIMx_CNT matches the capture/compare register
                                                  * 0010: Set channel 1 to inactive level on match. OC1REF signal is forced
                                                  *       low when the counter TIMx_CNT matches the capture/compare register
                                                  * 0011: Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCR1.
                                                  * 0100: Force inactive level - OC1REF is forced low.
                                                  * 0101: Force active level - OC1REF is forced high.
                                                  * 0110: PWM mode 1
                                                  * 0111: PWM mode 2
                                                  * 1000: Retrigerrable OPM mode 1
                                                  * 1001: Retrigerrable OPM mode 2
                                                  * 1010: Reserved
                                                  * 1011: Reserved
                                                  * 1100: Combined PWM mode 1
                                                  * 1101: Combined PWM mode 2
                                                  * 1110: Asymmetric PWM mode 1
                                                  * 1111: Asymmetric PWM mode 2
                                                  */
            myBitfield<uint32_t, 7, 7> OC1CE;    // Output Compare 1 clear enable
            myBitfield<uint32_t, 8, 9> CC2S;     // Capture/Compare 2 selection
            myBitfield<uint32_t, 10, 10> OC2FE;  // Output Compare 2 fast enable
            myBitfield<uint32_t, 11, 11> OC2PE;  // Output Compare 2 preload enable
            myBitfield<uint32_t, 12, 14> OC2Ma;  // Output Compare 2 mode
            myBitfield<uint32_t, 15, 15> OC2CE;  // Output Compare 2 clear enable
            myBitfield<uint32_t, 16, 16> OC1Mb;  // Output Compare 1 mode - bit 3
                                                 // bits 17 - 23 are reserved, must be kept at reset value.
            myBitfield<uint32_t, 24, 24> OC2Mb;  // Output Compare 2 mode - bit 3
        };

        void setOC1M(uint32_t value) {
            uint32_t tmp = raw;
            tmp &= ~0x0001'0070;
            tmp |= (value & 0b0111) << 4;
            tmp |= (value & 0b1000) << 13;
            raw = tmp;
        }
        void setOC2M(uint32_t value) {
            uint32_t tmp = raw;
            tmp &= ~0x0100'7000;
            tmp |= (value & 0b0111) << 12;
            tmp |= (value & 0b1000) << 21;
            raw = tmp;
        }
        BitfieldInput bitfieldInput;
        BitfieldOutput bitfieldOutput;
        uint32_t raw;
    };

    union CCMR2_t {
        union BitfieldInput {
            myBitfield<uint32_t, 0, 1> CC3S;      // Capture/Compare 3 Selection
            myBitfield<uint32_t, 2, 3> IC3PSC;    // Input capture 3 prescaler
            myBitfield<uint32_t, 4, 7> IC3F;      // Input capture 3 filte
            myBitfield<uint32_t, 8, 9> CC4S;      // Capture/Compare 4 selection
            myBitfield<uint32_t, 10, 11> IC4PSC;  // Input capture 4 prescaler
            myBitfield<uint32_t, 12, 15> IC4F;    // Input capture 4 filter
        };
        union BitfieldOutput {
            myBitfield<uint32_t, 0, 1> CC3S;     // Capture/Compare 1 Selection, for more information loock at CCMR1_t
            myBitfield<uint32_t, 2, 2> OC3FE;    // Output Compare 1 fast enable, for more information loock at CCMR1_t
            myBitfield<uint32_t, 3, 3> OC3PE;    // Output Compare 1 preload enable, for more information loock at CCMR1_t
            myBitfield<uint32_t, 4, 6> OC3Ma;    // Output Compare 1 mode, for more information loock at CCMR1_t
            myBitfield<uint32_t, 7, 7> OC3CE;    // Output Compare 1 clear enable, for more information loock at CCMR1_t
            myBitfield<uint32_t, 8, 9> CC4S;     // Capture/Compare 2 selection, for more information loock at CCMR1_t
            myBitfield<uint32_t, 10, 10> OC4FE;  // Output Compare 2 fast enable, for more information loock at CCMR1_t
            myBitfield<uint32_t, 11, 11> OC4PE;  // Output Compare 2 preload enable, for more information loock at CCMR1_t
            myBitfield<uint32_t, 12, 14> OC4Ma;  // Output Compare 2 mode, for more information loock at CCMR1_t
            myBitfield<uint32_t, 15, 15> OC4CE;  // Output Compare 2 clear enable, for more information loock at CCMR1_t
            myBitfield<uint32_t, 16, 16> OC3Mb;  // Output Compare 1 mode - bit 3, for more information loock at CCMR1_t
                                                 // bits 17 - 23 are reserved, must be kept at reset value.
            myBitfield<uint32_t, 24, 24> OC4Mb;  // Output Compare 2 mode - bit 3, for more information loock at CCMR1_t
        };
        void setOC3M(uint32_t value) {
            uint32_t tmp = raw;
            tmp &= ~0x0001'0070;
            tmp |= (value & 0b0111) << 4;
            tmp |= (value & 0b1000) << 13;
            raw = tmp;
        }
        void setOC4M(uint32_t value) {
            uint32_t tmp = raw;
            tmp &= ~0x0100'7000;
            tmp |= (value & 0b0111) << 12;
            tmp |= (value & 0b1000) << 21;
            raw = tmp;
        }

        BitfieldInput bitfieldInput;
        BitfieldOutput bitfieldOutput;
        uint32_t raw;
    };
    union CCER_t {
        myBitfield<uint32_t, 0, 0> CC1E;     /* Capture/Compare 1 output enable
                                              * CC1 channel configured as output:
                                              * 0: Off - OC1 is not active. OC1 level is then function
                                              *    of MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits.
                                              * 1: On - OC1 signal is output on the corresponding output pin
                                              *    depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits.
                                              */
        myBitfield<uint32_t, 1, 1> CC1P;     /* Capture/Compare 1 output polarity;
                                              * CC1 channel configured as output:
                                              * 0: OC1 active high
                                              * 1: OC1 active low
                                              */
        myBitfield<uint32_t, 2, 2> CC1NE;    /* Capture/Compare 1 complementary output enable
                                              * 0: Off - OC1N is not active. OC1N level is then function
                                              *    of MOE, OSSI, OSSR, OIS1, OIS1N and CC1E bits.
                                              * 1: On - OC1N signal is output on the corresponding output pin
                                              *    depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1E bits.
                                              */
        myBitfield<uint32_t, 3, 3> CC1NP;    /* Capture/Compare 1 complementary output polarity
                                              * CC1 channel configured as output:
                                              * 0: OC1N active high.
                                              * 1: OC1N active low.
                                              */
        myBitfield<uint32_t, 4, 4> CC2E;     // Capture/Compare 2 output enableRefer to CC1E description
        myBitfield<uint32_t, 5, 5> CC2P;     // Capture/Compare 2 output polarityRefer to CC1P description
        myBitfield<uint32_t, 6, 6> CC2NE;    // Capture/Compare 2 complementary output enableRefer to CC1NE description
        myBitfield<uint32_t, 7, 7> CC2NP;    // Capture/Compare 2 complementary output polarityRefer to CC1NP description
        myBitfield<uint32_t, 8, 8> CC3E;     // Capture/Compare 3 output enableRefer to CC1E description
        myBitfield<uint32_t, 9, 9> CC3P;     // Capture/Compare 3 output polarityRefer to CC1P description
        myBitfield<uint32_t, 10, 10> CC3NE;  // Capture/Compare 3 complementary output enableRefer to CC1NE description
        myBitfield<uint32_t, 11, 11> CC3NP;  // Capture/Compare 3 complementary output polarityRefer to CC1NP description
        myBitfield<uint32_t, 12, 12> CC4E;   // Capture/Compare 4 output enableRefer to CC1E description
        myBitfield<uint32_t, 13, 13> CC4P;   // Capture/Compare 4 output polarityRefer to CC1P description
        // bit 14 reserved
        myBitfield<uint32_t, 15, 15> CC4NP;  // Capture/Compare 4 complementary output polarityRefer to CC1NP description
        myBitfield<uint32_t, 16, 16> CC5E;   // Capture/Compare 5 output enableRefer to CC1E description
        myBitfield<uint32_t, 17, 17> CC5P;   // Capture/Compare 5 output polarityRefer to CC1P description
        // bits 18-19 are reserved, must be kept at reset value.
        myBitfield<uint32_t, 20, 20> CC6E;  // Capture/Compare 6 output enableRefer to CC1E description
        myBitfield<uint32_t, 3, 3> CC6P;    // Capture/Compare 6 output polarityRefer to CC1P description
        // bits 22 - 31 reserved

        uint32_t raw;
    };

    union BDTR_t {
        myBitfield<uint32_t, 0, 7> DTG;
        myBitfield<uint32_t, 8, 9> LOCK;
        myBitfield<uint32_t, 10, 10> OSSI;
        myBitfield<uint32_t, 11, 11> OSSR;
        myBitfield<uint32_t, 12, 12> BKE;
        myBitfield<uint32_t, 13, 13> BKP;
        myBitfield<uint32_t, 14, 14> AOE;
        myBitfield<uint32_t, 15, 15> MOE;
        myBitfield<uint32_t, 16, 19> BKF;
        myBitfield<uint32_t, 20, 23> BK2F;
        myBitfield<uint32_t, 24, 24> BK2E;
        myBitfield<uint32_t, 25, 25> BK2P;

        uint32_t raw;
    };

    CR1_t CR1;
    CR2_t CR2;
    SMCR_t SMCR;
    uint32_t DIER;
    uint32_t SR;
    uint32_t EGR;    // event generation register
    CCMR1_t CCMR1;   // capture/compare mode register 1
    CCMR2_t CCMR2;   // capture/compare mode register 2
    CCER_t CCER;     // capture/compare enable register
    uint32_t CNT;    // counter
    uint32_t PSC;    // prescaler
    uint32_t ARR;    // auto-reload register
    uint32_t RCR;    // repetition counter register
    uint32_t CCR1;   // capture/compare register 1
    uint32_t CCR2;   // capture/compare register 2
    uint32_t CCR3;   // capture/compare register 3
    uint32_t CCR4;   // capture/compare register 4
    BDTR_t BDTR;     // break and dead-time register
    uint32_t DCR;    // DMA control register
    uint32_t DMAR;   // DMA address for full transfer
    uint32_t OR;     // option registers
    uint32_t CCMR3;  // capture/compare mode register 3
    uint32_t CCR5;   // capture/compare register 5
    uint32_t CCR6;   // capture/compare register 6
};                   // namespace stm32f3xx

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

constexpr Interrupt operator&(Interrupt a, Interrupt b) {
    return static_cast<Interrupt>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

constexpr uint32_t operator&(uint32_t a, Interrupt b) {
    return a & static_cast<uint32_t>(b);
}
}  // namespace timer_detail

using timer_detail::operator&;

class Timer {
 public:
    enum class Direction { UpCounter = 0x0, DownCounter = 0x1 };
    enum class ClockSource { Internal, ETRPin };
    using Interrupt = timer_detail::Interrupt;

    Timer(void *addr)
        : timer(*static_cast<TimerRegisterMap *>(addr)),
          compare1(*static_cast<TimerRegisterMap *>(addr), 1),
          compare2(*static_cast<TimerRegisterMap *>(addr), 2),
          compare3(*static_cast<TimerRegisterMap *>(addr), 3),
          compare4(*static_cast<TimerRegisterMap *>(addr), 4) {
        ClockManager::enable(*static_cast<TIM_TypeDef *>(addr));
    }

    /**
     * @brief This function enable timer.
     */
    void enable() { timer.CR1.CEN = 1; }
    /**
     * @brief This function disable timer.
     */
    void disable() { timer.CR1.CEN = 0; }

    void enableInterupt() {
        NVIC_SetPriority(TIM1_CC_IRQn, 6);
        NVIC_ClearPendingIRQ(TIM1_CC_IRQn);
        NVIC_EnableIRQ(TIM1_CC_IRQn);
    }

    void enableInterrupts(Interrupt interrupt) { timer.DIER |= static_cast<uint32_t>(interrupt); }

    /**
     * @brief This function set timer direction.
     *
     * @param direction - new timer direction.
     */
    void setDirection(Direction direction) { timer.CR1.DIR = static_cast<uint32_t>(direction); }
    /**
     * This function return current direction setting
     * @return Direction
     */
    Direction getDirection() { return static_cast<Direction>(timer.CR1.DIR.get()); }

    void enableAutoReload() { timer.CR1.ARPE = 1; }
    void disableAutoReload() { timer.CR1.ARPE = 0; }

    void setAutoReloadRegister(uint32_t value) { timer.ARR = value; }

    void setPrescaler(uint16_t prescaler) { timer.PSC = prescaler; }
    uint32_t getPrescaler() { return timer.PSC; }

    void setClockSource(ClockSource clockSource) {
        switch (clockSource) {
            case ClockSource::Internal:
                break;
            case ClockSource::ETRPin:
                timer.SMCR.ETF = 0;
                timer.SMCR.ETPS = 0;
                timer.SMCR.ETP = 0;
                timer.SMCR.ECE = 1;
                break;
        }
    }

    class OutputCompare {
     public:
        enum class Mode {
            Frozen = 0,  //! The comparison between the output compare register TIMx_CCRx and the counter TIMx_CNT has no effect on the outputs.(this
                         //! mode is used to generate a timing base).
            SetChannelToActiveOnMatch,    //! Set channel x to active level on match. OCxREF signal is forced high when the counter TIMx_CNT matches
                                          //! the capture/compare register
            SetChannelToInactiveOnMatch,  //! Set channel x to inactive level on match. OC1REF signal is forced low when the counter TIMx_CNT matches
                                          //! the capture/compare register x (TIMx_CCRx).
            Toggle,                       //! OCxREF toggles when TIMx_CNT=TIMx_CCRx
            ForceInactiveLevel,           //! OCxREF is forced low.
            ForceActiveLevel,             //! OCxREF is forced high.
            PWMMode1,  //! In upcounting, channel x is active as long as TIMx_CNT<TIMx_CCRx else inactive. In downcounting, channel x is inactive
                       //! (OCxREF=‘0’) as long as TIMx_CNT>TIMx_CCR1 else active (OCxREF=’1’).
            PWMMode2,  //! In upcounting, channel 1 is inactive as long as TIMx_CNT<TIMx_CCR1 else active. In downcounting, channel 1 is active as
                       //! long as TIMx_CNT>TIMx_CCR1 else inactive
            OnePulseMode1,
            OnePulseMode2,
            CombinedPWMMode1 = 0b1100,  //! OC1REF has the same behavior as in PWM mode 1. OC1REFC is the logical OR between OC1REF and OC2REF.
            CombinedPWMMode2,           //! OC1REF has the same behavior as in PWM mode 2. OC1REFC is the logical AND between OC1REF and OC2REF.
            AssymmetricPWMMode1,  //! OC1REF has the same behavior as in PWM mode 1. OC1REFC outputs OC1REF when the counter is counting up, OC2REF
                                  //! when it is counting down.
            AssymmetricPWMMode2   //! OC1REF has the same behavior as in PWM mode 2. OC1REFC outputs OC1REF when the counter is counting up, OC2REF
                                  //! when it is counting down.
        };

        enum class Polarity { ActiveHigh = 0, ActiveLow };
        enum class IdleState { Low, High };

        void setValue(uint32_t value) {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCR1 = value;
                    break;
                case 2:
                    timer.CCR2 = value;
                    break;
                case 3:
                    timer.CCR3 = value;
                    break;
                case 4:
                    timer.CCR4 = value;
                    break;
            }
        }
        uint32_t getValue() {
            switch (compareChannelNumber) {
                case 1:
                    return timer.CCR1;
                case 2:
                    return timer.CCR2;
                case 3:
                    return timer.CCR3;
                case 4:
                    return timer.CCR4;
            }
        }
        void enablePreload() {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCMR1.bitfieldOutput.OC1PE = 1;
                    break;
                case 2:
                    timer.CCMR1.bitfieldOutput.OC2PE = 1;
                    break;
                case 3:
                    timer.CCMR2.bitfieldOutput.OC3PE = 1;
                    break;
                case 4:
                    timer.CCMR2.bitfieldOutput.OC4PE = 1;
                    break;
            }
        }
        void disablePreload() {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCMR1.bitfieldOutput.OC1PE = 0;
                    break;
                case 2:
                    timer.CCMR1.bitfieldOutput.OC2PE = 0;
                    break;
                case 3:
                    timer.CCMR2.bitfieldOutput.OC3PE = 0;
                    break;
                case 4:
                    timer.CCMR2.bitfieldOutput.OC4PE = 0;
                    break;
            }
        }
        void setMode(Mode mode) {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCMR1.bitfieldOutput.CC1S = 0;  // configure compare as output
                    timer.CCMR1.setOC1M(static_cast<uint32_t>(mode));
                    break;
                case 2:
                    timer.CCMR1.bitfieldOutput.CC2S = 0;  // configure compare as output
                    timer.CCMR1.setOC2M(static_cast<uint32_t>(mode));
                    break;
                case 3:
                    timer.CCMR2.bitfieldOutput.CC3S = 0;  // configure compare as output
                    timer.CCMR2.setOC3M(static_cast<uint32_t>(mode));
                    break;
                case 4:
                    timer.CCMR2.bitfieldOutput.CC4S = 0;  // configure compare as output
                    timer.CCMR2.setOC4M(static_cast<uint32_t>(mode));
                    break;
            }
        }
        void outputEnable(Polarity polarity, IdleState idleState) {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCER.CC1P = static_cast<uint32_t>(polarity);
                    timer.CR2.OIS1 = static_cast<uint32_t>(idleState);
                    timer.CCER.CC1E = 1;  // enable
                    break;
                case 2:
                    timer.CCER.CC2P = static_cast<uint32_t>(polarity);
                    timer.CR2.OIS2 = static_cast<uint32_t>(idleState);
                    timer.CCER.CC2E = 1;  // enable
                    break;
                case 3:
                    timer.CCER.CC3P = static_cast<uint32_t>(polarity);
                    timer.CR2.OIS3 = static_cast<uint32_t>(idleState);
                    timer.CCER.CC3E = 1;  // enable
                    break;
                case 4:
                    timer.CCER.CC4P = static_cast<uint32_t>(polarity);
                    timer.CR2.OIS4 = static_cast<uint32_t>(idleState);
                    timer.CCER.CC4E = 1;  // enable
                    break;
            }
        }
        /**
         * Note: idleState is by default disabled. To enable idleState functionality call TODO function.
         * @param polarity
         * @param idleState
         */
        void complementaryOutputEnable(Polarity polarity, IdleState idleState) {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCER.CC1NP = static_cast<uint32_t>(polarity);
                    timer.CR2.OIS1N = static_cast<uint32_t>(idleState);
                    timer.CCER.CC1NE = 1;  // enable
                    break;
                case 2:
                    timer.CCER.CC2NP = static_cast<uint32_t>(polarity);
                    timer.CR2.OIS2N = static_cast<uint32_t>(idleState);
                    timer.CCER.CC2NE = 1;  // enable
                    break;
                case 3:
                    timer.CCER.CC3P = static_cast<uint32_t>(polarity);
                    timer.CR2.OIS2N = static_cast<uint32_t>(idleState);
                    timer.CCER.CC3E = 1;  // enable
                    break;
            }
        }
        void outputDisable() {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCER.CC1E = 0;
                    break;
                case 2:
                    timer.CCER.CC2E = 0;
                    break;
                case 3:
                    timer.CCER.CC3E = 0;
                    break;
                case 4:
                    timer.CCER.CC4E = 0;
                    break;
            }
        }
        void complementaryOutputDisable() {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCER.CC1NE = 0;
                    break;
                case 2:
                    timer.CCER.CC2NE = 0;
                    break;
                case 3:
                    timer.CCER.CC3NE = 0;
                    break;
            }
        }

     private:
        TimerRegisterMap &timer;
        uint_fast8_t compareChannelNumber;

        OutputCompare(TimerRegisterMap &timer, uint_fast8_t channel) : timer(timer), compareChannelNumber(channel) {}
        friend Timer;
    };

    void enableCompareOutputs() { timer.BDTR.MOE = 1; }
    uint32_t getValue() { return timer.CNT; }
    void setValue(uint32_t value) { timer.CNT = value; }

    enum class OffStateForRunMode { HiZ_GPIO, InactiveState };
    enum class OffStateForIdleMode { HiZ_GPIO, IdleState };
    void configureOffState(OffStateForRunMode ossr, OffStateForIdleMode ossi) {
        timer.BDTR.OSSR = static_cast<uint32_t>(ossr);
        timer.BDTR.OSSI = static_cast<uint32_t>(ossi);
    }

    OutputCompare compare1;
    OutputCompare compare2;
    OutputCompare compare3;
    OutputCompare compare4;

 private:
    TimerRegisterMap &timer;
};

}  // namespace stm32f3xx
}  // namespace microhal

#endif /* _MICROHAL_TIMER_STM32F3XX_H_ */
