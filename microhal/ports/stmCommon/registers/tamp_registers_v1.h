#ifndef _MICROHAL_PERIPHERAL_REGISTER_TAMP
#define _MICROHAL_PERIPHERAL_REGISTER_TAMP

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include "registersBaseAddressDefinitions.h"

// Supported MCU variant 1: STM32G071, STM32G070, STM32G081, STM32G071, STM32G070, STM32G081
// Supported MCU variant 2: STM32G474xx, STM32G484xx, STM32G471xx, STM32G431xx, STM32G473xx, STM32G441xx, STM32GBK1CBT6, STM32G483xx

#if defined(MCU_TYPE_STM32G0XX)
#define _MICROHAL_REGISTERS_TAMP_VARIANT 1
#endif

#if _MICROHAL_REGISTERS_TAMP_VARIANT == 1
#define _MICROHAL_REGISTERS_TAMP_BKP_REGISTES_COUNT 5
#endif

#if _MICROHAL_REGISTERS_TAMP_VARIANT == 2
#define _MICROHAL_REGISTERS_TAMP_BKP_REGISTES_COUNT 32
#define _MICROHAL_REGISTERS_TAMP_HAS_TAMP3
#endif
namespace microhal {
namespace registers {
/**
 *
 */
struct TAMP {
    // control register 1
    union CR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TAMP1E; /*!< TAMP1E */
            microhal::Bitfield<uint32_t, 1, 1> TAMP2E; /*!< TAMP2E */
#ifdef __MICROHAL_REGISTERS_TAMP_HAS_TAMP3
            microhal::Bitfield<uint32_t, 2, 1> TAMP3E; /*!< TAMP3E */
#endif
            microhal::Bitfield<uint32_t, 16, 1> ITAMP1E; /*!< ITAMP1E */
            microhal::Bitfield<uint32_t, 18, 1> ITAMP3E; /*!< ITAMP3E */
            microhal::Bitfield<uint32_t, 19, 1> ITAMP4E; /*!< ITAMP4E */
            microhal::Bitfield<uint32_t, 20, 1> ITAMP5E; /*!< ITAMP5E */
            microhal::Bitfield<uint32_t, 21, 1> ITAMP6E; /*!< ITAMP6E */
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
            microhal::Bitfield<uint32_t, 0, 1> TAMP1NOER; /*!< TAMP1NOER */
            microhal::Bitfield<uint32_t, 1, 1> TAMP2NOER; /*!< TAMP2NOER */
#ifdef __MICROHAL_REGISTERS_TAMP_HAS_TAMP3
            microhal::Bitfield<uint32_t, 2, 1> TAMP3NOER; /*!< TAMP3NOER */
#endif
            microhal::Bitfield<uint32_t, 16, 1> TAMP1MSK; /*!< TAMP1MSK */
            microhal::Bitfield<uint32_t, 17, 1> TAMP2MSK; /*!< TAMP2MSK */
#ifdef __MICROHAL_REGISTERS_TAMP_HAS_TAMP3
            microhal::Bitfield<uint32_t, 18, 1> TAMP3MSK; /*!< TAMP3MSK */
#endif
            microhal::Bitfield<uint32_t, 24, 1> TAMP1TRG; /*!< TAMP1TRG */
            microhal::Bitfield<uint32_t, 25, 1> TAMP2TRG; /*!< TAMP2TRG */
#ifdef __MICROHAL_REGISTERS_TAMP_HAS_TAMP3
            microhal::Bitfield<uint32_t, 26, 1> TAMP3TRG; /*!< TAMP3TRG */
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

    // TAMP filter control register
    union FLTCR {
        union {
            microhal::Bitfield<uint32_t, 0, 3> TAMPFREQ;  /*!< TAMPFREQ */
            microhal::Bitfield<uint32_t, 3, 2> TAMPFLT;   /*!< TAMPFLT */
            microhal::Bitfield<uint32_t, 5, 2> TAMPPRCH;  /*!< TAMPPRCH */
            microhal::Bitfield<uint32_t, 7, 1> TAMPPUDIS; /*!< TAMPPUDIS */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        FLTCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        FLTCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        FLTCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        FLTCR operator&(uint32_t value) const {
            FLTCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        FLTCR operator|(uint32_t value) const {
            FLTCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<FLTCR, AccessType::ReadOnly>;
        friend class VolatileRegister<FLTCR, AccessType::WriteOnly>;
        friend class VolatileRegister<FLTCR, AccessType::ReadWrite>;
    };

    // TAMP interrupt enable register
    union IER {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TAMP1IE; /*!< TAMP1IE */
            microhal::Bitfield<uint32_t, 1, 1> TAMP2IE; /*!< TAMP2IE */
#ifdef __MICROHAL_REGISTERS_TAMP_HAS_TAMP3
            microhal::Bitfield<uint32_t, 2, 1> TAMP3IE; /*!< TAMP3IE */
#endif
            microhal::Bitfield<uint32_t, 16, 1> ITAMP1IE; /*!< ITAMP1IE */
            microhal::Bitfield<uint32_t, 18, 1> ITAMP3IE; /*!< ITAMP3IE */
            microhal::Bitfield<uint32_t, 19, 1> ITAMP4IE; /*!< ITAMP4IE */
            microhal::Bitfield<uint32_t, 20, 1> ITAMP5IE; /*!< ITAMP5IE */
            microhal::Bitfield<uint32_t, 21, 1> ITAMP6IE; /*!< ITAMP6IE */
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

    // TAMP status register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TAMP1F;   /*!< TAMP1F */
            microhal::Bitfield<uint32_t, 1, 1> TAMP2F;   /*!< TAMP2F */
            microhal::Bitfield<uint32_t, 16, 1> ITAMP1F; /*!< ITAMP1F */
#ifdef __MICROHAL_REGISTERS_TAMP_HAS_TAMP3
            microhal::Bitfield<uint32_t, 2, 1> TAMP3F; /*!< TAMP3F */
#endif
            microhal::Bitfield<uint32_t, 18, 1> ITAMP3F; /*!< ITAMP3F */
            microhal::Bitfield<uint32_t, 19, 1> ITAMP4F; /*!< ITAMP4F */
            microhal::Bitfield<uint32_t, 20, 1> ITAMP5F; /*!< ITAMP5F */
            microhal::Bitfield<uint32_t, 21, 1> ITAMP6F; /*!< ITAMP6F */
            microhal::Bitfield<uint32_t, 22, 1> ITAMP7F; /*!< ITAMP7F */
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

    // TAMP masked interrupt status register
    union MISR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TAMP1MF; /*!< TAMP1MF: */
            microhal::Bitfield<uint32_t, 1, 1> TAMP2MF; /*!< TAMP2MF */
#ifdef __MICROHAL_REGISTERS_TAMP_HAS_TAMP3
            microhal::Bitfield<uint32_t, 2, 1> TAMP3MF; /*!< TAMP3MF */
#endif
            microhal::Bitfield<uint32_t, 16, 1> ITAMP1MF; /*!< ITAMP1MF */
            microhal::Bitfield<uint32_t, 18, 1> ITAMP3MF; /*!< ITAMP3MF */
            microhal::Bitfield<uint32_t, 19, 1> ITAMP4MF; /*!< ITAMP4MF */
            microhal::Bitfield<uint32_t, 20, 1> ITAMP5MF; /*!< ITAMP5MF */
            microhal::Bitfield<uint32_t, 21, 1> ITAMP6MF; /*!< ITAMP6MF */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MISR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MISR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MISR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MISR operator&(uint32_t value) const {
            MISR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MISR operator|(uint32_t value) const {
            MISR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MISR, AccessType::ReadOnly>;
        friend class VolatileRegister<MISR, AccessType::WriteOnly>;
        friend class VolatileRegister<MISR, AccessType::ReadWrite>;
    };

    // TAMP status clear register
    union SCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> CTAMP1F; /*!< CTAMP1F */
            microhal::Bitfield<uint32_t, 1, 1> CTAMP2F; /*!< CTAMP2F */
#ifdef __MICROHAL_REGISTERS_TAMP_HAS_TAMP3
            microhal::Bitfield<uint32_t, 2, 1> CTAMP3F; /*!< CTAMP3F */
#endif
            microhal::Bitfield<uint32_t, 16, 1> CITAMP1F; /*!< CITAMP1F */
            microhal::Bitfield<uint32_t, 18, 1> CITAMP3F; /*!< CITAMP3F */
            microhal::Bitfield<uint32_t, 19, 1> CITAMP4F; /*!< CITAMP4F */
            microhal::Bitfield<uint32_t, 20, 1> CITAMP5F; /*!< CITAMP5F */
            microhal::Bitfield<uint32_t, 21, 1> CITAMP6F; /*!< CITAMP6F */
            microhal::Bitfield<uint32_t, 22, 1> CITAMP7F; /*!< CITAMP7F */
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

    // TAMP backup register
    union BKPxR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> BKP; /*!< BKP */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        BKPxR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        BKPxR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        BKPxR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        BKPxR operator&(uint32_t value) const {
            BKPxR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        BKPxR operator|(uint32_t value) const {
            BKPxR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<BKPxR, AccessType::ReadOnly>;
        friend class VolatileRegister<BKPxR, AccessType::WriteOnly>;
        friend class VolatileRegister<BKPxR, AccessType::ReadWrite>;
    };

    // TAMP hardware configuration register 2
    union HWCFGR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 8> PTIONREG_OUT; /*!< PTIONREG_OUT */
            microhal::Bitfield<uint32_t, 8, 4> TRUST_ZONE;   /*!< TRUST_ZONE */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        HWCFGR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        HWCFGR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        HWCFGR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        HWCFGR2 operator&(uint32_t value) const {
            HWCFGR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        HWCFGR2 operator|(uint32_t value) const {
            HWCFGR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<HWCFGR2, AccessType::ReadOnly>;
        friend class VolatileRegister<HWCFGR2, AccessType::WriteOnly>;
        friend class VolatileRegister<HWCFGR2, AccessType::ReadWrite>;
    };

    // TAMP hardware configuration register 1
    union HWCFGR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 8> BACKUP_REGS;    /*!< BACKUP_REGS */
            microhal::Bitfield<uint32_t, 8, 4> TAMPER;         /*!< TAMPER */
            microhal::Bitfield<uint32_t, 12, 4> ACTIVE_TAMPER; /*!< ACTIVE_TAMPER */
            microhal::Bitfield<uint32_t, 16, 16> INT_TAMPER;   /*!< INT_TAMPER */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        HWCFGR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        HWCFGR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        HWCFGR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        HWCFGR1 operator&(uint32_t value) const {
            HWCFGR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        HWCFGR1 operator|(uint32_t value) const {
            HWCFGR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<HWCFGR1, AccessType::ReadOnly>;
        friend class VolatileRegister<HWCFGR1, AccessType::WriteOnly>;
        friend class VolatileRegister<HWCFGR1, AccessType::ReadWrite>;
    };

    // EXTI IP Version register
    union VERR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> MINREV; /*!< Minor Revision number */
            microhal::Bitfield<uint32_t, 4, 4> MAJREV; /*!< Major Revision number */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        VERR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        VERR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        VERR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        VERR operator&(uint32_t value) const {
            VERR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        VERR operator|(uint32_t value) const {
            VERR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<VERR, AccessType::ReadOnly>;
        friend class VolatileRegister<VERR, AccessType::WriteOnly>;
        friend class VolatileRegister<VERR, AccessType::ReadWrite>;
    };

    // EXTI Identification register
    union IPIDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> IPID; /*!< IP Identification */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IPIDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IPIDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IPIDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IPIDR operator&(uint32_t value) const {
            IPIDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IPIDR operator|(uint32_t value) const {
            IPIDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IPIDR, AccessType::ReadOnly>;
        friend class VolatileRegister<IPIDR, AccessType::WriteOnly>;
        friend class VolatileRegister<IPIDR, AccessType::ReadWrite>;
    };

    // EXTI Size ID register
    union SIDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> SID; /*!< Size Identification */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SIDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SIDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SIDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SIDR operator&(uint32_t value) const {
            SIDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SIDR operator|(uint32_t value) const {
            SIDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SIDR, AccessType::ReadOnly>;
        friend class VolatileRegister<SIDR, AccessType::WriteOnly>;
        friend class VolatileRegister<SIDR, AccessType::ReadWrite>;
    };

    VolatileRegister<CR1, AccessType::ReadWrite> cr1;     /*!< control register 1	Address offset: 0x0 */
    VolatileRegister<CR2, AccessType::ReadWrite> cr2;     /*!< control register 2	Address offset: 0x4 */
    uint32_t reserved0[1];                                /*!< Reserved register	Address offset: 0x8 */
    VolatileRegister<FLTCR, AccessType::ReadWrite> fltcr; /*!< TAMP filter control register	Address offset: 0xC */
    uint32_t reserved1[7];                                /*!< Reserved register	Address offset: 0x10 */
    VolatileRegister<IER, AccessType::ReadWrite> ier;     /*!< TAMP interrupt enable register	Address offset: 0x2C */
    VolatileRegister<SR, AccessType::ReadWrite> sr;       /*!< TAMP status register	Address offset: 0x30 */
    VolatileRegister<MISR, AccessType::ReadWrite> misr;   /*!< TAMP masked interrupt status register	Address offset: 0x34 */
    uint32_t reserved2[1];                                /*!< Reserved register	Address offset: 0x38 */
    VolatileRegister<SCR, AccessType::ReadWrite> scr;     /*!< TAMP status clear register	Address offset: 0x3C */
    uint32_t reserved3[48];                               /*!< Reserved register	Address offset: 0x40 */
    VolatileRegister<BKPxR, AccessType::ReadWrite>
        bkpxr[_MICROHAL_REGISTERS_TAMP_BKP_REGISTES_COUNT];                /*!< TAMP backup register	Address offset: 0x100 */
    uint32_t reserved4[187 - _MICROHAL_REGISTERS_TAMP_BKP_REGISTES_COUNT]; /*!< Reserved register	Address offset: 0x114 */
    VolatileRegister<HWCFGR2, AccessType::ReadWrite> hwcfgr2;              /*!< TAMP hardware configuration register 2	Address offset: 0x3EC */
    VolatileRegister<HWCFGR1, AccessType::ReadWrite> hwcfgr1;              /*!< TAMP hardware configuration register 1	Address offset: 0x3F0 */
    VolatileRegister<VERR, AccessType::ReadWrite> verr;                    /*!< EXTI IP Version register	Address offset: 0x3F4 */
    VolatileRegister<IPIDR, AccessType::ReadWrite> ipidr;                  /*!< EXTI Identification register	Address offset: 0x3F8 */
    VolatileRegister<SIDR, AccessType::ReadWrite> sidr;                    /*!< EXTI Size ID register	Address offset: 0x3FC */
};

#if defined(_MICROHAL_TAMP_BASE_ADDRESS)
[[maybe_unused]] inline TAMP *const tamp = reinterpret_cast<TAMP *>(_MICROHAL_TAMP_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_TAMP
