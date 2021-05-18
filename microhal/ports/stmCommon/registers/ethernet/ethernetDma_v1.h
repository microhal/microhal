/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2021, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_DMA
#define _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_DMA

#include "../registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32F107, STM32F103, STM32F101
namespace microhal {
namespace registers {
/**
 *
 */
struct ETHERNET_DMA {
    // Ethernet DMA bus mode register
    union DMABMR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> SR;    /*!< Software reset */
            microhal::Bitfield<uint32_t, 1, 1> DA;    /*!< DMA Arbitration */
            microhal::Bitfield<uint32_t, 2, 5> DSL;   /*!< Descriptor skip length */
            microhal::Bitfield<uint32_t, 8, 6> PBL;   /*!< Programmable burst length */
            microhal::Bitfield<uint32_t, 14, 2> RTPR; /*!< Rx Tx priority ratio */
            microhal::Bitfield<uint32_t, 16, 1> FB;   /*!< Fixed burst */
            microhal::Bitfield<uint32_t, 17, 6> RDP;  /*!< Rx DMA PBL */
            microhal::Bitfield<uint32_t, 23, 1> USP;  /*!< Use separate PBL */
            microhal::Bitfield<uint32_t, 24, 1> FPM;  /*!< 4xPBL mode */
            microhal::Bitfield<uint32_t, 25, 1> AAB;  /*!< Address-aligned beats */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMABMR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMABMR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMABMR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMABMR operator&(uint32_t value) const {
            DMABMR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMABMR operator|(uint32_t value) const {
            DMABMR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMABMR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMABMR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMABMR, AccessType::ReadWrite>;
    };

    // Ethernet DMA transmit poll demand register
    union DMATPDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> TPD; /*!< Transmit poll demand */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMATPDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMATPDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMATPDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMATPDR operator&(uint32_t value) const {
            DMATPDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMATPDR operator|(uint32_t value) const {
            DMATPDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMATPDR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMATPDR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMATPDR, AccessType::ReadWrite>;
    };

    // EHERNET DMA receive poll demand register
    union DMARPDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> RPD; /*!< Receive poll demand */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMARPDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMARPDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMARPDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMARPDR operator&(uint32_t value) const {
            DMARPDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMARPDR operator|(uint32_t value) const {
            DMARPDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMARPDR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMARPDR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMARPDR, AccessType::ReadWrite>;
    };

    // Ethernet DMA receive descriptor list address register
    union DMARDLAR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> SRL; /*!< Start of receive list */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMARDLAR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMARDLAR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMARDLAR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMARDLAR operator&(uint32_t value) const {
            DMARDLAR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMARDLAR operator|(uint32_t value) const {
            DMARDLAR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMARDLAR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMARDLAR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMARDLAR, AccessType::ReadWrite>;
    };

    // Ethernet DMA transmit descriptor list address register
    union DMATDLAR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> STL; /*!< Start of transmit list */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMATDLAR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMATDLAR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMATDLAR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMATDLAR operator&(uint32_t value) const {
            DMATDLAR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMATDLAR operator|(uint32_t value) const {
            DMATDLAR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMATDLAR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMATDLAR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMATDLAR, AccessType::ReadWrite>;
    };

    // Ethernet DMA status register
    union DMASR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TS;    /*!< Transmit status */
            microhal::Bitfield<uint32_t, 1, 1> TPSS;  /*!< Transmit process stopped status */
            microhal::Bitfield<uint32_t, 2, 1> TBUS;  /*!< Transmit buffer unavailable status */
            microhal::Bitfield<uint32_t, 3, 1> TJTS;  /*!< Transmit jabber timeout status */
            microhal::Bitfield<uint32_t, 4, 1> ROS;   /*!< Receive overflow status */
            microhal::Bitfield<uint32_t, 5, 1> TUS;   /*!< Transmit underflow status */
            microhal::Bitfield<uint32_t, 6, 1> RS;    /*!< Receive status */
            microhal::Bitfield<uint32_t, 7, 1> RBUS;  /*!< Receive buffer unavailable status */
            microhal::Bitfield<uint32_t, 8, 1> RPSS;  /*!< Receive process stopped status */
            microhal::Bitfield<uint32_t, 9, 1> PWTS;  /*!< Receive watchdog timeout status */
            microhal::Bitfield<uint32_t, 10, 1> ETS;  /*!< Early transmit status */
            microhal::Bitfield<uint32_t, 13, 1> FBES; /*!< Fatal bus error status */
            microhal::Bitfield<uint32_t, 14, 1> ERS;  /*!< Early receive status */
            microhal::Bitfield<uint32_t, 15, 1> AIS;  /*!< Abnormal interrupt summary */
            microhal::Bitfield<uint32_t, 16, 1> NIS;  /*!< Normal interrupt summary */
            microhal::Bitfield<uint32_t, 17, 3> RPS;  /*!< Receive process state */
            microhal::Bitfield<uint32_t, 20, 3> TPS;  /*!< Transmit process state */
            microhal::Bitfield<uint32_t, 23, 3> EBS;  /*!< Error bits status */
            microhal::Bitfield<uint32_t, 27, 1> MMCS; /*!< MMC status */
            microhal::Bitfield<uint32_t, 28, 1> PMTS; /*!< PMT status */
            microhal::Bitfield<uint32_t, 29, 1> TSTS; /*!< Time stamp trigger status */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMASR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMASR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMASR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMASR operator&(uint32_t value) const {
            DMASR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMASR operator|(uint32_t value) const {
            DMASR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMASR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMASR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMASR, AccessType::ReadWrite>;
    };

    // Ethernet DMA operation mode register
    union DMAOMR {
        union {
            microhal::Bitfield<uint32_t, 1, 1> SR;      /*!< SR */
            microhal::Bitfield<uint32_t, 2, 1> OSF;     /*!< OSF */
            microhal::Bitfield<uint32_t, 3, 2> RTC;     /*!< RTC */
            microhal::Bitfield<uint32_t, 6, 1> FUGF;    /*!< FUGF */
            microhal::Bitfield<uint32_t, 7, 1> FEF;     /*!< FEF */
            microhal::Bitfield<uint32_t, 13, 1> ST;     /*!< ST */
            microhal::Bitfield<uint32_t, 14, 3> TTC;    /*!< TTC */
            microhal::Bitfield<uint32_t, 20, 1> FTF;    /*!< FTF */
            microhal::Bitfield<uint32_t, 21, 1> TSF;    /*!< TSF */
            microhal::Bitfield<uint32_t, 24, 1> DFRF;   /*!< DFRF */
            microhal::Bitfield<uint32_t, 25, 1> RSF;    /*!< RSF */
            microhal::Bitfield<uint32_t, 26, 1> DTCEFD; /*!< DTCEFD */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAOMR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAOMR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAOMR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAOMR operator&(uint32_t value) const {
            DMAOMR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAOMR operator|(uint32_t value) const {
            DMAOMR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAOMR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAOMR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAOMR, AccessType::ReadWrite>;
    };

    // Ethernet DMA interrupt enable register
    union DMAIER {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TIE;    /*!< Transmit interrupt enable */
            microhal::Bitfield<uint32_t, 1, 1> TPSIE;  /*!< Transmit process stopped interrupt enable */
            microhal::Bitfield<uint32_t, 2, 1> TBUIE;  /*!< Transmit buffer unavailable interrupt enable */
            microhal::Bitfield<uint32_t, 3, 1> TJTIE;  /*!< Transmit jabber timeout interrupt enable */
            microhal::Bitfield<uint32_t, 4, 1> ROIE;   /*!< Overflow interrupt enable */
            microhal::Bitfield<uint32_t, 5, 1> TUIE;   /*!< Underflow interrupt enable */
            microhal::Bitfield<uint32_t, 6, 1> RIE;    /*!< Receive interrupt enable */
            microhal::Bitfield<uint32_t, 7, 1> RBUIE;  /*!< Receive buffer unavailable interrupt enable */
            microhal::Bitfield<uint32_t, 8, 1> RPSIE;  /*!< Receive process stopped interrupt enable */
            microhal::Bitfield<uint32_t, 9, 1> RWTIE;  /*!< receive watchdog timeout interrupt enable */
            microhal::Bitfield<uint32_t, 10, 1> ETIE;  /*!< Early transmit interrupt enable */
            microhal::Bitfield<uint32_t, 13, 1> FBEIE; /*!< Fatal bus error interrupt enable */
            microhal::Bitfield<uint32_t, 14, 1> ERIE;  /*!< Early receive interrupt enable */
            microhal::Bitfield<uint32_t, 15, 1> AISE;  /*!< Abnormal interrupt summary enable */
            microhal::Bitfield<uint32_t, 16, 1> NISE;  /*!< Normal interrupt summary enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAIER &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAIER &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAIER &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAIER operator&(uint32_t value) const {
            DMAIER tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAIER operator|(uint32_t value) const {
            DMAIER tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAIER, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAIER, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAIER, AccessType::ReadWrite>;
    };

    // Ethernet DMA missed frame and buffer overflow counter register
    union DMAMFBOCR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> MFC;  /*!< Missed frames by the controller */
            microhal::Bitfield<uint32_t, 16, 1> OMFC; /*!< Overflow bit for missed frame counter */
            microhal::Bitfield<uint32_t, 17, 11> MFA; /*!< Missed frames by the application */
            microhal::Bitfield<uint32_t, 28, 1> OFOC; /*!< Overflow bit for FIFO overflow counter */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMAMFBOCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMAMFBOCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMAMFBOCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMAMFBOCR operator&(uint32_t value) const {
            DMAMFBOCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMAMFBOCR operator|(uint32_t value) const {
            DMAMFBOCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMAMFBOCR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMAMFBOCR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMAMFBOCR, AccessType::ReadWrite>;
    };

    // Ethernet DMA current host transmit descriptor register
    union DMACHTDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> HTDAP; /*!< Host transmit descriptor address pointer */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMACHTDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMACHTDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMACHTDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMACHTDR operator&(uint32_t value) const {
            DMACHTDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMACHTDR operator|(uint32_t value) const {
            DMACHTDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMACHTDR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMACHTDR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMACHTDR, AccessType::ReadWrite>;
    };

    // Ethernet DMA current host receive descriptor register
    union DMACHRDR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> HRDAP; /*!< Host receive descriptor address pointer */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMACHRDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMACHRDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMACHRDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMACHRDR operator&(uint32_t value) const {
            DMACHRDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMACHRDR operator|(uint32_t value) const {
            DMACHRDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMACHRDR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMACHRDR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMACHRDR, AccessType::ReadWrite>;
    };

    // Ethernet DMA current host transmit buffer address register
    union DMACHTBAR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> HTBAP; /*!< Host transmit buffer address pointer */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMACHTBAR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMACHTBAR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMACHTBAR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMACHTBAR operator&(uint32_t value) const {
            DMACHTBAR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMACHTBAR operator|(uint32_t value) const {
            DMACHTBAR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMACHTBAR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMACHTBAR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMACHTBAR, AccessType::ReadWrite>;
    };

    // Ethernet DMA current host receive buffer address register
    union DMACHRBAR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> HRBAP; /*!< Host receive buffer address pointer */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        DMACHRBAR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        DMACHRBAR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        DMACHRBAR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        DMACHRBAR operator&(uint32_t value) const {
            DMACHRBAR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        DMACHRBAR operator|(uint32_t value) const {
            DMACHRBAR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<DMACHRBAR, AccessType::ReadOnly>;
        friend class VolatileRegister<DMACHRBAR, AccessType::WriteOnly>;
        friend class VolatileRegister<DMACHRBAR, AccessType::ReadWrite>;
    };

    VolatileRegister<DMABMR, AccessType::ReadWrite> dmabmr;     /*!< Ethernet DMA bus mode register	Address offset: 0x0 */
    VolatileRegister<DMATPDR, AccessType::ReadWrite> dmatpdr;   /*!< Ethernet DMA transmit poll demand register	Address offset: 0x4 */
    VolatileRegister<DMARPDR, AccessType::ReadWrite> dmarpdr;   /*!< EHERNET DMA receive poll demand register	Address offset: 0x8 */
    VolatileRegister<DMARDLAR, AccessType::ReadWrite> dmardlar; /*!< Ethernet DMA receive descriptor list address register	Address offset: 0xC */
    VolatileRegister<DMATDLAR, AccessType::ReadWrite> dmatdlar; /*!< Ethernet DMA transmit descriptor list address register	Address offset: 0x10
                                                                 */
    VolatileRegister<DMASR, AccessType::ReadWrite> dmasr;       /*!< Ethernet DMA status register	Address offset: 0x14 */
    VolatileRegister<DMAOMR, AccessType::ReadWrite> dmaomr;     /*!< Ethernet DMA operation mode register	Address offset: 0x18 */
    VolatileRegister<DMAIER, AccessType::ReadWrite> dmaier;     /*!< Ethernet DMA interrupt enable register	Address offset: 0x1C */
    VolatileRegister<DMAMFBOCR, AccessType::ReadWrite>
        dmamfbocr;         /*!< Ethernet DMA missed frame and buffer overflow counter register	Address offset: 0x20 */
    uint32_t reserved0[9]; /*!< Reserved register	Address offset: 0x24 */
    VolatileRegister<DMACHTDR, AccessType::ReadWrite> dmachtdr; /*!< Ethernet DMA current host transmit descriptor register	Address offset: 0x48
                                                                 */
    VolatileRegister<DMACHRDR, AccessType::ReadWrite> dmachrdr; /*!< Ethernet DMA current host receive descriptor register	Address offset: 0x4C
                                                                 */
    VolatileRegister<DMACHTBAR, AccessType::ReadWrite>
        dmachtbar; /*!< Ethernet DMA current host transmit buffer address register	Address offset: 0x50 */
    VolatileRegister<DMACHRBAR, AccessType::ReadWrite>
        dmachrbar; /*!< Ethernet DMA current host receive buffer address register	Address offset: 0x54 */
};

#if defined(_MICROHAL_ETHERNET_DMA_BASE_ADDRESS)
[[maybe_unused]] static ETHERNET_DMA *const ethernetDma = reinterpret_cast<ETHERNET_DMA *>(_MICROHAL_ETHERNET_DMA_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_DMA
