#ifndef _MICROHAL_PERIPHERAL_REGISTER_IWDG
#define _MICROHAL_PERIPHERAL_REGISTER_IWDG

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU version 1: STM32L4x6, STM32L4x3, STM32L4x1, STM32L4x2, STM32L4x5, STM32L0x3, STM32L0x2, STM32L0x1, STM32L4R7, STM32L4S9, STM32L4R9,
//                          STM32L4S5, STM32L4S7, STM32L4R5, STM32F7x3, STM32F7x2, STM32F730, STM32H742x, STM32H7x3, STM32H743x, STM32H753x,
//                          STM32H7x5_CM7, STM32H7x5_CM4, STM32F0x2, STM32F0x8, STM32F0x1, STM32F0x0, STM32F302, STM32F303, STM32F3x4, STM32F301,
//                          STM32F3x8, STM32F373, STM32H7x7_CM4, STM32H750x, STM32H7x7_CM7
// Supported MCU version 2: STM32F7x9, STM32F765, STM32F7x8, STM32F745, STM32F750, STM32F7x7, STM32F7x6
// Supported MCU version 3: STM32L100, STM32L162, STM32L151, STM32F407, STM32F429, STM32F469, STM32F427, STM32F405, STM32F446, STM32F217, STM32F215,
//                          STM32F411, STM32F413, STM32F412, STM32F401, STM32F410, STM32F102, STM32F107, STM32F103, STM32F101, STM32F100

#if defined(STM32L4x6) || defined(STM32L4x3) || defined(STM32L4x1) || defined(STM32L4x2) || defined(STM32L4x5) || defined(STM32L0x3) ||           \
    defined(STM32L0x2) || defined(STM32L0x1) || defined(STM32L4R7) || defined(STM32L4S9) || defined(STM32L4R9) || defined(STM32L4S5) ||           \
    defined(STM32L4S7) || defined(STM32L4R5) || defined(STM32F7x3) || defined(STM32F7x2) || defined(STM32F730) || defined(STM32H742x) ||          \
    defined(STM32H7x3) || defined(STM32H743x) || defined(STM32H753x) || defined(STM32H7x5_CM7) || defined(STM32H7x5_CM4) || defined(STM32F0x2) || \
    defined(STM32F0x8) || defined(STM32F0x1) || defined(STM32F0x0) || defined(STM32F302) || defined(STM32F303) || defined(STM32F3x4) ||           \
    defined(STM32F301) || defined(STM32F3x8) || defined(STM32F373) || defined(STM32H7x7_CM4) || defined(STM32H750x) ||                            \
    defined(STM32H7x7_CM7)  // version 1
#define _MICROHAL_REGISTERS_IWDG_SR_HAS_WVU
#define _MICROHAL_REGISTERS_IWDG_HAS_WINR
#endif

#if defined(STM32F7x9) || defined(STM32F765) || defined(STM32F7x8) || defined(STM32F745) || defined(STM32F750) || defined(STM32F7x7) || \
    defined(STM32F7x6)  // version 2
#define _MICROHAL_REGISTERS_IWDG_HAS_WINR
#endif

#if defined(STM32L100) || defined(STM32L162) || defined(STM32L151) || defined(STM32F407) || defined(STM32F429) || defined(STM32F469) || \
    defined(STM32F427) || defined(STM32F405) || defined(STM32F446) || defined(STM32F217) || defined(STM32F215) || defined(STM32F411) || \
    defined(STM32F413) || defined(STM32F412) || defined(STM32F401) || defined(STM32F410) || defined(STM32F102) || defined(STM32F107) || \
    defined(STM32F103) || defined(STM32F101) || defined(STM32F100)  // version 3
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct IWDG {
    // Key register
    union KR {
        union {
            microhal::Bitfield<uint32_t, 0, 16>
                KEY; /*!< Key value (write only, read 0x0000) These bits must be written by software at regular intervals with the key value 0xAAAA,
                        otherwise the watchdog generates a reset when the counter reaches 0. Writing the key value 0x5555 to enable access to the
                        IWDG_PR, IWDG_RLR and IWDG_WINR registers (see Section23.3.6: Register access protection) Writing the key value CCCCh starts
                        the watchdog (except if the hardware watchdog option is selected) */
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
            microhal::Bitfield<uint32_t, 0, 3>
                pr; /*!< Prescaler divider These bits are write access protected see Section23.3.6: Register access protection. They are written by
                       software to select the prescaler divider feeding the counter clock. PVU bit of IWDG_SR must be reset in order to be able to
                       change the prescaler divider. Note: Reading this register returns the prescaler value from the VDD voltage domain. This value
                       may not be up to date/valid if a write operation to this register is ongoing. For this reason the value read from this register
                       is valid only when the PVU bit in the IWDG_SR register is reset. */
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
            microhal::Bitfield<uint32_t, 0, 12>
                RL; /*!< Watchdog counter reload value These bits are write access protected see Section23.3.6. They are written by software to define
                       the value to be loaded in the watchdog counter each time the value 0xAAAA is written in the IWDG_KR register. The watchdog
                       counter counts down from this value. The timeout period is a function of this value and the clock prescaler. Refer to the
                       datasheet for the timeout information. The RVU bit in the IWDG_SR register must be reset in order to be able to change the
                       reload value. Note: Reading this register returns the reload value from the VDD voltage domain. This value may not be up to
                       date/valid if a write operation to this register is ongoing on this register. For this reason the value read from this register
                       is valid only when the RVU bit in the IWDG_SR register is reset. */
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
#ifdef _MICROHAL_REGISTERS_IWDG_SR_HAS_WVU
            microhal::Bitfield<uint32_t, 2, 1> WVU; /*!< Watchdog counter window value update */
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

    // Window register
    union WINR {
        union {
            microhal::Bitfield<uint32_t, 0, 12>
                WIN; /*!< Watchdog counter window value These bits are write access protected see Section23.3.6. These bits contain the high limit of
                        the window value to be compared to the downcounter. To prevent a reset, the downcounter must be reloaded when its value is
                        lower than the window register value and greater than 0x0 The WVU bit in the IWDG_SR register must be reset in order to be
                        able to change the reload value. Note: Reading this register returns the reload value from the VDD voltage domain. This value
                        may not be valid if a write operation to this register is ongoing. For this reason the value read from this register is valid
                        only when the WVU bit in the IWDG_SR register is reset. */
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

    VolatileRegister<KR, AccessType::ReadWrite> kr;   /*!< Key register	Address offset: 0x0 */
    VolatileRegister<PR, AccessType::ReadWrite> pr;   /*!< Prescaler register	Address offset: 0x4 */
    VolatileRegister<RLR, AccessType::ReadWrite> rlr; /*!< Reload register	Address offset: 0x8 */
    VolatileRegister<SR, AccessType::ReadWrite> sr;   /*!< Status register	Address offset: 0xC */
#ifdef _MICROHAL_REGISTERS_IWDG_HAS_WINR
    VolatileRegister<WINR, AccessType::ReadWrite> winr; /*!< Window register	Address offset: 0x10 */
#endif
};

#if defined(_MICROHAL_IWDG_BASE_ADDRESS)
[[maybe_unused]] static IWDG *const iwdg = reinterpret_cast<IWDG *>(_MICROHAL_IWDG_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_IWDG
