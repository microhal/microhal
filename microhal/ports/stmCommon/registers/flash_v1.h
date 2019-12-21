#ifndef _MICROHAL_PERIPHERAL_REGISTER_Flash
#define _MICROHAL_PERIPHERAL_REGISTER_Flash

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU version 1: STM32F0x1, STM32F0x2, STM32F0x8
// Supported MCU version 2: STM32F0x0, STM32F302, STM32F303, STM32F3x4
// Supported MCU version 3: STM32F301
// Supported MCU version 4: STM32F3x8, STM32F373
// Supported MCU version 5: STM32F102, STM32F107, STM32F103, STM32F101
// Supported MCU version 6: STM32F100

#if defined(STM32F0x1) || defined(STM32F0x2) || defined(STM32F0x8)  // version 1
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_LATENCY
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_PREFETCH
#define _MICROHAL_REGISTERS_FLASH_OBR_HAS_nBOOT0
#define _MICROHAL_REGISTERS_FLASH_OBR_HAS_BOOT_SEL
#define _MICROHAL_REGISTERS_FLASH_OBR_HAS_RAM_PARITY_CHECK
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_FORCE_OPTLOAD
#define _MICROHAL_REGISTERS_FLASH_OBR_VERSION 1
#endif

#if defined(STM32F0x0) || defined(STM32F302) || defined(STM32F303) || defined(STM32F3x4)  // version 2
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_LATENCY
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_PREFETCH
#define _MICROHAL_REGISTERS_FLASH_OBR_HAS_RAM_PARITY_CHECK
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_FORCE_OPTLOAD
#define _MICROHAL_REGISTERS_FLASH_OBR_VERSION 1
#endif

#if defined(STM32F301)  // version 3
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_LATENCY
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_PREFETCH
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_FORCE_OPTLOAD
#define _MICROHAL_REGISTERS_FLASH_OBR_VERSION 1
#endif

#if defined(STM32F3x8) || defined(STM32F373)  // version 4
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_LATENCY
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_PREFETCH
#define _MICROHAL_REGISTERS_FLASH_OBR_HAS_SDADC12_VDD_MONITOR
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_FORCE_OPTLOAD
#define _MICROHAL_REGISTERS_FLASH_OBR_VERSION 1
#endif

#if defined(STM32F102) || defined(STM32F107) || defined(STM32F103) || defined(STM32F101)  // version 5
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_LATENCY
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_PREFETCH
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_HLFCYA
#define _MICROHAL_REGISTERS_FLASH_OBR_VERSION 2
#endif

#if defined(STM32F100)  // version 6
#define _MICROHAL_REGISTERS_FLASH_ACR_HAS_HLFCYA
#define _MICROHAL_REGISTERS_FLASH_OBR_VERSION 2
#endif

#if defined(_MICROHAL_REGISTERS_FLASH_OBR_HAS_BOOT_SEL) && defined(_MICROHAL_REGISTERS_FLASH_OBR_HAS_SDADC12_VDD_MONITOR)
#error
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
#ifdef _MICROHAL_REGISTERS_FLASH_ACR_HAS_LATENCY
            microhal::Bitfield<uint32_t, 0, 3> LATENCY; /*!< Latency */
#endif
#ifdef _MICROHAL_REGISTERS_FLASH_ACR_HAS_HLFCYA
            microhal::Bitfield<uint32_t, 3, 1> HLFCYA; /*!< Flash half cycle access enable */
#endif
#ifdef _MICROHAL_REGISTERS_FLASH_ACR_HAS_PREFETCH
            microhal::Bitfield<uint32_t, 4, 1> PRFTBE; /*!< Prefetch buffer enable */
            microhal::Bitfield<uint32_t, 5, 1> PRFTBS; /*!< Prefetch buffer status */
#endif
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
            microhal::Bitfield<uint32_t, 0, 32> FKEYR; /*!< Flash Key */
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

    // Flash status register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> BSY;      /*!< Busy */
            microhal::Bitfield<uint32_t, 2, 1> PGERR;    /*!< Programming error */
            microhal::Bitfield<uint32_t, 4, 1> WRPRTERR; /*!< Write protection error */
            microhal::Bitfield<uint32_t, 5, 1> EOP;      /*!< End of operation */
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

    // Flash control register
    union CR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PG;     /*!< Programming */
            microhal::Bitfield<uint32_t, 1, 1> PER;    /*!< Page erase */
            microhal::Bitfield<uint32_t, 2, 1> MER;    /*!< Mass erase */
            microhal::Bitfield<uint32_t, 4, 1> OPTPG;  /*!< Option byte programming */
            microhal::Bitfield<uint32_t, 5, 1> OPTER;  /*!< Option byte erase */
            microhal::Bitfield<uint32_t, 6, 1> STRT;   /*!< Start */
            microhal::Bitfield<uint32_t, 7, 1> LOCK;   /*!< Lock */
            microhal::Bitfield<uint32_t, 9, 1> OPTWRE; /*!< Option bytes write enable */
            microhal::Bitfield<uint32_t, 10, 1> ERRIE; /*!< Error interrupt enable */
            microhal::Bitfield<uint32_t, 12, 1> EOPIE; /*!< End of operation interrupt enable */
#ifdef _MICROHAL_REGISTERS_FLASH_ACR_HAS_FORCE_OPTLOAD
            microhal::Bitfield<uint32_t, 13, 1> FORCE_OPTLOAD; /*!< Force option byte loading */
#endif
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

    // Flash address register
    union AR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> FAR; /*!< Flash address */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        AR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        AR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        AR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        AR operator&(uint32_t value) const {
            AR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        AR operator|(uint32_t value) const {
            AR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<AR, AccessType::ReadOnly>;
        friend class VolatileRegister<AR, AccessType::WriteOnly>;
        friend class VolatileRegister<AR, AccessType::ReadWrite>;
    };

    // Option byte register
    union OBR {
#if _MICROHAL_REGISTERS_FLASH_OBR_VERSION == 1
        union {
            microhal::Bitfield<uint32_t, 0, 1> OPTERR;      /*!< Option byte error */
            microhal::Bitfield<uint32_t, 1, 2> RDPRT;       /*!< Read protection level status */
            microhal::Bitfield<uint32_t, 8, 1> WDG_SW;      /*!< WDG_SW */
            microhal::Bitfield<uint32_t, 9, 1> nRST_STOP;   /*!< nRST_STOP */
            microhal::Bitfield<uint32_t, 10, 1> nRST_STDBY; /*!< nRST_STDBY */
#ifdef _MICROHAL_REGISTERS_FLASH_OBR_HAS_nBOOT0
            microhal::Bitfield<uint32_t, 11, 1> nBOOT0; /*!< nBOOT0 */
#endif
            microhal::Bitfield<uint32_t, 12, 1> nBOOT1;       /*!< BOOT1 */
            microhal::Bitfield<uint32_t, 13, 1> VDDA_MONITOR; /*!< VDDA_MONITOR */
#ifdef _MICROHAL_REGISTERS_FLASH_OBR_HAS_RAM_PARITY_CHECK
            microhal::Bitfield<uint32_t, 14, 1> RAM_PARITY_CHECK; /*!< RAM_PARITY_CHECK */
#endif
#ifdef _MICROHAL_REGISTERS_FLASH_OBR_HAS_BOOT_SEL
            microhal::Bitfield<uint32_t, 15, 1> BOOT_SEL; /*!< BOOT_SEL */
#endif
#ifdef _MICROHAL_REGISTERS_FLASH_OBR_HAS_SDADC12_VDD_MONITOR
            microhal::Bitfield<uint32_t, 15, 1> SDADC12_VDD_MONITOR; /*!< SDADC12_VDD_MONITOR */
#endif
            microhal::Bitfield<uint32_t, 16, 8> Data0; /*!< Data0 */
            microhal::Bitfield<uint32_t, 24, 8> Data1; /*!< Data1 */
        };
#elif _MICROHAL_REGISTERS_FLASH_OBR_VERSION == 2
        union {
            microhal::Bitfield<uint32_t, 0, 1> OPTERR;     /*!< Option byte error */
            microhal::Bitfield<uint32_t, 1, 1> RDPRT;      /*!< Read protection */
            microhal::Bitfield<uint32_t, 2, 1> WDG_SW;     /*!< WDG_SW */
            microhal::Bitfield<uint32_t, 3, 1> nRST_STOP;  /*!< nRST_STOP */
            microhal::Bitfield<uint32_t, 4, 1> nRST_STDBY; /*!< nRST_STDBY */
            microhal::Bitfield<uint32_t, 10, 8> Data0;     /*!< Data0 */
            microhal::Bitfield<uint32_t, 18, 8> Data1;     /*!< Data1 */
        };
#endif
        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        OBR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OBR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OBR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        OBR operator&(uint32_t value) const {
            OBR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        OBR operator|(uint32_t value) const {
            OBR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<OBR, AccessType::ReadOnly>;
        friend class VolatileRegister<OBR, AccessType::WriteOnly>;
        friend class VolatileRegister<OBR, AccessType::ReadWrite>;
    };

    // Write protection register
    union WRPR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> WRP; /*!< Write protect */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        WRPR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        WRPR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        WRPR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        WRPR operator&(uint32_t value) const {
            WRPR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        WRPR operator|(uint32_t value) const {
            WRPR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<WRPR, AccessType::ReadOnly>;
        friend class VolatileRegister<WRPR, AccessType::WriteOnly>;
        friend class VolatileRegister<WRPR, AccessType::ReadWrite>;
    };

    VolatileRegister<ACR, AccessType::ReadWrite> acr;         /*!< Flash access control register	Address offset: 0x0 */
    VolatileRegister<KEYR, AccessType::ReadWrite> keyr;       /*!< Flash key register	Address offset: 0x4 */
    VolatileRegister<OPTKEYR, AccessType::ReadWrite> optkeyr; /*!< Flash option key register	Address offset: 0x8 */
    VolatileRegister<SR, AccessType::ReadWrite> sr;           /*!< Flash status register	Address offset: 0xC */
    VolatileRegister<CR, AccessType::ReadWrite> cr;           /*!< Flash control register	Address offset: 0x10 */
    VolatileRegister<AR, AccessType::ReadWrite> ar;           /*!< Flash address register	Address offset: 0x14 */
    uint32_t reserved0;                                       /*!< Reserved register	Address offset: 0x18 */
    VolatileRegister<OBR, AccessType::ReadWrite> obr;         /*!< Option byte register	Address offset: 0x1C */
    VolatileRegister<WRPR, AccessType::ReadWrite> wrpr;       /*!< Write protection register	Address offset: 0x20 */
};

#if defined(_MICROHAL_FLASH_BASE_ADDRESS)
[[maybe_unused]] static FLASH *const flash = reinterpret_cast<FLASH *>(_MICROHAL_FLASH_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_Flash
