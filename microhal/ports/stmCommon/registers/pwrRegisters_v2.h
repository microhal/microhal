#ifndef _MICROHAL_PERIPHERAL_REGISTER_PWR
#define _MICROHAL_PERIPHERAL_REGISTER_PWR

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32G071, STM32G070, STM32G081, STM32G071, STM32G070, STM32G081
namespace microhal {
namespace registers {
/**
 *
 */
struct PWR {
    // Power control register 1
    union CR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 3> LPMS;      /*!< Low-power mode selection */
            microhal::Bitfield<uint32_t, 3, 1> FPD_STOP;  /*!< Flash memory powered down during Stop mode */
            microhal::Bitfield<uint32_t, 4, 1> FPD_LPRUN; /*!< Flash memory powered down during Low-power run mode */
            microhal::Bitfield<uint32_t, 5, 1> FPD_LPSLP; /*!< Flash memory powered down during Low-power sleep mode */
            microhal::Bitfield<uint32_t, 8, 1> DBP;       /*!< Disable backup domain write protection */
            microhal::Bitfield<uint32_t, 9, 2> VOS;       /*!< Voltage scaling range selection */
            microhal::Bitfield<uint32_t, 14, 1> LPR;      /*!< Low-power run */
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

    // Power control register 2
    union CR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PVDE;  /*!< Power voltage detector enable */
            microhal::Bitfield<uint32_t, 1, 3> PVDFT; /*!< Power voltage detector falling threshold selection */
            microhal::Bitfield<uint32_t, 4, 3> PVDRT; /*!< Power voltage detector rising threshold selection */
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

    // Power control register 3
    union CR3 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> EWUP1;  /*!< Enable Wakeup pin WKUP1 */
            microhal::Bitfield<uint32_t, 1, 1> EWUP2;  /*!< Enable Wakeup pin WKUP2 */
            microhal::Bitfield<uint32_t, 3, 1> EWUP4;  /*!< Enable Wakeup pin WKUP4 */
            microhal::Bitfield<uint32_t, 4, 1> EWUP5;  /*!< Enable WKUP5 wakeup pin */
            microhal::Bitfield<uint32_t, 5, 1> EWUP6;  /*!< Enable WKUP6 wakeup pin */
            microhal::Bitfield<uint32_t, 8, 1> RRS;    /*!< SRAM retention in Standby mode */
            microhal::Bitfield<uint32_t, 9, 1> ULPEN;  /*!< Enable the periodical sampling mode for PDR detection */
            microhal::Bitfield<uint32_t, 10, 1> APC;   /*!< Apply pull-up and pull-down configuration */
            microhal::Bitfield<uint32_t, 15, 1> EIWUL; /*!< Enable internal wakeup line */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CR3 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CR3 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CR3 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CR3 operator&(uint32_t value) const {
            CR3 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CR3 operator|(uint32_t value) const {
            CR3 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CR3, AccessType::ReadOnly>;
        friend class VolatileRegister<CR3, AccessType::WriteOnly>;
        friend class VolatileRegister<CR3, AccessType::ReadWrite>;
    };

    // Power control register 4
    union CR4 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> WP1;  /*!< Wakeup pin WKUP1 polarity */
            microhal::Bitfield<uint32_t, 1, 1> WP2;  /*!< Wakeup pin WKUP2 polarity */
            microhal::Bitfield<uint32_t, 3, 1> WP4;  /*!< Wakeup pin WKUP4 polarity */
            microhal::Bitfield<uint32_t, 4, 1> WP5;  /*!< Wakeup pin WKUP5 polarity */
            microhal::Bitfield<uint32_t, 5, 1> WP6;  /*!< WKUP6 wakeup pin polarity */
            microhal::Bitfield<uint32_t, 8, 1> VBE;  /*!< VBAT battery charging enable */
            microhal::Bitfield<uint32_t, 9, 1> VBRS; /*!< VBAT battery charging resistor selection */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CR4 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CR4 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CR4 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CR4 operator&(uint32_t value) const {
            CR4 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CR4 operator|(uint32_t value) const {
            CR4 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CR4, AccessType::ReadOnly>;
        friend class VolatileRegister<CR4, AccessType::WriteOnly>;
        friend class VolatileRegister<CR4, AccessType::ReadWrite>;
    };

    // Power status register 1
    union SR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> WUF1;  /*!< Wakeup flag 1 */
            microhal::Bitfield<uint32_t, 1, 1> WUF2;  /*!< Wakeup flag 2 */
            microhal::Bitfield<uint32_t, 3, 1> WUF4;  /*!< Wakeup flag 4 */
            microhal::Bitfield<uint32_t, 4, 1> WUF5;  /*!< Wakeup flag 5 */
            microhal::Bitfield<uint32_t, 5, 1> WUF6;  /*!< Wakeup flag 6 */
            microhal::Bitfield<uint32_t, 8, 1> SBF;   /*!< Standby flag */
            microhal::Bitfield<uint32_t, 15, 1> WUFI; /*!< Wakeup flag internal */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SR1 operator&(uint32_t value) const {
            SR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SR1 operator|(uint32_t value) const {
            SR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SR1, AccessType::ReadOnly>;
        friend class VolatileRegister<SR1, AccessType::WriteOnly>;
        friend class VolatileRegister<SR1, AccessType::ReadWrite>;
    };

    // Power status register 2
    union SR2 {
        union {
            microhal::Bitfield<uint32_t, 7, 1> FLASH_RDY; /*!< Flash ready flag */
            microhal::Bitfield<uint32_t, 8, 1> REGLPS;    /*!< Low-power regulator started */
            microhal::Bitfield<uint32_t, 9, 1> REGLPF;    /*!< Low-power regulator flag */
            microhal::Bitfield<uint32_t, 10, 1> VOSF;     /*!< Voltage scaling flag */
            microhal::Bitfield<uint32_t, 11, 1> PVDO;     /*!< Power voltage detector output */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SR2 operator&(uint32_t value) const {
            SR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SR2 operator|(uint32_t value) const {
            SR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SR2, AccessType::ReadOnly>;
        friend class VolatileRegister<SR2, AccessType::WriteOnly>;
        friend class VolatileRegister<SR2, AccessType::ReadWrite>;
    };

    // Power status clear register
    union SCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> CWUF1; /*!< Clear wakeup flag 1 */
            microhal::Bitfield<uint32_t, 1, 1> CWUF2; /*!< Clear wakeup flag 2 */
            microhal::Bitfield<uint32_t, 3, 1> CWUF4; /*!< Clear wakeup flag 4 */
            microhal::Bitfield<uint32_t, 4, 1> CWUF5; /*!< Clear wakeup flag 5 */
            microhal::Bitfield<uint32_t, 5, 1> CWUF6; /*!< Clear wakeup flag 6 */
            microhal::Bitfield<uint32_t, 8, 1> CSBF;  /*!< Clear standby flag */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SCR operator&(uint32_t value) const {
            SCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SCR operator|(uint32_t value) const {
            SCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SCR, AccessType::ReadOnly>;
        friend class VolatileRegister<SCR, AccessType::WriteOnly>;
        friend class VolatileRegister<SCR, AccessType::ReadWrite>;
    };

    // Power Port x pull-up control register
    union PUCRx {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PU0;   /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 1, 1> PU1;   /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 2, 1> PU2;   /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 3, 1> PU3;   /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 4, 1> PU4;   /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 5, 1> PU5;   /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 6, 1> PU6;   /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 7, 1> PU7;   /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 8, 1> PU8;   /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 9, 1> PU9;   /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 10, 1> PU10; /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 11, 1> PU11; /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 12, 1> PU12; /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 13, 1> PU13; /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 14, 1> PU14; /*!< Port x pull-up bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 15, 1> PU15; /*!< Port x pull-up bit y (y=0..15) */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PUCRx &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PUCRx &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PUCRx &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PUCRx operator&(uint32_t value) const {
            PUCRx tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PUCRx operator|(uint32_t value) const {
            PUCRx tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PUCRx, AccessType::ReadOnly>;
        friend class VolatileRegister<PUCRx, AccessType::WriteOnly>;
        friend class VolatileRegister<PUCRx, AccessType::ReadWrite>;
    };

    // Power Port x pull-down control register
    union PDCRx {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PD0;   /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 1, 1> PD1;   /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 2, 1> PD2;   /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 3, 1> PD3;   /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 4, 1> PD4;   /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 5, 1> PD5;   /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 6, 1> PD6;   /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 7, 1> PD7;   /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 8, 1> PD8;   /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 9, 1> PD9;   /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 10, 1> PD10; /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 11, 1> PD11; /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 12, 1> PD12; /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 13, 1> PD13; /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 14, 1> PD14; /*!< Port x pull-down bit y (y=0..15) */
            microhal::Bitfield<uint32_t, 15, 1> PD15; /*!< Port x pull-down bit y (y=0..15) */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PDCRx &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PDCRx &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PDCRx &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PDCRx operator&(uint32_t value) const {
            PDCRx tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PDCRx operator|(uint32_t value) const {
            PDCRx tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PDCRx, AccessType::ReadOnly>;
        friend class VolatileRegister<PDCRx, AccessType::WriteOnly>;
        friend class VolatileRegister<PDCRx, AccessType::ReadWrite>;
    };

    VolatileRegister<CR1, AccessType::ReadWrite> cr1;     /*!< Power control register 1	Address offset: 0x0 */
    VolatileRegister<CR2, AccessType::ReadWrite> cr2;     /*!< Power control register 2	Address offset: 0x4 */
    VolatileRegister<CR3, AccessType::ReadWrite> cr3;     /*!< Power control register 3	Address offset: 0x8 */
    VolatileRegister<CR4, AccessType::ReadWrite> cr4;     /*!< Power control register 4	Address offset: 0xC */
    VolatileRegister<SR1, AccessType::ReadWrite> sr1;     /*!< Power status register 1	Address offset: 0x10 */
    VolatileRegister<SR2, AccessType::ReadWrite> sr2;     /*!< Power status register 2	Address offset: 0x14 */
    VolatileRegister<SCR, AccessType::ReadWrite> scr;     /*!< Power status clear register	Address offset: 0x18 */
    uint32_t reserved0[1];                                /*!< Reserved register	Address offset: 0x1c */
    VolatileRegister<PUCRx, AccessType::ReadWrite> pucra; /*!< Power Port A pull-up control register	Address offset: 0x20 */
    VolatileRegister<PDCRx, AccessType::ReadWrite> pdcra; /*!< Power Port A pull-down control register	Address offset: 0x24 */
    VolatileRegister<PUCRx, AccessType::ReadWrite> pucrb; /*!< Power Port B pull-up control register	Address offset: 0x28 */
    VolatileRegister<PDCRx, AccessType::ReadWrite> pdcrb; /*!< Power Port B pull-down control register	Address offset: 0x2C */
    VolatileRegister<PUCRx, AccessType::ReadWrite> pucrc; /*!< Power Port C pull-up control register	Address offset: 0x30 */
    VolatileRegister<PDCRx, AccessType::ReadWrite> pdcrc; /*!< Power Port C pull-down control register	Address offset: 0x34 */
    VolatileRegister<PUCRx, AccessType::ReadWrite> pucrd; /*!< Power Port D pull-up control register	Address offset: 0x38 */
    VolatileRegister<PDCRx, AccessType::ReadWrite> pdcrd; /*!< Power Port D pull-down control register	Address offset: 0x3C */
    uint32_t reserved1[2];                                /*!< Reserved register	Address offset: 0x40 */
    VolatileRegister<PUCRx, AccessType::ReadWrite> pucrf; /*!< Power Port F pull-up control register	Address offset: 0x48 */
    VolatileRegister<PDCRx, AccessType::ReadWrite> pdcrf; /*!< Power Port F pull-down control register	Address offset: 0x4C */
};

#if defined(_MICROHAL_PWR_BASE_ADDRESS)
[[maybe_unused]] static PWR *const pwr = reinterpret_cast<PWR *>(_MICROHAL_PWR_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_PWR
