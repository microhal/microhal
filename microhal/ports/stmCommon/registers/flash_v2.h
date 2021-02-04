#ifndef _MICROHAL_PERIPHERAL_REGISTER_FLASH
#define _MICROHAL_PERIPHERAL_REGISTER_FLASH

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU version 1: STM32F407, STM32F411, STM32F413, STM32F412, STM32F401, STM32F405, STM32F217, STM32F215
// Supported MCU version 2: STM32F446
// Supported MCU version 3: STM32F410

#if defined(STM32F407) || defined(STM32F411) || defined(STM32F413) || defined(STM32F412) || defined(STM32F401) || defined(STM32F405) || \
    defined(STM32F217) || defined(STM32F215)  // version 1
#define _MICROHAL_REGISTERS_FLASH_SR_HAS_nWRP 12
#define _MICROHAL_REGISTERS_FLASH_HAS_OPTCR
#endif

#if defined(STM32F446)  // version 2
#define _MICROHAL_REGISTERS_FLASH_SR_HAS_RDERR
#define _MICROHAL_REGISTERS_FLASH_SR_HAS_SPRMOD
#define _MICROHAL_REGISTERS_FLASH_SR_HAS_nWRP 8
#define _MICROHAL_REGISTERS_FLASH_HAS_OPTCR
#endif

#if defined(STM32F410)  // version 3
#define _MICROHAL_REGISTERS_FLASH_SR_HAS_SPRMOD
#define _MICROHAL_REGISTERS_FLASH_SR_HAS_nWRP 5
#define _MICROHAL_REGISTERS_FLASH_HAS_OPTCR
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct FLASH {
    // Flash access control register
    union ACR {
        union {
            microhal::Bitfield<uint32_t, 0, 3> LATENCY; /*!< Latency */
            microhal::Bitfield<uint32_t, 8, 1> PRFTEN;  /*!< Prefetch enable */
            microhal::Bitfield<uint32_t, 9, 1> ICEN;    /*!< Instruction cache enable */
            microhal::Bitfield<uint32_t, 10, 1> DCEN;   /*!< Data cache enable */
            microhal::Bitfield<uint32_t, 11, 1> ICRST;  /*!< Instruction cache reset */
            microhal::Bitfield<uint32_t, 12, 1> DCRST;  /*!< Data cache reset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ACR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ACR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ACR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ACR operator&(uint32_t value) const {
            ACR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ACR operator|(uint32_t value) const {
            ACR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ACR, AccessType::ReadOnly>;
        friend class VolatileRegister<ACR, AccessType::WriteOnly>;
        friend class VolatileRegister<ACR, AccessType::ReadWrite>;
    };

    // Flash key register
    union KEYR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> KEY; /*!< FPEC key */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        KEYR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        KEYR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        KEYR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        KEYR operator&(uint32_t value) const {
            KEYR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        KEYR operator|(uint32_t value) const {
            KEYR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<KEYR, AccessType::ReadOnly>;
        friend class VolatileRegister<KEYR, AccessType::WriteOnly>;
        friend class VolatileRegister<KEYR, AccessType::ReadWrite>;
    };

    // Flash option key register
    union OPTKEYR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> OPTKEY; /*!< Option byte key */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        OPTKEYR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OPTKEYR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OPTKEYR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        OPTKEYR operator&(uint32_t value) const {
            OPTKEYR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        OPTKEYR operator|(uint32_t value) const {
            OPTKEYR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<OPTKEYR, AccessType::ReadOnly>;
        friend class VolatileRegister<OPTKEYR, AccessType::WriteOnly>;
        friend class VolatileRegister<OPTKEYR, AccessType::ReadWrite>;
    };

    // Status register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> EOP;    /*!< End of operation */
            microhal::Bitfield<uint32_t, 1, 1> OPERR;  /*!< Operation error */
            microhal::Bitfield<uint32_t, 4, 1> WRPERR; /*!< Write protection error */
            microhal::Bitfield<uint32_t, 5, 1> PGAERR; /*!< Programming alignment error */
            microhal::Bitfield<uint32_t, 6, 1> PGPERR; /*!< Programming parallelism error */
            microhal::Bitfield<uint32_t, 7, 1> PGSERR; /*!< Programming sequence error */
#ifdef _MICROHAL_REGISTERS_FLASH_SR_HAS_RDERR
            microhal::Bitfield<uint32_t, 8, 1> RDERR; /*!< Read Protection Error */
#endif
            microhal::Bitfield<uint32_t, 16, 1> BSY; /*!< Busy */
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

    // Control register
    union CR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PG;     /*!< Programming */
            microhal::Bitfield<uint32_t, 1, 1> SER;    /*!< Sector Erase */
            microhal::Bitfield<uint32_t, 2, 1> MER;    /*!< Mass Erase */
            microhal::Bitfield<uint32_t, 3, 4> SNB;    /*!< Sector number */
            microhal::Bitfield<uint32_t, 8, 2> PSIZE;  /*!< Program size */
            microhal::Bitfield<uint32_t, 16, 1> STRT;  /*!< Start */
            microhal::Bitfield<uint32_t, 24, 1> EOPIE; /*!< End of operation interrupt enable */
            microhal::Bitfield<uint32_t, 25, 1> ERRIE; /*!< Error interrupt enable */
            microhal::Bitfield<uint32_t, 31, 1> LOCK;  /*!< Lock */
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

#ifdef _MICROHAL_REGISTERS_FLASH_HAS_OPTCR
    // Flash option control register
    union OPTCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> OPTLOCK;                                   /*!< Option lock */
            microhal::Bitfield<uint32_t, 1, 1> OPTSTRT;                                   /*!< Option start */
            microhal::Bitfield<uint32_t, 2, 2> BOR_LEV;                                   /*!< BOR reset Level */
            microhal::Bitfield<uint32_t, 5, 1> WDG_SW;                                    /*!< WDG_SW User option bytes */
            microhal::Bitfield<uint32_t, 6, 1> nRST_STOP;                                 /*!< nRST_STOP User option bytes */
            microhal::Bitfield<uint32_t, 7, 1> nRST_STDBY;                                /*!< nRST_STDBY User option bytes */
            microhal::Bitfield<uint32_t, 8, 8> RDP;                                       /*!< Read protect */
            microhal::Bitfield<uint32_t, 16, _MICROHAL_REGISTERS_FLASH_SR_HAS_nWRP> nWRP; /*!< Not write protect */
#if _MICROHAL_REGISTERS_FLASH_SR_HAS_SPRMOD
            microhal::Bitfield<uint32_t, 31, 1> SPRMOD; /*!< Selection of Protection Mode of nWPRi bits */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        OPTCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OPTCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OPTCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        OPTCR operator&(uint32_t value) const {
            OPTCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        OPTCR operator|(uint32_t value) const {
            OPTCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<OPTCR, AccessType::ReadOnly>;
        friend class VolatileRegister<OPTCR, AccessType::WriteOnly>;
        friend class VolatileRegister<OPTCR, AccessType::ReadWrite>;
    };
#endif

    VolatileRegister<ACR, AccessType::ReadWrite> acr;         /*!< Flash access control register	Address offset: 0x0 */
    VolatileRegister<KEYR, AccessType::ReadWrite> keyr;       /*!< Flash key register	Address offset: 0x4 */
    VolatileRegister<OPTKEYR, AccessType::ReadWrite> optkeyr; /*!< Flash option key register	Address offset: 0x8 */
    VolatileRegister<SR, AccessType::ReadWrite> sr;           /*!< Status register	Address offset: 0xC */
    VolatileRegister<CR, AccessType::ReadWrite> cr;           /*!< Control register	Address offset: 0x10 */
#ifdef _MICROHAL_REGISTERS_FLASH_HAS_OPTCR
    VolatileRegister<OPTCR, AccessType::ReadWrite> optcr; /*!< Flash option control register	Address offset: 0x14 */
#endif
};

#if defined(_MICROHAL_FLASH_BASE_ADDRESS)
[[maybe_unused]] static FLASH *const flash = reinterpret_cast<FLASH *>(_MICROHAL_FLASH_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_FLASH
