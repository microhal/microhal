#ifndef _MICROHAL_PERIPHERAL_REGISTER_WWDG
#define _MICROHAL_PERIPHERAL_REGISTER_WWDG

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32G474xx, STM32G484xx, STM32G471xx, STM32G431xx, STM32G473xx, STM32G441xx, STM32GBK1CBT6, STM32G483xx
namespace microhal {
namespace registers {
/**
 *
 */
struct WWDG {
    // Key register
    union KR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> KEY; /*!< Key value (write only, read 0x0000) */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        KR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        KR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        KR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        KR operator&(uint32_t value) const {
            KR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        KR operator|(uint32_t value) const {
            KR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<KR, AccessType::ReadOnly>;
        friend class VolatileRegister<KR, AccessType::WriteOnly>;
        friend class VolatileRegister<KR, AccessType::ReadWrite>;
    };

    // Prescaler register
    union PR {
        union {
            microhal::Bitfield<uint32_t, 0, 3> PR; /*!< Prescaler divider */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PR operator&(uint32_t value) const {
            PR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PR operator|(uint32_t value) const {
            PR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PR, AccessType::ReadOnly>;
        friend class VolatileRegister<PR, AccessType::WriteOnly>;
        friend class VolatileRegister<PR, AccessType::ReadWrite>;
    };

    // Reload register
    union RLR {
        union {
            microhal::Bitfield<uint32_t, 0, 12> RL; /*!< Watchdog counter reload value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RLR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RLR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RLR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RLR operator&(uint32_t value) const {
            RLR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RLR operator|(uint32_t value) const {
            RLR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RLR, AccessType::ReadOnly>;
        friend class VolatileRegister<RLR, AccessType::WriteOnly>;
        friend class VolatileRegister<RLR, AccessType::ReadWrite>;
    };

    // Status register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PVU; /*!< Watchdog prescaler value update */
            microhal::Bitfield<uint32_t, 1, 1> RVU; /*!< Watchdog counter reload value update */
            microhal::Bitfield<uint32_t, 2, 1> WVU; /*!< Watchdog counter window value update */
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

    // Window register
    union WINR {
        union {
            microhal::Bitfield<uint32_t, 0, 12> WIN; /*!< Watchdog counter window value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        WINR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        WINR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        WINR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        WINR operator&(uint32_t value) const {
            WINR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        WINR operator|(uint32_t value) const {
            WINR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<WINR, AccessType::ReadOnly>;
        friend class VolatileRegister<WINR, AccessType::WriteOnly>;
        friend class VolatileRegister<WINR, AccessType::ReadWrite>;
    };

    VolatileRegister<KR, AccessType::ReadWrite> kr;     /*!< Key register	Address offset: 0x0 */
    VolatileRegister<PR, AccessType::ReadWrite> pr;     /*!< Prescaler register	Address offset: 0x4 */
    VolatileRegister<RLR, AccessType::ReadWrite> rlr;   /*!< Reload register	Address offset: 0x8 */
    VolatileRegister<SR, AccessType::ReadWrite> sr;     /*!< Status register	Address offset: 0xC */
    VolatileRegister<WINR, AccessType::ReadWrite> winr; /*!< Window register	Address offset: 0x10 */
};

#if defined(_MICROHAL_WWDG_BASE_ADDRESS)
[[maybe_unused]] static WWDG *const wwdg = reinterpret_cast<WWDG *>(_MICROHAL_WWDG_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_WWDG
