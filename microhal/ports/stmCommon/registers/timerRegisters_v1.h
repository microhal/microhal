#ifndef _MICROHAL_PERIPHERAL_REGISTER_TIM
#define _MICROHAL_PERIPHERAL_REGISTER_TIM

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include "registersBaseAddressDefinitions.h"

// Supported MCU: STM32F407, STM32F429, STM32F469, STM32F411, STM32F413, STM32F412, STM32F401, STM32F427, STM32F405, STM32F446, STM32F410, STM32F217,
//                STM32F215, STM32F107, STM32F103, STM32F0x2, STM32F0x8, STM32F0x1, STM32F0x0, STM32F102, STM32F107, STM32F101, STM32F100

// Supported MCU variant 2: STM32G070, STM32G071, STM32G081

#if defined(MCU_TYPE_STM32G0XX)
#define _MICROHAL_REGISTERS_TIMER_VARIANT 2
#endif

#if _MICROHAL_REGISTERS_TIMER_VARIANT == 2
#define _MICROHAL_REGISTERS_TIMER_HAS_UIF
#define _MICROHAL_REGISTERS_TIMER_HAS_OIS5
#define _MICROHAL_REGISTERS_TIMER_HAS_OIS6
#define _MICROHAL_REGISTERS_TIMER_HAS_MMS2
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct TIM {
    // control register 1
    union CR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> CEN;  /*!< Counter enable */
            microhal::Bitfield<uint32_t, 1, 1> UDIS; /*!< Update disable */
            microhal::Bitfield<uint32_t, 2, 1> URS;  /*!< Update request source */
            microhal::Bitfield<uint32_t, 3, 1> OPM;  /*!< One-pulse mode */
            microhal::Bitfield<uint32_t, 4, 1> DIR;  /*!< Direction */
            microhal::Bitfield<uint32_t, 5, 2> CMS;  /*!< Center-aligned mode selection */
            microhal::Bitfield<uint32_t, 7, 1> ARPE; /*!< Auto-reload preload enable */
            microhal::Bitfield<uint32_t, 8, 2> CKD;  /*!< Clock division */
#ifdef _MICROHAL_REGISTERS_TIMER_HAS_UIF
            microhal::Bitfield<uint32_t, 11, 1> UIFREMAP; /*!< UIF status bit remapping */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CR1 operator&(uint32_t value) const {
            CR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CR1 operator|(uint32_t value) const {
            CR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CR1, AccessType::ReadOnly>;
        friend class VolatileRegister<CR1, AccessType::WriteOnly>;
        friend class VolatileRegister<CR1, AccessType::ReadWrite>;
    };

    // control register 2
    union CR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> CCPC;   /*!< Capture/compare preloaded control */
            microhal::Bitfield<uint32_t, 2, 1> CCUS;   /*!< Capture/compare control update selection */
            microhal::Bitfield<uint32_t, 3, 1> CCDS;   /*!< Capture/compare DMA selection */
            microhal::Bitfield<uint32_t, 4, 3> MMS;    /*!< Master mode selection */
            microhal::Bitfield<uint32_t, 7, 1> TI1S;   /*!< TI1 selection */
            microhal::Bitfield<uint32_t, 8, 1> OIS1;   /*!< Output Idle state 1 */
            microhal::Bitfield<uint32_t, 9, 1> OIS1N;  /*!< Output Idle state 1 */
            microhal::Bitfield<uint32_t, 10, 1> OIS2;  /*!< Output Idle state 2 */
            microhal::Bitfield<uint32_t, 11, 1> OIS2N; /*!< Output Idle state 2 */
            microhal::Bitfield<uint32_t, 12, 1> OIS3;  /*!< Output Idle state 3 */
            microhal::Bitfield<uint32_t, 13, 1> OIS3N; /*!< Output Idle state 3 */
            microhal::Bitfield<uint32_t, 14, 1> OIS4;  /*!< Output Idle state 4 */
#ifdef _MICROHAL_REGISTERS_TIMER_HAS_OIS5
            microhal::Bitfield<uint32_t, 16, 1> OIS5; /*!< Output Idle state 5 (OC5 output) */
#endif
#ifdef _MICROHAL_REGISTERS_TIMER_HAS_OIS6
            microhal::Bitfield<uint32_t, 18, 1> OIS6; /*!< Output Idle state 6 (OC6 output) */
#endif
#ifdef _MICROHAL_REGISTERS_TIMER_HAS_MMS2
            microhal::Bitfield<uint32_t, 20, 4> MMS2; /*!< Master mode selection 2 */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CR2 operator&(uint32_t value) const {
            CR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CR2 operator|(uint32_t value) const {
            CR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CR2, AccessType::ReadOnly>;
        friend class VolatileRegister<CR2, AccessType::WriteOnly>;
        friend class VolatileRegister<CR2, AccessType::ReadWrite>;
    };

    // slave mode control register
    union SMCR {
        union {
            microhal::Bitfield<uint32_t, 0, 3> SMS;   /*!< Slave mode selection */
            microhal::Bitfield<uint32_t, 4, 3> TS;    /*!< Trigger selection */
            microhal::Bitfield<uint32_t, 7, 1> MSM;   /*!< Master/Slave mode */
            microhal::Bitfield<uint32_t, 8, 4> ETF;   /*!< External trigger filter */
            microhal::Bitfield<uint32_t, 12, 2> ETPS; /*!< External trigger prescaler */
            microhal::Bitfield<uint32_t, 14, 1> ECE;  /*!< External clock enable */
            microhal::Bitfield<uint32_t, 15, 1> ETP;  /*!< External trigger polarity */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SMCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SMCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SMCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SMCR operator&(uint32_t value) const {
            SMCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SMCR operator|(uint32_t value) const {
            SMCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SMCR, AccessType::ReadOnly>;
        friend class VolatileRegister<SMCR, AccessType::WriteOnly>;
        friend class VolatileRegister<SMCR, AccessType::ReadWrite>;
    };

    // DMA/Interrupt enable register
    union DIER {
        union {
            microhal::Bitfield<uint32_t, 0, 1> UIE;    /*!< Update interrupt enable */
            microhal::Bitfield<uint32_t, 1, 1> CC1IE;  /*!< Capture/Compare 1 interrupt enable */
            microhal::Bitfield<uint32_t, 2, 1> CC2IE;  /*!< Capture/Compare 2 interrupt enable */
            microhal::Bitfield<uint32_t, 3, 1> CC3IE;  /*!< Capture/Compare 3 interrupt enable */
            microhal::Bitfield<uint32_t, 4, 1> CC4IE;  /*!< Capture/Compare 4 interrupt enable */
            microhal::Bitfield<uint32_t, 5, 1> COMIE;  /*!< COM interrupt enable */
            microhal::Bitfield<uint32_t, 6, 1> TIE;    /*!< Trigger interrupt enable */
            microhal::Bitfield<uint32_t, 7, 1> BIE;    /*!< Break interrupt enable */
            microhal::Bitfield<uint32_t, 8, 1> UDE;    /*!< Update DMA request enable */
            microhal::Bitfield<uint32_t, 9, 1> CC1DE;  /*!< Capture/Compare 1 DMA request enable */
            microhal::Bitfield<uint32_t, 10, 1> CC2DE; /*!< Capture/Compare 2 DMA request enable */
            microhal::Bitfield<uint32_t, 11, 1> CC3DE; /*!< Capture/Compare 3 DMA request enable */
            microhal::Bitfield<uint32_t, 12, 1> CC4DE; /*!< Capture/Compare 4 DMA request enable */
            microhal::Bitfield<uint32_t, 13, 1> COMDE; /*!< COM DMA request enable */
            microhal::Bitfield<uint32_t, 14, 1> TDE;   /*!< Trigger DMA request enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DIER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DIER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DIER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DIER operator&(uint32_t value) const {
            DIER tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DIER operator|(uint32_t value) const {
            DIER tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DIER, AccessType::ReadOnly>;
        friend class VolatileRegister<DIER, AccessType::WriteOnly>;
        friend class VolatileRegister<DIER, AccessType::ReadWrite>;
    };

    // status register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> UIF;    /*!< Update interrupt flag */
            microhal::Bitfield<uint32_t, 1, 1> CC1IF;  /*!< Capture/compare 1 interrupt flag */
            microhal::Bitfield<uint32_t, 2, 1> CC2IF;  /*!< Capture/Compare 2 interrupt flag */
            microhal::Bitfield<uint32_t, 3, 1> CC3IF;  /*!< Capture/Compare 3 interrupt flag */
            microhal::Bitfield<uint32_t, 4, 1> CC4IF;  /*!< Capture/Compare 4 interrupt flag */
            microhal::Bitfield<uint32_t, 5, 1> COMIF;  /*!< COM interrupt flag */
            microhal::Bitfield<uint32_t, 6, 1> TIF;    /*!< Trigger interrupt flag */
            microhal::Bitfield<uint32_t, 7, 1> BIF;    /*!< Break interrupt flag */
            microhal::Bitfield<uint32_t, 9, 1> CC1OF;  /*!< Capture/Compare 1 overcapture flag */
            microhal::Bitfield<uint32_t, 10, 1> CC2OF; /*!< Capture/compare 2 overcapture flag */
            microhal::Bitfield<uint32_t, 11, 1> CC3OF; /*!< Capture/Compare 3 overcapture flag */
            microhal::Bitfield<uint32_t, 12, 1> CC4OF; /*!< Capture/Compare 4 overcapture flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SR operator&(uint32_t value) const {
            SR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SR operator|(uint32_t value) const {
            SR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SR, AccessType::ReadOnly>;
        friend class VolatileRegister<SR, AccessType::WriteOnly>;
        friend class VolatileRegister<SR, AccessType::ReadWrite>;
    };

    // event generation register
    union EGR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> UG;   /*!< Update generation */
            microhal::Bitfield<uint32_t, 1, 1> CC1G; /*!< Capture/compare 1 generation */
            microhal::Bitfield<uint32_t, 2, 1> CC2G; /*!< Capture/compare 2 generation */
            microhal::Bitfield<uint32_t, 3, 1> CC3G; /*!< Capture/compare 3 generation */
            microhal::Bitfield<uint32_t, 4, 1> CC4G; /*!< Capture/compare 4 generation */
            microhal::Bitfield<uint32_t, 5, 1> COMG; /*!< Capture/Compare control update generation */
            microhal::Bitfield<uint32_t, 6, 1> TG;   /*!< Trigger generation */
            microhal::Bitfield<uint32_t, 7, 1> BG;   /*!< Break generation */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        EGR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        EGR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        EGR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        EGR operator&(uint32_t value) const {
            EGR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        EGR operator|(uint32_t value) const {
            EGR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<EGR, AccessType::ReadOnly>;
        friend class VolatileRegister<EGR, AccessType::WriteOnly>;
        friend class VolatileRegister<EGR, AccessType::ReadWrite>;
    };

    union CCMR1 {
        union BitfieldInput {
            microhal::Bitfield<uint32_t, 0, 2> CC1S;     /* Capture/Compare 1 Selection
                                                          * 00: Forbidden in input capture mode
                                                          * 01: CC1 channel is configured as input, IC1 is mapped on TI1
                                                          * 10: CC1 channel is configured as input, IC1 is mapped on TI2
                                                          * 11: CC1 channel is configured as input, IC1 is mapped on TRC.
                                                          *     This mode is working only if an internal trigger input
                                                          *     is selected through TS bit (TIMx_SMCR register)
                                                          */
            microhal::Bitfield<uint32_t, 2, 2> IC1PSC;   // Input capture 1 prescaler
            microhal::Bitfield<uint32_t, 4, 4> IC1F;     // Input capture 1 filte
            microhal::Bitfield<uint32_t, 8, 2> CC2S;     // Capture/Compare 2 selection
            microhal::Bitfield<uint32_t, 10, 2> IC2PSC;  // Input capture 2 prescaler
            microhal::Bitfield<uint32_t, 12, 4> IC2F;    // Input capture 2 filter
        };

        union BitfieldOutput {
            microhal::Bitfield<uint32_t, 0, 2> CC1S;  /* Capture/Compare 1 Selection
                                                       * 00: CC1 channel is configured as output
                                                       * 01: Forbidden in output compare mode
                                                       * 10: Forbidden in output compare mode
                                                       * 11: Forbidden in output compare mode
                                                       */
            microhal::Bitfield<uint32_t, 2, 1> OC1FE; /* Output Compare 1 fast enable
                                                       * 0: CC1 behaves normally depending on counter and CCR1 values even
                                                       *    when the trigger is ON. The minimum delay to activate CC1 output
                                                       *    when an edge occurs on the trigger input is 5 clock cycles.
                                                       * 1: An active edge on the trigger input acts like a compare match
                                                       *    on CC1 output. Then, OC is set to the compare level independently
                                                       *    from the result of the comparison. Delay to sample the trigger
                                                       *    input and to activate CC1 output is reduced to 3 clock cycles.
                                                       *    OCFE acts only if the channel is configured in PWM1 or PWM2 mode.
                                                       */
            microhal::Bitfield<uint32_t, 3, 1> OC1PE; /* Output Compare 1 preload enable
                                                       * 0: Preload register on TIMx_CCR1 disabled. TIMx_CCR1 can be written
                                                       *    at anytime, the new value is taken in account immediately.
                                                       * 1: Preload register on TIMx_CCR1 enabled. Read/Write operations
                                                       *    access the preload register. TIMx_CCR1 preload value is loaded
                                                       *    in the active register at each update event.
                                                       */

            microhal::Bitfield<uint32_t, 4, 3> OC1Ma;   /* Output Compare 1 mode
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
            microhal::Bitfield<uint32_t, 7, 1> OC1CE;   // Output Compare 1 clear enable
            microhal::Bitfield<uint32_t, 8, 2> CC2S;    // Capture/Compare 2 selection
            microhal::Bitfield<uint32_t, 10, 1> OC2FE;  // Output Compare 2 fast enable, Look at CC1S
            microhal::Bitfield<uint32_t, 11, 1> OC2PE;  // Output Compare 2 preload enable, Look at CC1S
            microhal::Bitfield<uint32_t, 12, 3> OC2Ma;  // Output Compare 2 mode
            microhal::Bitfield<uint32_t, 15, 1> OC2CE;  // Output Compare 2 clear enable, Look at CC1S
        };

        void setOC1M(uint32_t value) {
            uint32_t tmp = raw;
            tmp &= ~0x0001'0070;
            tmp |= (value & 0b0111) << 4;
            // tmp |= (value & 0b1000) << 13;
            raw = tmp;
        }
        void setOC2M(uint32_t value) {
            uint32_t tmp = raw;
            tmp &= ~0x0100'7000;
            tmp |= (value & 0b0111) << 12;
            // tmp |= (value & 0b1000) << 21;
            raw = tmp;
        }

        BitfieldInput bitfieldInput;
        BitfieldOutput bitfieldOutput;

     private:
        uint32_t raw;
        friend class VolatileRegister<CCMR1, AccessType::ReadOnly>;
        friend class VolatileRegister<CCMR1, AccessType::WriteOnly>;
        friend class VolatileRegister<CCMR1, AccessType::ReadWrite>;
    };

    // capture/compare mode register (output mode)
    union CCMR2 {
        union BitfieldInput {
            microhal::Bitfield<uint32_t, 0, 2> CC3S;    /*!< Capture/compare 3 selection */
            microhal::Bitfield<uint32_t, 2, 2> IC3PSC;  /*!< Input capture 3 prescaler */
            microhal::Bitfield<uint32_t, 4, 4> IC3F;    /*!< Input capture 3 filter */
            microhal::Bitfield<uint32_t, 8, 2> CC4S;    /*!< Capture/Compare 4 selection */
            microhal::Bitfield<uint32_t, 10, 2> IC4PSC; /*!< Input capture 4 prescaler */
            microhal::Bitfield<uint32_t, 12, 4> IC4F;   /*!< Input capture 4 filter */
        };
        union {
            microhal::Bitfield<uint32_t, 0, 2> CC3S;   /*!< Capture/Compare 3 selection */
            microhal::Bitfield<uint32_t, 2, 1> OC3FE;  /*!< Output compare 3 fast enable */
            microhal::Bitfield<uint32_t, 3, 1> OC3PE;  /*!< Output compare 3 preload enable */
            microhal::Bitfield<uint32_t, 4, 3> OC3M;   /*!< Output compare 3 mode */
            microhal::Bitfield<uint32_t, 7, 1> OC3CE;  /*!< Output compare 3 clear enable */
            microhal::Bitfield<uint32_t, 8, 2> CC4S;   /*!< Capture/Compare 4 selection */
            microhal::Bitfield<uint32_t, 10, 1> OC4FE; /*!< Output compare 4 fast enable */
            microhal::Bitfield<uint32_t, 11, 1> OC4PE; /*!< Output compare 4 preload enable */
            microhal::Bitfield<uint32_t, 12, 3> OC4M;  /*!< Output compare 4 mode */
            microhal::Bitfield<uint32_t, 15, 1> OC4CE; /*!< Output compare 4 clear enable */
        };
        union BitfieldOutput {
            microhal::Bitfield<uint32_t, 0, 2> CC3S;    // Capture/Compare 1 Selection, for more information loock at CCMR1_t
            microhal::Bitfield<uint32_t, 2, 1> OC3FE;   // Output Compare 1 fast enable, for more information loock at CCMR1_t
            microhal::Bitfield<uint32_t, 3, 1> OC3PE;   // Output Compare 1 preload enable, for more information loock at CCMR1_t
            microhal::Bitfield<uint32_t, 4, 3> OC3Ma;   // Output Compare 1 mode, for more information loock at CCMR1_t
            microhal::Bitfield<uint32_t, 7, 1> OC3CE;   // Output Compare 1 clear enable, for more information loock at CCMR1_t
            microhal::Bitfield<uint32_t, 8, 2> CC4S;    // Capture/Compare 2 selection, for more information loock at CCMR1_t
            microhal::Bitfield<uint32_t, 10, 1> OC4FE;  // Output Compare 2 fast enable, for more information loock at CCMR1_t
            microhal::Bitfield<uint32_t, 11, 1> OC4PE;  // Output Compare 2 preload enable, for more information loock at CCMR1_t
            microhal::Bitfield<uint32_t, 12, 3> OC4Ma;  // Output Compare 2 mode, for more information loock at CCMR1_t
            microhal::Bitfield<uint32_t, 15, 1> OC4CE;  // Output Compare 2 clear enable, for more information loock at CCMR1_t
        };

        void setOC3M(uint32_t value) {
            uint32_t tmp = raw;
            tmp &= ~0x0001'0070;
            tmp |= (value & 0b0111) << 4;
            // tmp |= (value & 0b1000) << 13;
            raw = tmp;
        }
        void setOC4M(uint32_t value) {
            uint32_t tmp = raw;
            tmp &= ~0x0100'7000;
            tmp |= (value & 0b0111) << 12;
            // tmp |= (value & 0b1000) << 21;
            raw = tmp;
        }

        BitfieldInput bitfieldInput;
        BitfieldOutput bitfieldOutput;

     private:
        uint32_t raw;
        friend class VolatileRegister<CCMR2, AccessType::ReadOnly>;
        friend class VolatileRegister<CCMR2, AccessType::WriteOnly>;
        friend class VolatileRegister<CCMR2, AccessType::ReadWrite>;
    };

    // capture/compare enable register
    union CCER {
        union {
            microhal::Bitfield<uint32_t, 0, 1> CC1E;   /*!< Capture/Compare 1 output enable */
            microhal::Bitfield<uint32_t, 1, 1> CC1P;   /*!< Capture/Compare 1 output Polarity */
            microhal::Bitfield<uint32_t, 2, 1> CC1NE;  /*!< Capture/Compare 1 complementary output enable */
            microhal::Bitfield<uint32_t, 3, 1> CC1NP;  /*!< Capture/Compare 1 output Polarity */
            microhal::Bitfield<uint32_t, 4, 1> CC2E;   /*!< Capture/Compare 2 output enable */
            microhal::Bitfield<uint32_t, 5, 1> CC2P;   /*!< Capture/Compare 2 output Polarity */
            microhal::Bitfield<uint32_t, 6, 1> CC2NE;  /*!< Capture/Compare 2 complementary output enable */
            microhal::Bitfield<uint32_t, 7, 1> CC2NP;  /*!< Capture/Compare 2 output Polarity */
            microhal::Bitfield<uint32_t, 8, 1> CC3E;   /*!< Capture/Compare 3 output enable */
            microhal::Bitfield<uint32_t, 9, 1> CC3P;   /*!< Capture/Compare 3 output Polarity */
            microhal::Bitfield<uint32_t, 10, 1> CC3NE; /*!< Capture/Compare 3 complementary output enable */
            microhal::Bitfield<uint32_t, 11, 1> CC3NP; /*!< Capture/Compare 3 output Polarity */
            microhal::Bitfield<uint32_t, 12, 1> CC4E;  /*!< Capture/Compare 4 output enable */
            microhal::Bitfield<uint32_t, 13, 1> CC4P;  /*!< Capture/Compare 3 output Polarity */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CCER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CCER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CCER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CCER operator&(uint32_t value) const {
            CCER tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CCER operator|(uint32_t value) const {
            CCER tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CCER, AccessType::ReadOnly>;
        friend class VolatileRegister<CCER, AccessType::WriteOnly>;
        friend class VolatileRegister<CCER, AccessType::ReadWrite>;
    };

    // counter
    union CNT {
        union {
            microhal::Bitfield<uint32_t, 0, 16> cnt; /*!< counter value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CNT &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CNT &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CNT &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CNT operator&(uint32_t value) const {
            CNT tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CNT operator|(uint32_t value) const {
            CNT tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CNT, AccessType::ReadOnly>;
        friend class VolatileRegister<CNT, AccessType::WriteOnly>;
        friend class VolatileRegister<CNT, AccessType::ReadWrite>;
    };

    // prescaler
    union PSC {
        union {
            microhal::Bitfield<uint32_t, 0, 16> psc; /*!< Prescaler value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PSC &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PSC &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PSC &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PSC operator&(uint32_t value) const {
            PSC tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PSC operator|(uint32_t value) const {
            PSC tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PSC, AccessType::ReadOnly>;
        friend class VolatileRegister<PSC, AccessType::WriteOnly>;
        friend class VolatileRegister<PSC, AccessType::ReadWrite>;
    };

    // auto-reload register
    union ARR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> arr; /*!< Auto-reload value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ARR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ARR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ARR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ARR operator&(uint32_t value) const {
            ARR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ARR operator|(uint32_t value) const {
            ARR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ARR, AccessType::ReadOnly>;
        friend class VolatileRegister<ARR, AccessType::WriteOnly>;
        friend class VolatileRegister<ARR, AccessType::ReadWrite>;
    };

    // capture/compare register 1
    union CCRx {
        union {
            microhal::Bitfield<uint32_t, 0, 16> ccr; /*!< Capture/Compare 1 value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CCRx &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CCRx &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CCRx &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CCRx operator&(uint32_t value) const {
            CCRx tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CCRx operator|(uint32_t value) const {
            CCRx tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CCRx, AccessType::ReadOnly>;
        friend class VolatileRegister<CCRx, AccessType::WriteOnly>;
        friend class VolatileRegister<CCRx, AccessType::ReadWrite>;
    };

    // DMA control register
    union DCR {
        union {
            microhal::Bitfield<uint32_t, 0, 5> DBA; /*!< DMA base address */
            microhal::Bitfield<uint32_t, 8, 5> DBL; /*!< DMA burst length */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DCR operator&(uint32_t value) const {
            DCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DCR operator|(uint32_t value) const {
            DCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DCR, AccessType::ReadOnly>;
        friend class VolatileRegister<DCR, AccessType::WriteOnly>;
        friend class VolatileRegister<DCR, AccessType::ReadWrite>;
    };

    // DMA address for full transfer
    union DMAR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> DMAB; /*!< DMA register for burst accesses */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAR operator&(uint32_t value) const {
            DMAR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAR operator|(uint32_t value) const {
            DMAR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAR, AccessType::ReadWrite>;
    };

    // repetition counter register
    union RCR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> REP; /*!< Repetition counter value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RCR operator&(uint32_t value) const {
            RCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RCR operator|(uint32_t value) const {
            RCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RCR, AccessType::ReadOnly>;
        friend class VolatileRegister<RCR, AccessType::WriteOnly>;
        friend class VolatileRegister<RCR, AccessType::ReadWrite>;
    };

    // break and dead-time register
    union BDTR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> DTG;   /*!< Dead-time generator setup */
            microhal::Bitfield<uint32_t, 8, 2> LOCK;  /*!< Lock configuration */
            microhal::Bitfield<uint32_t, 10, 1> OSSI; /*!< Off-state selection for Idle mode */
            microhal::Bitfield<uint32_t, 11, 1> OSSR; /*!< Off-state selection for Run mode */
            microhal::Bitfield<uint32_t, 12, 1> BKE;  /*!< Break enable */
            microhal::Bitfield<uint32_t, 13, 1> BKP;  /*!< Break polarity */
            microhal::Bitfield<uint32_t, 14, 1> AOE;  /*!< Automatic output enable */
            microhal::Bitfield<uint32_t, 15, 1> MOE;  /*!< Main output enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        BDTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        BDTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        BDTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        BDTR operator&(uint32_t value) const {
            BDTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        BDTR operator|(uint32_t value) const {
            BDTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<BDTR, AccessType::ReadOnly>;
        friend class VolatileRegister<BDTR, AccessType::WriteOnly>;
        friend class VolatileRegister<BDTR, AccessType::ReadWrite>;
    };

    VolatileRegister<CR1, AccessType::ReadWrite> cr1;     /*!< control register 1	        Address offset: 0x0 */
    VolatileRegister<CR2, AccessType::ReadWrite> cr2;     /*!< control register 2	        Address offset: 0x4 */
    VolatileRegister<SMCR, AccessType::ReadWrite> smcr;   /*!< slave mode control register	Address offset: 0x8 */
    VolatileRegister<DIER, AccessType::ReadWrite> dier;   /*!< DMA/Interrupt enable register	Address offset: 0xC */
    VolatileRegister<SR, AccessType::ReadWrite> sr;       /*!< status register                  Address offset: 0x10 */
    VolatileRegister<EGR, AccessType::ReadWrite> egr;     /*!< event generation register	Address offset: 0x14 */
    VolatileRegister<CCMR1, AccessType::ReadWrite> ccmr1; /*!< capture/compare mode register 	Address offset: 0x18 */
    VolatileRegister<CCMR2, AccessType::ReadWrite> ccmr2; /*!< capture/compare mode register 	Address offset: 0x1C */
    VolatileRegister<CCER, AccessType::ReadWrite> ccer;   /*!< capture/compare enable register	Address offset: 0x20 */
    VolatileRegister<CNT, AccessType::ReadWrite> cnt;     /*!< counter	                        Address offset: 0x24 */
    VolatileRegister<PSC, AccessType::ReadWrite> psc;     /*!< prescaler                        Address offset: 0x28 */
    VolatileRegister<ARR, AccessType::ReadWrite> arr;     /*!< auto-reload register             Address offset: 0x2C */
    VolatileRegister<RCR, AccessType::ReadWrite> rcr;     /*!< repetition counter register	Address offset: 0x30 */
    VolatileRegister<CCRx, AccessType::ReadWrite> ccr[4]; /*!< capture/compare register 1	Address offset: 0x34 */
    // VolatileRegister<CCRx, AccessType::ReadWrite> ccr2;   /*!< capture/compare register 2	Address offset: 0x38 */
    // VolatileRegister<CCRx, AccessType::ReadWrite> ccr3;   /*!< capture/compare register 3	Address offset: 0x3C */
    // VolatileRegister<CCRx, AccessType::ReadWrite> ccr4;   /*!< capture/compare register 4	Address offset: 0x40 */
    VolatileRegister<BDTR, AccessType::ReadWrite> bdtr; /*!< break and dead-time register	Address offset: 0x44 */
    VolatileRegister<DCR, AccessType::ReadWrite> dcr;   /*!< DMA control register	        Address offset: 0x48 */
    VolatileRegister<DMAR, AccessType::ReadWrite> dmar; /*!< DMA address for full transfer	Address offset: 0x4C */
};

#if defined(_MICROHAL_TIM1_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim1 = reinterpret_cast<TIM *>(_MICROHAL_TIM1_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM2_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim2 = reinterpret_cast<TIM *>(_MICROHAL_TIM2_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM3_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim3 = reinterpret_cast<TIM *>(_MICROHAL_TIM3_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM4_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim4 = reinterpret_cast<TIM *>(_MICROHAL_TIM4_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM5_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim5 = reinterpret_cast<TIM *>(_MICROHAL_TIM5_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM6_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim6 = reinterpret_cast<TIM *>(_MICROHAL_TIM6_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM7_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim7 = reinterpret_cast<TIM *>(_MICROHAL_TIM7_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM8_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim8 = reinterpret_cast<TIM *>(_MICROHAL_TIM8_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM9_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim9 = reinterpret_cast<TIM *>(_MICROHAL_TIM9_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM10_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim10 = reinterpret_cast<TIM *>(_MICROHAL_TIM10_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM11_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim11 = reinterpret_cast<TIM *>(_MICROHAL_TIM11_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM12_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim12 = reinterpret_cast<TIM *>(_MICROHAL_TIM12_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM13_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim13 = reinterpret_cast<TIM *>(_MICROHAL_TIM13_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM14_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim14 = reinterpret_cast<TIM *>(_MICROHAL_TIM14_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_TIM15_BASE_ADDRESS)
[[maybe_unused]] static TIM *const tim15 = reinterpret_cast<TIM *>(_MICROHAL_TIM15_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_TIM
