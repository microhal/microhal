#ifndef _MICROHAL_PERIPHERAL_REGISTER_USART
#define _MICROHAL_PERIPHERAL_REGISTER_USART

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include "registersBaseAddressDefinitions.h"

// Supported MCU version 1: STM32L4x6, STM32L4x3, STM32L4x1, STM32L4x2, STM32L4x5, STM32F7x9, STM32F765, STM32F7x8, STM32F745, STM32F750, STM32F7x7,
//                          STM32F7x6, STM32L0x3, STM32L0x2, STM32L0x1, STM32L4R7, STM32L4S9, STM32L4R9, STM32L4S5, STM32L4S7, STM32L4R5
// Supported MCU version 2: STM32L4x1, STM32L4x2
// Supported MCU version 3: STM32F7x3, STM32F7x2, STM32F730
// Supported MCU version 4: STM32F0x2, STM32F0x8, STM32F0x1, STM32F0x0
// Supported MCU version 5: STM32F302, STM32F303, STM32F3x4, STM32F301, STM32F3x8, STM32F373
// Supported MCU version 6: STM32H742x, STM32H7x3, STM32H743x, STM32H7x7_CM4, STM32H753x, STM32H7x5_CM7, STM32H7x5_CM4, STM32H750x, STM32H7x7_CM7,
//                          STM32G474xx, STM32G484xx, STM32G471xx, STM32G431xx, STM32G473xx, STM32G441xx, STM32GBK1CBT6, STM32G483xx

#if defined(STM32L4x6) || defined(STM32L4x3) || defined(STM32L4x1) || defined(STM32L4x2) || defined(STM32L4x5) || defined(STM32F7x9) || \
    defined(STM32F765) || defined(STM32F7x8) || defined(STM32F745) || defined(STM32F750) || defined(STM32F7x7) || defined(STM32F7x6) || \
    defined(STM32L0x3) || defined(STM32L0x2) || defined(STM32L0x1) || defined(STM32L4R7) || defined(STM32L4S9) || defined(STM32L4R9) || \
    defined(STM32L4S5) || defined(STM32L4S7) || defined(STM32L4R5)  // version 1
#define _MICROHAL_REGISTERS_USART_CR1_HAS_UCSM
#define _MICROHAL_REGISTERS_USART_CR3_HAS_WUS_WUFIE
#define _MICROHAL_REGISTERS_USART_ISR_HAS_RWU_WUF
#define _MICROHAL_REGISTERS_USART_ISR_HAS_REACK
#define _MICROHAL_REGISTERS_USART_ICR_HAS_WUCF
#define _MICROHAL_REGISTERS_USART_CR1_HAS_M1
#endif

#if defined(STM32L4x1) || defined(STM32L4x2)  // version 2
#define _MICROHAL_REGISTERS_USART_CR1_HAS_UCSM
#define _MICROHAL_REGISTERS_USART_CR3_HAS_WUS_WUFIE
#define _MICROHAL_REGISTERS_USART_CR3_HAS_UCESM
#define _MICROHAL_REGISTERS_USART_CR3_HAS_TCBGTIE
#define _MICROHAL_REGISTERS_USART_ISR_HAS_RWU_WUF
#define _MICROHAL_REGISTERS_USART_ISR_HAS_TCBGT
#define _MICROHAL_REGISTERS_USART_BRR_HAS_BRR
#define _MICROHAL_REGISTERS_USART_ISR_HAS_REACK
#define _MICROHAL_REGISTERS_USART_ICR_HAS_WUCF
#define _MICROHAL_REGISTERS_USART_CR1_HAS_M1
#endif

#if defined(STM32F7x3) || defined(STM32F7x2) || defined(STM32F730)  // version 3
#define _MICROHAL_REGISTERS_USART_CR3_HAS_TCBGTIE
#define _MICROHAL_REGISTERS_USART_BRR_HAS_BRR_16
#define _MICROHAL_REGISTERS_USART_ISR_HAS_TCBGT
#define _MICROHAL_REGISTERS_USART_ICR_HAS_TCBGTCF
#define _MICROHAL_REGISTERS_USART_CR1_HAS_M1
#endif

#if defined(STM32F0x2) || defined(STM32F0x8) || defined(STM32F0x1) || defined(STM32F0x0)  // version 4
#define _MICROHAL_REGISTERS_USART_CR1_HAS_UCSM
#define _MICROHAL_REGISTERS_USART_CR3_HAS_WUS_WUFIE
#define _MICROHAL_REGISTERS_USART_ISR_HAS_RWU_WUF
#define _MICROHAL_REGISTERS_USART_ISR_HAS_REACK
#define _MICROHAL_REGISTERS_USART_ICR_HAS_WUCF
#define _MICROHAL_REGISTERS_USART_CR1_HAS_M1
#endif

#if defined(STM32F302) || defined(STM32F303) || defined(STM32F3x4) || defined(STM32F301) || defined(STM32F3x8) || defined(STM32F373)  // version 5
#define _MICROHAL_REGISTERS_USART_CR1_HAS_UCSM
#define _MICROHAL_REGISTERS_USART_CR3_HAS_WUS_WUFIE
#define _MICROHAL_REGISTERS_USART_ISR_HAS_RWU_WUF
#define _MICROHAL_REGISTERS_USART_ISR_HAS_REACK
#define _MICROHAL_REGISTERS_USART_ICR_HAS_WUCF
#endif

#if defined(STM32H742x) || defined(STM32H7x3) || defined(STM32H743x) || defined(STM32H7x7_CM4) || defined(STM32H753x) || defined(STM32H7x5_CM7) || \
    defined(STM32H7x5_CM4) || defined(STM32H750x) || defined(STM32H7x7_CM7) || defined(STM32G474xx) || defined(STM32G484xx) ||                     \
    defined(STM32G471xx) || defined(STM32G431xx) || defined(STM32G473xx) || defined(STM32G441xx) || defined(STM32GBK1CBT6) ||                      \
    defined(STM32G483xx)  // version 6
#define _MICROHAL_REGISTERS_USART_CR1_HAS_UCSM
#define _MICROHAL_REGISTERS_USART_CR1_HAS_M1
#define _MICROHAL_REGISTERS_USART_CR1_HAS_FIFO
#define _MICROHAL_REGISTERS_USART_CR2_HAS_SLVEN_DIS_NSS
#define _MICROHAL_REGISTERS_USART_CR3_HAS_WUS_WUFIE
#define _MICROHAL_REGISTERS_USART_CR3_HAS_TCBGTIE
#define _MICROHAL_REGISTERS_USART_CR3_HAS_FIFO
#define _MICROHAL_REGISTERS_USART_ISR_HAS_UDR
#define _MICROHAL_REGISTERS_USART_ISR_HAS_RWU_WUF
#define _MICROHAL_REGISTERS_USART_ISR_HAS_REACK
#define _MICROHAL_REGISTERS_USART_ISR_HAS_TCBGT
#define _MICROHAL_REGISTERS_USART_ISR_HAS_FIFO
#define _MICROHAL_REGISTERS_USART_ICR_HAS_TXFECF
#define _MICROHAL_REGISTERS_USART_ICR_HAS_TCBGTCF
#define _MICROHAL_REGISTERS_USART_ICR_HAS_UDRCF
#define _MICROHAL_REGISTERS_USART_ICR_HAS_WUCF
#define _MICROHAL_REGISTERS_USART_HAS_PRESC
#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct USART {
    // Control register 1
    union CR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> UE; /*!< USART enable */
#ifdef _MICROHAL_REGISTERS_USART_CR1_HAS_UCSM
            microhal::Bitfield<uint32_t, 1, 1> UESM; /*!< USART enable in Stop mode */
#endif
            microhal::Bitfield<uint32_t, 2, 1> RE;     /*!< Receiver enable */
            microhal::Bitfield<uint32_t, 3, 1> TE;     /*!< Transmitter enable */
            microhal::Bitfield<uint32_t, 4, 1> IDLEIE; /*!< IDLE interrupt enable */
            microhal::Bitfield<uint32_t, 5, 1> RXNEIE; /*!< RXNE interrupt enable */
            microhal::Bitfield<uint32_t, 6, 1> TCIE;   /*!< Transmission complete interrupt enable */
            microhal::Bitfield<uint32_t, 7, 1> TXEIE;  /*!< interrupt enable */
            microhal::Bitfield<uint32_t, 8, 1> PEIE;   /*!< PE interrupt enable */
            microhal::Bitfield<uint32_t, 9, 1> PS;     /*!< Parity selection */
            microhal::Bitfield<uint32_t, 10, 1> PCE;   /*!< Parity control enable */
            microhal::Bitfield<uint32_t, 11, 1> WAKE;  /*!< Receiver wakeup method */
            microhal::Bitfield<uint32_t, 12, 1> M;     /*!< Word length */
            microhal::Bitfield<uint32_t, 13, 1> MME;   /*!< Mute mode enable */
            microhal::Bitfield<uint32_t, 14, 1> CMIE;  /*!< Character match interrupt enable */
            microhal::Bitfield<uint32_t, 15, 1> OVER8; /*!< Oversampling mode */
            microhal::Bitfield<uint32_t, 16, 5> DEDT;  /*!< Driver Enable deassertion time */
            microhal::Bitfield<uint32_t, 21, 5> DEAT;  /*!< Driver Enable assertion time */
            microhal::Bitfield<uint32_t, 26, 1> RTOIE; /*!< Receiver timeout interrupt enable */
            microhal::Bitfield<uint32_t, 27, 1> EOBIE; /*!< End of Block interrupt enable */
#ifdef _MICROHAL_REGISTERS_USART_CR1_HAS_M1
            microhal::Bitfield<uint32_t, 28, 1> M1; /*!< Word length */
#endif
#ifdef _MICROHAL_REGISTERS_USART_CR1_HAS_FIFO
            microhal::Bitfield<uint32_t, 29, 1> FIFOEN; /*!< FIFO mode enable */
            microhal::Bitfield<uint32_t, 30, 1> TXFEIE; /*!< TXFIFO empty interrupt enable */
            microhal::Bitfield<uint32_t, 31, 1> RXFFIE; /*!< RXFIFO Full interrupt enable */
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
#if _MICROHAL_REGISTERS_USART_CR2_HAS_SLVEN_DIS_NSS
            microhal::Bitfield<uint32_t, 0, 1> SLVEN;   /*!< Synchronous Slave mode enable */
            microhal::Bitfield<uint32_t, 3, 1> DIS_NSS; /*!< When the DSI_NSS bit is set, the NSS pin input is ignored */
#endif
            microhal::Bitfield<uint32_t, 4, 1> ADDM7;     /*!< 7-bit Address Detection/4-bit Address Detection */
            microhal::Bitfield<uint32_t, 5, 1> LBDL;      /*!< LIN break detection length */
            microhal::Bitfield<uint32_t, 6, 1> LBDIE;     /*!< LIN break detection interrupt enable */
            microhal::Bitfield<uint32_t, 8, 1> LBCL;      /*!< Last bit clock pulse */
            microhal::Bitfield<uint32_t, 9, 1> CPHA;      /*!< Clock phase */
            microhal::Bitfield<uint32_t, 10, 1> CPOL;     /*!< Clock polarity */
            microhal::Bitfield<uint32_t, 11, 1> CLKEN;    /*!< Clock enable */
            microhal::Bitfield<uint32_t, 12, 2> STOP;     /*!< STOP bits */
            microhal::Bitfield<uint32_t, 14, 1> LINEN;    /*!< LIN mode enable */
            microhal::Bitfield<uint32_t, 15, 1> SWAP;     /*!< Swap TX/RX pins */
            microhal::Bitfield<uint32_t, 16, 1> RXINV;    /*!< RX pin active level inversion */
            microhal::Bitfield<uint32_t, 17, 1> TXINV;    /*!< TX pin active level inversion */
            microhal::Bitfield<uint32_t, 18, 1> DATAINV;  /*!< Binary data inversion */
            microhal::Bitfield<uint32_t, 19, 1> MSBFIRST; /*!< Most significant bit first */
            microhal::Bitfield<uint32_t, 20, 1> ABREN;    /*!< Auto baud rate enable */
            microhal::Bitfield<uint32_t, 21, 2> ABRMOD;   /*!< Auto baud rate mode */
            microhal::Bitfield<uint32_t, 23, 1> RTOEN;    /*!< Receiver timeout enable */
            microhal::Bitfield<uint32_t, 24, 4> ADD0_3;   /*!< Address of the USART node */
            microhal::Bitfield<uint32_t, 28, 4> ADD4_7;   /*!< Address of the USART node */
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
            microhal::Bitfield<uint32_t, 0, 1> EIE;      /*!< Error interrupt enable */
            microhal::Bitfield<uint32_t, 1, 1> IREN;     /*!< IrDA mode enable */
            microhal::Bitfield<uint32_t, 2, 1> IRLP;     /*!< IrDA low-power */
            microhal::Bitfield<uint32_t, 3, 1> HDSEL;    /*!< Half-duplex selection */
            microhal::Bitfield<uint32_t, 4, 1> NACK;     /*!< Smartcard NACK enable */
            microhal::Bitfield<uint32_t, 5, 1> SCEN;     /*!< Smartcard mode enable */
            microhal::Bitfield<uint32_t, 6, 1> DMAR;     /*!< DMA enable receiver */
            microhal::Bitfield<uint32_t, 7, 1> DMAT;     /*!< DMA enable transmitter */
            microhal::Bitfield<uint32_t, 8, 1> RTSE;     /*!< RTS enable */
            microhal::Bitfield<uint32_t, 9, 1> CTSE;     /*!< CTS enable */
            microhal::Bitfield<uint32_t, 10, 1> CTSIE;   /*!< CTS interrupt enable */
            microhal::Bitfield<uint32_t, 11, 1> ONEBIT;  /*!< One sample bit method enable */
            microhal::Bitfield<uint32_t, 12, 1> OVRDIS;  /*!< Overrun Disable */
            microhal::Bitfield<uint32_t, 13, 1> DDRE;    /*!< DMA Disable on Reception Error */
            microhal::Bitfield<uint32_t, 14, 1> DEM;     /*!< Driver enable mode */
            microhal::Bitfield<uint32_t, 15, 1> DEP;     /*!< Driver enable polarity selection */
            microhal::Bitfield<uint32_t, 17, 3> SCARCNT; /*!< Smartcard auto-retry count */
#ifdef _MICROHAL_REGISTERS_USART_CR3_HAS_WUS_WUFIE
            microhal::Bitfield<uint32_t, 20, 2> WUS;   /*!< Wakeup from Stop mode interrupt flag selection */
            microhal::Bitfield<uint32_t, 22, 1> WUFIE; /*!< Wakeup from Stop mode interrupt enable */
#endif
#ifdef _MICROHAL_REGISTERS_USART_CR3_HAS_UCESM
            microhal::Bitfield<uint32_t, 23, 1> UCESM; /*!< USART Clock Enable in Stop mode */
#endif
#ifdef _MICROHAL_REGISTERS_USART_CR3_HAS_FIFO
            microhal::Bitfield<uint32_t, 23, 1> TXFTIE; /*!< TXFIFO threshold interrupt enable */
#endif
#ifdef _MICROHAL_REGISTERS_USART_CR3_HAS_TCBGTIE
            microhal::Bitfield<uint32_t, 24, 1> TCBGTIE; /*!< Transmission complete before guard time interrupt enable */
#endif
#ifdef _MICROHAL_REGISTERS_USART_CR3_HAS_FIFO
            microhal::Bitfield<uint32_t, 25, 3> RXFTCFG; /*!< Receive FIFO threshold configuration */
            microhal::Bitfield<uint32_t, 28, 1> RXFTIE;  /*!< RXFIFO threshold interrupt enable */
            microhal::Bitfield<uint32_t, 29, 3> TXFTCFG; /*!< TXFIFO threshold configuration */
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

    // Baud rate register
    union BRR {
        union {
#ifdef _MICROHAL_REGISTERS_USART_BRR_HAS_BRR
            microhal::Bitfield<uint32_t, 0, 12> BRR; /*!< USARTDIV */
#elif _MICROHAL_REGISTERS_USART_BRR_HAS_BRR_16
            microhal::Bitfield<uint32_t, 0, 16> BRR; /*!< USARTDIV */
#else
            microhal::Bitfield<uint32_t, 0, 4> DIV_Fraction;  /*!< DIV_Fraction */
            microhal::Bitfield<uint32_t, 4, 12> DIV_Mantissa; /*!< DIV_Mantissa */
#endif
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

    // Receiver timeout register
    union RTOR {
        union {
            microhal::Bitfield<uint32_t, 0, 24> RTO;  /*!< Receiver timeout value */
            microhal::Bitfield<uint32_t, 24, 8> BLEN; /*!< Block Length */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RTOR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RTOR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RTOR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RTOR operator&(uint32_t value) const {
            RTOR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RTOR operator|(uint32_t value) const {
            RTOR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RTOR, AccessType::ReadOnly>;
        friend class VolatileRegister<RTOR, AccessType::WriteOnly>;
        friend class VolatileRegister<RTOR, AccessType::ReadWrite>;
    };

    // Request register
    union RQR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> ABRRQ; /*!< Auto baud rate request */
            microhal::Bitfield<uint32_t, 1, 1> SBKRQ; /*!< Send break request */
            microhal::Bitfield<uint32_t, 2, 1> MMRQ;  /*!< Mute mode request */
            microhal::Bitfield<uint32_t, 3, 1> RXFRQ; /*!< Receive data flush request */
            microhal::Bitfield<uint32_t, 4, 1> TXFRQ; /*!< Transmit data flush request */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RQR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RQR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RQR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RQR operator&(uint32_t value) const {
            RQR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RQR operator|(uint32_t value) const {
            RQR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RQR, AccessType::ReadOnly>;
        friend class VolatileRegister<RQR, AccessType::WriteOnly>;
        friend class VolatileRegister<RQR, AccessType::ReadWrite>;
    };

    // Interrupt & status register
    union ISR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PE;    /*!< Parity error */
            microhal::Bitfield<uint32_t, 1, 1> FE;    /*!< Framing error */
            microhal::Bitfield<uint32_t, 2, 1> NF;    /*!< Noise detected flag */
            microhal::Bitfield<uint32_t, 3, 1> ORE;   /*!< Overrun error */
            microhal::Bitfield<uint32_t, 4, 1> IDLE;  /*!< Idle line detected */
            microhal::Bitfield<uint32_t, 5, 1> RXNE;  /*!< Read data register not empty */
            microhal::Bitfield<uint32_t, 6, 1> TC;    /*!< Transmission complete */
            microhal::Bitfield<uint32_t, 7, 1> TXE;   /*!< Transmit data register empty */
            microhal::Bitfield<uint32_t, 8, 1> LBDF;  /*!< LIN break detection flag */
            microhal::Bitfield<uint32_t, 9, 1> CTSIF; /*!< CTS interrupt flag */
            microhal::Bitfield<uint32_t, 10, 1> CTS;  /*!< CTS flag */
            microhal::Bitfield<uint32_t, 11, 1> RTOF; /*!< Receiver timeout */
            microhal::Bitfield<uint32_t, 12, 1> EOBF; /*!< End of block flag */
#ifdef _MICROHAL_REGISTERS_USART_ISR_HAS_UDR
            microhal::Bitfield<uint32_t, 13, 1> UDR; /*!< SPI slave underrun error flag */
#endif
            microhal::Bitfield<uint32_t, 14, 1> ABRE; /*!< Auto baud rate error */
            microhal::Bitfield<uint32_t, 15, 1> ABRF; /*!< Auto baud rate flag */
            microhal::Bitfield<uint32_t, 16, 1> BUSY; /*!< Busy flag */
            microhal::Bitfield<uint32_t, 17, 1> CMF;  /*!< character match flag */
            microhal::Bitfield<uint32_t, 18, 1> SBKF; /*!< Send break flag */
#ifdef _MICROHAL_REGISTERS_USART_ISR_HAS_RWU_WUF
            microhal::Bitfield<uint32_t, 19, 1> RWU; /*!< Receiver wakeup from Mute mode */
            microhal::Bitfield<uint32_t, 20, 1> WUF; /*!< Wakeup from Stop mode flag */
#endif
            microhal::Bitfield<uint32_t, 21, 1> TEACK; /*!< Transmit enable acknowledge flag */
#ifdef _MICROHAL_REGISTERS_USART_ISR_HAS_REACK
            microhal::Bitfield<uint32_t, 22, 1> REACK; /*!< Receive enable acknowledge flag */
#endif
#ifdef _MICROHAL_REGISTERS_USART_ISR_HAS_FIFO
            microhal::Bitfield<uint32_t, 23, 1> TXFE; /*!< TXFIFO Empty */
            microhal::Bitfield<uint32_t, 24, 1> RXFF; /*!< RXFIFO Full */
#endif
#ifdef _MICROHAL_REGISTERS_USART_ISR_HAS_TCBGT
            microhal::Bitfield<uint32_t, 25, 1> TCBGT; /*!< Transmission complete before guard time completion */
#endif
#ifdef _MICROHAL_REGISTERS_USART_ISR_HAS_FIFO
            microhal::Bitfield<uint32_t, 26, 1> RXFT; /*!< RXFIFO threshold flag */
            microhal::Bitfield<uint32_t, 27, 1> TXFT; /*!< TXFIFO threshold flag */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ISR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ISR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ISR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ISR operator&(uint32_t value) const {
            ISR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ISR operator|(uint32_t value) const {
            ISR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ISR, AccessType::ReadOnly>;
        friend class VolatileRegister<ISR, AccessType::WriteOnly>;
        friend class VolatileRegister<ISR, AccessType::ReadWrite>;
    };

    // Interrupt flag clear register
    union ICR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PECF;   /*!< Parity error clear flag */
            microhal::Bitfield<uint32_t, 1, 1> FECF;   /*!< Framing error clear flag */
            microhal::Bitfield<uint32_t, 2, 1> NCF;    /*!< Noise detected clear flag */
            microhal::Bitfield<uint32_t, 3, 1> ORECF;  /*!< Overrun error clear flag */
            microhal::Bitfield<uint32_t, 4, 1> IDLECF; /*!< Idle line detected clear flag */
#ifdef _MICROHAL_REGISTERS_USART_ICR_HAS_TXFECF
            microhal::Bitfield<uint32_t, 5, 1> TXFECF; /*!< TXFIFO empty clear flag */
#endif
            microhal::Bitfield<uint32_t, 6, 1> TCCF; /*!< Transmission complete clear flag */
#ifdef _MICROHAL_REGISTERS_USART_ICR_HAS_TCBGTCF
            microhal::Bitfield<uint32_t, 7, 1> TCBGTCF; /*!< Transmission completed before guard time clear flag */
#endif
            microhal::Bitfield<uint32_t, 8, 1> LBDCF;  /*!< LIN break detection clear flag */
            microhal::Bitfield<uint32_t, 9, 1> CTSCF;  /*!< CTS clear flag */
            microhal::Bitfield<uint32_t, 11, 1> RTOCF; /*!< Receiver timeout clear flag */
            microhal::Bitfield<uint32_t, 12, 1> EOBCF; /*!< End of block clear flag */
#ifdef _MICROHAL_REGISTERS_USART_ICR_HAS_UDRCF
            microhal::Bitfield<uint32_t, 13, 1> UDRCF; /*!< SPI slave underrun clear flag */
#endif
            microhal::Bitfield<uint32_t, 17, 1> CMCF; /*!< Character match clear flag */
#ifdef _MICROHAL_REGISTERS_USART_ICR_HAS_WUCF
            microhal::Bitfield<uint32_t, 20, 1> WUCF; /*!< Wakeup from Stop mode clear flag */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        ICR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        ICR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        ICR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        ICR operator&(uint32_t value) const {
            ICR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        ICR operator|(uint32_t value) const {
            ICR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<ICR, AccessType::ReadOnly>;
        friend class VolatileRegister<ICR, AccessType::WriteOnly>;
        friend class VolatileRegister<ICR, AccessType::ReadWrite>;
    };

    // Receive data register
    union RDR {
        union {
            microhal::Bitfield<uint32_t, 0, 9> rdr; /*!< Receive data value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RDR operator&(uint32_t value) const {
            RDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RDR operator|(uint32_t value) const {
            RDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RDR, AccessType::ReadOnly>;
        friend class VolatileRegister<RDR, AccessType::WriteOnly>;
        friend class VolatileRegister<RDR, AccessType::ReadWrite>;
    };

    // Transmit data register
    union TDR {
        union {
            microhal::Bitfield<uint32_t, 0, 9> tdr; /*!< Transmit data value */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TDR operator&(uint32_t value) const {
            TDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TDR operator|(uint32_t value) const {
            TDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TDR, AccessType::ReadOnly>;
        friend class VolatileRegister<TDR, AccessType::WriteOnly>;
        friend class VolatileRegister<TDR, AccessType::ReadWrite>;
    };

    // USART prescaler register
    union PRESC {
        union {
            microhal::Bitfield<uint32_t, 0, 4> PRESCALER; /*!< Clock prescaler */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PRESC &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PRESC &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PRESC &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PRESC operator&(uint32_t value) const {
            PRESC tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PRESC operator|(uint32_t value) const {
            PRESC tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PRESC, AccessType::ReadOnly>;
        friend class VolatileRegister<PRESC, AccessType::WriteOnly>;
        friend class VolatileRegister<PRESC, AccessType::ReadWrite>;
    };

    VolatileRegister<CR1, AccessType::ReadWrite> cr1;   /*!< Control register 1	Address offset: 0x0 */
    VolatileRegister<CR2, AccessType::ReadWrite> cr2;   /*!< Control register 2	Address offset: 0x4 */
    VolatileRegister<CR3, AccessType::ReadWrite> cr3;   /*!< Control register 3	Address offset: 0x8 */
    VolatileRegister<BRR, AccessType::ReadWrite> brr;   /*!< Baud rate register	Address offset: 0xC */
    VolatileRegister<GTPR, AccessType::ReadWrite> gtpr; /*!< Guard time and prescaler register	Address offset: 0x10 */
    VolatileRegister<RTOR, AccessType::ReadWrite> rtor; /*!< Receiver timeout register	Address offset: 0x14 */
    VolatileRegister<RQR, AccessType::ReadWrite> rqr;   /*!< Request register	Address offset: 0x18 */
    VolatileRegister<ISR, AccessType::ReadWrite> isr;   /*!< Interrupt & status register	Address offset: 0x1C */
    VolatileRegister<ICR, AccessType::ReadWrite> icr;   /*!< Interrupt flag clear register	Address offset: 0x20 */
    VolatileRegister<RDR, AccessType::ReadWrite> rdr;   /*!< Receive data register	Address offset: 0x24 */
    VolatileRegister<TDR, AccessType::ReadWrite> tdr;   /*!< Transmit data register	Address offset: 0x28 */
#ifdef _MICROHAL_REGISTERS_USART_HAS_PRESC
    VolatileRegister<PRESC, AccessType::ReadWrite> presc; /*!< USART prescaler register	Address offset: 0x2C */
#endif
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
