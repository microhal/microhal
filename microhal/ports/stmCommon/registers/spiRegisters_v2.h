// Supported MCU: STM32H742x, STM32H7x3, STM32H743x, STM32H753x, STM32H7x5_CM7, STM32H7x5_CM4, STM32H750x

#ifndef _MICROHAL_PERIPHERAL_REGISTER_SPI
#define _MICROHAL_PERIPHERAL_REGISTER_SPI

#include "bitfield.h"
#include "registersBaseAddressDefinitions.h"
#include "volatileRegister.h"

namespace microhal {
namespace registers {
/**
 *
 */
struct SPI {
    // control register 1
    union CR1 {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SPE;       /*!< Serial Peripheral Enable */
            microhal::Bitfield<uint32_t, 8, 1> MASRX;     /*!< Master automatic SUSP in Receive mode */
            microhal::Bitfield<uint32_t, 9, 1> CSTART;    /*!< Master transfer start */
            microhal::Bitfield<uint32_t, 10, 1> CSUSP;    /*!< Master SUSPend request */
            microhal::Bitfield<uint32_t, 11, 1> HDDIR;    /*!< Rx/Tx direction at Half-duplex mode */
            microhal::Bitfield<uint32_t, 12, 1> SSI;      /*!< Internal SS signal input level */
            microhal::Bitfield<uint32_t, 13, 1> CRC33_17; /*!< 32-bit CRC polynomial configuration */
            microhal::Bitfield<uint32_t, 14, 1> RCRCI;    /*!< CRC calculation initialization pattern control for receiver */
            microhal::Bitfield<uint32_t, 15, 1> TCRCI;    /*!< CRC calculation initialization pattern control for transmitter */
            microhal::Bitfield<uint32_t, 16, 1> IOLOCK;   /*!< Locking the AF configuration of associated IOs */
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
            microhal::Bitfield<uint32_t, 0, 16> TSIZE; /*!< Number of data at current transfer */
            microhal::Bitfield<uint32_t, 16, 16> TSER; /*!< Number of data transfer extension to be reload into TSIZE just when a previous */
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

    // configuration register 1
    union CFG1 {
        union {
            microhal::Bitfield<uint32_t, 0, 5> DSIZE;    /*!< Number of bits in at single SPI data frame */
            microhal::Bitfield<uint32_t, 5, 4> FTHVL;    /*!< threshold level */
            microhal::Bitfield<uint32_t, 9, 2> UDRCFG;   /*!< Behavior of slave transmitter at underrun condition */
            microhal::Bitfield<uint32_t, 11, 2> UDRDET;  /*!< Detection of underrun condition at slave transmitter */
            microhal::Bitfield<uint32_t, 14, 1> RXDMAEN; /*!< Rx DMA stream enable */
            microhal::Bitfield<uint32_t, 15, 1> TXDMAEN; /*!< Tx DMA stream enable */
            microhal::Bitfield<uint32_t, 16, 5> CRCSIZE; /*!< Length of CRC frame to be transacted and compared */
            microhal::Bitfield<uint32_t, 22, 1> CRCEN;   /*!< Hardware CRC computation enable */
            microhal::Bitfield<uint32_t, 28, 3> MBR;     /*!< Master baud rate */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CFG1 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CFG1 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CFG1 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CFG1 operator&(uint32_t value) const {
            CFG1 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CFG1 operator|(uint32_t value) const {
            CFG1 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CFG1, AccessType::ReadOnly>;
        friend class VolatileRegister<CFG1, AccessType::WriteOnly>;
        friend class VolatileRegister<CFG1, AccessType::ReadWrite>;
    };

    // configuration register 2
    union CFG2 {
        union {
            microhal::Bitfield<uint32_t, 0, 4> MSSI;     /*!< Master SS Idleness */
            microhal::Bitfield<uint32_t, 4, 4> MIDI;     /*!< Master Inter-Data Idleness */
            microhal::Bitfield<uint32_t, 15, 1> IOSWP;   /*!< Swap functionality of MISO and MOSI pins */
            microhal::Bitfield<uint32_t, 17, 2> COMM;    /*!< SPI Communication Mode */
            microhal::Bitfield<uint32_t, 19, 3> SP;      /*!< Serial Protocol */
            microhal::Bitfield<uint32_t, 22, 1> MASTER;  /*!< SPI Master */
            microhal::Bitfield<uint32_t, 23, 1> LSBFRST; /*!< Data frame format */
            microhal::Bitfield<uint32_t, 24, 1> CPHA;    /*!< Clock phase */
            microhal::Bitfield<uint32_t, 25, 1> CPOL;    /*!< Clock polarity */
            microhal::Bitfield<uint32_t, 26, 1> SSM;     /*!< Software management of SS signal input */
            microhal::Bitfield<uint32_t, 28, 1> SSIOP;   /*!< SS input/output polarity */
            microhal::Bitfield<uint32_t, 29, 1> SSOE;    /*!< SS output enable */
            microhal::Bitfield<uint32_t, 30, 1> SSOM;    /*!< SS output management in master mode */
            microhal::Bitfield<uint32_t, 31, 1> AFCNTR;  /*!< Alternate function GPIOs control */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CFG2 &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CFG2 &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CFG2 &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CFG2 operator&(uint32_t value) const {
            CFG2 tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CFG2 operator|(uint32_t value) const {
            CFG2 tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CFG2, AccessType::ReadOnly>;
        friend class VolatileRegister<CFG2, AccessType::WriteOnly>;
        friend class VolatileRegister<CFG2, AccessType::ReadWrite>;
    };

    // Interrupt Enable Register
    union IER {
        union {
            microhal::Bitfield<uint32_t, 0, 1> RXPIE;    /*!< RXP Interrupt Enable */
            microhal::Bitfield<uint32_t, 1, 1> TXPIE;    /*!< TXP interrupt enable */
            microhal::Bitfield<uint32_t, 2, 1> DPXPIE;   /*!< DXP interrupt enabled */
            microhal::Bitfield<uint32_t, 3, 1> EOTIE;    /*!< EOT, SUSP and TXC interrupt enable */
            microhal::Bitfield<uint32_t, 4, 1> TXTFIE;   /*!< TXTFIE interrupt enable */
            microhal::Bitfield<uint32_t, 5, 1> UDRIE;    /*!< UDR interrupt enable */
            microhal::Bitfield<uint32_t, 6, 1> OVRIE;    /*!< OVR interrupt enable */
            microhal::Bitfield<uint32_t, 7, 1> CRCEIE;   /*!< CRC Interrupt enable */
            microhal::Bitfield<uint32_t, 8, 1> TIFREIE;  /*!< TIFRE interrupt enable */
            microhal::Bitfield<uint32_t, 9, 1> MODFIE;   /*!< Mode Fault interrupt enable */
            microhal::Bitfield<uint32_t, 10, 1> TSERFIE; /*!< Additional number of transactions reload interrupt enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IER operator&(uint32_t value) const {
            IER tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IER operator|(uint32_t value) const {
            IER tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IER, AccessType::ReadOnly>;
        friend class VolatileRegister<IER, AccessType::WriteOnly>;
        friend class VolatileRegister<IER, AccessType::ReadWrite>;
    };

    // Status Register
    union SR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> RXP;      /*!< Rx-Packet available */
            microhal::Bitfield<uint32_t, 1, 1> TXP;      /*!< Tx-Packet space available */
            microhal::Bitfield<uint32_t, 2, 1> DXP;      /*!< Duplex Packet */
            microhal::Bitfield<uint32_t, 3, 1> EOT;      /*!< End Of Transfer */
            microhal::Bitfield<uint32_t, 4, 1> TXTF;     /*!< Transmission Transfer Filled */
            microhal::Bitfield<uint32_t, 5, 1> UDR;      /*!< Underrun at slave transmission mode */
            microhal::Bitfield<uint32_t, 6, 1> OVR;      /*!< Overrun */
            microhal::Bitfield<uint32_t, 7, 1> CRCE;     /*!< CRC Error */
            microhal::Bitfield<uint32_t, 8, 1> TIFRE;    /*!< TI frame format error */
            microhal::Bitfield<uint32_t, 9, 1> MODF;     /*!< Mode Fault */
            microhal::Bitfield<uint32_t, 10, 1> TSERF;   /*!< Additional number of SPI data to be transacted was reload */
            microhal::Bitfield<uint32_t, 11, 1> SUSP;    /*!< SUSPend */
            microhal::Bitfield<uint32_t, 12, 1> TXC;     /*!< TxFIFO transmission complete */
            microhal::Bitfield<uint32_t, 13, 2> RXPLVL;  /*!< RxFIFO Packing LeVeL */
            microhal::Bitfield<uint32_t, 15, 1> RXWNE;   /*!< RxFIFO Word Not Empty */
            microhal::Bitfield<uint32_t, 16, 16> CTSIZE; /*!< Number of data frames remaining in current TSIZE session */
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

    // Interrupt/Status Flags Clear Register
    union IFCR {
        union {
            microhal::Bitfield<uint32_t, 3, 1> EOTC;    /*!< End Of Transfer flag clear */
            microhal::Bitfield<uint32_t, 4, 1> TXTFC;   /*!< Transmission Transfer Filled flag clear */
            microhal::Bitfield<uint32_t, 5, 1> UDRC;    /*!< Underrun flag clear */
            microhal::Bitfield<uint32_t, 6, 1> OVRC;    /*!< Overrun flag clear */
            microhal::Bitfield<uint32_t, 7, 1> CRCEC;   /*!< CRC Error flag clear */
            microhal::Bitfield<uint32_t, 8, 1> TIFREC;  /*!< TI frame format error flag clear */
            microhal::Bitfield<uint32_t, 9, 1> MODFC;   /*!< Mode Fault flag clear */
            microhal::Bitfield<uint32_t, 10, 1> TSERFC; /*!< TSERFC flag clear */
            microhal::Bitfield<uint32_t, 11, 1> SUSPC;  /*!< SUSPend flag clear */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        IFCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        IFCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        IFCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        IFCR operator&(uint32_t value) const {
            IFCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        IFCR operator|(uint32_t value) const {
            IFCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<IFCR, AccessType::ReadOnly>;
        friend class VolatileRegister<IFCR, AccessType::WriteOnly>;
        friend class VolatileRegister<IFCR, AccessType::ReadWrite>;
    };

    // Transmit Data Register
    union TXDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> TXDR; /*!< Transmit data register */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TXDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TXDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TXDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TXDR operator&(uint32_t value) const {
            TXDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TXDR operator|(uint32_t value) const {
            TXDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TXDR, AccessType::ReadOnly>;
        friend class VolatileRegister<TXDR, AccessType::WriteOnly>;
        friend class VolatileRegister<TXDR, AccessType::ReadWrite>;
    };

    // Receive Data Register
    union RXDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> RXDR; /*!< Receive data register */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RXDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RXDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RXDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RXDR operator&(uint32_t value) const {
            RXDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RXDR operator|(uint32_t value) const {
            RXDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RXDR, AccessType::ReadOnly>;
        friend class VolatileRegister<RXDR, AccessType::WriteOnly>;
        friend class VolatileRegister<RXDR, AccessType::ReadWrite>;
    };

    // Polynomial Register
    union CRCPOLY {
        union {
            microhal::Bitfield<uint32_t, 0, 32> CRCPOLY; /*!< CRC polynomial register */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CRCPOLY &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CRCPOLY &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CRCPOLY &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CRCPOLY operator&(uint32_t value) const {
            CRCPOLY tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CRCPOLY operator|(uint32_t value) const {
            CRCPOLY tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CRCPOLY, AccessType::ReadOnly>;
        friend class VolatileRegister<CRCPOLY, AccessType::WriteOnly>;
        friend class VolatileRegister<CRCPOLY, AccessType::ReadWrite>;
    };

    // Transmitter CRC Register
    union TXCRC {
        union {
            microhal::Bitfield<uint32_t, 0, 32> TXCRC; /*!< CRC register for transmitter */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        TXCRC &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        TXCRC &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        TXCRC &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        TXCRC operator&(uint32_t value) const {
            TXCRC tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        TXCRC operator|(uint32_t value) const {
            TXCRC tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<TXCRC, AccessType::ReadOnly>;
        friend class VolatileRegister<TXCRC, AccessType::WriteOnly>;
        friend class VolatileRegister<TXCRC, AccessType::ReadWrite>;
    };

    // Receiver CRC Register
    union RXCRC {
        union {
            microhal::Bitfield<uint32_t, 0, 32> RXCRC; /*!< CRC register for receiver */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        RXCRC &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        RXCRC &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        RXCRC &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        RXCRC operator&(uint32_t value) const {
            RXCRC tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        RXCRC operator|(uint32_t value) const {
            RXCRC tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<RXCRC, AccessType::ReadOnly>;
        friend class VolatileRegister<RXCRC, AccessType::WriteOnly>;
        friend class VolatileRegister<RXCRC, AccessType::ReadWrite>;
    };

    // Underrun Data Register
    union UDRDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> UDRDR; /*!< Data at slave underrun condition */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        UDRDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        UDRDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        UDRDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        UDRDR operator&(uint32_t value) const {
            UDRDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        UDRDR operator|(uint32_t value) const {
            UDRDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<UDRDR, AccessType::ReadOnly>;
        friend class VolatileRegister<UDRDR, AccessType::WriteOnly>;
        friend class VolatileRegister<UDRDR, AccessType::ReadWrite>;
    };

    // configuration register
    union CGFR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> I2SMOD;  /*!< I2S mode selection */
            microhal::Bitfield<uint32_t, 1, 3> I2SCFG;  /*!< I2S configuration mode */
            microhal::Bitfield<uint32_t, 4, 2> I2SSTD;  /*!< I2S standard selection */
            microhal::Bitfield<uint32_t, 7, 1> PCMSYNC; /*!< PCM frame synchronization */
            microhal::Bitfield<uint32_t, 8, 2> DATLEN;  /*!< Data length to be transferred */
            microhal::Bitfield<uint32_t, 10, 1> CHLEN;  /*!< Channel length (number of bits per audio channel) */
            microhal::Bitfield<uint32_t, 11, 1> CKPOL;  /*!< Serial audio clock polarity */
            microhal::Bitfield<uint32_t, 12, 1> FIXCH;  /*!< Word select inversion */
            microhal::Bitfield<uint32_t, 13, 1> WSINV;  /*!< Fixed channel length in SLAVE */
            microhal::Bitfield<uint32_t, 14, 1> DATFMT; /*!< Data format */
            microhal::Bitfield<uint32_t, 16, 8> I2SDIV; /*!< I2S linear prescaler */
            microhal::Bitfield<uint32_t, 24, 1> ODD;    /*!< Odd factor for the prescaler */
            microhal::Bitfield<uint32_t, 25, 1> MCKOE;  /*!< Master clock output enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        CGFR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        CGFR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        CGFR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        CGFR operator&(uint32_t value) const {
            CGFR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        CGFR operator|(uint32_t value) const {
            CGFR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<CGFR, AccessType::ReadOnly>;
        friend class VolatileRegister<CGFR, AccessType::WriteOnly>;
        friend class VolatileRegister<CGFR, AccessType::ReadWrite>;
    };

    VolatileRegister<CR1, AccessType::ReadWrite> cr1;                       /*!< control register 1	Address offset: 0x0 */
    VolatileRegister<CR2, AccessType::ReadWrite> cr2;                       /*!< control register 2	Address offset: 0x4 */
    VolatileRegister<CFG1, AccessType::ReadWrite> cfg1;                     /*!< configuration register 1	Address offset: 0x8 */
    VolatileRegister<CFG2, AccessType::ReadWrite> cfg2;                     /*!< configuration register 2	Address offset: 0xC */
    VolatileRegister<IER, AccessType::ReadWrite> ier;                       /*!< Interrupt Enable Register	Address offset: 0x10 */
    VolatileRegister<SR, AccessType::ReadWrite> sr;                         /*!< Status Register	Address offset: 0x14 */
    VolatileRegister<IFCR, AccessType::ReadWrite> ifcr;                     /*!< Interrupt/Status Flags Clear Register	Address offset: 0x18 */
    VolatileRegister<reserved0[1.0], AccessType::ReadWrite> reserved0[1.0]; /*!< Reserved register	Address offset: 0x1c */
    VolatileRegister<TXDR, AccessType::ReadWrite> txdr;                     /*!< Transmit Data Register	Address offset: 0x20 */
    VolatileRegister<reserved1[3.0], AccessType::ReadWrite> reserved1[3.0]; /*!< Reserved register	Address offset: 0x24 */
    VolatileRegister<RXDR, AccessType::ReadWrite> rxdr;                     /*!< Receive Data Register	Address offset: 0x30 */
    VolatileRegister<reserved2[3.0], AccessType::ReadWrite> reserved2[3.0]; /*!< Reserved register	Address offset: 0x34 */
    VolatileRegister<CRCPOLY, AccessType::ReadWrite> crcpoly;               /*!< Polynomial Register	Address offset: 0x40 */
    VolatileRegister<TXCRC, AccessType::ReadWrite> txcrc;                   /*!< Transmitter CRC Register	Address offset: 0x44 */
    VolatileRegister<RXCRC, AccessType::ReadWrite> rxcrc;                   /*!< Receiver CRC Register	Address offset: 0x48 */
    VolatileRegister<UDRDR, AccessType::ReadWrite> udrdr;                   /*!< Underrun Data Register	Address offset: 0x4C */
    VolatileRegister<CGFR, AccessType::ReadWrite> cgfr;                     /*!< configuration register	Address offset: 0x50 */
};
}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_SPI
