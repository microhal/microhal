#ifndef _MICROHAL_PERIPHERAL_REGISTER_USART
#define _MICROHAL_PERIPHERAL_REGISTER_USART

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include "registersBaseAddressDefinitions.h"

// Supported MCU version 1: STM32L100, STM32L162, STM32L151, STM32F407, STM32F429, STM32F469, STM32F411, STM32F413, STM32F412, STM32F401, STM32F427,
//                          STM32F405, STM32F446, STM32F410, STM32F217, STM32F215
// Supported MCU version 2: STM32F102, STM32F107, STM32F103, STM32F101, STM32F100

#if defined(STM32L100) || defined(STM32L162) || defined(STM32L151) || defined(STM32F407) || defined(STM32F429) || defined(STM32F469) || \
    defined(STM32F411) || defined(STM32F413) || defined(STM32F412) || defined(STM32F401) || defined(STM32F427) || defined(STM32F405) || \
    defined(STM32F446) || defined(STM32F410) || defined(STM32F217) || defined(STM32F215)  // version 1
#define _MICROHAL_REGISTERS_USART_CR1_HAS_OVER8
#define _MICROHAL_REGISTERS_USART_CR3_HAS_ONEBIT
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct USART {
    // Status register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PE;   /*!< Parity error */
            microhal::Bitfield<uint32_t, 1, 1> FE;   /*!< Framing error */
            microhal::Bitfield<uint32_t, 2, 1> NF;   /*!< Noise detected flag */
            microhal::Bitfield<uint32_t, 2, 1> NE;   /*!< Noise error flag, for STM32F102, STM32F107, STM32F103, STM32F101, STM32F100 */
            microhal::Bitfield<uint32_t, 3, 1> ORE;  /*!< Overrun error */
            microhal::Bitfield<uint32_t, 4, 1> IDLE; /*!< IDLE line detected */
            microhal::Bitfield<uint32_t, 5, 1> RXNE; /*!< Read data register not empty */
            microhal::Bitfield<uint32_t, 6, 1> TC;   /*!< Transmission complete */
            microhal::Bitfield<uint32_t, 7, 1> TXE;  /*!< Transmit data register empty */
            microhal::Bitfield<uint32_t, 8, 1> LBD;  /*!< LIN break detection flag */
            microhal::Bitfield<uint32_t, 9, 1> CTS;  /*!< CTS flag */
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

    // Data register
    union DR {
        union {
            microhal::Bitfield<uint32_t, 0, 9> data; /*!< Data value */
        };

        uint32_t get() const { return raw; }

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

    // Baud rate register
    union BRR {
        union {
            microhal::Bitfield<uint32_t, 0, 4> DIV_Fraction;  /*!< fraction of USARTDIV */
            microhal::Bitfield<uint32_t, 4, 12> DIV_Mantissa; /*!< mantissa of USARTDIV */
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

    // Control register 1
    union CR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SBK;    /*!< Send break */
            microhal::Bitfield<uint32_t, 1, 1> RWU;    /*!< Receiver wakeup */
            microhal::Bitfield<uint32_t, 2, 1> RE;     /*!< Receiver enable */
            microhal::Bitfield<uint32_t, 3, 1> TE;     /*!< Transmitter enable */
            microhal::Bitfield<uint32_t, 4, 1> IDLEIE; /*!< IDLE interrupt enable */
            microhal::Bitfield<uint32_t, 5, 1> RXNEIE; /*!< RXNE interrupt enable */
            microhal::Bitfield<uint32_t, 6, 1> TCIE;   /*!< Transmission complete interrupt enable */
            microhal::Bitfield<uint32_t, 7, 1> TXEIE;  /*!< TXE interrupt enable */
            microhal::Bitfield<uint32_t, 8, 1> PEIE;   /*!< PE interrupt enable */
            microhal::Bitfield<uint32_t, 9, 1> PS;     /*!< Parity selection */
            microhal::Bitfield<uint32_t, 10, 1> PCE;   /*!< Parity control enable */
            microhal::Bitfield<uint32_t, 11, 1> WAKE;  /*!< Wakeup method */
            microhal::Bitfield<uint32_t, 12, 1> M;     /*!< Word length */
            microhal::Bitfield<uint32_t, 13, 1> UE;    /*!< USART enable */
#ifdef _MICROHAL_REGISTERS_USART_CR1_HAS_OVER8
            microhal::Bitfield<uint32_t, 15, 1> OVER8; /*!< Oversampling mode */
#endif
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
            microhal::Bitfield<uint32_t, 0, 4> ADD;    /*!< Address of the USART node */
            microhal::Bitfield<uint32_t, 5, 1> LBDL;   /*!< lin break detection length */
            microhal::Bitfield<uint32_t, 6, 1> LBDIE;  /*!< LIN break detection interrupt enable */
            microhal::Bitfield<uint32_t, 8, 1> LBCL;   /*!< Last bit clock pulse */
            microhal::Bitfield<uint32_t, 9, 1> CPHA;   /*!< Clock phase */
            microhal::Bitfield<uint32_t, 10, 1> CPOL;  /*!< Clock polarity */
            microhal::Bitfield<uint32_t, 11, 1> CLKEN; /*!< Clock enable */
            microhal::Bitfield<uint32_t, 12, 2> STOP;  /*!< STOP bits */
            microhal::Bitfield<uint32_t, 14, 1> LINEN; /*!< LIN mode enable */
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

    // Control register 3
    union CR3 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> EIE;    /*!< Error interrupt enable */
            microhal::Bitfield<uint32_t, 1, 1> IREN;   /*!< IrDA mode enable */
            microhal::Bitfield<uint32_t, 2, 1> IRLP;   /*!< IrDA low-power */
            microhal::Bitfield<uint32_t, 3, 1> HDSEL;  /*!< Half-duplex selection */
            microhal::Bitfield<uint32_t, 4, 1> NACK;   /*!< Smartcard NACK enable */
            microhal::Bitfield<uint32_t, 5, 1> SCEN;   /*!< Smartcard mode enable */
            microhal::Bitfield<uint32_t, 6, 1> DMAR;   /*!< DMA enable receiver */
            microhal::Bitfield<uint32_t, 7, 1> DMAT;   /*!< DMA enable transmitter */
            microhal::Bitfield<uint32_t, 8, 1> RTSE;   /*!< RTS enable */
            microhal::Bitfield<uint32_t, 9, 1> CTSE;   /*!< CTS enable */
            microhal::Bitfield<uint32_t, 10, 1> CTSIE; /*!< CTS interrupt enable */
#ifdef _MICROHAL_REGISTERS_USART_CR3_HAS_ONEBIT
            microhal::Bitfield<uint32_t, 11, 1> ONEBIT; /*!< One sample bit method enable */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CR3 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CR3 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CR3 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CR3 operator&(uint32_t value) const {
            CR3 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CR3 operator|(uint32_t value) const {
            CR3 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CR3, AccessType::ReadOnly>;
        friend class VolatileRegister<CR3, AccessType::WriteOnly>;
        friend class VolatileRegister<CR3, AccessType::ReadWrite>;
    };

    // Guard time and prescaler register
    union GTPR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> PSC; /*!< Prescaler value */
            microhal::Bitfield<uint32_t, 8, 8> GT;  /*!< Guard time value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        GTPR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        GTPR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        GTPR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        GTPR operator&(uint32_t value) const {
            GTPR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        GTPR operator|(uint32_t value) const {
            GTPR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<GTPR, AccessType::ReadOnly>;
        friend class VolatileRegister<GTPR, AccessType::WriteOnly>;
        friend class VolatileRegister<GTPR, AccessType::ReadWrite>;
    };

    VolatileRegister<SR, AccessType::ReadWrite> sr;     /*!< Status register	Address offset: 0x0 */
    VolatileRegister<DR, AccessType::ReadWrite> dr;     /*!< Data register	Address offset: 0x4 */
    VolatileRegister<BRR, AccessType::ReadWrite> brr;   /*!< Baud rate register	Address offset: 0x8 */
    VolatileRegister<CR1, AccessType::ReadWrite> cr1;   /*!< Control register 1	Address offset: 0xC */
    VolatileRegister<CR2, AccessType::ReadWrite> cr2;   /*!< Control register 2	Address offset: 0x10 */
    VolatileRegister<CR3, AccessType::ReadWrite> cr3;   /*!< Control register 3	Address offset: 0x14 */
    VolatileRegister<GTPR, AccessType::ReadWrite> gtpr; /*!< Guard time and prescaler register	Address offset: 0x18 */
};

#if defined(_MICROHAL_USART1_BASE_ADDRESS)
[[maybe_unused]] static USART *const usart1 = reinterpret_cast<USART *>(_MICROHAL_USART1_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_USART2_BASE_ADDRESS)
[[maybe_unused]] static USART *const usart2 = reinterpret_cast<USART *>(_MICROHAL_USART2_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_USART3_BASE_ADDRESS)
[[maybe_unused]] static USART *const usart3 = reinterpret_cast<USART *>(_MICROHAL_USART3_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_UART4_BASE_ADDRESS)
[[maybe_unused]] static USART *const uart4 = reinterpret_cast<USART *>(_MICROHAL_UART4_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_UART5_BASE_ADDRESS)
[[maybe_unused]] static USART *const uart5 = reinterpret_cast<USART *>(_MICROHAL_UART5_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_USART6_BASE_ADDRESS)
[[maybe_unused]] static USART *const usart6 = reinterpret_cast<USART *>(_MICROHAL_USART6_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_USART
