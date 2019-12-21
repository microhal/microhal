#ifndef _MICROHAL_PERIPHERAL_REGISTER_IWDG
#define _MICROHAL_PERIPHERAL_REGISTER_IWDG

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32G474xx, STM32G484xx, STM32G471xx, STM32G431xx, STM32G473xx, STM32G441xx, STM32GBK1CBT6, STM32G483xx

namespace microhal {
namespace registers {
/**
 *
 */
struct IWDG {
    // Control register
    union CR {
        union {
            microhal::Bitfield<uint32_t, 0, 7> T;    /*!< 7-bit counter (MSB to LSB) */
            microhal::Bitfield<uint32_t, 7, 1> WDGA; /*!< Activation bit */
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

    // Configuration register
    union CFR {
        union {
            microhal::Bitfield<uint32_t, 0, 7> W;      /*!< 7-bit window value */
            microhal::Bitfield<uint32_t, 9, 1> EWI;    /*!< Early wakeup interrupt */
            microhal::Bitfield<uint32_t, 11, 3> WDGTB; /*!< Timer base */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CFR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CFR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CFR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CFR operator&(uint32_t value) const {
            CFR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CFR operator|(uint32_t value) const {
            CFR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CFR, AccessType::ReadOnly>;
        friend class VolatileRegister<CFR, AccessType::WriteOnly>;
        friend class VolatileRegister<CFR, AccessType::ReadWrite>;
    };

    // Status register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> EWIF; /*!< Early wakeup interrupt flag */
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

    VolatileRegister<CR, AccessType::ReadWrite> cr;   /*!< Control register	Address offset: 0x0 */
    VolatileRegister<CFR, AccessType::ReadWrite> cfr; /*!< Configuration register	Address offset: 0x4 */
    VolatileRegister<SR, AccessType::ReadWrite> sr;   /*!< Status register	Address offset: 0x8 */
};

#if defined(_MICROHAL_IWDG_BASE_ADDRESS)
[[maybe_unused]] static IWDG *const iwdg = reinterpret_cast<IWDG *>(_MICROHAL_IWDG_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_IWDG
