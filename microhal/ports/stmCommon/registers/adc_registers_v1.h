#ifndef _MICROHAL_PERIPHERAL_REGISTER_ADC
#define _MICROHAL_PERIPHERAL_REGISTER_ADC

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU version 1: STM32G474xx, STM32G474xx, STM32G484xx, STM32G484xx, STM32G471xx, STM32G471xx, STM32G431xx, STM32G473xx, STM32G473xx,
//                          STM32G441xx, STM32GBK1CBT6, STM32G483xx, STM32G483xx
// Supported MCU version 2: STM32L4x6
// Supported MCU version 3: STM32L4x3, STM32L4x1, STM32L4x2, STM32L4x5
// Supported MCU version 4: STM32L4R7, STM32L4S9, STM32L4R9, STM32L4S5, STM32L4S7, STM32L4R5
// Supported MCU version 5: STM32F302, STM32F303, STM32F3x4, STM32F3x8
// Supported MCU version 6: STM32F301
// Supported MCU version 7: STM32G041, STM32G030, STM32G031,

#if defined(STM32G474xx) || defined(STM32G474xx) || defined(STM32G484xx) || defined(STM32G484xx) || defined(STM32G471xx) || defined(STM32G471xx) || \
    defined(STM32G431xx) || defined(STM32G473xx) || defined(STM32G473xx) || defined(STM32G441xx) || defined(STM32GBK1CBT6) ||                       \
    defined(STM32G483xx) || defined(STM32G483xx)  // version 1
#define _MICROHAL_REGISTERS_ADC_CFGR_HAS_SPLITTED_ALIGN
#define _MICROHAL_REGISTERS_ADC_CFGR2_HAS_GCOMP_SWTRIG_BULF_SMPTRIG
#define _MICROHAL_REGISTERS_ADC_TR1_HAS_AWDFILT
#define _MICROHAL_REGISTERS_JSQR_BITFIELD_V1
#define _MICROHAL_REGISTERS_OFR_HAS_OFFSETPOS_SATEN
#define _MICROHAL_REGISTERS_ADC_HAS_GCOMP
#define _MICROHAL_REGISTERS_ADC_CFGR_HAS_JQDIS
#define _MICROHAL_REGISTERS_ADC_SMPR1_HAS_SMP0
#define _MICROHAL_REGISTERS_ADC_SMPR1_HAS_SMPPLUS
#define _MICROHAL_REGISTERS_ADC_AWDxCR_AWD2CH_0_19
#define _MICROHAL_REGISTERS_ADC_HAS_CFGR2
#define _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
#define _MICROHAL_REGISTERS_ADC_HAS_DIFFERENTIAL_MODE
#endif

#if defined(STM32L4x6)  // version 2
#define _MICROHAL_REGISTERS_ADC_CFGR_HAS_DFSDMCFG
#define _MICROHAL_REGISTERS_ADC_CFGR_HAS_JQDIS
#define _MICROHAL_REGISTERS_ADC_SMPR1_HAS_SMP0
#define _MICROHAL_REGISTERS_ADC_SMPR1_HAS_SMPPLUS
#define _MICROHAL_REGISTERS_ADC_AWDxCR_AWD2CH_0_19
#define _MICROHAL_REGISTERS_ADC_HAS_CFGR2
#define _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
#define _MICROHAL_REGISTERS_ADC_HAS_DIFFERENTIAL_MODE
#endif

#if defined(STM32L4x3) || defined(STM32L4x1) || defined(STM32L4x2) || defined(STM32L4x5)  // version 3
#define _MICROHAL_REGISTERS_ADC_CFGR_HAS_AUTOFF
#define _MICROHAL_REGISTERS_ADC_AWDxCR_AWD2CH_1_18
#define _MICROHAL_REGISTERS_ADC_HAS_CFGR2
#define _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
#define _MICROHAL_REGISTERS_ADC_HAS_DIFFERENTIAL_MODE
#endif

#if defined(STM32L4R7) || defined(STM32L4S9) || defined(STM32L4R9) || defined(STM32L4S5) || defined(STM32L4S7) || defined(STM32L4R5)  // version 4
#define _MICROHAL_REGISTERS_ADC_CFGR_HAS_JQDIS
#define _MICROHAL_REGISTERS_ADC_SMPR1_HAS_SMP0
#define _MICROHAL_REGISTERS_ADC_SMPR1_HAS_SMPPLUS
#define _MICROHAL_REGISTERS_ADC_HAS_CFGR2
#define _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
#define _MICROHAL_REGISTERS_ADC_HAS_DIFFERENTIAL_MODE
#endif

#if defined(STM32F302) || defined(STM32F303) || defined(STM32F3x4) || defined(STM32F3x8)  // version 5
#define _MICROHAL_REGISTERS_ADC_CFGR_HAS_AUTOFF
#define _MICROHAL_REGISTERS_ADC_AWDxCR_AWD2CH_1_18
#define _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
#define _MICROHAL_REGISTERS_ADC_HAS_DIFFERENTIAL_MODE
#endif

#if defined(STM32F301)  // version 6
#define _MICROHAL_REGISTERS_ADC_AWDxCR_AWD2CH_1_18
#define _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
#define _MICROHAL_REGISTERS_ADC_HAS_DIFFERENTIAL_MODE
#endif

#if defined(_MICROHAL_STM32G0XX_STM32G071xx)  // version 7
#define _MICROHAL_REGISTERS_ADC_CFGR_HAS_AUTOFF
#define _MICROHAL_REGISTERS_ADC_AWDxCR_AWD2CH_0_19
#define _MICROHAL_REGISTERS_ADC_HAS_CFGR2
#define _MICROHAL_REGISTERS_ADC_CFGR2_HAS_CKMODE
#define _MICROHAL_REGISTERS_ADC_CFGR1_HAS_CHSELRMOD
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct ADC {
    // ADC interrupt and status register
    union ISR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ADRDY; /*!< ADC ready */
            microhal::Bitfield<uint32_t, 1, 1> EOSMP; /*!< End of sampling flag */
            microhal::Bitfield<uint32_t, 2, 1> EOC;   /*!< End of conversion flag */
            microhal::Bitfield<uint32_t, 3, 1> EOS;   /*!< End of regular sequence flag */
            microhal::Bitfield<uint32_t, 4, 1> OVR;   /*!< ADC overrun */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
            microhal::Bitfield<uint32_t, 5, 1> JEOC; /*!< Injected channel end of conversion flag */
            microhal::Bitfield<uint32_t, 6, 1> JEOS; /*!< Injected channel end of sequence flag */
#endif
            microhal::Bitfield<uint32_t, 7, 1> AWD1; /*!< Analog watchdog 1 flag */
            microhal::Bitfield<uint32_t, 8, 1> AWD2; /*!< Analog watchdog 2 flag */
            microhal::Bitfield<uint32_t, 9, 1> AWD3; /*!< Analog watchdog 3 flag */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
            microhal::Bitfield<uint32_t, 10, 1> JQOVF; /*!< Injected context queue overflow */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ISR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ISR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ISR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ISR operator&(uint32_t value) const {
            ISR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ISR operator|(uint32_t value) const {
            ISR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ISR, AccessType::ReadOnly>;
        friend class VolatileRegister<ISR, AccessType::WriteOnly>;
        friend class VolatileRegister<ISR, AccessType::ReadWrite>;
    };

    // ADC interrupt enable register
    union IER {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ADRDYIE; /*!< ADC ready interrupt enable */
            microhal::Bitfield<uint32_t, 1, 1> EOSMPIE; /*!< End of sampling flag interrupt enable for regular conversions */
            microhal::Bitfield<uint32_t, 2, 1> EOCIE;   /*!< End of regular conversion interrupt enable */
            microhal::Bitfield<uint32_t, 3, 1> EOSIE;   /*!< End of regular sequence of conversions interrupt enable */
            microhal::Bitfield<uint32_t, 4, 1> OVRIE;   /*!< Overrun interrupt enable */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
            microhal::Bitfield<uint32_t, 5, 1> JEOCIE; /*!< End of injected conversion interrupt enable */
            microhal::Bitfield<uint32_t, 6, 1> JEOSIE; /*!< End of injected sequence of conversions interrupt enable */
#endif
            microhal::Bitfield<uint32_t, 7, 1> AWD1IE; /*!< Analog watchdog 1 interrupt enable */
            microhal::Bitfield<uint32_t, 8, 1> AWD2IE; /*!< Analog watchdog 2 interrupt enable */
            microhal::Bitfield<uint32_t, 9, 1> AWD3IE; /*!< Analog watchdog 3 interrupt enable */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
            microhal::Bitfield<uint32_t, 10, 1> JQOVFIE; /*!< Injected context queue overflow interrupt enable */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IER operator&(uint32_t value) const {
            IER tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IER operator|(uint32_t value) const {
            IER tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IER, AccessType::ReadOnly>;
        friend class VolatileRegister<IER, AccessType::WriteOnly>;
        friend class VolatileRegister<IER, AccessType::ReadWrite>;
    };

    // ADC control register
    union CR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ADEN;    /*!< ADC enable control */
            microhal::Bitfield<uint32_t, 1, 1> ADDIS;   /*!< ADC disable command */
            microhal::Bitfield<uint32_t, 2, 1> ADSTART; /*!< ADC start of regular conversion */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
            microhal::Bitfield<uint32_t, 3, 1> JADSTART; /*!< ADC start of injected conversion */
#endif
            microhal::Bitfield<uint32_t, 4, 1> ADSTP; /*!< ADC stop of regular conversion command */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
            microhal::Bitfield<uint32_t, 5, 1> JADSTP; /*!< ADC stop of regular conversion command */
#endif
            microhal::Bitfield<uint32_t, 28, 2> ADVREGEN; /*!< ADC voltage regulator enable */
                                                          // microhal::Bitfield<uint32_t, 29, 1> DEEPPWD;  /*!< DEEPPWD */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_DIFFERENTIAL_MODE
            microhal::Bitfield<uint32_t, 30, 1> ADCALDIF; /*!< Differential mode for calibration */
#endif
            microhal::Bitfield<uint32_t, 31, 1> ADCAL; /*!< ADC calibration */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CR operator&(uint32_t value) const {
            CR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CR operator|(uint32_t value) const {
            CR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CR, AccessType::ReadOnly>;
        friend class VolatileRegister<CR, AccessType::WriteOnly>;
        friend class VolatileRegister<CR, AccessType::ReadWrite>;
    };

    // ADC configuration register 1
    union CFGR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> DMAEN;  /*!< Direct memory access enable */
            microhal::Bitfield<uint32_t, 1, 1> DMACFG; /*!< Direct memory access configuration */
#ifdef _MICROHAL_REGISTERS_ADC_CFGR_HAS_DFSDMCFG
            microhal::Bitfield<uint32_t, 2, 1> DFSDMCFG; /*!< DFSDMCFG */
#endif
            microhal::Bitfield<uint32_t, 3, 2> RES;     /*!< Data resolution */
            microhal::Bitfield<uint32_t, 5, 1> ALIGN_5; /*!< Data alignment */
            microhal::Bitfield<uint32_t, 6, 4> EXTSEL;  /*!< External trigger selection for regular group */
            microhal::Bitfield<uint32_t, 10, 2> EXTEN;  /*!< External trigger enable and polarity selection for regular channels */
            microhal::Bitfield<uint32_t, 12, 1> OVRMOD; /*!< Overrun Mode */
            microhal::Bitfield<uint32_t, 13, 1> CONT;   /*!< Single / continuous conversion mode for regular conversions */
            microhal::Bitfield<uint32_t, 14, 1> AUTDLY; /*!< Delayed conversion mode */
#ifdef _MICROHAL_REGISTERS_ADC_CFGR_HAS_SPLITTED_ALIGN
            microhal::Bitfield<uint32_t, 15, 1> ALIGN; /*!< ALIGN */
#endif
#ifdef _MICROHAL_REGISTERS_ADC_CFGR_HAS_AUTOFF
            microhal::Bitfield<uint32_t, 15, 1> AUTOFF; /*!< AUTOFF */
#endif
            microhal::Bitfield<uint32_t, 16, 1> DISCEN;  /*!< Discontinuous mode for regular channels */
            microhal::Bitfield<uint32_t, 17, 3> DISCNUM; /*!< Discontinuous mode channel count */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
            microhal::Bitfield<uint32_t, 20, 1> JDISCEN; /*!< Discontinuous mode on injected channels */
            microhal::Bitfield<uint32_t, 21, 1> JQM;     /*!< JSQR queue mode */
#else
#ifdef _MICROHAL_REGISTERS_ADC_CFGR1_HAS_CHSELRMOD
            microhal::Bitfield<uint32_t, 21, 1> CHSELRMOD; /*!< Mode selection of the ADC_CHSELR register */
#endif
#endif

            microhal::Bitfield<uint32_t, 22, 1> AWD1SGL; /*!< Enable the watchdog 1 on a single channel or on all channels */
            microhal::Bitfield<uint32_t, 23, 1> AWD1EN;  /*!< Analog watchdog 1 enable on regular channels */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
            microhal::Bitfield<uint32_t, 24, 1> JAWD1EN; /*!< Analog watchdog 1 enable on injected channels */
            microhal::Bitfield<uint32_t, 25, 1> JAUTO;   /*!< Automatic injected group conversion */
#endif
            microhal::Bitfield<uint32_t, 26, 5> AWDCH1CH; /*!< Analog watchdog 1 channel selection */
#if defined(_MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS) && defined(_MICROHAL_REGISTERS_ADC_CFGR_HAS_JQDIS)
            microhal::Bitfield<uint32_t, 31, 1> JQDIS; /*!< Injected Queue disable */
#endif
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

    // ADC configuration register 2
    union CFGR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ROVSE; /*!< DMAEN */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
            microhal::Bitfield<uint32_t, 1, 1> JOVSE; /*!< DMACFG */
#endif
            microhal::Bitfield<uint32_t, 2, 3> OVSR;   /*!< RES */
            microhal::Bitfield<uint32_t, 5, 4> OVSS;   /*!< ALIGN */
            microhal::Bitfield<uint32_t, 9, 1> TROVS;  /*!< Triggered Regular Oversampling */
            microhal::Bitfield<uint32_t, 10, 1> ROVSM; /*!< EXTEN */
#ifdef _MICROHAL_REGISTERS_ADC_CFGR2_HAS_GCOMP_SWTRIG_BULF_SMPTRIG
            microhal::Bitfield<uint32_t, 16, 1> GCOMP;   /*!< GCOMP */
            microhal::Bitfield<uint32_t, 25, 1> SWTRIG;  /*!< SWTRIG */
            microhal::Bitfield<uint32_t, 26, 1> BULB;    /*!< BULB */
            microhal::Bitfield<uint32_t, 27, 1> SMPTRIG; /*!< SMPTRIG */
#endif
#ifdef _MICROHAL_REGISTERS_ADC_CFGR2_HAS_CKMODE
            microhal::Bitfield<uint32_t, 30, 2> CKMODE; /*!< ADC clock mode */
#endif
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

    // ADC sample time register 1
    union SMPR1 {
        union {
#ifdef _MICROHAL_REGISTERS_ADC_SMPR1_HAS_SMP0
            microhal::Bitfield<uint32_t, 0, 3> SMP0; /*!< SMP0 */
#endif
            microhal::Bitfield<uint32_t, 3, 3> SMP1;  /*!< Channel 1 sampling time selection */
            microhal::Bitfield<uint32_t, 6, 3> SMP2;  /*!< Channel 2 sampling time selection */
            microhal::Bitfield<uint32_t, 9, 3> SMP3;  /*!< Channel 3 sampling time selection */
            microhal::Bitfield<uint32_t, 12, 3> SMP4; /*!< Channel 4 sampling time selection */
            microhal::Bitfield<uint32_t, 15, 3> SMP5; /*!< Channel 5 sampling time selection */
            microhal::Bitfield<uint32_t, 18, 3> SMP6; /*!< Channel 6 sampling time selection */
            microhal::Bitfield<uint32_t, 21, 3> SMP7; /*!< Channel 7 sampling time selection */
            microhal::Bitfield<uint32_t, 24, 3> SMP8; /*!< Channel 8 sampling time selection */
            microhal::Bitfield<uint32_t, 27, 3> SMP9; /*!< Channel 9 sampling time selection */
#ifdef _MICROHAL_REGISTERS_ADC_SMPR1_HAS_SMPPLUS
            microhal::Bitfield<uint32_t, 31, 1> SMPPLUS; /*!< Addition of one clock cycle to the sampling time */
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

    // ADC sample time register 2
    union SMPR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 3> SMP10;  /*!< Channel 10 sampling time selection */
            microhal::Bitfield<uint32_t, 3, 3> SMP11;  /*!< Channel 11 sampling time selection */
            microhal::Bitfield<uint32_t, 6, 3> SMP12;  /*!< Channel 12 sampling time selection */
            microhal::Bitfield<uint32_t, 9, 3> SMP13;  /*!< Channel 13 sampling time selection */
            microhal::Bitfield<uint32_t, 12, 3> SMP14; /*!< Channel 14 sampling time selection */
            microhal::Bitfield<uint32_t, 15, 3> SMP15; /*!< Channel 15 sampling time selection */
            microhal::Bitfield<uint32_t, 18, 3> SMP16; /*!< Channel 16 sampling time selection */
            microhal::Bitfield<uint32_t, 21, 3> SMP17; /*!< Channel 17 sampling time selection */
            microhal::Bitfield<uint32_t, 24, 3> SMP18; /*!< Channel 18 sampling time selection */
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

    // ADC watchdog threshold register 1
    union TR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 12> LT1; /*!< Analog watchdog 1 lower threshold */
#ifdef _MICROHAL_REGISTERS_ADC_TR1_HAS_AWDFILT
            microhal::Bitfield<uint32_t, 12, 3> AWDFILT; /*!< AWDFILT */
#endif
            microhal::Bitfield<uint32_t, 16, 12> HT1; /*!< Analog watchdog 1 higher threshold */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TR1 operator&(uint32_t value) const {
            TR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TR1 operator|(uint32_t value) const {
            TR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TR1, AccessType::ReadOnly>;
        friend class VolatileRegister<TR1, AccessType::WriteOnly>;
        friend class VolatileRegister<TR1, AccessType::ReadWrite>;
    };

    // ADC watchdog threshold register 2 and register 3
    union TR2_3 {
        union {
            microhal::Bitfield<uint32_t, 0, 8> LT;  /*!< Analog watchdog 2 or 3 lower threshold */
            microhal::Bitfield<uint32_t, 16, 8> HT; /*!< Analog watchdog 2 or 3 higher threshold */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TR2_3 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TR2_3 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TR2_3 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TR2_3 operator&(uint32_t value) const {
            TR2_3 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TR2_3 operator|(uint32_t value) const {
            TR2_3 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TR2_3, AccessType::ReadOnly>;
        friend class VolatileRegister<TR2_3, AccessType::WriteOnly>;
        friend class VolatileRegister<TR2_3, AccessType::ReadWrite>;
    };

    // channel selection register
    union CHSELR {
        union {
            microhal::Bitfield<uint32_t, 0, 19> CHSEL; /*!< Channel-x selection */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CHSELR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CHSELR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CHSELR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CHSELR operator&(uint32_t value) const {
            CHSELR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CHSELR operator|(uint32_t value) const {
            CHSELR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CHSELR, AccessType::ReadOnly>;
        friend class VolatileRegister<CHSELR, AccessType::WriteOnly>;
        friend class VolatileRegister<CHSELR, AccessType::ReadWrite>;
    };

    // channel selection register CHSELRMOD = 1 in ADC_CFGR1
    union CHSELR_1 {
        union {
            microhal::Bitfield<uint32_t, 0, 4> SQ1;  /*!< conversion of the sequence */
            microhal::Bitfield<uint32_t, 4, 4> SQ2;  /*!< conversion of the sequence */
            microhal::Bitfield<uint32_t, 8, 4> SQ3;  /*!< conversion of the sequence */
            microhal::Bitfield<uint32_t, 12, 4> SQ4; /*!< conversion of the sequence */
            microhal::Bitfield<uint32_t, 16, 4> SQ5; /*!< conversion of the sequence */
            microhal::Bitfield<uint32_t, 20, 4> SQ6; /*!< conversion of the sequence */
            microhal::Bitfield<uint32_t, 24, 4> SQ7; /*!< conversion of the sequence */
            microhal::Bitfield<uint32_t, 28, 4> SQ8; /*!< conversion of the sequence */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CHSELR_1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CHSELR_1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CHSELR_1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CHSELR_1 operator&(uint32_t value) const {
            CHSELR_1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CHSELR_1 operator|(uint32_t value) const {
            CHSELR_1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CHSELR_1, AccessType::ReadOnly>;
        friend class VolatileRegister<CHSELR_1, AccessType::WriteOnly>;
        friend class VolatileRegister<CHSELR_1, AccessType::ReadWrite>;
    };

    // ADC regular sequence register 1
    union SQR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 4> L;    /*!< Regular channel sequence length */
            microhal::Bitfield<uint32_t, 6, 5> SQ1;  /*!< 1st conversion in regular sequence */
            microhal::Bitfield<uint32_t, 12, 5> SQ2; /*!< 2nd conversion in regular sequence */
            microhal::Bitfield<uint32_t, 18, 5> SQ3; /*!< 3rd conversion in regular sequence */
            microhal::Bitfield<uint32_t, 24, 5> SQ4; /*!< 4th conversion in regular sequence */
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

    // ADC regular sequence register 2
    union SQR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 5> SQ5;  /*!< 5th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 6, 5> SQ6;  /*!< 6th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 12, 5> SQ7; /*!< 7th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 18, 5> SQ8; /*!< 8th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 24, 5> SQ9; /*!< 9th conversion in regular sequence */
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

    // ADC regular sequence register 3
    union SQR3 {
        union {
            microhal::Bitfield<uint32_t, 0, 5> SQ10;  /*!< 10th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 6, 5> SQ11;  /*!< 11th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 12, 5> SQ12; /*!< 13th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 18, 5> SQ13; /*!< 13th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 24, 5> SQ14; /*!< 14th conversion in regular sequence */
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

    // ADC regular sequence register 4
    union SQR4 {
        union {
            microhal::Bitfield<uint32_t, 0, 5> SQ15; /*!< 15th conversion in regular sequence */
            microhal::Bitfield<uint32_t, 6, 5> SQ16; /*!< 16th conversion in regular sequence */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SQR4 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SQR4 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SQR4 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SQR4 operator&(uint32_t value) const {
            SQR4 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SQR4 operator|(uint32_t value) const {
            SQR4 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SQR4, AccessType::ReadOnly>;
        friend class VolatileRegister<SQR4, AccessType::WriteOnly>;
        friend class VolatileRegister<SQR4, AccessType::ReadWrite>;
    };

    // ADC regular Data Register
    union DR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> RDATA; /*!< Regular Data converted */
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

    // ADC injected sequence register
    union JSQR {
#ifdef _MICROHAL_REGISTERS_JSQR_BITFIELD_V1
        union {
            microhal::Bitfield<uint32_t, 0, 2> JL;      /*!< Injected channel sequence length */
            microhal::Bitfield<uint32_t, 2, 5> JEXTSEL; /*!< External Trigger Selection for injected group */
            microhal::Bitfield<uint32_t, 7, 2> JEXTEN;  /*!< JEXTEN */
            microhal::Bitfield<uint32_t, 9, 5> JSQ1;    /*!< 1st conversion in the injected sequence */
            microhal::Bitfield<uint32_t, 15, 5> JSQ2;   /*!< 2nd conversion in the injected sequence */
            microhal::Bitfield<uint32_t, 21, 5> JSQ3;   /*!< 3rd conversion in the injected sequence */
            microhal::Bitfield<uint32_t, 27, 5> JSQ4;   /*!< 4th conversion in the injected sequence */
        };
#else
        union {
            microhal::Bitfield<uint32_t, 0, 2> JL;      /*!< Injected channel sequence length */
            microhal::Bitfield<uint32_t, 2, 4> JEXTSEL; /*!< External Trigger Selection for injected group */
            microhal::Bitfield<uint32_t, 6, 2> JEXTEN;  /*!< External Trigger Enable and Polarity Selection for injected channels */
            microhal::Bitfield<uint32_t, 8, 5> JSQ1;    /*!< 1st conversion in the injected sequence */
            microhal::Bitfield<uint32_t, 14, 5> JSQ2;   /*!< 2nd conversion in the injected sequence */
            microhal::Bitfield<uint32_t, 20, 5> JSQ3;   /*!< 3rd conversion in the injected sequence */
            microhal::Bitfield<uint32_t, 26, 5> JSQ4;   /*!< 4th conversion in the injected sequence */
        };
#endif

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

    // ADC offset register
    union OFRx {
        union {
            microhal::Bitfield<uint32_t, 0, 12> OFFSET; /*!< Data offset for the channel programmed into bits OFFSETx_CH */
#ifdef _MICROHAL_REGISTERS_OFR_HAS_OFFSETPOS_SATEN
            microhal::Bitfield<uint32_t, 24, 1> OFFSETPOS; /*!< OFFSETPOS */
            microhal::Bitfield<uint32_t, 25, 1> SATEN;     /*!< SATEN */
#endif
            microhal::Bitfield<uint32_t, 26, 5> OFFSET_CH; /*!< Channel selection for the Data offset*/
            microhal::Bitfield<uint32_t, 31, 1> OFFSET_EN; /*!< Offset Enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        OFRx &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OFRx &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OFRx &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        OFRx operator&(uint32_t value) const {
            OFRx tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        OFRx operator|(uint32_t value) const {
            OFRx tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<OFRx, AccessType::ReadOnly>;
        friend class VolatileRegister<OFRx, AccessType::WriteOnly>;
        friend class VolatileRegister<OFRx, AccessType::ReadWrite>;
    };

    // injected data register 1
    union JDRx {
        union {
            microhal::Bitfield<uint32_t, 0, 16> JDATA; /*!< Injected data */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        JDRx &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        JDRx &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        JDRx &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        JDRx operator&(uint32_t value) const {
            JDRx tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        JDRx operator|(uint32_t value) const {
            JDRx tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<JDRx, AccessType::ReadOnly>;
        friend class VolatileRegister<JDRx, AccessType::WriteOnly>;
        friend class VolatileRegister<JDRx, AccessType::ReadWrite>;
    };

    // ADC Analog Watchdog x Configuration Register
    union AWDxCR {
        union {
#ifdef _MICROHAL_REGISTERS_ADC_AWDxCR_AWD2CH_0_19
            microhal::Bitfield<uint32_t, 0, 19> AWDxCH; /*!< Analog watchdog x channel selection */
#elif defined(_MICROHAL_REGISTERS_ADC_AWDxCR_AWD2CH_1_18)
            microhal::Bitfield<uint32_t, 1, 18> AWDxCH; /*!< Analog watchdog x channel selection */
#else
            microhal::Bitfield<uint32_t, 0, 18> AWDxCH; /*!< Analog watchdog x channel selection */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        AWDxCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        AWDxCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        AWDxCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        AWDxCR operator&(uint32_t value) const {
            AWDxCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        AWDxCR operator|(uint32_t value) const {
            AWDxCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<AWDxCR, AccessType::ReadOnly>;
        friend class VolatileRegister<AWDxCR, AccessType::WriteOnly>;
        friend class VolatileRegister<AWDxCR, AccessType::ReadWrite>;
    };

    // ADC Differential Mode Selection Register
    union DIFSEL {
        union {
            microhal::Bitfield<uint32_t, 0, 1> DIFSEL_0;     /*!< Differential mode for channels 0 */
            microhal::Bitfield<uint32_t, 1, 18> DIFSEL_1_18; /*!< Differential mode for channels 15 to 1 */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DIFSEL &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DIFSEL &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DIFSEL &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DIFSEL operator&(uint32_t value) const {
            DIFSEL tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DIFSEL operator|(uint32_t value) const {
            DIFSEL tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DIFSEL, AccessType::ReadOnly>;
        friend class VolatileRegister<DIFSEL, AccessType::WriteOnly>;
        friend class VolatileRegister<DIFSEL, AccessType::ReadWrite>;
    };

    // ADC Calibration Factors
    union CALFACT {
        union {
            microhal::Bitfield<uint32_t, 0, 7> CALFACT_S; /*!< Calibration Factors In Single-Ended mode */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_DIFFERENTIAL_MODE
            microhal::Bitfield<uint32_t, 16, 7> CALFACT_D; /*!< Calibration Factors in differential mode */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CALFACT &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CALFACT &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CALFACT &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CALFACT operator&(uint32_t value) const {
            CALFACT tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CALFACT operator|(uint32_t value) const {
            CALFACT tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CALFACT, AccessType::ReadOnly>;
        friend class VolatileRegister<CALFACT, AccessType::WriteOnly>;
        friend class VolatileRegister<CALFACT, AccessType::ReadWrite>;
    };

    // Gain compensation Register
    union GCOMP {
        union {
            microhal::Bitfield<uint32_t, 0, 14> GCOMPCOEFF; /*!< GCOMPCOEFF */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        GCOMP &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        GCOMP &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        GCOMP &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        GCOMP operator&(uint32_t value) const {
            GCOMP tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        GCOMP operator|(uint32_t value) const {
            GCOMP tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<GCOMP, AccessType::ReadOnly>;
        friend class VolatileRegister<GCOMP, AccessType::WriteOnly>;
        friend class VolatileRegister<GCOMP, AccessType::ReadWrite>;
    };

    VolatileRegister<ISR, AccessType::ReadWrite> isr;     /*!< ADC interrupt and status register	Address offset: 0x0 */
    VolatileRegister<IER, AccessType::ReadWrite> ier;     /*!< ADC interrupt enable register	Address offset: 0x4 */
    VolatileRegister<CR, AccessType::ReadWrite> cr;       /*!< ADC control register	Address offset: 0x8 */
    VolatileRegister<CFGR1, AccessType::ReadWrite> cfgr1; /*!< ADC configuration register	Address offset: 0xC */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_CFGR2
    VolatileRegister<CFGR2, AccessType::ReadWrite> cfgr2; /*!< ADC configuration register 2	Address offset: 0x10 */
#else
    uint32_t reserved0;                                 /*!< Reserved register	Address offset: 0x10 */
#endif
    VolatileRegister<SMPR1, AccessType::ReadWrite> smpr1; /*!< ADC sample time register 1	Address offset: 0x14 */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_SMPR2
    VolatileRegister<SMPR2, AccessType::ReadWrite> smpr2; /*!< ADC sample time register 2	Address offset: 0x18 */
#else
    uint32_t reserved01;
#endif
    uint32_t reserved1;                                 /*!< Reserved register	Address offset: 0x1c */
    VolatileRegister<TR1, AccessType::ReadWrite> tr1;   /*!< ADC watchdog threshold register 1	Address offset: 0x20 */
    VolatileRegister<TR2_3, AccessType::ReadWrite> tr2; /*!< ADC watchdog threshold register 2	Address offset: 0x24 */
#ifdef _MICROHAL_REGISTERS_ADC_CFGR1_HAS_CHSELRMOD
    union {
        VolatileRegister<CHSELR, AccessType::ReadWrite> chselr_mod0; /*!< channel selection register	Address offset: 0x28 */
        VolatileRegister<CHSELR_1, AccessType::ReadWrite>
            chselr_mod1; /*!< channel selection register CHSELRMOD = 1 in ADC_CFGR1	Address offset: 0x28 */
    };
#endif
    VolatileRegister<TR2_3, AccessType::ReadWrite> tr3; /*!< ADC watchdog threshold register 3	Address offset: 0x28 or 2C */
#ifndef _MICROHAL_REGISTERS_ADC_CFGR1_HAS_CHSELRMOD
    uint32_t reserved2;                                 /*!< Reserved register	Address offset: 0x2c */
    VolatileRegister<SQR1, AccessType::ReadWrite> sqr1; /*!< ADC regular sequence register 1	Address offset: 0x30 */
    VolatileRegister<SQR2, AccessType::ReadWrite> sqr2; /*!< ADC regular sequence register 2	Address offset: 0x34 */
    VolatileRegister<SQR3, AccessType::ReadWrite> sqr3; /*!< ADC regular sequence register 3	Address offset: 0x38 */
    VolatileRegister<SQR4, AccessType::ReadWrite> sqr4; /*!< ADC regular sequence register 4	Address offset: 0x3C */
#else
    uint32_t reserved21[4];
#endif
    VolatileRegister<DR, AccessType::ReadWrite> dr; /*!< ADC regular Data Register	Address offset: 0x40 */
    uint32_t reserved3[2];                          /*!< Reserved register	Address offset: 0x44 */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
    VolatileRegister<JSQR, AccessType::ReadWrite> jsqr; /*!< ADC injected sequence register	Address offset: 0x4C */
#else
    uint32_t reserved31;
#endif
    uint32_t reserved4[4]; /*!< Reserved register	Address offset: 0x50 */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_OFR
    VolatileRegister<OFRx, AccessType::ReadWrite> ofr[4]; /*!< ADC offset register 1	Address offset: 0x60 */
    // VolatileRegister<OFRx, AccessType::ReadWrite> ofr2;       /*!< ADC offset register 2	Address offset: 0x64 */
    // VolatileRegister<OFRx, AccessType::ReadWrite> ofr3;       /*!< ADC offset register 3	Address offset: 0x68 */
    // VolatileRegister<OFRx, AccessType::ReadWrite> ofr4;       /*!< ADC offset register 4	Address offset: 0x6C */
    uint32_t reserved5[4]; /*!< Reserved register	Address offset: 0x70 */
#else
    uint32_t reserved5[8];
#endif
#ifdef _MICROHAL_REGISTERS_ADC_HAS_INJECTED_CHANNELS
    VolatileRegister<JDRx, AccessType::ReadWrite> jdr[4]; /*!< injected data register 1	Address offset: 0x80 */
    // VolatileRegister<JDRx, AccessType::ReadWrite> jdr2;       /*!< injected data register 2	Address offset: 0x84 */
    // VolatileRegister<JDRx, AccessType::ReadWrite> jdr3;       /*!< injected data register 3	Address offset: 0x88 */
    // VolatileRegister<JDRx, AccessType::ReadWrite> jdr4;       /*!< injected data register 4	Address offset: 0x8C */
#else
    uint32_t reserved51[4];
#endif
    uint32_t reserved6[4];                                  /*!< Reserved register	Address offset: 0x90 */
    VolatileRegister<AWDxCR, AccessType::ReadWrite> awd2cr; /*!< ADC Analog Watchdog 2 Configuration Register	Address offset: 0xA0 */
    VolatileRegister<AWDxCR, AccessType::ReadWrite> awd3cr; /*!< ADC Analog Watchdog 3 Configuration Register	Address offset: 0xA4 */
    uint32_t reserved7[2];                                  /*!< Reserved register	Address offset: 0xa8 */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_DIFFERENTIAL_MODE
    VolatileRegister<DIFSEL, AccessType::ReadWrite> difsel; /*!< ADC Differential Mode Selection Register	Address offset: 0xB0 */
#else
    uint32_t reserved8;
#endif
    VolatileRegister<CALFACT, AccessType::ReadWrite> calfact; /*!< ADC Calibration Factors	Address offset: 0xB4 */
#ifdef _MICROHAL_REGISTERS_ADC_HAS_GCOMP
    uint32_t reserved9[2];                                /*!< Reserved register	Address offset: 0xb8 */
    VolatileRegister<GCOMP, AccessType::ReadWrite> gcomp; /*!< Gain compensation Register	Address offset: 0xC0 */
#endif
};

#if defined(_MICROHAL_ADC1_BASE_ADDRESS)
[[maybe_unused]] static ADC *const adc1 = reinterpret_cast<ADC *>(_MICROHAL_ADC1_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_ADC2_BASE_ADDRESS)
[[maybe_unused]] static ADC *const adc2 = reinterpret_cast<ADC *>(_MICROHAL_ADC2_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_ADC
