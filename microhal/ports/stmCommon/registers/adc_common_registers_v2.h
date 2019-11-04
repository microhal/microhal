#ifndef _MICROHAL_PERIPHERAL_REGISTER_C_ADC
#define _MICROHAL_PERIPHERAL_REGISTER_C_ADC

#include "bitfield.h"
#include "registersBaseAddressDefinitions.h"
#include "volatileRegister.h"

// Supported MCU version 1: STM32F765, STM32F7x3, STM32F7x2, STM32F745, STM32F730, STM32F750, STM32F7x6, STM32F407, STM32F429, STM32F469, STM32F427,
// STM32F405, STM32F446, STM32F217, STM32F215
// Supported MCU version 2 : STM32F411, STM32F413, STM32F412, STM32F401, STM32F410

#if defined(STM32F765) || defined(STM32F7x3) || defined(STM32F7x2) || defined(STM32F745) || defined(STM32F730) || defined(STM32F750) || \
    defined(STM32F7x6) || defined(STM32F407) || defined(STM32F429) || defined(STM32F469) || defined(STM32F427) || defined(STM32F405) || \
    defined(STM32F446) || defined(STM32F217) || defined(STM32F215)  // version1
#define _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_MULT
#define _MICROHAL_REGISTERS_ADC_COMMON_HAS_CDR
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
            microhal::Bitfield<uint32_t, 0, 1> AWD1;    /*!< Analog watchdog flag of ADC 1 */
            microhal::Bitfield<uint32_t, 1, 1> EOC1;    /*!< End of conversion of ADC 1 */
            microhal::Bitfield<uint32_t, 2, 1> JEOC1;   /*!< Injected channel end of conversion of ADC 1 */
            microhal::Bitfield<uint32_t, 3, 1> JSTRT1;  /*!< Injected channel Start flag of ADC 1 */
            microhal::Bitfield<uint32_t, 4, 1> STRT1;   /*!< Regular channel Start flag of ADC 1 */
            microhal::Bitfield<uint32_t, 5, 1> OVR1;    /*!< Overrun flag of ADC 1 */
            microhal::Bitfield<uint32_t, 8, 1> AWD2;    /*!< Analog watchdog flag of ADC 2 */
            microhal::Bitfield<uint32_t, 9, 1> EOC2;    /*!< End of conversion of ADC 2 */
            microhal::Bitfield<uint32_t, 10, 1> JEOC2;  /*!< Injected channel end of conversion of ADC 2 */
            microhal::Bitfield<uint32_t, 11, 1> JSTRT2; /*!< Injected channel Start flag of ADC 2 */
            microhal::Bitfield<uint32_t, 12, 1> STRT2;  /*!< Regular channel Start flag of ADC 2 */
            microhal::Bitfield<uint32_t, 13, 1> OVR2;   /*!< Overrun flag of ADC 2 */
            microhal::Bitfield<uint32_t, 16, 1> AWD3;   /*!< Analog watchdog flag of ADC 3 */
            microhal::Bitfield<uint32_t, 17, 1> EOC3;   /*!< End of conversion of ADC 3 */
            microhal::Bitfield<uint32_t, 18, 1> JEOC3;  /*!< Injected channel end of conversion of ADC 3 */
            microhal::Bitfield<uint32_t, 19, 1> JSTRT3; /*!< Injected channel Start flag of ADC 3 */
            microhal::Bitfield<uint32_t, 20, 1> STRT3;  /*!< Regular channel Start flag of ADC 3 */
            microhal::Bitfield<uint32_t, 21, 1> OVR3;   /*!< Overrun flag of ADC3 */
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
        union {
#ifdef _MICROHAL_REGISTERS_ADC_COMMON_CCR_HAS_MULT
            microhal::Bitfield<uint32_t, 0, 5> MULT; /*!< Multi ADC mode selection */
#endif
            microhal::Bitfield<uint32_t, 8, 4> DELAY;    /*!< Delay between 2 sampling phases */
            microhal::Bitfield<uint32_t, 13, 1> DDS;     /*!< DMA disable selection for multi-ADC mode */
            microhal::Bitfield<uint32_t, 14, 2> DMA;     /*!< Direct memory access mode for multi ADC mode */
            microhal::Bitfield<uint32_t, 16, 2> ADCPRE;  /*!< ADC prescaler */
            microhal::Bitfield<uint32_t, 22, 1> VBATE;   /*!< VBAT enable */
            microhal::Bitfield<uint32_t, 23, 1> TSVREFE; /*!< Temperature sensor and VREFINT enable */
        };

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
            microhal::Bitfield<uint32_t, 0, 16> DATA1;  /*!< 1st data item of a pair of regular conversions */
            microhal::Bitfield<uint32_t, 16, 16> DATA2; /*!< 2nd data item of a pair of regular conversions */
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

    VolatileRegister<CSR, AccessType::ReadWrite> csr; /*!< ADC Common status register	Address offset: 0x0 */
    VolatileRegister<CCR, AccessType::ReadWrite> ccr; /*!< ADC common control register	Address offset: 0x4 */
#ifdef _MICROHAL_REGISTERS_ADC_COMMON_HAS_CDR
    VolatileRegister<CDR, AccessType::ReadWrite> cdr; /*!< ADC common regular data register for dual and triple modes	Address offset: 0x8 */
#endif
};
}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_C_ADC
