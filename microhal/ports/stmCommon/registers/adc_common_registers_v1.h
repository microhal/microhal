#ifndef _MICROHAL_PERIPHERAL_REGISTER_ADC3_Common
#define _MICROHAL_PERIPHERAL_REGISTER_ADC3_Common

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"
// Supported MCU version 1: STM32H742x, STM32H7x3, STM32H743x, STM32H753x, STM32H7x5_CM7, STM32H7x5_CM4
// Supported MCU version 2: STM32H750x
// Supported MCU version 3: STM32F3x4
// Supported MCU version 4: STM32L4x6, STM32L4R7, STM32L4S9, STM32L4R9, STM32L4S5, STM32L4S7, STM32L4R5,STM32G474xx, STM32G484xx, STM32G471xx,
// STM32G431xx, STM32G473xx, STM32G441xx, STM32GBK1CBT6, STM32G483xx
// Supported MCU version 5: STM32L4x5, STM32F302, STM32F303

#if defined(STM32H742x) || defined(STM32H7x3) || defined(STM32H743x) || defined(STM32H753x) || defined(STM32H7x5_CM7) || \
    defined(STM32H7x5_CM4)  // version 1
#define _MICROHAL_REGISTERS_ADC_COMMON_HAS_CDR2
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_PRESC
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_DAMDF
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_V1
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_TSEN_VBATEN
#endif

#if defined(STM32H750x)  // version 2
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_PRESC
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_DAMDF
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_V1
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_TSEN_VBATEN
#endif

#if defined(STM32F3x4)  // vesion 3
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_DMACFG
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_V1
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_TSEN_VBATEN
#endif

#if defined(STM32L4x6) || defined(STM32L4R7) || defined(STM32L4S9) || defined(STM32L4R9) || defined(STM32L4S5) || defined(STM32L4S7) ||           \
    defined(STM32L4R5) || defined(STM32G474xx) || defined(STM32G484xx) || defined(STM32G471xx) || defined(STM32G431xx) || defined(STM32G473xx) || \
    defined(STM32G441xx) || defined(STM32GBK1CBT6) || defined(STM32G483xx)  // version 4
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_V1
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_DMACFG
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_PRESC
#endif

#if defined(_MICROHAL_STM32G0XX_STM32G071xx) || defined(_MICROHAL_STM32G0XX_STM32G070xx)  // version 5
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_V1
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_DMACFG
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_PRESC
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_TSEN_VBATEN
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct ADCCommon {
    // ADC Common status register
    union CSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ADRDY_MST;  /*!< Master ADC ready */
            microhal::Bitfield<uint32_t, 1, 1> EOSMP_MST;  /*!< End of Sampling phase flag of the master ADC */
            microhal::Bitfield<uint32_t, 2, 1> EOC_MST;    /*!< End of regular conversion of the master ADC */
            microhal::Bitfield<uint32_t, 3, 1> EOS_MST;    /*!< End of regular sequence flag of the master ADC */
            microhal::Bitfield<uint32_t, 4, 1> OVR_MST;    /*!< Overrun flag of the master ADC */
            microhal::Bitfield<uint32_t, 5, 1> JEOC_MST;   /*!< End of injected conversion flag of the master ADC */
            microhal::Bitfield<uint32_t, 6, 1> JEOS_MST;   /*!< End of injected sequence flag of the master ADC */
            microhal::Bitfield<uint32_t, 7, 1> AWD1_MST;   /*!< Analog watchdog 1 flag of the master ADC */
            microhal::Bitfield<uint32_t, 8, 1> AWD2_MST;   /*!< Analog watchdog 2 flag of the master ADC */
            microhal::Bitfield<uint32_t, 9, 1> AWD3_MST;   /*!< Analog watchdog 3 flag of the master ADC */
            microhal::Bitfield<uint32_t, 10, 1> JQOVF_MST; /*!< Injected Context Queue Overflow flag of the master ADC */
            microhal::Bitfield<uint32_t, 16, 1> ADRDY_SLV; /*!< Slave ADC ready */
            microhal::Bitfield<uint32_t, 17, 1> EOSMP_SLV; /*!< End of Sampling phase flag of the slave ADC */
            microhal::Bitfield<uint32_t, 18, 1> EOC_SLV;   /*!< End of regular conversion of the slave ADC */
            microhal::Bitfield<uint32_t, 19, 1> EOS_SLV;   /*!< End of regular sequence flag of the slave ADC */
            microhal::Bitfield<uint32_t, 20, 1> OVR_SLV;   /*!< Overrun flag of the slave ADC */
            microhal::Bitfield<uint32_t, 21, 1> JEOC_SLV;  /*!< End of injected conversion flag of the slave ADC */
            microhal::Bitfield<uint32_t, 22, 1> JEOS_SLV;  /*!< End of injected sequence flag of the slave ADC */
            microhal::Bitfield<uint32_t, 23, 1> AWD1_SLV;  /*!< Analog watchdog 1 flag of the slave ADC */
            microhal::Bitfield<uint32_t, 24, 1> AWD2_SLV;  /*!< Analog watchdog 2 flag of the slave ADC */
            microhal::Bitfield<uint32_t, 25, 1> AWD3_SLV;  /*!< Analog watchdog 3 flag of the slave ADC */
            microhal::Bitfield<uint32_t, 26, 1> JQOVF_SLV; /*!< Injected Context Queue Overflow flag of the slave ADC */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CSR operator&(uint32_t value) const {
            CSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CSR operator|(uint32_t value) const {
            CSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CSR, AccessType::ReadOnly>;
        friend class VolatileRegister<CSR, AccessType::WriteOnly>;
        friend class VolatileRegister<CSR, AccessType::ReadWrite>;
    };

    // ADC common control register
    union CCR {
#ifdef _MICROHAL_REGISTERS_ADC_COMMON_CCR_V1
        union {
            microhal::Bitfield<uint32_t, 0, 5> DUAL;  /*!< Dual ADC mode selection */
            microhal::Bitfield<uint32_t, 8, 4> DELAY; /*!< Delay between 2 sampling phases */
#ifdef _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_DMACFG
            microhal::Bitfield<uint32_t, 13, 1> DMACFG; /*!< DMA configuration (for dual ADC mode) */
#endif
#if _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_DAMDF
            microhal::Bitfield<uint32_t, 14, 2> DAMDF; /*!< Dual ADC Mode Data Format */
#else
            microhal::Bitfield<uint32_t, 14, 2> MDMA;    /*!< Direct memory access mode for dual ADC mode */
#endif
            microhal::Bitfield<uint32_t, 16, 2> CKMODE; /*!< ADC clock mode */
#ifdef _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_PRESC
            microhal::Bitfield<uint32_t, 18, 4> PRESC; /*!< ADC prescaler */
#endif
            microhal::Bitfield<uint32_t, 22, 1> VREFEN; /*!< VREFINT enable */
#ifdef _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_TSEN_VBATEN
            microhal::Bitfield<uint32_t, 23, 1> TSEN;   /*!< Temperature sensor enable */
            microhal::Bitfield<uint32_t, 24, 1> VBATEN; /*!< VBAT enable */
#else
            microhal::Bitfield<uint32_t, 23, 1> CH17SEL; /*!< CH17 selection */
            microhal::Bitfield<uint32_t, 24, 1> CH18SEL; /*!< CH18 selection */
#endif
        };
#else
        union {
            microhal::Bitfield<uint32_t, 0, 5> MULT;    /*!< Multi ADC mode selection */
            microhal::Bitfield<uint32_t, 8, 4> DELAY;   /*!< Delay between 2 sampling phases */
            microhal::Bitfield<uint32_t, 13, 1> DMACFG; /*!< DMA configuration (for multi-ADC mode) */
            microhal::Bitfield<uint32_t, 14, 2> MDMA;   /*!< Direct memory access mode for multi ADC mode */
            microhal::Bitfield<uint32_t, 16, 2> CKMODE; /*!< ADC clock mode */
            microhal::Bitfield<uint32_t, 22, 1> VREFEN; /*!< VREFINT enable */
            microhal::Bitfield<uint32_t, 23, 1> TSEN;   /*!< Temperature sensor enable */
            microhal::Bitfield<uint32_t, 24, 1> VBATEN; /*!< VBAT enable */
        };
#endif
        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CCR operator&(uint32_t value) const {
            CCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CCR operator|(uint32_t value) const {
            CCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CCR, AccessType::ReadOnly>;
        friend class VolatileRegister<CCR, AccessType::WriteOnly>;
        friend class VolatileRegister<CCR, AccessType::ReadWrite>;
    };

    // ADC common regular data register for dual and triple modes
    union CDR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> RDATA_MST;  /*!< Regular data of the master ADC */
            microhal::Bitfield<uint32_t, 16, 16> RDATA_SLV; /*!< Regular data of the slave ADC */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CDR operator&(uint32_t value) const {
            CDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CDR operator|(uint32_t value) const {
            CDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CDR, AccessType::ReadOnly>;
        friend class VolatileRegister<CDR, AccessType::WriteOnly>;
        friend class VolatileRegister<CDR, AccessType::ReadWrite>;
    };

    // ADC x common regular data register for 32-bit dual mode
    union CDR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 32> RDATA_ALT; /*!< Regular data of the master/slave alternated ADCs */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CDR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CDR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CDR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CDR2 operator&(uint32_t value) const {
            CDR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CDR2 operator|(uint32_t value) const {
            CDR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CDR2, AccessType::ReadOnly>;
        friend class VolatileRegister<CDR2, AccessType::WriteOnly>;
        friend class VolatileRegister<CDR2, AccessType::ReadWrite>;
    };

    VolatileRegister<CSR, AccessType::ReadWrite> csr; /*!< ADC Common status register	Address offset: 0x0 */
    uint32_t reserved0;                               /*!< Reserved register	Address offset: 0x4 */
    VolatileRegister<CCR, AccessType::ReadWrite> ccr; /*!< ADC common control register	Address offset: 0x8 */
    VolatileRegister<CDR, AccessType::ReadWrite> cdr; /*!< ADC common regular data register for dual and triple modes	Address offset: 0xC */
#ifdef _MICROHAL_REGISTERS_ADC_COMMON_HAS_CDR2
    VolatileRegister<CDR2, AccessType::ReadWrite> cdr2; /*!< ADC x common regular data register for 32-bit dual mode	Address offset: 0x10 */
#endif
};

#if defined(_MICROHAL_ADC_COMMON_BASE_ADDRESS)
[[maybe_unused]] static ADCCommon *const adc12Common = reinterpret_cast<ADCCommon *>(_MICROHAL_ADC_COMMON_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_ADC3_Common
