#ifndef _MICROHAL_PERIPHERAL_REGISTER_GPIO_V2_H_
#define _MICROHAL_PERIPHERAL_REGISTER_GPIO_V2_H_

#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32F102, STM32F107, STM32F103, STM32F101, STM32F100

namespace microhal {
namespace registers {
/**
 *
 */
struct GPIO {
    // Port configuration register low (GPIOn_CRL)
    union CRL {
        union {
            microhal::Bitfield<uint32_t, 0, 2> MODE0;  /*!< Port n.0 mode bits */
            microhal::Bitfield<uint32_t, 2, 2> CNF0;   /*!< Port n.0 configuration bits */
            microhal::Bitfield<uint32_t, 4, 2> MODE1;  /*!< Port n.1 mode bits */
            microhal::Bitfield<uint32_t, 6, 2> CNF1;   /*!< Port n.1 configuration bits */
            microhal::Bitfield<uint32_t, 8, 2> MODE2;  /*!< Port n.2 mode bits */
            microhal::Bitfield<uint32_t, 10, 2> CNF2;  /*!< Port n.2 configuration bits */
            microhal::Bitfield<uint32_t, 12, 2> MODE3; /*!< Port n.3 mode bits */
            microhal::Bitfield<uint32_t, 14, 2> CNF3;  /*!< Port n.3 configuration bits */
            microhal::Bitfield<uint32_t, 16, 2> MODE4; /*!< Port n.4 mode bits */
            microhal::Bitfield<uint32_t, 18, 2> CNF4;  /*!< Port n.4 configuration bits */
            microhal::Bitfield<uint32_t, 20, 2> MODE5; /*!< Port n.5 mode bits */
            microhal::Bitfield<uint32_t, 22, 2> CNF5;  /*!< Port n.5 configuration bits */
            microhal::Bitfield<uint32_t, 24, 2> MODE6; /*!< Port n.6 mode bits */
            microhal::Bitfield<uint32_t, 26, 2> CNF6;  /*!< Port n.6 configuration bits */
            microhal::Bitfield<uint32_t, 28, 2> MODE7; /*!< Port n.7 mode bits */
            microhal::Bitfield<uint32_t, 30, 2> CNF7;  /*!< Port n.7 configuration bits */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CRL &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CRL &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CRL &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CRL operator&(uint32_t value) const {
            CRL tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CRL operator|(uint32_t value) const {
            CRL tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CRL, AccessType::ReadOnly>;
        friend class VolatileRegister<CRL, AccessType::WriteOnly>;
        friend class VolatileRegister<CRL, AccessType::ReadWrite>;
    };

    // Port configuration register high (GPIOn_CRL)
    union CRH {
        union {
            microhal::Bitfield<uint32_t, 0, 2> MODE8;   /*!< Port n.8 mode bits */
            microhal::Bitfield<uint32_t, 2, 2> CNF8;    /*!< Port n.8 configuration bits */
            microhal::Bitfield<uint32_t, 4, 2> MODE9;   /*!< Port n.9 mode bits */
            microhal::Bitfield<uint32_t, 6, 2> CNF9;    /*!< Port n.9 configuration bits */
            microhal::Bitfield<uint32_t, 8, 2> MODE10;  /*!< Port n.10 mode bits */
            microhal::Bitfield<uint32_t, 10, 2> CNF10;  /*!< Port n.10 configuration bits */
            microhal::Bitfield<uint32_t, 12, 2> MODE11; /*!< Port n.11 mode bits */
            microhal::Bitfield<uint32_t, 14, 2> CNF11;  /*!< Port n.11 configuration bits */
            microhal::Bitfield<uint32_t, 16, 2> MODE12; /*!< Port n.12 mode bits */
            microhal::Bitfield<uint32_t, 18, 2> CNF12;  /*!< Port n.12 configuration bits */
            microhal::Bitfield<uint32_t, 20, 2> MODE13; /*!< Port n.13 mode bits */
            microhal::Bitfield<uint32_t, 22, 2> CNF13;  /*!< Port n.13 configuration bits */
            microhal::Bitfield<uint32_t, 24, 2> MODE14; /*!< Port n.14 mode bits */
            microhal::Bitfield<uint32_t, 26, 2> CNF14;  /*!< Port n.14 configuration bits */
            microhal::Bitfield<uint32_t, 28, 2> MODE15; /*!< Port n.15 mode bits */
            microhal::Bitfield<uint32_t, 30, 2> CNF15;  /*!< Port n.15 configuration bits */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CRH &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CRH &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CRH &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CRH operator&(uint32_t value) const {
            CRH tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CRH operator|(uint32_t value) const {
            CRH tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CRH, AccessType::ReadOnly>;
        friend class VolatileRegister<CRH, AccessType::WriteOnly>;
        friend class VolatileRegister<CRH, AccessType::ReadWrite>;
    };

    // Port input data register (GPIOn_IDR)
    union IDR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> IDR0;   /*!< Port input data */
            microhal::Bitfield<uint32_t, 1, 1> IDR1;   /*!< Port input data */
            microhal::Bitfield<uint32_t, 2, 1> IDR2;   /*!< Port input data */
            microhal::Bitfield<uint32_t, 3, 1> IDR3;   /*!< Port input data */
            microhal::Bitfield<uint32_t, 4, 1> IDR4;   /*!< Port input data */
            microhal::Bitfield<uint32_t, 5, 1> IDR5;   /*!< Port input data */
            microhal::Bitfield<uint32_t, 6, 1> IDR6;   /*!< Port input data */
            microhal::Bitfield<uint32_t, 7, 1> IDR7;   /*!< Port input data */
            microhal::Bitfield<uint32_t, 8, 1> IDR8;   /*!< Port input data */
            microhal::Bitfield<uint32_t, 9, 1> IDR9;   /*!< Port input data */
            microhal::Bitfield<uint32_t, 10, 1> IDR10; /*!< Port input data */
            microhal::Bitfield<uint32_t, 11, 1> IDR11; /*!< Port input data */
            microhal::Bitfield<uint32_t, 12, 1> IDR12; /*!< Port input data */
            microhal::Bitfield<uint32_t, 13, 1> IDR13; /*!< Port input data */
            microhal::Bitfield<uint32_t, 14, 1> IDR14; /*!< Port input data */
            microhal::Bitfield<uint32_t, 15, 1> IDR15; /*!< Port input data */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IDR operator&(uint32_t value) const {
            IDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IDR operator|(uint32_t value) const {
            IDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IDR, AccessType::ReadOnly>;
        friend class VolatileRegister<IDR, AccessType::WriteOnly>;
        friend class VolatileRegister<IDR, AccessType::ReadWrite>;
    };

    // Port output data register (GPIOn_ODR)
    union ODR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ODR0;   /*!< Port output data */
            microhal::Bitfield<uint32_t, 1, 1> ODR1;   /*!< Port output data */
            microhal::Bitfield<uint32_t, 2, 1> ODR2;   /*!< Port output data */
            microhal::Bitfield<uint32_t, 3, 1> ODR3;   /*!< Port output data */
            microhal::Bitfield<uint32_t, 4, 1> ODR4;   /*!< Port output data */
            microhal::Bitfield<uint32_t, 5, 1> ODR5;   /*!< Port output data */
            microhal::Bitfield<uint32_t, 6, 1> ODR6;   /*!< Port output data */
            microhal::Bitfield<uint32_t, 7, 1> ODR7;   /*!< Port output data */
            microhal::Bitfield<uint32_t, 8, 1> ODR8;   /*!< Port output data */
            microhal::Bitfield<uint32_t, 9, 1> ODR9;   /*!< Port output data */
            microhal::Bitfield<uint32_t, 10, 1> ODR10; /*!< Port output data */
            microhal::Bitfield<uint32_t, 11, 1> ODR11; /*!< Port output data */
            microhal::Bitfield<uint32_t, 12, 1> ODR12; /*!< Port output data */
            microhal::Bitfield<uint32_t, 13, 1> ODR13; /*!< Port output data */
            microhal::Bitfield<uint32_t, 14, 1> ODR14; /*!< Port output data */
            microhal::Bitfield<uint32_t, 15, 1> ODR15; /*!< Port output data */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ODR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ODR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ODR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ODR operator&(uint32_t value) const {
            ODR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ODR operator|(uint32_t value) const {
            ODR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ODR, AccessType::ReadOnly>;
        friend class VolatileRegister<ODR, AccessType::WriteOnly>;
        friend class VolatileRegister<ODR, AccessType::ReadWrite>;
    };

    // Port bit set/reset register (GPIOn_BSRR)
    union BSRR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> BS0;   /*!< Set bit 0 */
            microhal::Bitfield<uint32_t, 1, 1> BS1;   /*!< Set bit 1 */
            microhal::Bitfield<uint32_t, 2, 1> BS2;   /*!< Set bit 1 */
            microhal::Bitfield<uint32_t, 3, 1> BS3;   /*!< Set bit 3 */
            microhal::Bitfield<uint32_t, 4, 1> BS4;   /*!< Set bit 4 */
            microhal::Bitfield<uint32_t, 5, 1> BS5;   /*!< Set bit 5 */
            microhal::Bitfield<uint32_t, 6, 1> BS6;   /*!< Set bit 6 */
            microhal::Bitfield<uint32_t, 7, 1> BS7;   /*!< Set bit 7 */
            microhal::Bitfield<uint32_t, 8, 1> BS8;   /*!< Set bit 8 */
            microhal::Bitfield<uint32_t, 9, 1> BS9;   /*!< Set bit 9 */
            microhal::Bitfield<uint32_t, 10, 1> BS10; /*!< Set bit 10 */
            microhal::Bitfield<uint32_t, 11, 1> BS11; /*!< Set bit 11 */
            microhal::Bitfield<uint32_t, 12, 1> BS12; /*!< Set bit 12 */
            microhal::Bitfield<uint32_t, 13, 1> BS13; /*!< Set bit 13 */
            microhal::Bitfield<uint32_t, 14, 1> BS14; /*!< Set bit 14 */
            microhal::Bitfield<uint32_t, 15, 1> BS15; /*!< Set bit 15 */
            microhal::Bitfield<uint32_t, 16, 1> BR0;  /*!< Reset bit 0 */
            microhal::Bitfield<uint32_t, 17, 1> BR1;  /*!< Reset bit 1 */
            microhal::Bitfield<uint32_t, 18, 1> BR2;  /*!< Reset bit 2 */
            microhal::Bitfield<uint32_t, 19, 1> BR3;  /*!< Reset bit 3 */
            microhal::Bitfield<uint32_t, 20, 1> BR4;  /*!< Reset bit 4 */
            microhal::Bitfield<uint32_t, 21, 1> BR5;  /*!< Reset bit 5 */
            microhal::Bitfield<uint32_t, 22, 1> BR6;  /*!< Reset bit 6 */
            microhal::Bitfield<uint32_t, 23, 1> BR7;  /*!< Reset bit 7 */
            microhal::Bitfield<uint32_t, 24, 1> BR8;  /*!< Reset bit 8 */
            microhal::Bitfield<uint32_t, 25, 1> BR9;  /*!< Reset bit 9 */
            microhal::Bitfield<uint32_t, 26, 1> BR10; /*!< Reset bit 10 */
            microhal::Bitfield<uint32_t, 27, 1> BR11; /*!< Reset bit 11 */
            microhal::Bitfield<uint32_t, 28, 1> BR12; /*!< Reset bit 12 */
            microhal::Bitfield<uint32_t, 29, 1> BR13; /*!< Reset bit 13 */
            microhal::Bitfield<uint32_t, 30, 1> BR14; /*!< Reset bit 14 */
            microhal::Bitfield<uint32_t, 31, 1> BR15; /*!< Reset bit 15 */
            microhal::Bitfield<uint32_t, 0, 16> BSR;  /*!< Set bits */
            microhal::Bitfield<uint32_t, 16, 16> BR;  /*!< Reset bits */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        BSRR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        BSRR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        BSRR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        BSRR operator&(uint32_t value) const {
            BSRR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        BSRR operator|(uint32_t value) const {
            BSRR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<BSRR, AccessType::ReadOnly>;
        friend class VolatileRegister<BSRR, AccessType::WriteOnly>;
        friend class VolatileRegister<BSRR, AccessType::ReadWrite>;
    };

    // Port bit reset register (GPIOn_BRR)
    union BRR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> BR0;   /*!< Reset bit 0 */
            microhal::Bitfield<uint32_t, 1, 1> BR1;   /*!< Reset bit 1 */
            microhal::Bitfield<uint32_t, 2, 1> BR2;   /*!< Reset bit 1 */
            microhal::Bitfield<uint32_t, 3, 1> BR3;   /*!< Reset bit 3 */
            microhal::Bitfield<uint32_t, 4, 1> BR4;   /*!< Reset bit 4 */
            microhal::Bitfield<uint32_t, 5, 1> BR5;   /*!< Reset bit 5 */
            microhal::Bitfield<uint32_t, 6, 1> BR6;   /*!< Reset bit 6 */
            microhal::Bitfield<uint32_t, 7, 1> BR7;   /*!< Reset bit 7 */
            microhal::Bitfield<uint32_t, 8, 1> BR8;   /*!< Reset bit 8 */
            microhal::Bitfield<uint32_t, 9, 1> BR9;   /*!< Reset bit 9 */
            microhal::Bitfield<uint32_t, 10, 1> BR10; /*!< Reset bit 10 */
            microhal::Bitfield<uint32_t, 11, 1> BR11; /*!< Reset bit 11 */
            microhal::Bitfield<uint32_t, 12, 1> BR12; /*!< Reset bit 12 */
            microhal::Bitfield<uint32_t, 13, 1> BR13; /*!< Reset bit 13 */
            microhal::Bitfield<uint32_t, 14, 1> BR14; /*!< Reset bit 14 */
            microhal::Bitfield<uint32_t, 15, 1> BR15; /*!< Reset bit 15 */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        BRR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        BRR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        BRR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        BRR operator&(uint32_t value) const {
            BRR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        BRR operator|(uint32_t value) const {
            BRR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<BRR, AccessType::ReadOnly>;
        friend class VolatileRegister<BRR, AccessType::WriteOnly>;
        friend class VolatileRegister<BRR, AccessType::ReadWrite>;
    };

    // Port configuration lock register
    union LCKR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> LCK0;   /*!< Port A Lock bit 0 */
            microhal::Bitfield<uint32_t, 1, 1> LCK1;   /*!< Port A Lock bit 1 */
            microhal::Bitfield<uint32_t, 2, 1> LCK2;   /*!< Port A Lock bit 2 */
            microhal::Bitfield<uint32_t, 3, 1> LCK3;   /*!< Port A Lock bit 3 */
            microhal::Bitfield<uint32_t, 4, 1> LCK4;   /*!< Port A Lock bit 4 */
            microhal::Bitfield<uint32_t, 5, 1> LCK5;   /*!< Port A Lock bit 5 */
            microhal::Bitfield<uint32_t, 6, 1> LCK6;   /*!< Port A Lock bit 6 */
            microhal::Bitfield<uint32_t, 7, 1> LCK7;   /*!< Port A Lock bit 7 */
            microhal::Bitfield<uint32_t, 8, 1> LCK8;   /*!< Port A Lock bit 8 */
            microhal::Bitfield<uint32_t, 9, 1> LCK9;   /*!< Port A Lock bit 9 */
            microhal::Bitfield<uint32_t, 10, 1> LCK10; /*!< Port A Lock bit 10 */
            microhal::Bitfield<uint32_t, 11, 1> LCK11; /*!< Port A Lock bit 11 */
            microhal::Bitfield<uint32_t, 12, 1> LCK12; /*!< Port A Lock bit 12 */
            microhal::Bitfield<uint32_t, 13, 1> LCK13; /*!< Port A Lock bit 13 */
            microhal::Bitfield<uint32_t, 14, 1> LCK14; /*!< Port A Lock bit 14 */
            microhal::Bitfield<uint32_t, 15, 1> LCK15; /*!< Port A Lock bit 15 */
            microhal::Bitfield<uint32_t, 16, 1> LCKK;  /*!< Lock key */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        LCKR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        LCKR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        LCKR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        LCKR operator&(uint32_t value) const {
            LCKR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        LCKR operator|(uint32_t value) const {
            LCKR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<LCKR, AccessType::ReadOnly>;
        friend class VolatileRegister<LCKR, AccessType::WriteOnly>;
        friend class VolatileRegister<LCKR, AccessType::ReadWrite>;
    };

    VolatileRegister<CRL, AccessType::ReadWrite> crl;   /*!< Port configuration register low (GPIOn_CRL)	Address offset: 0x0 */
    VolatileRegister<CRH, AccessType::ReadWrite> crh;   /*!< Port configuration register high (GPIOn_CRL)	Address offset: 0x4 */
    VolatileRegister<IDR, AccessType::ReadWrite> idr;   /*!< Port input data register (GPIOn_IDR)	Address offset: 0x8 */
    VolatileRegister<ODR, AccessType::ReadWrite> odr;   /*!< Port output data register (GPIOn_ODR)	Address offset: 0xC */
    VolatileRegister<BSRR, AccessType::ReadWrite> bsrr; /*!< Port bit set/reset register (GPIOn_BSRR)	Address offset: 0x10 */
    VolatileRegister<BRR, AccessType::ReadWrite> brr;   /*!< Port bit reset register (GPIOn_BRR)	Address offset: 0x14 */
    VolatileRegister<LCKR, AccessType::ReadWrite> lckr; /*!< Port configuration lock register	Address offset: 0x18 */
};
}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_GPIO_V2_H_
