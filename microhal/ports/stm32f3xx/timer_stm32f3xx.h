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
        Bitfield bitfield;
        uint32_t raw;
    };

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

    union CCMR_t {
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
        void setOC1M(uint32_t value) {
            uint32_t tmp = raw;
            tmp &= ~0x0001'0070;
            tmp |= (value & 0b0111) << 4;
            tmp |= (value & 0b1000) << 13;
            raw = tmp;
        }
        void setOC2M(uint32_t value) {
            uint32_t tmp = raw;
            tmp &= ~0x1000'7000;
            tmp |= (value & 0b0111) << 4;
            tmp |= (value & 0b1000) << 13;
            raw = tmp;
        }
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
        Bitfield bitfield;
        uint32_t raw;
    };

    CR1_t CR1;
    CR2_t CR2;
    SMCR_t SMCR;
    uint32_t DIER;
    uint32_t SR;
    uint32_t EGR;    // event generation register
    CCMR_t CCMR1;    // capture/compare mode register 1
    uint32_t CCMR2;  // capture/compare mode register 2
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

class Timer {
 public:
    enum class Direction { UpCounter = 0x0, DownCounter = 0x1 };
    enum class ClockSource { Internal, ETRPin };
    Timer(void *addr) : timer(*static_cast<TimerRegisterMap *>(addr)) { ClockManager::enable(*static_cast<TIM_TypeDef *>(addr)); }

    /**
     * @brief This function enable timer.
     */
    void enable() { timer.CR1.bitfield.CEN = 1; }
    /**
     * @brief This function disable timer.
     */
    void disable() { timer.CR1.bitfield.CEN = 0; }

    /**
     * @brief This function set timer direction.
     *
     * @param direction - new timer direction.
     */
    void setDirection(Direction direction) { timer.CR1.bitfield.DIR = static_cast<uint32_t>(direction); }
    /**
     * This function return current direction setting
     * @return Direction
     */
    Direction getDirection() { return static_cast<Direction>(timer.CR1.bitfield.DIR); }

    void enableAutoReload() { timer.CR1.bitfield.ARPE = 1; }
    void disableAutoReload() { timer.CR1.bitfield.ARPE = 0; }

    void setAutoReloadRegister(uint32_t value) { timer.ARR = value; }

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

    enum PinMode { Inactive, SetOnCompare, ClearOnCompare, ToggleOnCompare };
    void configureOutputCompare(uint8_t channel, uint16_t value) {
        if (channel == 1) {
            timer.CCR1 = value;
            timer.CCMR1.bitfieldOutput.OC1CE = 0;
            timer.CCMR1.setOC1M(0);
            timer.CCER.bitfield.CC1P = 0;
            timer.CCER.bitfield.CC1E = 1;
        }
    }

 private:
    TimerRegisterMap &timer;
};

}  // namespace stm32f3xx
}  // namespace microhal

#endif /* _MICROHAL_TIMER_STM32F3XX_H_ */
