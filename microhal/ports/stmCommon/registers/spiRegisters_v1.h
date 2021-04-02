#ifndef _MICROHAL_PERIPHERAL_REGISTER_SPI
#define _MICROHAL_PERIPHERAL_REGISTER_SPI

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU version 1: STM32F7x9, STM32F765, STM32F765, STM32F7x8, STM32F745, STM32F745, STM32F750, STM32F750, STM32F7x7, STM32F7x6, STM32F7x6,
//                          STM32F765, STM32F745, STM32F7x3, STM32F7x2, STM32F730, STM32G071, STM32G041, STM32G030, STM32G070, STM32G081, STM32G031
// Supported MCU version 2: STM32G474xx, STM32G474xx, STM32G484xx, STM32G484xx, STM32G471xx, STM32G471xx, STM32G431xx, STM32G473xx, STM32G473xx,
//                          STM32G441xx, STM32GBK1CBT6, STM32G483xx, STM32G483xx
// Supported MCU version 3: STM32L100
// Supported MCU version 4: STM32L162, STM32L151, STM32L0x3, STM32L0x2, STM32L0x1, STM32F407, STM32F429, STM32F469, STM32F413, STM32F427, STM32F405,
//                          STM32F446, STM32F410, STM32F217, STM32F215
// Supported MCU version 5: STM32F102, STM32F107, STM32F103, STM32F101
// Supported MCU version 6: STM32F0x2, STM32F0x8, STM32F0x1, STM32F0x0, STM32F3x4
// Supported MCU version 7: STM32F302, STM32F303, STM32F301, STM32F3x8, STM32F373, STM32F3x4
// Supported MCU version 8: STM32L4x6, STM32L4x3, STM32L4x1, STM32L4x2, STM32L4x5, STM32L4R7, STM32L4S9, STM32L4R9, STM32L4S5, STM32L4S7, STM32L4R5
// Supported MCU version 9: STM32F102, STM32F100

#if defined(STM32F7x9) || defined(STM32F765) || defined(STM32F765) || defined(STM32F7x8) || defined(STM32F745) || defined(STM32F745) || \
    defined(STM32F750) || defined(STM32F750) || defined(STM32F7x7) || defined(STM32F7x6) || defined(STM32F7x6) || defined(STM32F765) || \
    defined(STM32F745) || defined(STM32F7x3) || defined(STM32F7x2) || defined(STM32F730) || defined(_MICROHAL_STM32G0XX_STM32G0xx)  // version 1
#define _MICROHAL_REGISTERS_SPI_CR1_HAS_CRCL
#define _MICROHAL_REGISTERS_SPI_SR_HAS_CHSIDE_UDR
#define _MICROHAL_REGISTERS_SPI_I2SCFGR_HAS_ASTRTEN
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_NSSP
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_DS
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_FRXTH
#define _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
#define _MICROHAL_REGISTERS_SPI_HAS_I2S
#endif

#if defined(STM32G474xx) || defined(STM32G474xx) || defined(STM32G484xx) || defined(STM32G484xx) || defined(STM32G471xx) || defined(STM32G471xx) || \
    defined(STM32G431xx) || defined(STM32G473xx) || defined(STM32G473xx) || defined(STM32G441xx) || defined(STM32GBK1CBT6) ||                       \
    defined(STM32G483xx) || defined(STM32G483xx)  // version 2
#define _MICROHAL_REGISTERS_SPI_SR_HAS_TIFRFE
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_NSSP
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_DS
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_FRXTH
#define _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
#define _MICROHAL_REGISTERS_SPI_HAS_I2S
#endif

#if defined(STM32L100)  // version 3
#define _MICROHAL_REGISTERS_SPI_HAS_I2S
#endif

#if defined(STM32L162) || defined(STM32L151) || defined(STM32L0x3) || defined(STM32L0x2) || defined(STM32L0x1) || defined(STM32F407) || \
    defined(STM32F429) || defined(STM32F469) || defined(STM32F413) || defined(STM32F427) || defined(STM32F405) || defined(STM32F446) || \
    defined(STM32F410) || defined(STM32F217) || defined(STM32F215)  // version 4
#define _MICROHAL_REGISTERS_SPI_SR_HAS_CHSIDE_UDR
#define _MICROHAL_REGISTERS_SPI_SR_HAS_TIFRFE
#define _MICROHAL_REGISTERS_SPI_HAS_I2S
#endif

#if defined(STM32F102) || defined(STM32F107) || defined(STM32F103) || defined(STM32F101)  // version 5
#define _MICROHAL_REGISTERS_SPI_SR_HAS_CHSIDE_UDR
#define _MICROHAL_REGISTERS_SPI_HAS_I2S
#endif

#if defined(STM32F0x2) || defined(STM32F0x8) || defined(STM32F0x1) || defined(STM32F0x0) || defined(STM32F3x4)  // version 6
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_NSSP
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_DS
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_FRXTH
#define _MICROHAL_REGISTERS_SPI_SR_HAS_CHSIDE_UDR
#define _MICROHAL_REGISTERS_SPI_SR_HAS_TIFRFE
#define _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
#define _MICROHAL_REGISTERS_SPI_HAS_I2S
#endif

#if defined(STM32F302) || defined(STM32F303) || defined(STM32F301) || defined(STM32F3x8) || defined(STM32F373) || defined(STM32F3x4)  // version 7
#define _MICROHAL_REGISTERS_SPI_CR1_HAS_CRCL
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_NSSP
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_DS
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_FRXTH
#define _MICROHAL_REGISTERS_SPI_SR_HAS_CHSIDE_UDR
#define _MICROHAL_REGISTERS_SPI_SR_HAS_TIFRFE
#define _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
#define _MICROHAL_REGISTERS_SPI_HAS_I2S
#endif

#if defined(STM32L4x6) || defined(STM32L4x3) || defined(STM32L4x1) || defined(STM32L4x2) || defined(STM32L4x5) || defined(STM32L4R7) || \
    defined(STM32L4S9) || defined(STM32L4R9) || defined(STM32L4S5) || defined(STM32L4S7) || defined(STM32L4R5)  // version 8
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_NSSP
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_DS
#define _MICROHAL_REGISTERS_SPI_CR2_HAS_FRXTH
#define _MICROHAL_REGISTERS_SPI_SR_HAS_TIFRFE
#define _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
#endif

#if defined(STM32F102) || defined(STM32F100)  // version 9

#endif

namespace microhal {
namespace registers {
/**
 *
 */
struct SPI {
    // control register 1
    union CR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> CPHA;     /*!< Clock phase */
            microhal::Bitfield<uint32_t, 1, 1> CPOL;     /*!< Clock polarity */
            microhal::Bitfield<uint32_t, 0, 2> mode;     /*!< Clock polarity */
            microhal::Bitfield<uint32_t, 2, 1> MSTR;     /*!< Master selection */
            microhal::Bitfield<uint32_t, 3, 3> BR;       /*!< Baud rate control */
            microhal::Bitfield<uint32_t, 6, 1> SPE;      /*!< SPI enable */
            microhal::Bitfield<uint32_t, 7, 1> LSBFIRST; /*!< Frame format */
            microhal::Bitfield<uint32_t, 8, 1> SSI;      /*!< Internal slave select */
            microhal::Bitfield<uint32_t, 9, 1> SSM;      /*!< Software slave management */
            microhal::Bitfield<uint32_t, 10, 1> RXONLY;  /*!< Receive only */
#ifdef _MICROHAL_REGISTERS_SPI_CR1_HAS_CRCL
            microhal::Bitfield<uint32_t, 11, 1> CRCL; /*!< CRC length */
#else
            microhal::Bitfield<uint32_t, 11, 1> DFF; /*!< Data frame format */
#endif
            microhal::Bitfield<uint32_t, 12, 1> CRCNEXT;  /*!< CRC transfer next */
            microhal::Bitfield<uint32_t, 13, 1> CRCEN;    /*!< Hardware CRC calculation enable */
            microhal::Bitfield<uint32_t, 14, 1> BIDIOE;   /*!< Output enable in bidirectional mode */
            microhal::Bitfield<uint32_t, 15, 1> BIDIMODE; /*!< Bidirectional data mode enable */
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

    // control register 2
    union CR2 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> RXDMAEN; /*!< Rx buffer DMA enable */
            microhal::Bitfield<uint32_t, 1, 1> TXDMAEN; /*!< Tx buffer DMA enable */
            microhal::Bitfield<uint32_t, 2, 1> SSOE;    /*!< SS output enable */
#ifdef _MICROHAL_REGISTERS_SPI_CR2_HAS_NSSP
            microhal::Bitfield<uint32_t, 3, 1> NSSP; /*!< NSS pulse management */
#endif
            microhal::Bitfield<uint32_t, 4, 1> FRF;    /*!< Frame format */
            microhal::Bitfield<uint32_t, 5, 1> ERRIE;  /*!< Error interrupt enable */
            microhal::Bitfield<uint32_t, 6, 1> RXNEIE; /*!< RX buffer not empty interrupt enable */
            microhal::Bitfield<uint32_t, 7, 1> TXEIE;  /*!< Tx buffer empty interrupt enable */
#ifdef _MICROHAL_REGISTERS_SPI_CR2_HAS_DS
            microhal::Bitfield<uint32_t, 8, 4> DS; /*!< Data size */
#endif
#ifdef _MICROHAL_REGISTERS_SPI_CR2_HAS_FRXTH
            microhal::Bitfield<uint32_t, 12, 1> FRXTH;   /*!< FIFO reception threshold */
            microhal::Bitfield<uint32_t, 13, 1> LDMA_RX; /*!< Last DMA transfer for reception */
            microhal::Bitfield<uint32_t, 14, 1> LDMA_TX; /*!< Last DMA transfer for transmission */
#endif
        };

#ifdef _MICROHAL_REGISTERS_SPI_CR2_HAS_DS
        void setDataSize(uint16_t dataSize)
        //[[expects axiom: dataSize >= 4 && dataSize <= 16]]
        {
            DS = dataSize - 1;
        }

        uint16_t getDataSize() { return DS + 1; }
#endif
#ifdef _MICROHAL_REGISTERS_SPI_CR2_HAS_FRXTH
        enum class RxFIFOThreshold : uint32_t { Half = 0, Quater = 1 };
        void setFIFOThreshold(RxFIFOThreshold threshold) { FRXTH = static_cast<uint32_t>(threshold); }
        RxFIFOThreshold getFIFOThreshold() { return static_cast<RxFIFOThreshold>(FRXTH.get()); }
#endif

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

    // status register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> RXNE; /*!< Receive buffer not empty */
            microhal::Bitfield<uint32_t, 1, 1> TXE;  /*!< Transmit buffer empty */
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_CHSIDE_UDR
            microhal::Bitfield<uint32_t, 2, 1> CHSIDE; /*!< Channel side */
            microhal::Bitfield<uint32_t, 3, 1> UDR;    /*!< Underrun flag */
#endif
            microhal::Bitfield<uint32_t, 4, 1> CRCERR; /*!< CRC error flag */
            microhal::Bitfield<uint32_t, 5, 1> MODF;   /*!< Mode fault */
            microhal::Bitfield<uint32_t, 6, 1> OVR;    /*!< Overrun flag */
            microhal::Bitfield<uint32_t, 7, 1> BSY;    /*!< Busy flag */
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_TIFRFE
            microhal::Bitfield<uint32_t, 8, 1> TIFRFE; /*!< TI frame format error */
#else
            microhal::Bitfield<uint32_t, 8, 1> FRE;  /*!< Frame format error */
#endif
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
            microhal::Bitfield<uint32_t, 9, 2> FRLVL;  /*!< FIFO reception level */
            microhal::Bitfield<uint32_t, 11, 2> FTLVL; /*!< FIFO transmission level */
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

    // data register
    union DR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> data; /*!< Data register */
            microhal::Bitfield<uint32_t, 0, 8> data8; /*!< Data register */
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

    // CRC polynomial register
    union CRCPR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> CRCPOLY; /*!< CRC polynomial register */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CRCPR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CRCPR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CRCPR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CRCPR operator&(uint32_t value) const {
            CRCPR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CRCPR operator|(uint32_t value) const {
            CRCPR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CRCPR, AccessType::ReadOnly>;
        friend class VolatileRegister<CRCPR, AccessType::WriteOnly>;
        friend class VolatileRegister<CRCPR, AccessType::ReadWrite>;
    };

    // RX CRC register
    union RXCRCR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> RxCRC; /*!< Rx CRC register */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RXCRCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RXCRCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RXCRCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RXCRCR operator&(uint32_t value) const {
            RXCRCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RXCRCR operator|(uint32_t value) const {
            RXCRCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RXCRCR, AccessType::ReadOnly>;
        friend class VolatileRegister<RXCRCR, AccessType::WriteOnly>;
        friend class VolatileRegister<RXCRCR, AccessType::ReadWrite>;
    };

    // TX CRC register
    union TXCRCR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> TxCRC; /*!< Tx CRC register */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TXCRCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TXCRCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TXCRCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TXCRCR operator&(uint32_t value) const {
            TXCRCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TXCRCR operator|(uint32_t value) const {
            TXCRCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TXCRCR, AccessType::ReadOnly>;
        friend class VolatileRegister<TXCRCR, AccessType::WriteOnly>;
        friend class VolatileRegister<TXCRCR, AccessType::ReadWrite>;
    };

    // I2S configuration register
    union I2SCFGR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> CHLEN;   /*!< Channel length (number of bits per audio channel) */
            microhal::Bitfield<uint32_t, 1, 2> DATLEN;  /*!< Data length to be transferred */
            microhal::Bitfield<uint32_t, 3, 1> CKPOL;   /*!< Steady state clock polarity */
            microhal::Bitfield<uint32_t, 4, 2> I2SSTD;  /*!< I2S standard selection */
            microhal::Bitfield<uint32_t, 7, 1> PCMSYNC; /*!< PCM frame synchronization */
            microhal::Bitfield<uint32_t, 8, 2> I2SCFG;  /*!< I2S configuration mode */
            microhal::Bitfield<uint32_t, 10, 1> I2SE;   /*!< I2S Enable */
            microhal::Bitfield<uint32_t, 11, 1> I2SMOD; /*!< I2S mode selection */
#ifdef _MICROHAL_REGISTERS_SPI_I2SCFGR_HAS_ASTRTEN
            microhal::Bitfield<uint32_t, 12, 1> ASTRTEN; /*!< Asynchronous start enable */
#endif
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        I2SCFGR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        I2SCFGR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        I2SCFGR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        I2SCFGR operator&(uint32_t value) const {
            I2SCFGR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        I2SCFGR operator|(uint32_t value) const {
            I2SCFGR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<I2SCFGR, AccessType::ReadOnly>;
        friend class VolatileRegister<I2SCFGR, AccessType::WriteOnly>;
        friend class VolatileRegister<I2SCFGR, AccessType::ReadWrite>;
    };

    // I2S prescaler register
    union I2SPR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> I2SDIV; /*!< I2S Linear prescaler */
            microhal::Bitfield<uint32_t, 8, 1> ODD;    /*!< Odd factor for the prescaler */
            microhal::Bitfield<uint32_t, 9, 1> MCKOE;  /*!< Master clock output enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        I2SPR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        I2SPR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        I2SPR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        I2SPR operator&(uint32_t value) const {
            I2SPR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        I2SPR operator|(uint32_t value) const {
            I2SPR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<I2SPR, AccessType::ReadOnly>;
        friend class VolatileRegister<I2SPR, AccessType::WriteOnly>;
        friend class VolatileRegister<I2SPR, AccessType::ReadWrite>;
    };

    VolatileRegister<CR1, AccessType::ReadWrite> cr1;       /*!< control register 1	Address offset: 0x0 */
    VolatileRegister<CR2, AccessType::ReadWrite> cr2;       /*!< control register 2	Address offset: 0x4 */
    VolatileRegister<SR, AccessType::ReadWrite> sr;         /*!< status register	Address offset: 0x8 */
    VolatileRegister<DR, AccessType::ReadWrite> dr;         /*!< data register	Address offset: 0xC */
    VolatileRegister<CRCPR, AccessType::ReadWrite> crcpr;   /*!< CRC polynomial register	Address offset: 0x10 */
    VolatileRegister<RXCRCR, AccessType::ReadWrite> rxcrcr; /*!< RX CRC register	Address offset: 0x14 */
    VolatileRegister<TXCRCR, AccessType::ReadWrite> txcrcr; /*!< TX CRC register	Address offset: 0x18 */
#ifdef _MICROHAL_REGISTERS_SPI_HAS_I2S
    VolatileRegister<I2SCFGR, AccessType::ReadWrite> i2scfgr; /*!< I2S configuration register	Address offset: 0x1C */
    VolatileRegister<I2SPR, AccessType::ReadWrite> i2spr;     /*!< I2S prescaler register	Address offset: 0x20 */
#endif
};

#if defined(_MICROHAL_SPI1_BASE_ADDRESS)
[[maybe_unused]] static SPI *const spi1 = reinterpret_cast<SPI *>(_MICROHAL_SPI1_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_SPI2_BASE_ADDRESS)
[[maybe_unused]] static SPI *const spi2 = reinterpret_cast<SPI *>(_MICROHAL_SPI2_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_SPI3_BASE_ADDRESS)
[[maybe_unused]] static SPI *const spi3 = reinterpret_cast<SPI *>(_MICROHAL_SPI3_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_SPI4_BASE_ADDRESS)
[[maybe_unused]] static SPI *const spi4 = reinterpret_cast<SPI *>(_MICROHAL_SPI4_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_SPI5_BASE_ADDRESS)
[[maybe_unused]] static SPI *const spi5 = reinterpret_cast<SPI *>(_MICROHAL_SPI5_BASE_ADDRESS);
#endif

#if defined(_MICROHAL_SPI6_BASE_ADDRESS)
[[maybe_unused]] static SPI *const spi6 = reinterpret_cast<SPI *>(_MICROHAL_SPI6_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_SPI
