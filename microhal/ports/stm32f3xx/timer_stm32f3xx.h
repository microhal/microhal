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

constexpr uint32_t maskGen(int begin, int end) {
    uint32_t mask = 0;
    for (int i = begin; i <= end; i++) {
        mask |= 1 << i;
    }
    return mask;
}

static_assert(maskGen(0, 0) == 0b01, "");
static_assert(maskGen(0, 1) == 0b11, "");

template <typename type, int begin, int end>
class myBitfield {
 public:
    void operator=(type value) {
        uint32_t tmp = data;
        tmp &= ~bitfieldMask;
        tmp |= (value & valueMask) << begin;
        data = tmp;
    }

    void operator|=(type value) {
        uint32_t tmp = data;
        tmp |= (value & valueMask) << begin;
        data = tmp;
    }

    void operator&=(type value) {
        uint32_t tmp = data;
        tmp &= (value | ~valueMask) << begin;
        data = tmp;
    }

    void set() { data |= bitfieldMask; }
    void clear() { data &= bitfieldMask; }

 private:
    type data;

    enum { bitfieldMask = maskGen(begin, end), valueMask = maskGen(0, end - begin) };
};

struct TimerRegisterMap {
    union CR1_t {
        struct Bitfield {
            uint32_t CEN : 1;       // Counter enable
            uint32_t UDIS : 1;      // Update disable
            uint32_t URS : 1;       // Update request source
            uint32_t OPM : 1;       // One pulse mode
            uint32_t DIR : 1;       // Direction; 0 -> upcounter1, 1 -> downcounter
            uint32_t CMS : 2;       // Center-aligned mode selection
            uint32_t ARPE : 1;      // Auto-reload preload enable
            uint32_t CKD : 2;       // Clock division
            uint32_t Reserved : 1;  // Reserved, must be kept at reset value
            uint32_t UIFREMAP : 1;  // UIF status bit remapping
        };
        myBitfield<uint32_t, 0, 0> CEN;
        myBitfield<uint32_t, 1, 1> UDIS;
        myBitfield<uint32_t, 2, 2> URS;
        myBitfield<uint32_t, 3, 3> OPM;
        myBitfield<uint32_t, 4, 4> DIR;
        myBitfield<uint32_t, 5, 6> CMS;
        myBitfield<uint32_t, 7, 7> ARPE;
        myBitfield<uint32_t, 8, 9> CKD;
        // bit 10 is reserved
        myBitfield<uint32_t, 11, 11> UIFREMAP;

        Bitfield bitfield;
        uint32_t raw;
    };
    static_assert(sizeof(CR1_t) == sizeof(uint32_t), "Microhal internal error, probably incorrect toolchain configuration.");

    union CR2_t {
        struct Bitfield {
            uint32_t CCPC : 1;       // Capture/compare preload control
            uint32_t Reserved1 : 1;  // Reserved, must be kept at reset value
            uint32_t CCUS : 1;       // Capture/compare control update selection
            uint32_t CCDS : 1;       // Capture/compare DMA selection
            uint32_t MMS : 2;        // Master mode selection
            uint32_t TI1S : 1;       // TI1 selection
            uint32_t OIS1 : 1;       // Output Idle state 1 (OC1 output)
            uint32_t OIS1N : 1;      // Output Idle state 1 (OC1N output)
            uint32_t OIS2 : 1;       // Output Idle state 2 (OC2 output)
            uint32_t OIS2N : 1;      // Output Idle state 2 (OC2N output)
            uint32_t OIS3 : 1;       // Output Idle state 3 (OC3 output)
            uint32_t OIS3N : 1;      // Output Idle state 3 (OC3N output)
            uint32_t OIS4 : 1;       // Output Idle state 4 (OC4 output)
            uint32_t Reserved2 : 1;  // Reserved, must be kept at reset value.
            uint32_t OIS5 : 1;       // Output Idle state 5 (OC5 output)
            uint32_t Reserved3 : 1;  // Reserved, must be kept at reset value.
            uint32_t OIS6 : 1;       // Output Idle state 6 (OC6 output)
            uint32_t Reserved4 : 1;  // Reserved, must be kept at reset value.
            uint32_t MMS2 : 4;       // Master mode selection 2
        };
        Bitfield bitfield;
        uint32_t raw;
    };

    union SMCR_t {
        struct Bitfield {
            uint32_t SMSa : 3;  // Slave mode selection
            uint32_t OCCS : 1;  // OCREF clear selection
            uint32_t TS : 3;    //  Trigger selection
            uint32_t MSM : 1;   // Master/slave mode
            uint32_t ETF : 4;   // External trigger filter
            uint32_t ETPS : 2;  // External trigger prescaler
            uint32_t ECE : 1;   // External clock enable
            uint32_t ETP : 1;   // External trigger polarity
            uint32_t SMSb : 1;  // Slave mode selection
        };
        Bitfield bitfield;
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
        struct Bitfield {
            uint32_t CC1S : 2;    // Capture/Compare 1 Selection
            uint32_t IC1PSC : 2;  // Input capture 1 prescaler
            uint32_t IC1F : 4;    // Input capture 1 filte
            uint32_t CC2S : 2;    // Capture/Compare 2 selection
            uint32_t IC2PSC : 2;  // Input capture 2 prescaler
            uint32_t IC2F : 4;    // Input capture 2 filter
        };
        struct BitfieldOutput {
            uint32_t CC1S : 2;      /* Capture/Compare 1 Selection
                                     * 00: CC1 channel is configured as output
                                     * 01: CC1 channel is configured as input, IC1 is mapped on TI1
                                     * 10: CC1 channel is configured as input, IC1 is mapped on TI2
                                     * 11: CC1 channel is configured as input, IC1 is mapped on TRC.
                                     *     This mode is working only if an internal trigger input
                                     *     is selected through TS bit (TIMx_SMCR register)
                                     */
            uint32_t OC1FE : 1;     /* Output Compare 1 fast enable
                                     * 0: CC1 behaves normally depending on counter and CCR1 values even
                                     *    when the trigger is ON. The minimum delay to activate CC1 output
                                     *    when an edge occurs on the trigger input is 5 clock cycles.
                                     * 1: An active edge on the trigger input acts like a compare match
                                     *    on CC1 output. Then, OC is set to the compare level independently
                                     *    from the result of the comparison. Delay to sample the trigger
                                     *    input and to activate CC1 output is reduced to 3 clock cycles.
                                     *    OCFE acts only if the channel is configured in PWM1 or PWM2 mode.
                                     */
            uint32_t OC1PE : 1;     /* Output Compare 1 preload enable
                                     * 0: Preload register on TIMx_CCR1 disabled. TIMx_CCR1 can be written
                                     *    at anytime, the new value is taken in account immediately.
                                     * 1: Preload register on TIMx_CCR1 enabled. Read/Write operations
                                     *    access the preload register. TIMx_CCR1 preload value is loaded
                                     *    in the active register at each update event.
                                     */
            uint32_t OC1Ma : 3;     /* Output Compare 1 mode
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
            uint32_t OC1CE : 1;     // Output Compare 1 clear enable
            uint32_t CC2S : 2;      // Capture/Compare 2 selection
            uint32_t OC2FE : 1;     // Output Compare 2 fast enable
            uint32_t OC2PE : 1;     // Output Compare 2 preload enable
            uint32_t OC2Ma : 3;     // Output Compare 2 mode
            uint32_t OC2CE : 1;     // Output Compare 2 clear enable
            uint32_t OC1Mb : 1;     // Output Compare 1 mode - bit 3
            uint32_t Reserved : 7;  // Reserved, must be kept at reset value.
            uint32_t OC2Mb : 1;     // Output Compare 2 mode - bit 3
        };
        myBitfield<uint32_t, 0, 1> CC1S;
        myBitfield<uint32_t, 3, 3> OC1PE;
        myBitfield<uint32_t, 8, 9> CC2S;
        myBitfield<uint32_t, 11, 11> OC2PE;

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
        Bitfield bitfield;
        BitfieldOutput bitfieldOutput;
        uint32_t raw;
    };

    union CCMR2_t {
        struct Bitfield {
            uint32_t CC3S : 2;    // Capture/Compare 3 Selection
            uint32_t IC3PSC : 2;  // Input capture 1 prescaler
            uint32_t IC3F : 4;    // Input capture 1 filte
            uint32_t CC4S : 2;    // Capture/Compare 2 selection
            uint32_t IC4PSC : 2;  // Input capture 2 prescaler
            uint32_t IC4F : 4;    // Input capture 2 filter
        };
        struct BitfieldOutput {
            uint32_t CC3S : 2;       // Capture/Compare 1 Selection
            uint32_t OC3FE : 1;      // Output Compare 1 fast enable
            uint32_t OC3PE : 1;      // Output Compare 1 preload enable
            uint32_t OC3Ma : 3;      // Output Compare 1 mode
            uint32_t OC3CE : 1;      // Output Compare 1 clear enable
            uint32_t CC4S : 2;       // Capture/Compare 2 selection
            uint32_t OC4FE : 1;      // Output Compare 2 fast enable
            uint32_t OC4PE : 1;      // Output Compare 2 preload enable
            uint32_t OC4Ma : 3;      // Output Compare 2 mode
            uint32_t OC4CE : 1;      // Output Compare 2 clear enable
            uint32_t OC3Mb : 1;      // Output Compare 1 mode - bit 3
            uint32_t Reserved : 7;   // Reserved, must be kept at reset value.
            uint32_t OC4Mb : 1;      // Output Compare 2 mode - bit 3
            uint32_t Reserved2 : 7;  // Reserved, must be kept at reset value.
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

        myBitfield<uint32_t, 0, 1> CC3S;
        myBitfield<uint32_t, 3, 3> OC3PE;
        myBitfield<uint32_t, 8, 9> CC4S;
        myBitfield<uint32_t, 11, 11> OC4PE;

        Bitfield bitfield;
        BitfieldOutput bitfieldOutput;
        uint32_t raw;
    };

    union CCER_t {
        struct Bitfield {
            uint32_t CC1E : 1;       /* Capture/Compare 1 output enable
                                      * CC1 channel configured as output:
                                      * 0: Off - OC1 is not active. OC1 level is then function
                                      *    of MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits.
                                      * 1: On - OC1 signal is output on the corresponding output pin
                                      *    depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits.
                                      */
            uint32_t CC1P : 1;       /* Capture/Compare 1 output polarity;
                                      * CC1 channel configured as output:
                                      * 0: OC1 active high
                                      * 1: OC1 active low
                                      */
            uint32_t CC1NE : 1;      /* Capture/Compare 1 complementary output enable
                                      * 0: Off - OC1N is not active. OC1N level is then function
                                      *    of MOE, OSSI, OSSR, OIS1, OIS1N and CC1E bits.
                                      * 1: On - OC1N signal is output on the corresponding output pin
                                      *    depending on MOE, OSSI, OSSR, OIS1, OIS1N and CC1E bits.
                                      */
            uint32_t CC1NP : 1;      /* Capture/Compare 1 complementary output polarity
                                      * CC1 channel configured as output:
                                      * 0: OC1N active high.
                                      * 1: OC1N active low.
                                      */
            uint32_t CC2E : 1;       // Capture/Compare 2 output enableRefer to CC1E description
            uint32_t CC2P : 1;       // Capture/Compare 2 output polarityRefer to CC1P description
            uint32_t CC2NE : 1;      // Capture/Compare 2 complementary output enableRefer to CC1NE description
            uint32_t CC2NP : 1;      // Capture/Compare 2 complementary output polarityRefer to CC1NP description
            uint32_t CC3E : 1;       // Capture/Compare 3 output enableRefer to CC1E description
            uint32_t CC3P : 1;       // Capture/Compare 3 output polarityRefer to CC1P description
            uint32_t CC3NE : 1;      // Capture/Compare 3 complementary output enableRefer to CC1NE description
            uint32_t CC3NP : 1;      // Capture/Compare 3 complementary output polarityRefer to CC1NP description
            uint32_t CC4E : 1;       // Capture/Compare 4 output enableRefer to CC1E description
            uint32_t CC4P : 1;       // Capture/Compare 4 output polarityRefer to CC1P description
            uint32_t Reserved1 : 1;  // Reserved, must be kept at reset value.
            uint32_t CC4NP : 1;      // Capture/Compare 4 complementary output polarityRefer to CC1NP description
            uint32_t CC5E : 1;       // Capture/Compare 5 output enableRefer to CC1E description
            uint32_t CC5P : 1;       // Capture/Compare 5 output polarityRefer to CC1P description
            uint32_t Reserved2 : 2;  // Reserved, must be kept at reset value.
            uint32_t CC6E : 1;       // Capture/Compare 6 output enableRefer to CC1E description
            uint32_t CC6P : 1;       // Capture/Compare 6 output polarityRefer to CC1P description
        };
        myBitfield<uint32_t, 0, 0> CC1E;
        myBitfield<uint32_t, 1, 1> CC1P;
        myBitfield<uint32_t, 2, 2> CC1NE;
        myBitfield<uint32_t, 3, 3> CC1NP;
        myBitfield<uint32_t, 4, 4> CC2E;
        myBitfield<uint32_t, 5, 5> CC2P;
        myBitfield<uint32_t, 6, 6> CC2NE;
        myBitfield<uint32_t, 7, 7> CC2NP;
        myBitfield<uint32_t, 8, 8> CC3E;
        myBitfield<uint32_t, 9, 9> CC3P;
        myBitfield<uint32_t, 10, 10> CC3NE;
        myBitfield<uint32_t, 11, 11> CC3NP;
        myBitfield<uint32_t, 12, 12> CC4E;
        myBitfield<uint32_t, 13, 13> CC4P;
        // bit 14 reserved
        myBitfield<uint32_t, 15, 15> CC4NP;
        myBitfield<uint32_t, 16, 16> CC5E;
        myBitfield<uint32_t, 17, 17> CC5P;
        // bit 18 reserved
        // bit 19 reserved
        myBitfield<uint32_t, 20, 20> CC6E;
        myBitfield<uint32_t, 3, 3> CC6P;
        // bits 22 - 31 reserved

        Bitfield bitfield;
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
    uint32_t BDTR;   // break and dead-time register
    uint32_t DCR;    // DMA control register
    uint32_t DMAR;   // DMA address for full transfer
    uint32_t OR;     // option registers
    uint32_t CCMR3;  // capture/compare mode register 3
    uint32_t CCR5;   // capture/compare register 5
    uint32_t CCR6;   // capture/compare register 6
};

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
    Direction getDirection() { return static_cast<Direction>(timer.CR1.DIR); }

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
                timer.SMCR.bitfield.ETF = 0;
                timer.SMCR.bitfield.ETPS = 0;
                timer.SMCR.bitfield.ETP = 0;
                timer.SMCR.bitfield.ECE = 1;
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
                    timer.CCMR1.OC1PE = 1;
                    break;
                case 2:
                    timer.CCMR1.OC2PE = 1;
                    break;
                case 3:
                    timer.CCMR2.OC3PE = 1;
                    break;
                case 4:
                    timer.CCMR2.OC4PE = 1;
                    break;
            }
        }
        void disablePreload() {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCMR1.OC1PE = 0;
                    break;
                case 2:
                    timer.CCMR1.OC2PE = 0;
                    break;
                case 3:
                    timer.CCMR2.OC3PE = 0;
                    break;
                case 4:
                    timer.CCMR2.OC4PE = 0;
                    break;
            }
        }
        void setMode(Mode mode) {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCMR1.CC1S = 0;  // configure compare as output
                    timer.CCMR1.setOC1M(static_cast<uint32_t>(mode));
                    break;
                case 2:
                    timer.CCMR1.CC2S = 0;  // configure compare as output
                    timer.CCMR1.setOC2M(static_cast<uint32_t>(mode));
                    break;
                case 3:
                    timer.CCMR2.CC3S = 0;  // configure compare as output
                    timer.CCMR2.setOC3M(static_cast<uint32_t>(mode));
                    break;
                case 4:
                    timer.CCMR2.CC4S = 0;  // configure compare as output
                    timer.CCMR2.setOC4M(static_cast<uint32_t>(mode));
                    break;
            }
        }
        void outputEnable(Polarity polarity) {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCER.CC1P = static_cast<uint32_t>(polarity);
                    timer.CCER.CC1E = 1;  // enable
                    break;
                case 2:
                    timer.CCER.CC2P = static_cast<uint32_t>(polarity);
                    timer.CCER.CC2E = 1;  // enable
                    break;
                case 3:
                    timer.CCER.CC3P = static_cast<uint32_t>(polarity);
                    timer.CCER.CC3E = 1;  // enable
                    break;
                case 4:
                    timer.CCER.CC4P = static_cast<uint32_t>(polarity);
                    timer.CCER.CC4E = 1;  // enable
                    break;
            }
        }
        void complementaryOutputEnable(Polarity polarity) {
            switch (compareChannelNumber) {
                case 1:
                    timer.CCER.CC1NP = static_cast<uint32_t>(polarity);
                    timer.CCER.CC1NE = 1;  // enable
                    break;
                case 2:
                    timer.CCER.CC2NP = static_cast<uint32_t>(polarity);
                    timer.CCER.CC2NE = 1;  // enable
                    break;
                case 3:
                    timer.CCER.CC3P = static_cast<uint32_t>(polarity);
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

    void enableCompareOutputs() { timer.BDTR |= TIM_BDTR_MOE; }
    uint32_t getValue() { return timer.CNT; }
    void setValue(uint32_t value) { timer.CNT = value; }

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
