#ifndef _MICROHAL_PERIPHERAL_REGISTER_WWDG
#define _MICROHAL_PERIPHERAL_REGISTER_WWDG

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU version 1: STM32L4x6, STM32L4x3, STM32L4x1, STM32L4x2, STM32L4x5, STM32F217, STM32F215, STM32L4R7, STM32L4S9, STM32L4R9, STM32L4S5,
//                          STM32L4S7, STM32L4R5, STM32F7x9, STM32F765, STM32F7x8, STM32F7x3, STM32F7x2, STM32F745, STM32F730, STM32F750, STM32F7x7,
//                          STM32F7x6, STM32F407, STM32F429, STM32F469, STM32F411, STM32F413, STM32F412, STM32F401, STM32F427, STM32F405, STM32F446,
//                          STM32F410, STM32L100, STM32L162, STM32L151, STM32L0x3, STM32L0x2, STM32L0x1,  STM32F102, STM32F107, STM32F103, STM32F101,
//                          STM32F100, STM32F0x2, STM32F0x8, STM32F0x1, STM32F0x0, STM32F302, STM32F303, STM32F3x4, STM32F301, STM32F3x8, STM32F373
// Supported MCU version 2: STM32H742x, STM32H7x3, STM32H743x, STM32H753x, STM32H7x5_CM7, STM32H7x5_CM4, STM32H7x7_CM4, STM32H750x, STM32H7x7_CM7

#if defined(STM32L4x6) || defined(STM32L4x3) || defined(STM32L4x1) || defined(STM32L4x2) || defined(STM32L4x5) || defined(STM32F217) || \
    defined(STM32F215) || defined(STM32L4R7) || defined(STM32L4S9) || defined(STM32L4R9) || defined(STM32L4S5) || defined(STM32L4S7) || \
    defined(STM32L4R5) || defined(STM32F7x9) || defined(STM32F765) || defined(STM32F7x8) || defined(STM32F7x3) || defined(STM32F7x2) || \
    defined(STM32F745) || defined(STM32F730) || defined(STM32F750) || defined(STM32F7x7) || defined(STM32F7x6) || defined(STM32F407) || \
    defined(STM32F429) || defined(STM32F469) || defined(STM32F411) || defined(STM32F413) || defined(STM32F412) || defined(STM32F401) || \
    defined(STM32F427) || defined(STM32F405) || defined(STM32F446) || defined(STM32F410) || defined(STM32L100) || defined(STM32L162) || \
    defined(STM32L151) || defined(STM32L0x3) || defined(STM32L0x2) || defined(STM32L0x1) || defined(STM32F102) || defined(STM32F107) || \
    defined(STM32F103) || defined(STM32F101) || defined(STM32F100) || defined(STM32F0x2) || defined(STM32F0x8) || defined(STM32F0x1) || \
    defined(STM32F0x0) || defined(STM32F302) || defined(STM32F303) || defined(STM32F3x4) || defined(STM32F301) || defined(STM32F3x8) || \
    defined(STM32F373)  // version 1
#define _MICROHAL_REGISTERS_WWDG_CFR_WDGTB_POSITION 7
#endif

#if defined(STM32H742x) || defined(STM32H7x3) || defined(STM32H743x) || defined(STM32H753x) || defined(STM32H7x5_CM7) || defined(STM32H7x5_CM4) || \
    defined(STM32H7x7_CM4) || defined(STM32H750x) || defined(STM32H7x7_CM7)  // version 2
#define _MICROHAL_REGISTERS_WWDG_CFR_WDGTB_POSITION 11
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct WWDG {
    // Control register
    union CR {
        union {
            microhal::Bitfield<uint32_t, 0, 7>
                T; /*!< 7-bit counter (MSB to LSB) These bits contain the value of the watchdog counter. It is decremented every (4096 x 2WDGTB[1:0])
                      PCLK cycles. A reset is produced when it is decremented from 0x40 to 0x3F (T6 becomes cleared). */
            microhal::Bitfield<uint32_t, 7, 1> WDGA; /*!< Activation bit This bit is set by software and only cleared by hardware after a reset. When
                                                        WDGA=1, the watchdog can generate a reset. */
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
            microhal::Bitfield<uint32_t, 0, 7> W; /*!< 7-bit window value These bits contain the window value to be compared to the downcounter. */
            microhal::Bitfield<uint32_t, _MICROHAL_REGISTERS_WWDG_CFR_WDGTB_POSITION, 2>
                WDGTB;                              /*!< Timer base The time base of the prescaler can be modified as follows: */
            microhal::Bitfield<uint32_t, 9, 1> EWI; /*!< Early wakeup interrupt When set, an interrupt occurs whenever the counter reaches the value
                                                       0x40. This interrupt is only cleared by hardware after a reset. */
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
            microhal::Bitfield<uint32_t, 0, 1>
                EWIF; /*!< Early wakeup interrupt flag This bit is set by hardware when the counter has reached the value 0x40. It must be cleared by
                         software by writing 0. A write of 1 has no effect. This bit is also set if the interrupt is not enabled. */
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

#if defined(_MICROHAL_WWDG_BASE_ADDRESS)
[[maybe_unused]] static WWDG *const wwdg = reinterpret_cast<WWDG *>(_MICROHAL_WWDG_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_WWDG
