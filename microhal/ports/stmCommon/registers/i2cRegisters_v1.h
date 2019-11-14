#ifndef _MICROHAL_PERIPHERAL_REGISTER_I2C
#define _MICROHAL_PERIPHERAL_REGISTER_I2C

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include "registersBaseAddressDefinitions.h"

// Supported MCU version 1: STM32F407, STM32F411, STM32F412, STM32F401, STM32F405, STM32F410, STM32F217, STM32F215, STM32F102, STM32F107, STM32F103,
//                          STM32F101, STM32F100, STM32L100, STM32L162, STM32L151
// Supported MCU version 2: STM32F429, STM32F469, STM32F427, STM32F446, STM32F413

#if defined(STM32F429) || defined(STM32F469) || defined(STM32F427) || defined(STM32F446) || defined(STM32F413)  // version 2
#define _MICROHAL_REGISTERS_I2C_HAS_FLTR
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct I2C {
    // Control register 1
    union CR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PE;        /*!< Peripheral enable */
            microhal::Bitfield<uint32_t, 1, 1> SMBUS;     /*!< SMBus mode */
            microhal::Bitfield<uint32_t, 3, 1> SMBTYPE;   /*!< SMBus type */
            microhal::Bitfield<uint32_t, 4, 1> ENARP;     /*!< ARP enable */
            microhal::Bitfield<uint32_t, 5, 1> ENPEC;     /*!< PEC enable */
            microhal::Bitfield<uint32_t, 6, 1> ENGC;      /*!< General call enable */
            microhal::Bitfield<uint32_t, 7, 1> NOSTRETCH; /*!< Clock stretching disable (Slave mode) */
            microhal::Bitfield<uint32_t, 8, 1> START;     /*!< Start generation */
            microhal::Bitfield<uint32_t, 9, 1> STOP;      /*!< Stop generation */
            microhal::Bitfield<uint32_t, 10, 1> ACK;      /*!< Acknowledge enable */
            microhal::Bitfield<uint32_t, 11, 1> POS;      /*!< Acknowledge/PEC Position (for data reception) */
            microhal::Bitfield<uint32_t, 12, 1> PEC;      /*!< Packet error checking */
            microhal::Bitfield<uint32_t, 13, 1> ALERT;    /*!< SMBus alert */
            microhal::Bitfield<uint32_t, 15, 1> SWRST;    /*!< Software reset */
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

    // Control register 2
    union CR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 6> FREQ;     /*!< Peripheral clock frequency */
            microhal::Bitfield<uint32_t, 8, 1> ITERREN;  /*!< Error interrupt enable */
            microhal::Bitfield<uint32_t, 9, 1> ITEVTEN;  /*!< Event interrupt enable */
            microhal::Bitfield<uint32_t, 10, 1> ITBUFEN; /*!< Buffer interrupt enable */
            microhal::Bitfield<uint32_t, 11, 1> DMAEN;   /*!< DMA requests enable */
            microhal::Bitfield<uint32_t, 12, 1> LAST;    /*!< DMA last transfer */
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

    // Own address register 1
    union OAR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ADD0;     /*!< Interface address */
            microhal::Bitfield<uint32_t, 1, 7> ADD7;     /*!< Interface address */
            microhal::Bitfield<uint32_t, 8, 2> ADD10;    /*!< Interface address */
            microhal::Bitfield<uint32_t, 15, 1> ADDMODE; /*!< Addressing mode (slave mode) */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        OAR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OAR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OAR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        OAR1 operator&(uint32_t value) const {
            OAR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        OAR1 operator|(uint32_t value) const {
            OAR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<OAR1, AccessType::ReadOnly>;
        friend class VolatileRegister<OAR1, AccessType::WriteOnly>;
        friend class VolatileRegister<OAR1, AccessType::ReadWrite>;
    };

    // Own address register 2
    union OAR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ENDUAL; /*!< Dual addressing mode enable */
            microhal::Bitfield<uint32_t, 1, 7> ADD2;   /*!< Interface address */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        OAR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        OAR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        OAR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        OAR2 operator&(uint32_t value) const {
            OAR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        OAR2 operator|(uint32_t value) const {
            OAR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<OAR2, AccessType::ReadOnly>;
        friend class VolatileRegister<OAR2, AccessType::WriteOnly>;
        friend class VolatileRegister<OAR2, AccessType::ReadWrite>;
    };

    // Data register
    union DR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> DATA; /*!< 8-bit data register */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DR operator&(uint32_t value) const {
            DR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DR operator|(uint32_t value) const {
            DR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DR, AccessType::ReadOnly>;
        friend class VolatileRegister<DR, AccessType::WriteOnly>;
        friend class VolatileRegister<DR, AccessType::ReadWrite>;
    };

    // Status register 1
    union SR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SB;        /*!< Start bit (Master mode) */
            microhal::Bitfield<uint32_t, 1, 1> ADDR;      /*!< Address sent (master mode)/matched (slave mode) */
            microhal::Bitfield<uint32_t, 2, 1> BTF;       /*!< Byte transfer finished */
            microhal::Bitfield<uint32_t, 3, 1> ADD10;     /*!< 10-bit header sent (Master mode) */
            microhal::Bitfield<uint32_t, 4, 1> STOPF;     /*!< Stop detection (slave mode) */
            microhal::Bitfield<uint32_t, 6, 1> RxNE;      /*!< Data register not empty (receivers) */
            microhal::Bitfield<uint32_t, 7, 1> TxE;       /*!< Data register empty (transmitters) */
            microhal::Bitfield<uint32_t, 8, 1> BERR;      /*!< Bus error */
            microhal::Bitfield<uint32_t, 9, 1> ARLO;      /*!< Arbitration lost (master mode) */
            microhal::Bitfield<uint32_t, 10, 1> AF;       /*!< Acknowledge failure */
            microhal::Bitfield<uint32_t, 11, 1> OVR;      /*!< Overrun/Underrun */
            microhal::Bitfield<uint32_t, 12, 1> PECERR;   /*!< PEC Error in reception */
            microhal::Bitfield<uint32_t, 14, 1> TIMEOUT;  /*!< Timeout or Tlow error */
            microhal::Bitfield<uint32_t, 15, 1> SMBALERT; /*!< SMBus alert */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SR1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SR1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SR1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SR1 operator&(uint32_t value) const {
            SR1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SR1 operator|(uint32_t value) const {
            SR1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SR1, AccessType::ReadOnly>;
        friend class VolatileRegister<SR1, AccessType::WriteOnly>;
        friend class VolatileRegister<SR1, AccessType::ReadWrite>;
    };

    // Status register 2
    union SR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> MSL;        /*!< Master/slave */
            microhal::Bitfield<uint32_t, 1, 1> BUSY;       /*!< Bus busy */
            microhal::Bitfield<uint32_t, 2, 1> TRA;        /*!< Transmitter/receiver */
            microhal::Bitfield<uint32_t, 4, 1> GENCALL;    /*!< General call address (Slave mode) */
            microhal::Bitfield<uint32_t, 5, 1> SMBDEFAULT; /*!< SMBus device default address (Slave mode) */
            microhal::Bitfield<uint32_t, 6, 1> SMBHOST;    /*!< SMBus host header (Slave mode) */
            microhal::Bitfield<uint32_t, 7, 1> DUALF;      /*!< Dual flag (Slave mode) */
            microhal::Bitfield<uint32_t, 8, 8> PEC;        /*!< acket error checking register */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        SR2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        SR2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        SR2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        SR2 operator&(uint32_t value) const {
            SR2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        SR2 operator|(uint32_t value) const {
            SR2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<SR2, AccessType::ReadOnly>;
        friend class VolatileRegister<SR2, AccessType::WriteOnly>;
        friend class VolatileRegister<SR2, AccessType::ReadWrite>;
    };

    // Clock control register
    union CCR {
        union {
            microhal::Bitfield<uint32_t, 0, 12> ccr;  /*!< Clock control register in Fast/Standard mode (Master mode) */
            microhal::Bitfield<uint32_t, 14, 1> DUTY; /*!< Fast mode duty cycle */
            microhal::Bitfield<uint32_t, 15, 1> F_S;  /*!< I2C master mode selection */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CCR operator&(uint32_t value) const {
            CCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CCR operator|(uint32_t value) const {
            CCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CCR, AccessType::ReadOnly>;
        friend class VolatileRegister<CCR, AccessType::WriteOnly>;
        friend class VolatileRegister<CCR, AccessType::ReadWrite>;
    };

    // TRISE register
    union TRISE {
        union {
            microhal::Bitfield<uint32_t, 0, 6> trise; /*!< Maximum rise time in Fast/Standard mode (Master mode) */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TRISE &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TRISE &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TRISE &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TRISE operator&(uint32_t value) const {
            TRISE tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TRISE operator|(uint32_t value) const {
            TRISE tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TRISE, AccessType::ReadOnly>;
        friend class VolatileRegister<TRISE, AccessType::WriteOnly>;
        friend class VolatileRegister<TRISE, AccessType::ReadWrite>;
    };

    // FLTR register
    union FLTR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> DNF;   /*!< Digital noise filter */
            microhal::Bitfield<uint32_t, 4, 1> ANOFF; /*!< Analog noise filter OFF */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        FLTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        FLTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        FLTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        FLTR operator&(uint32_t value) const {
            FLTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        FLTR operator|(uint32_t value) const {
            FLTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<FLTR, AccessType::ReadOnly>;
        friend class VolatileRegister<FLTR, AccessType::WriteOnly>;
        friend class VolatileRegister<FLTR, AccessType::ReadWrite>;
    };

    VolatileRegister<CR1, AccessType::ReadWrite> cr1;     /*!< Control register 1	Address offset: 0x0 */
    VolatileRegister<CR2, AccessType::ReadWrite> cr2;     /*!< Control register 2	Address offset: 0x4 */
    VolatileRegister<OAR1, AccessType::ReadWrite> oar1;   /*!< Own address register 1	Address offset: 0x8 */
    VolatileRegister<OAR2, AccessType::ReadWrite> oar2;   /*!< Own address register 2	Address offset: 0xC */
    VolatileRegister<DR, AccessType::ReadWrite> dr;       /*!< Data register	Address offset: 0x10 */
    VolatileRegister<SR1, AccessType::ReadWrite> sr1;     /*!< Status register 1	Address offset: 0x14 */
    VolatileRegister<SR2, AccessType::ReadWrite> sr2;     /*!< Status register 2	Address offset: 0x18 */
    VolatileRegister<CCR, AccessType::ReadWrite> ccr;     /*!< Clock control register	Address offset: 0x1C */
    VolatileRegister<TRISE, AccessType::ReadWrite> trise; /*!< TRISE register	Address offset: 0x20 */
#ifdef _MICROHAL_REGISTERS_I2C_HAS_FLTR
    VolatileRegister<FLTR, AccessType::ReadWrite> fltr; /*!< FLTR register	Address offset: 0x24 */
#endif
};

#if defined(_MICROHAL_I2C1_BASE_ADDRESS)
[[maybe_unused]] static I2C *const i2c1 = reinterpret_cast<I2C *>(_MICROHAL_I2C1_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_I2C2_BASE_ADDRESS)
[[maybe_unused]] static I2C *const i2c2 = reinterpret_cast<I2C *>(_MICROHAL_I2C2_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_I2C3_BASE_ADDRESS)
[[maybe_unused]] static I2C *const i2c3 = reinterpret_cast<I2C *>(_MICROHAL_I2C3_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_I2C
