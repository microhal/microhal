#ifndef _MICROHAL_PERIPHERAL_REGISTER_SYSCFG_COMP
#define _MICROHAL_PERIPHERAL_REGISTER_SYSCFG_COMP

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32F0x2, STM32F0x8, STM32F0x1
namespace microhal {
namespace registers {
/**
 *
 */
struct SYSCFG_COMP {
    // configuration register 1
    union CFGR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 2> MEM_MODE;           /*!< Memory mapping selection bits */
            microhal::Bitfield<uint32_t, 8, 1> ADC_DMA_RMP;        /*!< ADC DMA remapping bit */
            microhal::Bitfield<uint32_t, 9, 1> USART1_TX_DMA_RMP;  /*!< USART1_TX DMA remapping bit */
            microhal::Bitfield<uint32_t, 10, 1> USART1_RX_DMA_RMP; /*!< USART1_RX DMA request remapping bit */
            microhal::Bitfield<uint32_t, 11, 1> TIM16_DMA_RMP;     /*!< TIM16 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 12, 1> TIM17_DMA_RMP;     /*!< TIM17 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 16, 1> I2C_PB6_FM;        /*!< Fast Mode Plus (FM plus) driving capability activation bits. */
            microhal::Bitfield<uint32_t, 17, 1> I2C_PB7_FM;        /*!< Fast Mode Plus (FM+) driving capability activation bits. */
            microhal::Bitfield<uint32_t, 18, 1> I2C_PB8_FM;        /*!< Fast Mode Plus (FM+) driving capability activation bits. */
            microhal::Bitfield<uint32_t, 19, 1> I2C_PB9_FM;        /*!< Fast Mode Plus (FM+) driving capability activation bits. */
            microhal::Bitfield<uint32_t, 20, 1> I2C1_FM_plus;      /*!< FM+ driving capability activation for I2C1 */
            microhal::Bitfield<uint32_t, 21, 1> I2C2_FM_plus;      /*!< FM+ driving capability activation for I2C2 */
            microhal::Bitfield<uint32_t, 24, 1> SPI2_DMA_RMP;      /*!< SPI2 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 25, 1> USART2_DMA_RMP;    /*!< USART2 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 26, 1> USART3_DMA_RMP;    /*!< USART3 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 27, 1> I2C1_DMA_RMP;      /*!< I2C1 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 28, 1> TIM1_DMA_RMP;      /*!< TIM1 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 29, 1> TIM2_DMA_RMP;      /*!< TIM2 DMA request remapping bit */
            microhal::Bitfield<uint32_t, 30, 1> TIM3_DMA_RMP;      /*!< TIM3 DMA request remapping bit */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CFGR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CFGR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CFGR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CFGR1 operator&(uint32_t value) const {
            CFGR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CFGR1 operator|(uint32_t value) const {
            CFGR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CFGR1, AccessType::ReadOnly>;
        friend class VolatileRegister<CFGR1, AccessType::WriteOnly>;
        friend class VolatileRegister<CFGR1, AccessType::ReadWrite>;
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

    // configuration register 2
    union CFGR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LOCUP_LOCK;       /*!< Cortex-M0 LOCKUP bit enable bit */
            microhal::Bitfield<uint32_t, 1, 1> SRAM_PARITY_LOCK; /*!< SRAM parity lock bit */
            microhal::Bitfield<uint32_t, 2, 1> PVD_LOCK;         /*!< PVD lock enable bit */
            microhal::Bitfield<uint32_t, 8, 1> SRAM_PEF;         /*!< SRAM parity flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CFGR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CFGR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CFGR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CFGR2 operator&(uint32_t value) const {
            CFGR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CFGR2 operator|(uint32_t value) const {
            CFGR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CFGR2, AccessType::ReadOnly>;
        friend class VolatileRegister<CFGR2, AccessType::WriteOnly>;
        friend class VolatileRegister<CFGR2, AccessType::ReadWrite>;
    };

    // control and status register
    union COMP_CSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> COMP1EN;       /*!< Comparator 1 enable */
            microhal::Bitfield<uint32_t, 1, 1> COMP1_INP_DAC; /*!< COMP1_INP_DAC */
            microhal::Bitfield<uint32_t, 2, 2> COMP1MODE;     /*!< Comparator 1 mode */
            microhal::Bitfield<uint32_t, 4, 3> COMP1INSEL;    /*!< Comparator 1 inverting input selection */
            microhal::Bitfield<uint32_t, 8, 3> COMP1OUTSEL;   /*!< Comparator 1 output selection */
            microhal::Bitfield<uint32_t, 11, 1> COMP1POL;     /*!< Comparator 1 output polarity */
            microhal::Bitfield<uint32_t, 12, 2> COMP1HYST;    /*!< Comparator 1 hysteresis */
            microhal::Bitfield<uint32_t, 14, 1> COMP1OUT;     /*!< Comparator 1 output */
            microhal::Bitfield<uint32_t, 15, 1> COMP1LOCK;    /*!< Comparator 1 lock */
            microhal::Bitfield<uint32_t, 16, 1> COMP2EN;      /*!< Comparator 2 enable */
            microhal::Bitfield<uint32_t, 18, 2> COMP2MODE;    /*!< Comparator 2 mode */
            microhal::Bitfield<uint32_t, 20, 3> COMP2INSEL;   /*!< Comparator 2 inverting input selection */
            microhal::Bitfield<uint32_t, 23, 1> WNDWEN;       /*!< Window mode enable */
            microhal::Bitfield<uint32_t, 24, 3> COMP2OUTSEL;  /*!< Comparator 2 output selection */
            microhal::Bitfield<uint32_t, 27, 1> COMP2POL;     /*!< Comparator 2 output polarity */
            microhal::Bitfield<uint32_t, 28, 2> COMP2HYST;    /*!< Comparator 2 hysteresis */
            microhal::Bitfield<uint32_t, 30, 1> COMP2OUT;     /*!< Comparator 2 output */
            microhal::Bitfield<uint32_t, 31, 1> COMP2LOCK;    /*!< Comparator 2 lock */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        COMP_CSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        COMP_CSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        COMP_CSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        COMP_CSR operator&(uint32_t value) const {
            COMP_CSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        COMP_CSR operator|(uint32_t value) const {
            COMP_CSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<COMP_CSR, AccessType::ReadOnly>;
        friend class VolatileRegister<COMP_CSR, AccessType::WriteOnly>;
        friend class VolatileRegister<COMP_CSR, AccessType::ReadWrite>;
    };

    VolatileRegister<CFGR1, AccessType::ReadWrite> cfgr1;       /*!< configuration register 1	Address offset: 0x0 */
    uint32_t reserved0[1];                                      /*!< Reserved register	Address offset: 0x4 */
    VolatileRegister<EXTICR, AccessType::ReadWrite> exticr[4];  /*!< external interrupt configuration register 1	Address offset: 0x8 */
    VolatileRegister<CFGR2, AccessType::ReadWrite> cfgr2;       /*!< configuration register 2	Address offset: 0x18 */
    VolatileRegister<COMP_CSR, AccessType::ReadWrite> comp_csr; /*!< control and status register	Address offset: 0x1C */
};

#if defined(_MICROHAL_SYSCFG_COMP_BASE_ADDRESS)
[[maybe_unused]] inline SYSCFG_COMP *const syscfg = reinterpret_cast<SYSCFG_COMP *>(_MICROHAL_SYSCFG_COMP_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_SYSCFG_COMP
