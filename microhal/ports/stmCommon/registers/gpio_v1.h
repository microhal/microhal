#ifndef _MICROHAL_PERIPHERAL_REGISTER_GPIO
#define _MICROHAL_PERIPHERAL_REGISTER_GPIO

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU version 1: STM32L4x3, STM32L4x1, STM32L4x2, STM32L4x5, STM32F7x3, STM32F7x2, STM32F730, STM32F407, STM32F429, STM32F469, STM32F411,
// STM32F413, STM32F412, STM32F401, STM32F427, STM32F405, STM32F446, STM32F410, STM32F217, STM32F215
// Supported MCU version 1: STM32L100, STM32L162, STM32L151
// Supported MCU version 2: STM32G474xx, STM32G484xx, STM32G471xx, STM32G431xx, STM32G473xx, STM32G441xx, STM32GBK1CBT6, STM32G483xx
// Supported MCU version 2: STM32F0x2, STM32F0x8, STM32F0x1, STM32F0x0
// Supported MCU version 2: STM32F302, STM32F303, STM32F301, STM32F3x8, STM32F373
// Supported MCU version 2: STM32F3x4
// Supported MCU version 2: STM32F7x9, STM32F765, STM32F7x8, STM32F745, STM32F750, STM32F7x7, STM32F7x6
// Supported MCU version 3: STM32L4x6, STM32L4R7, STM32L4S9, STM32L4R9, STM32L4S5, STM32L4S7, STM32L4R5
#if defined(STM32F411xE)
#define STM32F411
#endif

#if defined(STM32F334x8)
#define STM32F3x4
#endif

#if defined(STM32L4x3) || defined(STM32L4x1) || defined(STM32L4x2) || defined(STM32L4x5) || defined(STM32F7x3) || defined(STM32F7x2) || \
    defined(STM32F730) || defined(STM32F407) || defined(STM32F429) || defined(STM32F469) || defined(STM32F411) || defined(STM32F413) || \
    defined(STM32F412) || defined(STM32F401) || defined(STM32F427) || defined(STM32F405) || defined(STM32F446) || defined(STM32F410) || \
    defined(STM32F217) || defined(STM32F215) || defined(STM32L100) || defined(STM32L162) || defined(STM32L151)
#define _MICROHAL_GPIO_REGISTER_VERSION 1
#endif

#if defined(STM32G474xx) || defined(STM32G484xx) || defined(STM32G471xx) || defined(STM32G431xx) || defined(STM32G473xx) || defined(STM32G441xx) || \
    defined(STM32GBK1CBT6) || defined(STM32G483xx) || defined(STM32F0x2) || defined(STM32F0x8) || defined(STM32F0x1) || defined(STM32F0x0) ||       \
    defined(STM32F302) || defined(STM32F303) || defined(STM32F301) || defined(STM32F3x8) || defined(STM32F373) || defined(STM32F3x4) ||             \
    defined(STM32F7x9) || defined(STM32F765) || defined(STM32F7x8) || defined(STM32F745) || defined(STM32F750) || defined(STM32F7x7) ||             \
    defined(STM32F7x6)
#define _MICROHAL_GPIO_REGISTER_VERSION 2
#endif

#if defined(STM32L4x6) || defined(STM32L4R7) || defined(STM32L4S9) || defined(STM32L4R9) || defined(STM32L4S5) || defined(STM32L4S7) || \
    defined(STM32L4R5)
#define _MICROHAL_GPIO_REGISTER_VERSION 3
#endif

#if _MICROHAL_GPIO_REGISTER_VERSION == 2
#define _MICROHAL_GPIO_REGISTER_HAS_BRR
#endif
#if _MICROHAL_GPIO_REGISTER_VERSION == 3
#define _MICROHAL_GPIO_REGISTER_HAS_BRR
#define _MICROHAL_GPIO_REGISTER_HAS_ASCR
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct GPIO {
    // GPIO port mode register
    union MODER {
        union {
            microhal::Bitfield<uint32_t, 0, 2>
                MODER0; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 2, 2>
                MODER1; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 4, 2>
                MODER2; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 6, 2>
                MODER3; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 8, 2>
                MODER4; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 10, 2>
                MODER5; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 12, 2>
                MODER6; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 14, 2>
                MODER7; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 16, 2>
                MODER8; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 18, 2>
                MODER9; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 20, 2>
                MODER10; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 22, 2>
                MODER11; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 24, 2>
                MODER12; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 26, 2>
                MODER13; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 28, 2>
                MODER14; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
            microhal::Bitfield<uint32_t, 30, 2>
                MODER15; /*!< [1:0]: Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O mode. */
        };

        operator uint32_t() const { return raw; }

        MODER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MODER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MODER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }

     private:
        uint32_t raw;
        friend class VolatileRegister<MODER, AccessType::ReadOnly>;
        friend class VolatileRegister<MODER, AccessType::WriteOnly>;
        friend class VolatileRegister<MODER, AccessType::ReadWrite>;
    };

    // GPIO port output type register
    union OTYPER {
        union {
            microhal::Bitfield<uint32_t, 0, 1>
                OT0; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 1, 1>
                OT1; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 2, 1>
                OT2; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 3, 1>
                OT3; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 4, 1>
                OT4; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 5, 1>
                OT5; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 6, 1>
                OT6; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 7, 1>
                OT7; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 8, 1>
                OT8; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 9, 1>
                OT9; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 10, 1>
                OT10; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 11, 1>
                OT11; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 12, 1>
                OT12; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 13, 1>
                OT13; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 14, 1>
                OT14; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
            microhal::Bitfield<uint32_t, 15, 1>
                OT15; /*!< Port x configuration bits (y = 0..15) These bits are written by software to configure the I/O output type. */
        };

        operator uint32_t() const { return raw; }

        OTYPER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OTYPER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OTYPER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }

     private:
        uint32_t raw;
        friend class VolatileRegister<OTYPER, AccessType::ReadOnly>;
        friend class VolatileRegister<OTYPER, AccessType::WriteOnly>;
        friend class VolatileRegister<OTYPER, AccessType::ReadWrite>;
    };

    // GPIO port output speed register
    union OSPEEDR {
        union {
            microhal::Bitfield<uint32_t, 0, 2> OSPEEDR0;   /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 2, 2> OSPEEDR1;   /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 4, 2> OSPEEDR2;   /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 6, 2> OSPEEDR3;   /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 8, 2> OSPEEDR4;   /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 10, 2> OSPEEDR5;  /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 12, 2> OSPEEDR6;  /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 14, 2> OSPEEDR7;  /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 16, 2> OSPEEDR8;  /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 18, 2> OSPEEDR9;  /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 20, 2> OSPEEDR10; /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 22, 2> OSPEEDR11; /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 24, 2> OSPEEDR12; /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 26, 2> OSPEEDR13; /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 28, 2> OSPEEDR14; /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 30, 2> OSPEEDR15; /*!< Port x configuration bits (y = 0..15) */
        };

        operator uint32_t() const { return raw; }

        OSPEEDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OSPEEDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OSPEEDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }

     private:
        uint32_t raw;
        friend class VolatileRegister<OSPEEDR, AccessType::ReadOnly>;
        friend class VolatileRegister<OSPEEDR, AccessType::WriteOnly>;
        friend class VolatileRegister<OSPEEDR, AccessType::ReadWrite>;
    };

    // GPIO port pull-up/pull-down register
    union PUPDR {
        union {
            microhal::Bitfield<uint32_t, 0, 2> PUPDR0;   /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 2, 2> PUPDR1;   /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 4, 2> PUPDR2;   /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 6, 2> PUPDR3;   /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 8, 2> PUPDR4;   /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 10, 2> PUPDR5;  /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 12, 2> PUPDR6;  /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 14, 2> PUPDR7;  /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 16, 2> PUPDR8;  /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 18, 2> PUPDR9;  /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 20, 2> PUPDR10; /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 22, 2> PUPDR11; /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 24, 2> PUPDR12; /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 26, 2> PUPDR13; /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 28, 2> PUPDR14; /*!< Port x configuration bits (y = 0..15) */
            microhal::Bitfield<uint32_t, 30, 2> PUPDR15; /*!< Port x configuration bits (y = 0..15) */
        };

        operator uint32_t() const { return raw; }

        PUPDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PUPDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PUPDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }

     private:
        uint32_t raw;
        friend class VolatileRegister<PUPDR, AccessType::ReadOnly>;
        friend class VolatileRegister<PUPDR, AccessType::WriteOnly>;
        friend class VolatileRegister<PUPDR, AccessType::ReadWrite>;
    };

    // GPIO port input data register
    union IDR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> IDR0;   /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 1, 1> IDR1;   /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 2, 1> IDR2;   /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 3, 1> IDR3;   /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 4, 1> IDR4;   /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 5, 1> IDR5;   /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 6, 1> IDR6;   /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 7, 1> IDR7;   /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 8, 1> IDR8;   /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 9, 1> IDR9;   /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 10, 1> IDR10; /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 11, 1> IDR11; /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 12, 1> IDR12; /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 13, 1> IDR13; /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 14, 1> IDR14; /*!< Port input data (y = 0..15) */
            microhal::Bitfield<uint32_t, 15, 1> IDR15; /*!< Port input data (y = 0..15) */
        };

        operator uint32_t() const { return raw; }

        IDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }

     private:
        uint32_t raw;
        friend class VolatileRegister<IDR, AccessType::ReadOnly>;
        friend class VolatileRegister<IDR, AccessType::WriteOnly>;
        friend class VolatileRegister<IDR, AccessType::ReadWrite>;
    };

    // GPIO port output data register
    union ODR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ODR0;   /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 1, 1> ODR1;   /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 2, 1> ODR2;   /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 3, 1> ODR3;   /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 4, 1> ODR4;   /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 5, 1> ODR5;   /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 6, 1> ODR6;   /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 7, 1> ODR7;   /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 8, 1> ODR8;   /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 9, 1> ODR9;   /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 10, 1> ODR10; /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 11, 1> ODR11; /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 12, 1> ODR12; /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 13, 1> ODR13; /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 14, 1> ODR14; /*!< Port output data (y = 0..15) */
            microhal::Bitfield<uint32_t, 15, 1> ODR15; /*!< Port output data (y = 0..15) */
        };

        operator uint32_t() const { return raw; }

        ODR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }

     private:
        uint32_t raw;
        friend class VolatileRegister<ODR, AccessType::ReadOnly>;
        friend class VolatileRegister<ODR, AccessType::WriteOnly>;
        friend class VolatileRegister<ODR, AccessType::ReadWrite>;
    };

    // GPIO port bit set/reset register
    union BSRR {
        union {
            microhal::Bitfield<uint32_t, 0, 1>
                BS0; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 1, 1>
                BS1; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 2, 1>
                BS2; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 3, 1>
                BS3; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 4, 1>
                BS4; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 5, 1>
                BS5; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 6, 1>
                BS6; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 7, 1>
                BS7; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 8, 1>
                BS8; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 9, 1>
                BS9; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 10, 1>
                BS10; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 11, 1>
                BS11; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 12, 1>
                BS12; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 13, 1>
                BS13; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 14, 1>
                BS14; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 15, 1>
                BS15; /*!< Port x set bit y (y= 0..15) These bits are write-only. A read to these bits returns the value 0x0000. */
            microhal::Bitfield<uint32_t, 16, 1> BR0;  /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the  value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 17, 1> BR1;  /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the  value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 18, 1> BR2;  /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the  value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 19, 1> BR3;  /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the  value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 20, 1> BR4;  /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the  value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 21, 1> BR5;  /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the  value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 22, 1> BR6;  /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the  value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 23, 1> BR7;  /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the  value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 24, 1> BR8;  /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the  value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 25, 1> BR9;  /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the  value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 26, 1> BR10; /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 27, 1> BR11; /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 28, 1> BR12; /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 29, 1> BR13; /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 30, 1> BR14; /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 31, 1> BR15; /*!< Port x reset bit y (y = 0..15) These bits are write-only. A read to these bits
                                                         returns the value 0x0000. Note: If both BSx and BRx are set, BSx has priority. */
            microhal::Bitfield<uint32_t, 0, 16> BS;   /*!< Set bits */
            microhal::Bitfield<uint32_t, 16, 16> BR;  /*!< Reset bits */
        };

        operator uint32_t() const { return raw; }

        BSRR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }

     private:
        uint32_t raw;
        friend class VolatileRegister<BSRR, AccessType::ReadOnly>;
        friend class VolatileRegister<BSRR, AccessType::WriteOnly>;
        friend class VolatileRegister<BSRR, AccessType::ReadWrite>;
    };

    // This register is used to lock the configuration of the port bits when a correct write sequence is applied to bit 16 (LCKK). The value
    // of bits [15:0] is used to lock the configuration of the GPIO. During the write sequence, the value of LCKR[15:0] must not change. When
    // the LOCK sequence has been applied on a port bit, the value of this port bit can no longer be modified until the next MCU reset or
    // peripheral reset.A specific write sequence is used to write to the GPIOx_LCKR register. Only word access (32-bit long) is allowed
    // during this locking sequence.Each lock bit freezes a specific configuration register (control and alternate function registers).
    union LCKR {
        union {
            microhal::Bitfield<uint32_t, 0, 1>
                LCK0; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 1, 1>
                LCK1; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 2, 1>
                LCK2; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 3, 1>
                LCK3; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 4, 1>
                LCK4; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 5, 1>
                LCK5; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 6, 1>
                LCK6; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 7, 1>
                LCK7; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 8, 1>
                LCK8; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 9, 1>
                LCK9; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 10, 1>
                LCK10; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 11, 1>
                LCK11; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 12, 1>
                LCK12; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 13, 1>
                LCK13; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 14, 1>
                LCK14; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 15, 1>
                LCK15; /*!< Port x lock bit y (y= 0..15) These bits are read/write but can only be written when the LCKK bit is 0. */
            microhal::Bitfield<uint32_t, 16, 1>
                LCKK; /*!< Lock key This bit can be read any time. It can only be modified using the lock key write sequence. LOCK key write
                         sequence: WR LCKR[16] = 1 + LCKR[15:0] WR LCKR[16] = 0 + LCKR[15:0] WR LCKR[16] = 1 + LCKR[15:0] RD LCKR RD LCKR[16]
                         = 1 (this read operation is optional but it confirms that the lock is active) Note: During the LOCK key write
                         sequence, the value of LCK[15:0] must not change. Any error in the lock sequence aborts the lock. After the first
                         lock sequence on any bit of the port, any read access on the LCKK bit will return 1 until the next MCU reset or
                         peripheral reset. */
        };

        operator uint32_t() const { return raw; }

        LCKR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }

     private:
        uint32_t raw;
        friend class VolatileRegister<LCKR, AccessType::ReadOnly>;
        friend class VolatileRegister<LCKR, AccessType::WriteOnly>;
        friend class VolatileRegister<LCKR, AccessType::ReadWrite>;
    };

    // GPIO alternate function low register
    union AFR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> AFRL0_8;   /*!< Alternate function selection for port x bit y (y = 0..7) */
            microhal::Bitfield<uint32_t, 4, 4> AFRL1_9;   /*!< Alternate function selection for port x bit y (y = 0..7) */
            microhal::Bitfield<uint32_t, 8, 4> AFRL2_10;  /*!< Alternate function selection for port x bit y (y = 0..7) */
            microhal::Bitfield<uint32_t, 12, 4> AFRL3_11; /*!< Alternate function selection for port x bit y (y = 0..7) */
            microhal::Bitfield<uint32_t, 16, 4> AFRL4_12; /*!< Alternate function selection for port x bit y (y = 0..7) */
            microhal::Bitfield<uint32_t, 20, 4> AFRL5_13; /*!< Alternate function selection for port x bit y (y = 0..7) */
            microhal::Bitfield<uint32_t, 24, 4> AFRL6_14; /*!< Alternate function selection for port x bit y (y = 0..7) */
            microhal::Bitfield<uint32_t, 28, 4> AFRL7_15; /*!< Alternate function selection for port x bit y (y = 0..7) */
        };

        operator uint32_t() const { return raw; }

        AFR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        AFR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        AFR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }

     private:
        uint32_t raw;
        friend class VolatileRegister<AFR, AccessType::ReadOnly>;
        friend class VolatileRegister<AFR, AccessType::WriteOnly>;
        friend class VolatileRegister<AFR, AccessType::ReadWrite>;
    };

#ifdef _MICROHAL_GPIO_REGISTER_HAS_BRR
    // GPIO port bit reset register
    union BRR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> BR0;   /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 1, 1> BR1;   /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 2, 1> BR2;   /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 3, 1> BR3;   /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 4, 1> BR4;   /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 5, 1> BR5;   /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 6, 1> BR6;   /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 7, 1> BR7;   /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 8, 1> BR8;   /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 9, 1> BR9;   /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 10, 1> BR10; /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 11, 1> BR11; /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 12, 1> BR12; /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 13, 1> BR13; /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 14, 1> BR14; /*!< Port Reset bit */
            microhal::Bitfield<uint32_t, 15, 1> BR15; /*!< Port Reset bit */
        };

        operator uint32_t() const { return raw; }

        BRR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }

     private:
        uint32_t raw;
        friend class VolatileRegister<BRR, AccessType::ReadOnly>;
        friend class VolatileRegister<BRR, AccessType::WriteOnly>;
        friend class VolatileRegister<BRR, AccessType::ReadWrite>;
    };
#endif

#ifdef _MICROHAL_GPIO_REGISTER_HAS_ASCR
    // GPIO port analog switch control register
    union ASCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ASC0;   /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 1, 1> ASC1;   /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 2, 1> ASC2;   /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 3, 1> ASC3;   /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 4, 1> ASC4;   /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 5, 1> ASC5;   /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 6, 1> ASC6;   /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 7, 1> ASC7;   /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 8, 1> ASC8;   /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 9, 1> ASC9;   /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 10, 1> ASC10; /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 11, 1> ASC11; /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 12, 1> ASC12; /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 13, 1> ASC13; /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 14, 1> ASC14; /*!< Port analog switch control */
            microhal::Bitfield<uint32_t, 15, 1> ASC15; /*!< Port analog switch control */
        };

        operator uint32_t() const { return raw; }

        ASCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }

     private:
        uint32_t raw;
        friend class VolatileRegister<ASCR, AccessType::ReadOnly>;
        friend class VolatileRegister<ASCR, AccessType::WriteOnly>;
        friend class VolatileRegister<ASCR, AccessType::ReadWrite>;
    };
#endif

    VolatileRegister<MODER, AccessType::ReadWrite> moder;     /*!< GPIO port mode register                  Address offset: 0x0 */
    VolatileRegister<OTYPER, AccessType::ReadWrite> otyper;   /*!< GPIO port output type register           Address offset: 0x4 */
    VolatileRegister<OSPEEDR, AccessType::ReadWrite> ospeedr; /*!< GPIO port output speed register          Address offset: 0x8 */
    VolatileRegister<PUPDR, AccessType::ReadWrite> pupdr;     /*!< GPIO port pull-up/pull-down register     Address offset: 0xC */
    VolatileRegister<IDR, AccessType::ReadWrite> idr;         /*!< GPIO port input data register            Address offset: 0x10 */
    VolatileRegister<ODR, AccessType::ReadWrite> odr;         /*!< GPIO port output data register           Address offset: 0x14 */
    VolatileRegister<BSRR, AccessType::ReadWrite> bsrr;       /*!< GPIO port bit set/reset register         Address offset: 0x18 */
    VolatileRegister<LCKR, AccessType::ReadWrite> lckr;       /*!< GPIO port configuration lock register    Address offset: 0x1C */
    VolatileRegister<AFR, AccessType::ReadWrite> afr[2];      /*!< GPIO alternate function low register     Address offset: 0x20 */
    // VolatileRegister<AFRH, AccessType::ReadWrite> afrh;       /*!< GPIO alternate function high register    Address offset: 0x24 */
#ifdef _MICROHAL_GPIO_REGISTER_HAS_BRR
    VolatileRegister<BRR, AccessType::ReadWrite> brr; /*!< GPIO port bit reset register                     Address offset: 0x28 */
#endif
#ifdef _MICROHAL_GPIO_REGISTER_HAS_ASCR
    VolatileRegister<ASCR, AccessType::ReadWrite> ascr; /*!< GPIO port analog switch control register       Address offset: 0x2C */
#endif
};

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_GPIO
