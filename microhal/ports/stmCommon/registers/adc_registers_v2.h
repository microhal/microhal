#ifndef _MICROHAL_PERIPHERAL_REGISTER_ADC
#define _MICROHAL_PERIPHERAL_REGISTER_ADC

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU version 1: STM32F102, STM32F107, STM32F101, STM32F103, STM32F101
// Supported MCU version 2: STM32F7x9, STM32F765, STM32F7x8, STM32F7x3, STM32F7x2, STM32F745, STM32F730, STM32F750, STM32F7x7, STM32F7x6, STM32F407,
//                          STM32F429, STM32F469, STM32F411, STM32F413, STM32F412, STM32F401, STM32F427, STM32F405, STM32F446, STM32F410, STM32F217,
//                          STM32F215

#if defined(STM32F102) || defined(STM32F107) || defined(STM32F101) || defined(STM32F103) || defined(STM32F101)  // version 1
#define _MICROHAL_REGISTERS_ADC_CR1_HAS_DUALMOD
#define _MICROHAL_REGISTERS_ADC_CR2_VERSION1
#endif

#ifdef version 2
#define _MICROHAL_REGISTERS_ADC_SR_HAS_OVR
#define _MICROHAL_REGISTERS_ADC_CR1_HAS_RES_OVRIE
#define _MICROHAL_REGISTERS_ADC_CR2_VERSION1
#define _MICROHAL_REGISTERS_ADC_SMPR1_HAS_SMP18
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct ADC {
    // status register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> AWD;   /*!< Analog watchdog flag */
            microhal::Bitfield<uint32_t, 1, 1> EOC;   /*!< Regular channel end of conversion */
            microhal::Bitfield<uint32_t, 2, 1> JEOC;  /*!< Injected channel end of conversion */
            microhal::Bitfield<uint32_t, 3, 1> JSTRT; /*!< Injected channel start flag */
            microhal::Bitfield<uint32_t, 4, 1> STRT;  /*!< Regular channel start flag */
#ifdef _MICROHAL_REGISTERS_ADC_SR_HAS_OVR
            microhal::Bitfield<uint32_t, 5, 1> OVR; /*!< Overrun */
#endif
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

    // control register 1
    union CR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 5> AWDCH;    /*!< Analog watchdog channel select bits */
            microhal::Bitfield<uint32_t, 5, 1> EOCIE;    /*!< Interrupt enable for EOC */
            microhal::Bitfield<uint32_t, 6, 1> AWDIE;    /*!< Analog watchdog interrupt enable */
            microhal::Bitfield<uint32_t, 7, 1> JEOCIE;   /*!< Interrupt enable for injected channels */
            microhal::Bitfield<uint32_t, 8, 1> SCAN;     /*!< Scan mode */
            microhal::Bitfield<uint32_t, 9, 1> AWDSGL;   /*!< Enable the watchdog on a single channel in scan mode */
            microhal::Bitfield<uint32_t, 10, 1> JAUTO;   /*!< Automatic injected group conversion */
            microhal::Bitfield<uint32_t, 11, 1> DISCEN;  /*!< Discontinuous mode on regular channels */
            microhal::Bitfield<uint32_t, 12, 1> JDISCEN; /*!< Discontinuous mode on injected channels */
            microhal::Bitfield<uint32_t, 13, 3> DISCNUM; /*!< Discontinuous mode channel count */
#ifdef _MICROHAL_REGISTERS_ADC_CR1_HAS_DUALMOD
            microhal::Bitfield<uint32_t, 16, 4> DUALMOD; /*!< Dual mode selection */
#endif
            microhal::Bitfield<uint32_t, 22, 1> JAWDEN; /*!< Analog watchdog enable on injected channels */
            microhal::Bitfield<uint32_t, 23, 1> AWDEN;  /*!< Analog watchdog enable on regular channels */
#if _MICROHAL_REGISTERS_ADC_CR1_HAS_RES_OVRIE
            microhal::Bitfield<uint32_t, 24, 2> RES;   /*!< Resolution */
            microhal::Bitfield<uint32_t, 26, 1> OVRIE; /*!< Overrun interrupt enable */
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
#ifdef _MICROHAL_REGISTERS_ADC_CR2_VERSION1
            microhal::Bitfield<uint32_t, 0, 1> ADON;      /*!< A/D converter ON / OFF */
            microhal::Bitfield<uint32_t, 1, 1> CONT;      /*!< Continuous conversion */
            microhal::Bitfield<uint32_t, 2, 1> CAL;       /*!< A/D calibration */
            microhal::Bitfield<uint32_t, 3, 1> RSTCAL;    /*!< Reset calibration */
            microhal::Bitfield<uint32_t, 8, 1> DMA;       /*!< Direct memory access mode */
            microhal::Bitfield<uint32_t, 11, 1> ALIGN;    /*!< Data alignment */
            microhal::Bitfield<uint32_t, 12, 3> JEXTSEL;  /*!< External event select for injected group */
            microhal::Bitfield<uint32_t, 15, 1> JEXTTRIG; /*!< External trigger conversion mode for injected channels */
            microhal::Bitfield<uint32_t, 17, 3> EXTSEL;   /*!< External event select for regular group */
            microhal::Bitfield<uint32_t, 20, 1> EXTTRIG;  /*!< External trigger conversion mode for regular channels */
            microhal::Bitfield<uint32_t, 21, 1> JSWSTART; /*!< Start conversion of injected channels */
            microhal::Bitfield<uint32_t, 22, 1> SWSTART;  /*!< Start conversion of regular channels */
            microhal::Bitfield<uint32_t, 23, 1> TSVREFE;  /*!< Temperature sensor and VREFINT enable */
#endif
#ifdef _MICROHAL_REGISTERS_ADC_CR2_VERSION2
            microhal::Bitfield<uint32_t, 0, 1> ADON;      /*!< A/D Converter ON / OFF */
            microhal::Bitfield<uint32_t, 1, 1> CONT;      /*!< Continuous conversion */
            microhal::Bitfield<uint32_t, 8, 1> DMA;       /*!< Direct memory access mode (for single ADC mode) */
            microhal::Bitfield<uint32_t, 9, 1> DDS;       /*!< DMA disable selection (for single ADC mode) */
            microhal::Bitfield<uint32_t, 10, 1> EOCS;     /*!< End of conversion selection */
            microhal::Bitfield<uint32_t, 11, 1> ALIGN;    /*!< Data alignment */
            microhal::Bitfield<uint32_t, 16, 4> JEXTSEL;  /*!< External event select for injected group */
            microhal::Bitfield<uint32_t, 20, 2> JEXTEN;   /*!< External trigger enable for injected channels */
            microhal::Bitfield<uint32_t, 22, 1> JSWSTART; /*!< Start conversion of injected channels */
            microhal::Bitfield<uint32_t, 24, 4> EXTSEL;   /*!< External event select for regular group */
            microhal::Bitfield<uint32_t, 28, 2> EXTEN;    /*!< External trigger enable for regular channels */
            microhal::Bitfield<uint32_t, 30, 1> SWSTART;  /*!< Start conversion of regular channels */

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

    // sample time register 1
    union SMPR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 3> SMP10;  /*!< Channel 10 sampling time selection */
            microhal::Bitfield<uint32_t, 3, 3> SMP11;  /*!< Channel 11 sampling time selection */
            microhal::Bitfield<uint32_t, 6, 3> SMP12;  /*!< Channel 12 sampling time selection */
            microhal::Bitfield<uint32_t, 9, 3> SMP13;  /*!< Channel 13 sampling time selection */
            microhal::Bitfield<uint32_t, 12, 3> SMP14; /*!< Channel 14 sampling time selection */
            microhal::Bitfield<uint32_t, 15, 3> SMP15; /*!< Channel 15 sampling time selection */
            microhal::Bitfield<uint32_t, 18, 3> SMP16; /*!< Channel 16 sampling time selection */
            microhal::Bitfield<uint32_t, 21, 3> SMP17; /*!< Channel 17 sampling time selection */
#ifdef _MICROHAL_REGISTERS_ADC_SMPR1_HAS_SMP18
            microhal::Bitfield<uint32_t, 24, 3> SMP18; /*!< Channel 18 sampling time selection */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SMPR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SMPR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SMPR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SMPR1 operator&(uint32_t value) const {
            SMPR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SMPR1 operator|(uint32_t value) const {
            SMPR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SMPR1, AccessType::ReadOnly>;
        friend class VolatileRegister<SMPR1, AccessType::WriteOnly>;
        friend class VolatileRegister<SMPR1, AccessType::ReadWrite>;
    };

    // sample time register 2
    union SMPR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 3> SMP0;  /*!< Channel 0 sampling time selection */
            microhal::Bitfield<uint32_t, 3, 3> SMP1;  /*!< Channel 1 sampling time selection */
            microhal::Bitfield<uint32_t, 6, 3> SMP2;  /*!< Channel 2 sampling time selection */
            microhal::Bitfield<uint32_t, 9, 3> SMP3;  /*!< Channel 3 sampling time selection */
            microhal::Bitfield<uint32_t, 12, 3> SMP4; /*!< Channel 4 sampling time selection */
            microhal::Bitfield<uint32_t, 15, 3> SMP5; /*!< Channel 5 sampling time selection */
            microhal::Bitfield<uint32_t, 18, 3> SMP6; /*!< Channel 6 sampling time selection */
            microhal::Bitfield<uint32_t, 21, 3> SMP7; /*!< Channel 7 sampling time selection */
            microhal::Bitfield<uint32_t, 24, 3> SMP8; /*!< Channel 8 sampling time selection */
            microhal::Bitfield<uint32_t, 27, 3> SMP9; /*!< Channel 9 sampling time selection */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SMPR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SMPR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SMPR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SMPR2 operator&(uint32_t value) const {
            SMPR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SMPR2 operator|(uint32_t value) const {
            SMPR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SMPR2, AccessType::ReadOnly>;
        friend class VolatileRegister<SMPR2, AccessType::WriteOnly>;
        friend class VolatileRegister<SMPR2, AccessType::ReadWrite>;
    };

    // injected channel data offset register x
    union JOFR {
        union {
            microhal::Bitfield<uint32_t, 0, 12> JOFFSETx; /*!< Data offset for injected channel x */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        JOFR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        JOFR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        JOFR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        JOFR operator&(uint32_t value) const {
            JOFR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        JOFR operator|(uint32_t value) const {
            JOFR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<JOFR, AccessType::ReadOnly>;
        friend class VolatileRegister<JOFR, AccessType::WriteOnly>;
        friend class VolatileRegister<JOFR, AccessType::ReadWrite>;
    };

    // watchdog higher threshold register
    union HTR {
        union {
            microhal::Bitfield<uint32_t, 0, 12> HT; /*!< Analog watchdog higher threshold */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        HTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        HTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        HTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        HTR operator&(uint32_t value) const {
            HTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        HTR operator|(uint32_t value) const {
            HTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<HTR, AccessType::ReadOnly>;
        friend class VolatileRegister<HTR, AccessType::WriteOnly>;
        friend class VolatileRegister<HTR, AccessType::ReadWrite>;
    };

    // watchdog lower threshold register
    union LTR {
        union {
            microhal::Bitfield<uint32_t, 0, 12> LT; /*!< Analog watchdog lower threshold */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        LTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        LTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        LTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        LTR operator&(uint32_t value) const {
            LTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        LTR operator|(uint32_t value) const {
            LTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<LTR, AccessType::ReadOnly>;
        friend class VolatileRegister<LTR, AccessType::WriteOnly>;
        friend class VolatileRegister<LTR, AccessType::ReadWrite>;
    };

    // regular sequence register 1
    union SQR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 5> SQ13;  /*!< 13th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 5, 5> SQ14;  /*!< 14th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 10, 5> SQ15; /*!< 15th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 15, 5> SQ16; /*!< 16th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 20, 4> L;    /*!< Regular channel sequence length */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SQR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SQR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SQR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SQR1 operator&(uint32_t value) const {
            SQR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SQR1 operator|(uint32_t value) const {
            SQR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SQR1, AccessType::ReadOnly>;
        friend class VolatileRegister<SQR1, AccessType::WriteOnly>;
        friend class VolatileRegister<SQR1, AccessType::ReadWrite>;
    };

    // regular sequence register 2
    union SQR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 5> SQ7;   /*!< 7th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 5, 5> SQ8;   /*!< 8th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 10, 5> SQ9;  /*!< 9th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 15, 5> SQ10; /*!< 10th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 20, 5> SQ11; /*!< 11th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 25, 5> SQ12; /*!< 12th conversion in regular sequence */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SQR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SQR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SQR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SQR2 operator&(uint32_t value) const {
            SQR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SQR2 operator|(uint32_t value) const {
            SQR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SQR2, AccessType::ReadOnly>;
        friend class VolatileRegister<SQR2, AccessType::WriteOnly>;
        friend class VolatileRegister<SQR2, AccessType::ReadWrite>;
    };

    // regular sequence register 3
    union SQR3 {
        union {
            microhal::Bitfield<uint32_t, 0, 5> SQ1;  /*!< 1st conversion in regular sequence */
            microhal::Bitfield<uint32_t, 5, 5> SQ2;  /*!< 2nd conversion in regular sequence */
            microhal::Bitfield<uint32_t, 10, 5> SQ3; /*!< 3rd conversion in regular sequence */
            microhal::Bitfield<uint32_t, 15, 5> SQ4; /*!< 4th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 20, 5> SQ5; /*!< 5th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 25, 5> SQ6; /*!< 6th conversion in regular sequence */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SQR3 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SQR3 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SQR3 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SQR3 operator&(uint32_t value) const {
            SQR3 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SQR3 operator|(uint32_t value) const {
            SQR3 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SQR3, AccessType::ReadOnly>;
        friend class VolatileRegister<SQR3, AccessType::WriteOnly>;
        friend class VolatileRegister<SQR3, AccessType::ReadWrite>;
    };

    // injected sequence register
    union JSQR {
        union {
            microhal::Bitfield<uint32_t, 0, 5> JSQ1;  /*!< 1st conversion in injected sequence */
            microhal::Bitfield<uint32_t, 5, 5> JSQ2;  /*!< 2nd conversion in injected sequence */
            microhal::Bitfield<uint32_t, 10, 5> JSQ3; /*!< 3rd conversion in injected sequence */
            microhal::Bitfield<uint32_t, 15, 5> JSQ4; /*!< 4th conversion in injected sequence */
            microhal::Bitfield<uint32_t, 20, 2> JL;   /*!< Injected sequence length */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        JSQR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        JSQR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        JSQR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        JSQR operator&(uint32_t value) const {
            JSQR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        JSQR operator|(uint32_t value) const {
            JSQR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<JSQR, AccessType::ReadOnly>;
        friend class VolatileRegister<JSQR, AccessType::WriteOnly>;
        friend class VolatileRegister<JSQR, AccessType::ReadWrite>;
    };

    // injected data register x
    union JDR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> JDATA; /*!< Injected data */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        JDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        JDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        JDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        JDR operator&(uint32_t value) const {
            JDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        JDR operator|(uint32_t value) const {
            JDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<JDR, AccessType::ReadOnly>;
        friend class VolatileRegister<JDR, AccessType::WriteOnly>;
        friend class VolatileRegister<JDR, AccessType::ReadWrite>;
    };

    // regular data register
    union DR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> DATA;      /*!< Regular data */
            microhal::Bitfield<uint32_t, 16, 16> ADC2DATA; /*!< ADC2 data */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DR operator&(uint32_t value) const {
            DR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DR operator|(uint32_t value) const {
            DR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DR, AccessType::ReadOnly>;
        friend class VolatileRegister<DR, AccessType::WriteOnly>;
        friend class VolatileRegister<DR, AccessType::ReadWrite>;
    };

    VolatileRegister<SR, AccessType::ReadWrite> sr;        /*!< status register	Address offset: 0x0 */
    VolatileRegister<CR1, AccessType::ReadWrite> cr1;      /*!< control register 1	Address offset: 0x4 */
    VolatileRegister<CR2, AccessType::ReadWrite> cr2;      /*!< control register 2	Address offset: 0x8 */
    VolatileRegister<SMPR1, AccessType::ReadWrite> smpr1;  /*!< sample time register 1	Address offset: 0xC */
    VolatileRegister<SMPR2, AccessType::ReadWrite> smpr2;  /*!< sample time register 2	Address offset: 0x10 */
    VolatileRegister<JOFR, AccessType::ReadWrite> jofr[4]; /*!< injected channel data offset register x	Address offset: 0x14 */
    VolatileRegister<HTR, AccessType::ReadWrite> htr;      /*!< watchdog higher threshold register	Address offset: 0x24 */
    VolatileRegister<LTR, AccessType::ReadWrite> ltr;      /*!< watchdog lower threshold register	Address offset: 0x28 */
    VolatileRegister<SQR1, AccessType::ReadWrite> sqr1;    /*!< regular sequence register 1	Address offset: 0x2C */
    VolatileRegister<SQR2, AccessType::ReadWrite> sqr2;    /*!< regular sequence register 2	Address offset: 0x30 */
    VolatileRegister<SQR3, AccessType::ReadWrite> sqr3;    /*!< regular sequence register 3	Address offset: 0x34 */
    VolatileRegister<JSQR, AccessType::ReadWrite> jsqr;    /*!< injected sequence register	Address offset: 0x38 */
    VolatileRegister<JDR, AccessType::ReadWrite> jdr[4];   /*!< injected data register x	Address offset: 0x3C */
    VolatileRegister<DR, AccessType::ReadWrite> dr;        /*!< regular data register	Address offset: 0x4C */
};

#if defined(_MICROHAL_ADC1_BASE_ADDRESS)
[[maybe_unused]] static ADC *const adc1 = reinterpret_cast<ADC *>(_MICROHAL_ADC1_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_ADC2_BASE_ADDRESS)
[[maybe_unused]] static ADC *const adc2 = reinterpret_cast<ADC *>(_MICROHAL_ADC2_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_ADC3_BASE_ADDRESS)
[[maybe_unused]] static ADC *const adc3 = reinterpret_cast<ADC *>(_MICROHAL_ADC3_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_ADC
