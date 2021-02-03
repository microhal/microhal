#ifndef _MICROHAL_PERIPHERAL_REGISTER_SYSCFG_COMP_OPAMP
#define _MICROHAL_PERIPHERAL_REGISTER_SYSCFG_COMP_OPAMP

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32F302, STM32F3x4, STM32F301, STM32F3x8, STM32F373
namespace microhal {
namespace registers {
/**
 *
 */

struct SYSCFG_COMP_OPAMP {
    // configuration register 1
    union SYSCFG_CFGR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 2> MEM_MODE;           /*!< Memory mapping selection bits */
            microhal::Bitfield<uint32_t, 5, 1> USB_IT_RMP;         /*!< USB interrupt remap */
            microhal::Bitfield<uint32_t, 6, 1> TIM1_ITR_RMP;       /*!< Timer 1 ITR3 selection */
            microhal::Bitfield<uint32_t, 7, 1> DAC_TRIG_RMP;       /*!< DAC trigger remap (when TSEL = 001) */
            microhal::Bitfield<uint32_t, 8, 1> ADC24_DMA_RMP;      /*!< ADC24 DMA remapping bit */
            microhal::Bitfield<uint32_t, 11, 1> TIM16_DMA_RMP;     /*!< TIM16 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 12, 1> TIM17_DMA_RMP;     /*!< TIM17 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 13, 1> TIM6_DAC1_DMA_RMP; /*!< TIM6 and DAC1 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 14, 1> TIM7_DAC2_DMA_RMP; /*!< TIM7 and DAC2 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 16, 1> I2C_PB6_FM;        /*!< Fast Mode Plus (FM+) driving capability activation bits. */
            microhal::Bitfield<uint32_t, 17, 1> I2C_PB7_FM;        /*!< Fast Mode Plus (FM+) driving capability activation bits. */
            microhal::Bitfield<uint32_t, 18, 1> I2C_PB8_FM;        /*!< Fast Mode Plus (FM+) driving capability activation bits. */
            microhal::Bitfield<uint32_t, 19, 1> I2C_PB9_FM;        /*!< Fast Mode Plus (FM+) driving capability activation bits. */
            microhal::Bitfield<uint32_t, 20, 1> I2C1_FM;           /*!< I2C1 Fast Mode Plus */
            microhal::Bitfield<uint32_t, 21, 1> I2C2_FM;           /*!< I2C2 Fast Mode Plus */
            microhal::Bitfield<uint32_t, 22, 2> ENCODER_MODE;      /*!< Encoder mode */
            microhal::Bitfield<uint32_t, 26, 6> FPU_IT;            /*!< Interrupt enable bits from FPU */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SYSCFG_CFGR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SYSCFG_CFGR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SYSCFG_CFGR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SYSCFG_CFGR1 operator&(uint32_t value) const {
            SYSCFG_CFGR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SYSCFG_CFGR1 operator|(uint32_t value) const {
            SYSCFG_CFGR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SYSCFG_CFGR1, AccessType::ReadOnly>;
        friend class VolatileRegister<SYSCFG_CFGR1, AccessType::WriteOnly>;
        friend class VolatileRegister<SYSCFG_CFGR1, AccessType::ReadWrite>;
    };

    // external interrupt configuration register 1
    union EXTICR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> EXTI0;  /*!< EXTI 0 configuration bits */
            microhal::Bitfield<uint32_t, 4, 4> EXTI1;  /*!< EXTI 1 configuration bits */
            microhal::Bitfield<uint32_t, 8, 4> EXTI2;  /*!< EXTI 2 configuration bits */
            microhal::Bitfield<uint32_t, 12, 4> EXTI3; /*!< EXTI 3 configuration bits */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        EXTICR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        EXTICR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        EXTICR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        EXTICR operator&(uint32_t value) const {
            EXTICR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        EXTICR operator|(uint32_t value) const {
            EXTICR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<EXTICR, AccessType::ReadOnly>;
        friend class VolatileRegister<EXTICR, AccessType::WriteOnly>;
        friend class VolatileRegister<EXTICR, AccessType::ReadWrite>;
    };

    // external interrupt configuration register 2
    union SYSCFG_EXTICR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 4> EXTI4;  /*!< EXTI 4 configuration bits */
            microhal::Bitfield<uint32_t, 4, 4> EXTI5;  /*!< EXTI 5 configuration bits */
            microhal::Bitfield<uint32_t, 8, 4> EXTI6;  /*!< EXTI 6 configuration bits */
            microhal::Bitfield<uint32_t, 12, 4> EXTI7; /*!< EXTI 7 configuration bits */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SYSCFG_EXTICR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SYSCFG_EXTICR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SYSCFG_EXTICR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SYSCFG_EXTICR2 operator&(uint32_t value) const {
            SYSCFG_EXTICR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SYSCFG_EXTICR2 operator|(uint32_t value) const {
            SYSCFG_EXTICR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SYSCFG_EXTICR2, AccessType::ReadOnly>;
        friend class VolatileRegister<SYSCFG_EXTICR2, AccessType::WriteOnly>;
        friend class VolatileRegister<SYSCFG_EXTICR2, AccessType::ReadWrite>;
    };

    // external interrupt configuration register 3
    union SYSCFG_EXTICR3 {
        union {
            microhal::Bitfield<uint32_t, 0, 4> EXTI8;   /*!< EXTI 8 configuration bits */
            microhal::Bitfield<uint32_t, 4, 4> EXTI9;   /*!< EXTI 9 configuration bits */
            microhal::Bitfield<uint32_t, 8, 4> EXTI10;  /*!< EXTI 10 configuration bits */
            microhal::Bitfield<uint32_t, 12, 4> EXTI11; /*!< EXTI 11 configuration bits */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SYSCFG_EXTICR3 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SYSCFG_EXTICR3 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SYSCFG_EXTICR3 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SYSCFG_EXTICR3 operator&(uint32_t value) const {
            SYSCFG_EXTICR3 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SYSCFG_EXTICR3 operator|(uint32_t value) const {
            SYSCFG_EXTICR3 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SYSCFG_EXTICR3, AccessType::ReadOnly>;
        friend class VolatileRegister<SYSCFG_EXTICR3, AccessType::WriteOnly>;
        friend class VolatileRegister<SYSCFG_EXTICR3, AccessType::ReadWrite>;
    };

    // external interrupt configuration register 4
    union SYSCFG_EXTICR4 {
        union {
            microhal::Bitfield<uint32_t, 0, 4> EXTI12;  /*!< EXTI 12 configuration bits */
            microhal::Bitfield<uint32_t, 4, 4> EXTI13;  /*!< EXTI 13 configuration bits */
            microhal::Bitfield<uint32_t, 8, 4> EXTI14;  /*!< EXTI 14 configuration bits */
            microhal::Bitfield<uint32_t, 12, 4> EXTI15; /*!< EXTI 15 configuration bits */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SYSCFG_EXTICR4 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SYSCFG_EXTICR4 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SYSCFG_EXTICR4 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SYSCFG_EXTICR4 operator&(uint32_t value) const {
            SYSCFG_EXTICR4 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SYSCFG_EXTICR4 operator|(uint32_t value) const {
            SYSCFG_EXTICR4 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SYSCFG_EXTICR4, AccessType::ReadOnly>;
        friend class VolatileRegister<SYSCFG_EXTICR4, AccessType::WriteOnly>;
        friend class VolatileRegister<SYSCFG_EXTICR4, AccessType::ReadWrite>;
    };

    // configuration register 2
    union SYSCFG_CFGR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LOCUP_LOCK;       /*!< Cortex-M0 LOCKUP bit enable bit */
            microhal::Bitfield<uint32_t, 1, 1> SRAM_PARITY_LOCK; /*!< SRAM parity lock bit */
            microhal::Bitfield<uint32_t, 2, 1> PVD_LOCK;         /*!< PVD lock enable bit */
            microhal::Bitfield<uint32_t, 4, 1> BYP_ADD_PAR;      /*!< Bypass address bit 29 in parity calculation */
            microhal::Bitfield<uint32_t, 8, 1> SRAM_PEF;         /*!< SRAM parity flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SYSCFG_CFGR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SYSCFG_CFGR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SYSCFG_CFGR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SYSCFG_CFGR2 operator&(uint32_t value) const {
            SYSCFG_CFGR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SYSCFG_CFGR2 operator|(uint32_t value) const {
            SYSCFG_CFGR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SYSCFG_CFGR2, AccessType::ReadOnly>;
        friend class VolatileRegister<SYSCFG_CFGR2, AccessType::WriteOnly>;
        friend class VolatileRegister<SYSCFG_CFGR2, AccessType::ReadWrite>;
    };

    // CCM SRAM protection register
    union SYSCFG_RCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PAGE0_WP; /*!< CCM SRAM page write protection bit */
            microhal::Bitfield<uint32_t, 1, 1> PAGE1_WP; /*!< CCM SRAM page write protection bit */
            microhal::Bitfield<uint32_t, 2, 1> PAGE2_WP; /*!< CCM SRAM page write protection bit */
            microhal::Bitfield<uint32_t, 3, 1> PAGE3_WP; /*!< CCM SRAM page write protection bit */
            microhal::Bitfield<uint32_t, 4, 1> PAGE4_WP; /*!< CCM SRAM page write protection bit */
            microhal::Bitfield<uint32_t, 5, 1> PAGE5_WP; /*!< CCM SRAM page write protection bit */
            microhal::Bitfield<uint32_t, 6, 1> PAGE6_WP; /*!< CCM SRAM page write protection bit */
            microhal::Bitfield<uint32_t, 7, 1> PAGE7_WP; /*!< CCM SRAM page write protection bit */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SYSCFG_RCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SYSCFG_RCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SYSCFG_RCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SYSCFG_RCR operator&(uint32_t value) const {
            SYSCFG_RCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SYSCFG_RCR operator|(uint32_t value) const {
            SYSCFG_RCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SYSCFG_RCR, AccessType::ReadOnly>;
        friend class VolatileRegister<SYSCFG_RCR, AccessType::WriteOnly>;
        friend class VolatileRegister<SYSCFG_RCR, AccessType::ReadWrite>;
    };

    // configuration register 3
    union SYSCFG_CFGR3 {
        union {
            microhal::Bitfield<uint32_t, 0, 2> SPI1_RX_DMA_RMP; /*!< SPI1_RX DMA remapping bit */
            microhal::Bitfield<uint32_t, 2, 2> SPI1_TX_DMA_RMP; /*!< SPI1_TX DMA remapping bit */
            microhal::Bitfield<uint32_t, 4, 2> I2C1_RX_DMA_RMP; /*!< I2C1_RX DMA remapping bit */
            microhal::Bitfield<uint32_t, 6, 2> ADC2_DMA_RMP_0;  /*!< ADC2 DMA channel remapping bit */
            microhal::Bitfield<uint32_t, 9, 1> ADC2_DMA_RMP_1;  /*!< ADC2 DMA controller remapping bit */
            microhal::Bitfield<uint32_t, 16, 1> DAC1_TRIG3_RMP; /*!< DAC1_CH1 / DAC1_CH2 Trigger remap */
            microhal::Bitfield<uint32_t, 17, 1> DAC1_TRIG5_RMP; /*!< DAC1_CH1 / DAC1_CH2 Trigger remap */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SYSCFG_CFGR3 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SYSCFG_CFGR3 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SYSCFG_CFGR3 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SYSCFG_CFGR3 operator&(uint32_t value) const {
            SYSCFG_CFGR3 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SYSCFG_CFGR3 operator|(uint32_t value) const {
            SYSCFG_CFGR3 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SYSCFG_CFGR3, AccessType::ReadOnly>;
        friend class VolatileRegister<SYSCFG_CFGR3, AccessType::WriteOnly>;
        friend class VolatileRegister<SYSCFG_CFGR3, AccessType::ReadWrite>;
    };

    // OPAMP2 control register
    union OPAMP2_CSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> OPAMP2EN;     /*!< OPAMP2 enable */
            microhal::Bitfield<uint32_t, 1, 1> FORCE_VP;     /*!< FORCE_VP */
            microhal::Bitfield<uint32_t, 2, 2> VP_SEL;       /*!< OPAMP2 Non inverting input selection */
            microhal::Bitfield<uint32_t, 5, 2> VM_SEL;       /*!< OPAMP2 inverting input selection */
            microhal::Bitfield<uint32_t, 7, 1> TCM_EN;       /*!< Timer controlled Mux mode enable */
            microhal::Bitfield<uint32_t, 8, 1> VMS_SEL;      /*!< OPAMP2 inverting input secondary selection */
            microhal::Bitfield<uint32_t, 9, 2> VPS_SEL;      /*!< OPAMP2 Non inverting input secondary selection */
            microhal::Bitfield<uint32_t, 11, 1> CALON;       /*!< Calibration mode enable */
            microhal::Bitfield<uint32_t, 12, 2> CAL_SEL;     /*!< Calibration selection */
            microhal::Bitfield<uint32_t, 14, 4> PGA_GAIN;    /*!< Gain in PGA mode */
            microhal::Bitfield<uint32_t, 18, 1> USER_TRIM;   /*!< User trimming enable */
            microhal::Bitfield<uint32_t, 19, 5> TRIMOFFSETP; /*!< Offset trimming value (PMOS) */
            microhal::Bitfield<uint32_t, 24, 5> TRIMOFFSETN; /*!< Offset trimming value (NMOS) */
            microhal::Bitfield<uint32_t, 29, 1> TSTREF;      /*!< TSTREF */
            microhal::Bitfield<uint32_t, 30, 1> OUTCAL;      /*!< OPAMP 2 ouput status flag */
            microhal::Bitfield<uint32_t, 31, 1> LOCK;        /*!< OPAMP 2 lock */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        OPAMP2_CSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OPAMP2_CSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OPAMP2_CSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        OPAMP2_CSR operator&(uint32_t value) const {
            OPAMP2_CSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        OPAMP2_CSR operator|(uint32_t value) const {
            OPAMP2_CSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<OPAMP2_CSR, AccessType::ReadOnly>;
        friend class VolatileRegister<OPAMP2_CSR, AccessType::WriteOnly>;
        friend class VolatileRegister<OPAMP2_CSR, AccessType::ReadWrite>;
    };

    // control and status register
    union COMP2_CSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> COMP2EN;         /*!< Comparator 2 enable */
            microhal::Bitfield<uint32_t, 2, 2> COMP2MODE;       /*!< Comparator 2 mode */
            microhal::Bitfield<uint32_t, 4, 3> COMP2INSEL;      /*!< Comparator 2 inverting input selection */
            microhal::Bitfield<uint32_t, 7, 1> COMP2INPSEL;     /*!< Comparator 2 non inverted input selection */
            microhal::Bitfield<uint32_t, 9, 1> COMP2INMSEL;     /*!< Comparator 1inverting input selection */
            microhal::Bitfield<uint32_t, 10, 4> COMP2_OUT_SEL;  /*!< Comparator 2 output selection */
            microhal::Bitfield<uint32_t, 15, 1> COMP2POL;       /*!< Comparator 2 output polarity */
            microhal::Bitfield<uint32_t, 16, 2> COMP2HYST;      /*!< Comparator 2 hysteresis */
            microhal::Bitfield<uint32_t, 18, 3> COMP2_BLANKING; /*!< Comparator 2 blanking source */
            microhal::Bitfield<uint32_t, 30, 1> COMP2OUT;       /*!< Comparator 2 output */
            microhal::Bitfield<uint32_t, 31, 1> COMP2LOCK;      /*!< Comparator 2 lock */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        COMP2_CSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        COMP2_CSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        COMP2_CSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        COMP2_CSR operator&(uint32_t value) const {
            COMP2_CSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        COMP2_CSR operator|(uint32_t value) const {
            COMP2_CSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<COMP2_CSR, AccessType::ReadOnly>;
        friend class VolatileRegister<COMP2_CSR, AccessType::WriteOnly>;
        friend class VolatileRegister<COMP2_CSR, AccessType::ReadWrite>;
    };

    // control and status register
    union COMP4_CSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> COMP4EN;         /*!< Comparator 4 enable */
            microhal::Bitfield<uint32_t, 2, 2> COMP4MODE;       /*!< Comparator 4 mode */
            microhal::Bitfield<uint32_t, 4, 3> COMP4INSEL;      /*!< Comparator 4 inverting input selection */
            microhal::Bitfield<uint32_t, 7, 1> COMP4INPSEL;     /*!< Comparator 4 non inverted input selection */
            microhal::Bitfield<uint32_t, 9, 1> COM4WINMODE;     /*!< Comparator 4 window mode */
            microhal::Bitfield<uint32_t, 10, 4> COMP4_OUT_SEL;  /*!< Comparator 4 output selection */
            microhal::Bitfield<uint32_t, 15, 1> COMP4POL;       /*!< Comparator 4 output polarity */
            microhal::Bitfield<uint32_t, 16, 2> COMP4HYST;      /*!< Comparator 4 hysteresis */
            microhal::Bitfield<uint32_t, 18, 3> COMP4_BLANKING; /*!< Comparator 4 blanking source */
            microhal::Bitfield<uint32_t, 30, 1> COMP4OUT;       /*!< Comparator 4 output */
            microhal::Bitfield<uint32_t, 31, 1> COMP4LOCK;      /*!< Comparator 4 lock */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        COMP4_CSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        COMP4_CSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        COMP4_CSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        COMP4_CSR operator&(uint32_t value) const {
            COMP4_CSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        COMP4_CSR operator|(uint32_t value) const {
            COMP4_CSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<COMP4_CSR, AccessType::ReadOnly>;
        friend class VolatileRegister<COMP4_CSR, AccessType::WriteOnly>;
        friend class VolatileRegister<COMP4_CSR, AccessType::ReadWrite>;
    };

    // control and status register
    union COMP6_CSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> COMP6EN;         /*!< Comparator 6 enable */
            microhal::Bitfield<uint32_t, 2, 2> COMP6MODE;       /*!< Comparator 6 mode */
            microhal::Bitfield<uint32_t, 4, 3> COMP6INSEL;      /*!< Comparator 6 inverting input selection */
            microhal::Bitfield<uint32_t, 7, 1> COMP6INPSEL;     /*!< Comparator 6 non inverted input selection */
            microhal::Bitfield<uint32_t, 9, 1> COM6WINMODE;     /*!< Comparator 6 window mode */
            microhal::Bitfield<uint32_t, 10, 4> COMP6_OUT_SEL;  /*!< Comparator 6 output selection */
            microhal::Bitfield<uint32_t, 15, 1> COMP6POL;       /*!< Comparator 6 output polarity */
            microhal::Bitfield<uint32_t, 16, 2> COMP6HYST;      /*!< Comparator 6 hysteresis */
            microhal::Bitfield<uint32_t, 18, 3> COMP6_BLANKING; /*!< Comparator 6 blanking source */
            microhal::Bitfield<uint32_t, 30, 1> COMP6OUT;       /*!< Comparator 6 output */
            microhal::Bitfield<uint32_t, 31, 1> COMP6LOCK;      /*!< Comparator 6 lock */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        COMP6_CSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        COMP6_CSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        COMP6_CSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        COMP6_CSR operator&(uint32_t value) const {
            COMP6_CSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        COMP6_CSR operator|(uint32_t value) const {
            COMP6_CSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<COMP6_CSR, AccessType::ReadOnly>;
        friend class VolatileRegister<COMP6_CSR, AccessType::WriteOnly>;
        friend class VolatileRegister<COMP6_CSR, AccessType::ReadWrite>;
    };

    VolatileRegister<SYSCFG_CFGR1, AccessType::ReadWrite> cfgr1;        /*!< configuration register 1	Address offset: 0x0 */
    VolatileRegister<SYSCFG_RCR, AccessType::ReadWrite> rcr;            /*!< CCM SRAM protection register	Address offset: 0x4 */
    VolatileRegister<EXTICR, AccessType::ReadWrite> exticr[4];          /*!< external interrupt configuration register 1	Address offset: 0x8 */
    VolatileRegister<SYSCFG_CFGR2, AccessType::ReadWrite> cfgr2;        /*!< configuration register 2	Address offset: 0x18 */
    uint32_t reserved1[1];                                              /*!< Reserved register	Address offset: 0x1c */
    VolatileRegister<COMP2_CSR, AccessType::ReadWrite> comp2_csr;       /*!< control and status register	Address offset: 0x20 */
    uint32_t reserved5[1];                                              /*!< Reserved register	Address offset: 0x24 */
    VolatileRegister<COMP4_CSR, AccessType::ReadWrite> comp4_csr;       /*!< control and status register	Address offset: 0x28 */
    uint32_t reserved6[1];                                              /*!< Reserved register	Address offset: 0x2c */
    VolatileRegister<COMP6_CSR, AccessType::ReadWrite> comp6_csr;       /*!< control and status register	Address offset: 0x30 */
    VolatileRegister<OPAMP2_CSR, AccessType::ReadWrite> opamp2_csr;     /*!< OPAMP2 control register	Address offset: 0x3C */
    uint32_t reserved4[4];                                              /*!< Reserved register	Address offset: 0x40 */
    VolatileRegister<SYSCFG_CFGR3, AccessType::ReadWrite> syscfg_cfgr3; /*!< configuration register 3	Address offset: 0x50 */
};

#if defined(_MICROHAL_SYSCFG_COMP_OPAMP_BASE_ADDRESS)
[[maybe_unused]] inline SYSCFG_COMP_OPAMP *const syscfg = reinterpret_cast<SYSCFG_COMP_OPAMP *>(_MICROHAL_SYSCFG_COMP_OPAMP_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_SYSCFG_COMP_OPAMP
