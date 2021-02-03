/*
 * exti_registers_v1.h
 *
 *  Created on: Dec 11, 2020
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_REGISTERS_EXTI_REGISTERS_V1_H_
#define SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_REGISTERS_EXTI_REGISTERS_V1_H_

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include "registersBaseAddressDefinitions.h"

#undef EXTI
// Supported MCU: STM32F7x9, STM32F765, STM32F7x8, STM32F745, STM32F750, STM32F7x7, STM32F7x6, STM32F407, STM32F429, STM32F469, STM32F411, STM32F413,
//                STM32F412, STM32F401, STM32F427, STM32F405, STM32F446, STM32F410, STM32F217, STM32F215,STM32F7x3, STM32F7x2, STM32F730, STM32L100,
//                STM32L162, STM32L151, STM32L0x3, STM32L0x2, STM32L0x1, STM32F102, STM32F103, STM32F101, STM32F107, STM32F100, STM32F0x2, STM32F0x8,
//                STM32F0x1, STM32F0x0, STM32F301, STM32F3x8, STM32F373

namespace microhal {
namespace registers {
/**
 *
 */
struct EXTI {
    // Pending register (EXTI_PR)
    union PR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PR0;   /*!< Pending bit 0 */
            microhal::Bitfield<uint32_t, 1, 1> PR1;   /*!< Pending bit 1 */
            microhal::Bitfield<uint32_t, 2, 1> PR2;   /*!< Pending bit 2 */
            microhal::Bitfield<uint32_t, 3, 1> PR3;   /*!< Pending bit 3 */
            microhal::Bitfield<uint32_t, 4, 1> PR4;   /*!< Pending bit 4 */
            microhal::Bitfield<uint32_t, 5, 1> PR5;   /*!< Pending bit 5 */
            microhal::Bitfield<uint32_t, 6, 1> PR6;   /*!< Pending bit 6 */
            microhal::Bitfield<uint32_t, 7, 1> PR7;   /*!< Pending bit 7 */
            microhal::Bitfield<uint32_t, 8, 1> PR8;   /*!< Pending bit 8 */
            microhal::Bitfield<uint32_t, 9, 1> PR9;   /*!< Pending bit 9 */
            microhal::Bitfield<uint32_t, 10, 1> PR10; /*!< Pending bit 10 */
            microhal::Bitfield<uint32_t, 11, 1> PR11; /*!< Pending bit 11 */
            microhal::Bitfield<uint32_t, 12, 1> PR12; /*!< Pending bit 12 */
            microhal::Bitfield<uint32_t, 13, 1> PR13; /*!< Pending bit 13 */
            microhal::Bitfield<uint32_t, 14, 1> PR14; /*!< Pending bit 14 */
            microhal::Bitfield<uint32_t, 15, 1> PR15; /*!< Pending bit 15 */
            microhal::Bitfield<uint32_t, 16, 1> PR16; /*!< Pending bit 16 */
            microhal::Bitfield<uint32_t, 17, 1> PR17; /*!< Pending bit 17 */
            microhal::Bitfield<uint32_t, 18, 1> PR18; /*!< Pending bit 18 */
            microhal::Bitfield<uint32_t, 19, 1> PR19; /*!< Pending bit 19 */
            microhal::Bitfield<uint32_t, 20, 1> PR20; /*!< Pending bit 20 */
            microhal::Bitfield<uint32_t, 21, 1> PR21; /*!< Pending bit 21 */
            microhal::Bitfield<uint32_t, 22, 1> PR22; /*!< Pending bit 22 */
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

    VolatileRegister<uint32_t, AccessType::ReadWrite> imr;   /*!< Interrupt mask register (EXTI_IMR)	Address offset: 0x0 */
    VolatileRegister<uint32_t, AccessType::ReadWrite> emr;   /*!< Event mask register (EXTI_EMR)	Address offset: 0x4 */
    VolatileRegister<uint32_t, AccessType::ReadWrite> rtsr;  /*!< Rising Trigger selection register (EXTI_RTSR)	Address offset: 0x8 */
    VolatileRegister<uint32_t, AccessType::ReadWrite> ftsr;  /*!< Falling Trigger selection register (EXTI_FTSR)	Address offset: 0xC */
    VolatileRegister<uint32_t, AccessType::ReadWrite> swier; /*!< Software interrupt event register (EXTI_SWIER)	Address offset: 0x10 */
    VolatileRegister<PR, AccessType::ReadWrite> pr;          /*!< Pending register (EXTI_PR)	Address offset: 0x14 */
};

#if defined(_MICROHAL_EXTI_BASE_ADDRESS)
[[maybe_unused]] inline EXTI *const exti1 = reinterpret_cast<EXTI *>(_MICROHAL_EXTI_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal

#endif /* SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_REGISTERS_EXTI_REGISTERS_V1_H_ */
