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

#ifndef _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_MAC
#define _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_MAC

#include "../registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32F107, STM32F103, STM32F101
namespace microhal {
namespace registers {
/**
 *
 */
struct ETHERNET_MAC {
    // Ethernet MAC configuration register (ETH_MACCR)
    union MACCR {
        union {
            microhal::Bitfield<uint32_t, 2, 1> RE;    /*!< Receiver enable */
            microhal::Bitfield<uint32_t, 3, 1> TE;    /*!< Transmitter enable */
            microhal::Bitfield<uint32_t, 4, 1> DC;    /*!< Deferral check */
            microhal::Bitfield<uint32_t, 5, 2> BL;    /*!< Back-off limit */
            microhal::Bitfield<uint32_t, 7, 1> APCS;  /*!< Automatic pad/CRC stripping */
            microhal::Bitfield<uint32_t, 9, 1> RD;    /*!< Retry disable */
            microhal::Bitfield<uint32_t, 10, 1> IPCO; /*!< IPv4 checksum offload */
            microhal::Bitfield<uint32_t, 11, 1> DM;   /*!< Duplex mode */
            microhal::Bitfield<uint32_t, 12, 1> LM;   /*!< Loopback mode */
            microhal::Bitfield<uint32_t, 13, 1> ROD;  /*!< Receive own disable */
            microhal::Bitfield<uint32_t, 14, 1> FES;  /*!< Fast Ethernet speed */
            microhal::Bitfield<uint32_t, 16, 1> CSD;  /*!< Carrier sense disable */
            microhal::Bitfield<uint32_t, 17, 3> IFG;  /*!< Interframe gap */
            microhal::Bitfield<uint32_t, 22, 1> JD;   /*!< Jabber disable */
            microhal::Bitfield<uint32_t, 23, 1> WD;   /*!< Watchdog disable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACCR operator&(uint32_t value) const {
            MACCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACCR operator|(uint32_t value) const {
            MACCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACCR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACCR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACCR, AccessType::ReadWrite>;
    };

    // Ethernet MAC frame filter register (ETH_MACCFFR)
    union MACFFR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PM;   /*!< Promiscuous mode */
            microhal::Bitfield<uint32_t, 1, 1> HU;   /*!< Hash unicast */
            microhal::Bitfield<uint32_t, 2, 1> HM;   /*!< Hash multicast */
            microhal::Bitfield<uint32_t, 3, 1> DAIF; /*!< Destination address inverse filtering */
            microhal::Bitfield<uint32_t, 4, 1> PAM;  /*!< Pass all multicast */
            microhal::Bitfield<uint32_t, 5, 1> BFD;  /*!< Broadcast frames disable */
            microhal::Bitfield<uint32_t, 6, 2> PCF;  /*!< Pass control frames */
            microhal::Bitfield<uint32_t, 8, 1> SAIF; /*!< Source address inverse filtering */
            microhal::Bitfield<uint32_t, 9, 1> SAF;  /*!< Source address filter */
            microhal::Bitfield<uint32_t, 10, 1> HPF; /*!< Hash or perfect filter */
            microhal::Bitfield<uint32_t, 31, 1> RA;  /*!< Receive all */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACFFR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACFFR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACFFR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACFFR operator&(uint32_t value) const {
            MACFFR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACFFR operator|(uint32_t value) const {
            MACFFR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACFFR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACFFR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACFFR, AccessType::ReadWrite>;
    };

    // Ethernet MAC hash table high register
    union MACHTHR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> HTH; /*!< Hash table high */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACHTHR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACHTHR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACHTHR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACHTHR operator&(uint32_t value) const {
            MACHTHR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACHTHR operator|(uint32_t value) const {
            MACHTHR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACHTHR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACHTHR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACHTHR, AccessType::ReadWrite>;
    };

    // Ethernet MAC hash table low register
    union MACHTLR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> HTL; /*!< Hash table low */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACHTLR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACHTLR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACHTLR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACHTLR operator&(uint32_t value) const {
            MACHTLR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACHTLR operator|(uint32_t value) const {
            MACHTLR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACHTLR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACHTLR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACHTLR, AccessType::ReadWrite>;
    };

    // Ethernet MAC MII address register (ETH_MACMIIAR)
    union MACMIIAR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> MB;  /*!< MII busy */
            microhal::Bitfield<uint32_t, 1, 1> MW;  /*!< MII write */
            microhal::Bitfield<uint32_t, 2, 3> CR;  /*!< Clock range */
            microhal::Bitfield<uint32_t, 6, 5> MR;  /*!< MII register */
            microhal::Bitfield<uint32_t, 11, 5> PA; /*!< PHY address */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACMIIAR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACMIIAR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACMIIAR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACMIIAR operator&(uint32_t value) const {
            MACMIIAR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACMIIAR operator|(uint32_t value) const {
            MACMIIAR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACMIIAR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACMIIAR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACMIIAR, AccessType::ReadWrite>;
    };

    // Ethernet MAC MII data register (ETH_MACMIIDR)
    union MACMIIDR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> MD; /*!< MII data */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACMIIDR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACMIIDR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACMIIDR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACMIIDR operator&(uint32_t value) const {
            MACMIIDR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACMIIDR operator|(uint32_t value) const {
            MACMIIDR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACMIIDR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACMIIDR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACMIIDR, AccessType::ReadWrite>;
    };

    // Ethernet MAC flow control register (ETH_MACFCR)
    union MACFCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> FCB_BPA; /*!< Flow control busy/back pressure activate */
            microhal::Bitfield<uint32_t, 1, 1> TFCE;    /*!< Transmit flow control enable */
            microhal::Bitfield<uint32_t, 2, 1> RFCE;    /*!< Receive flow control enable */
            microhal::Bitfield<uint32_t, 3, 1> UPFD;    /*!< Unicast pause frame detect */
            microhal::Bitfield<uint32_t, 4, 2> PLT;     /*!< Pause low threshold */
            microhal::Bitfield<uint32_t, 7, 1> ZQPD;    /*!< Zero-quanta pause disable */
            microhal::Bitfield<uint32_t, 16, 16> PT;    /*!< Pass control frames */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACFCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACFCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACFCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACFCR operator&(uint32_t value) const {
            MACFCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACFCR operator|(uint32_t value) const {
            MACFCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACFCR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACFCR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACFCR, AccessType::ReadWrite>;
    };

    // Ethernet MAC VLAN tag register (ETH_MACVLANTR)
    union MACVLANTR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> VLANTI; /*!< VLAN tag identifier (for receive frames) */
            microhal::Bitfield<uint32_t, 16, 1> VLANTC; /*!< 12-bit VLAN tag comparison */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACVLANTR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACVLANTR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACVLANTR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACVLANTR operator&(uint32_t value) const {
            MACVLANTR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACVLANTR operator|(uint32_t value) const {
            MACVLANTR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACVLANTR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACVLANTR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACVLANTR, AccessType::ReadWrite>;
    };

    // Ethernet MAC remote wakeup frame filter register (ETH_MACRWUFFR)
    union MACRWUFFR {
        union {};

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACRWUFFR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACRWUFFR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACRWUFFR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACRWUFFR operator&(uint32_t value) const {
            MACRWUFFR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACRWUFFR operator|(uint32_t value) const {
            MACRWUFFR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACRWUFFR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACRWUFFR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACRWUFFR, AccessType::ReadWrite>;
    };

    // Ethernet MAC PMT control and status register (ETH_MACPMTCSR)
    union MACPMTCSR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> PD;      /*!< Power down */
            microhal::Bitfield<uint32_t, 1, 1> MPE;     /*!< Magic Packet enable */
            microhal::Bitfield<uint32_t, 2, 1> WFE;     /*!< Wakeup frame enable */
            microhal::Bitfield<uint32_t, 5, 1> MPR;     /*!< Magic packet received */
            microhal::Bitfield<uint32_t, 6, 1> WFR;     /*!< Wakeup frame received */
            microhal::Bitfield<uint32_t, 9, 1> GU;      /*!< Global unicast */
            microhal::Bitfield<uint32_t, 31, 1> WFFRPR; /*!< Wakeup frame filter register pointer reset */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACPMTCSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACPMTCSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACPMTCSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACPMTCSR operator&(uint32_t value) const {
            MACPMTCSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACPMTCSR operator|(uint32_t value) const {
            MACPMTCSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACPMTCSR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACPMTCSR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACPMTCSR, AccessType::ReadWrite>;
    };

    // Ethernet MAC interrupt status register (ETH_MACSR)
    union MACSR {
        union {
            microhal::Bitfield<uint32_t, 3, 1> PMTS;  /*!< PMT status */
            microhal::Bitfield<uint32_t, 4, 1> MMCS;  /*!< MMC status */
            microhal::Bitfield<uint32_t, 5, 1> MMCRS; /*!< MMC receive status */
            microhal::Bitfield<uint32_t, 6, 1> MMCTS; /*!< MMC transmit status */
            microhal::Bitfield<uint32_t, 9, 1> TSTS;  /*!< Time stamp trigger status */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACSR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACSR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACSR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACSR operator&(uint32_t value) const {
            MACSR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACSR operator|(uint32_t value) const {
            MACSR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACSR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACSR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACSR, AccessType::ReadWrite>;
    };

    // Ethernet MAC interrupt mask register (ETH_MACIMR)
    union MACIMR {
        union {
            microhal::Bitfield<uint32_t, 3, 1> PMTIM; /*!< PMT interrupt mask */
            microhal::Bitfield<uint32_t, 9, 1> TSTIM; /*!< Time stamp trigger interrupt mask */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACIMR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACIMR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACIMR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACIMR operator&(uint32_t value) const {
            MACIMR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACIMR operator|(uint32_t value) const {
            MACIMR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACIMR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACIMR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACIMR, AccessType::ReadWrite>;
    };

    // Ethernet MAC address 0 high register (ETH_MACA0HR)
    union MACA0HR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> MACA0H; /*!< MAC address0 high */
            microhal::Bitfield<uint32_t, 31, 1> MO;     /*!< Always 1 */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACA0HR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACA0HR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACA0HR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACA0HR operator&(uint32_t value) const {
            MACA0HR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACA0HR operator|(uint32_t value) const {
            MACA0HR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACA0HR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACA0HR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACA0HR, AccessType::ReadWrite>;
    };

    // Ethernet MAC address 0 low register
    union MACA0LR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> MACA0L; /*!< MAC address0 low */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACA0LR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACA0LR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACA0LR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACA0LR operator&(uint32_t value) const {
            MACA0LR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACA0LR operator|(uint32_t value) const {
            MACA0LR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACA0LR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACA0LR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACA0LR, AccessType::ReadWrite>;
    };

    // Ethernet MAC address 1 high register (ETH_MACA1HR)
    union MACAxHR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> MACA1H; /*!< MAC address1 high */
            microhal::Bitfield<uint32_t, 24, 6> MBC;    /*!< Mask byte control */
            microhal::Bitfield<uint32_t, 30, 1> SA;     /*!< Source address */
            microhal::Bitfield<uint32_t, 31, 1> AE;     /*!< Address enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACAxHR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACAxHR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACAxHR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACAxHR operator&(uint32_t value) const {
            MACAxHR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACAxHR operator|(uint32_t value) const {
            MACAxHR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACAxHR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACAxHR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACAxHR, AccessType::ReadWrite>;
    };

    // Ethernet MAC address1 low register
    union MACAxLR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> MACA1L; /*!< MAC address1 low */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACAxLR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACAxLR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACAxLR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACAxLR operator&(uint32_t value) const {
            MACAxLR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACAxLR operator|(uint32_t value) const {
            MACAxLR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACAxLR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACAxLR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACAxLR, AccessType::ReadWrite>;
    };

    // Ethernet MAC address 2 high register (ETH_MACA2HR)
    union MACA2HR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> ETH_MACA2HR; /*!< Ethernet MAC address 2 high register */
            microhal::Bitfield<uint32_t, 24, 6> MBC;         /*!< Mask byte control */
            microhal::Bitfield<uint32_t, 30, 1> SA;          /*!< Source address */
            microhal::Bitfield<uint32_t, 31, 1> AE;          /*!< Address enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACA2HR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACA2HR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACA2HR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACA2HR operator&(uint32_t value) const {
            MACA2HR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACA2HR operator|(uint32_t value) const {
            MACA2HR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACA2HR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACA2HR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACA2HR, AccessType::ReadWrite>;
    };

    // Ethernet MAC address 2 low register
    union MACA2LR {
        union {
            microhal::Bitfield<uint32_t, 0, 31> MACA2L; /*!< MAC address2 low */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACA2LR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACA2LR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACA2LR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACA2LR operator&(uint32_t value) const {
            MACA2LR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACA2LR operator|(uint32_t value) const {
            MACA2LR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACA2LR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACA2LR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACA2LR, AccessType::ReadWrite>;
    };

    // Ethernet MAC address 3 high register (ETH_MACA3HR)
    union MACA3HR {
        union {
            microhal::Bitfield<uint32_t, 0, 16> MACA3H; /*!< MAC address3 high */
            microhal::Bitfield<uint32_t, 24, 6> MBC;    /*!< Mask byte control */
            microhal::Bitfield<uint32_t, 30, 1> SA;     /*!< Source address */
            microhal::Bitfield<uint32_t, 31, 1> AE;     /*!< Address enable */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACA3HR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACA3HR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACA3HR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACA3HR operator&(uint32_t value) const {
            MACA3HR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACA3HR operator|(uint32_t value) const {
            MACA3HR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACA3HR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACA3HR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACA3HR, AccessType::ReadWrite>;
    };

    // Ethernet MAC address 3 low register
    union MACA3LR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> MBCA3L; /*!< MAC address3 low */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MACA3LR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MACA3LR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MACA3LR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MACA3LR operator&(uint32_t value) const {
            MACA3LR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MACA3LR operator|(uint32_t value) const {
            MACA3LR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MACA3LR, AccessType::ReadOnly>;
        friend class VolatileRegister<MACA3LR, AccessType::WriteOnly>;
        friend class VolatileRegister<MACA3LR, AccessType::ReadWrite>;
    };

    VolatileRegister<MACCR, AccessType::ReadWrite> maccr;         /*!< Ethernet MAC configuration register (ETH_MACCR)	Address offset: 0x0 */
    VolatileRegister<MACFFR, AccessType::ReadWrite> macffr;       /*!< Ethernet MAC frame filter register (ETH_MACCFFR)	Address offset: 0x4 */
    VolatileRegister<MACHTHR, AccessType::ReadWrite> machthr;     /*!< Ethernet MAC hash table high register	Address offset: 0x8 */
    VolatileRegister<MACHTLR, AccessType::ReadWrite> machtlr;     /*!< Ethernet MAC hash table low register	Address offset: 0xC */
    VolatileRegister<MACMIIAR, AccessType::ReadWrite> macmiiar;   /*!< Ethernet MAC MII address register (ETH_MACMIIAR)	Address offset: 0x10 */
    VolatileRegister<MACMIIDR, AccessType::ReadWrite> macmiidr;   /*!< Ethernet MAC MII data register (ETH_MACMIIDR)	Address offset: 0x14 */
    VolatileRegister<MACFCR, AccessType::ReadWrite> macfcr;       /*!< Ethernet MAC flow control register (ETH_MACFCR)	Address offset: 0x18 */
    VolatileRegister<MACVLANTR, AccessType::ReadWrite> macvlantr; /*!< Ethernet MAC VLAN tag register (ETH_MACVLANTR)	Address offset: 0x1C */
    uint32_t reserved0[2];                                        /*!< Reserved register	Address offset: 0x20 */
    VolatileRegister<MACRWUFFR, AccessType::ReadWrite>
        macrwuffr; /*!< Ethernet MAC remote wakeup frame filter register (ETH_MACRWUFFR)	Address offset: 0x28 */
    VolatileRegister<MACPMTCSR, AccessType::ReadWrite>
        macpmtcsr;                                          /*!< Ethernet MAC PMT control and status register (ETH_MACPMTCSR)	Address offset: 0x2C */
    uint32_t reserved1[2];                                  /*!< Reserved register	Address offset: 0x30 */
    VolatileRegister<MACSR, AccessType::ReadWrite> macsr;   /*!< Ethernet MAC interrupt status register (ETH_MACSR)	Address offset: 0x38 */
    VolatileRegister<MACIMR, AccessType::ReadWrite> macimr; /*!< Ethernet MAC interrupt mask register (ETH_MACIMR)	Address offset: 0x3C */
    VolatileRegister<MACA0HR, AccessType::ReadWrite> maca0hr; /*!< Ethernet MAC address 0 high register (ETH_MACA0HR)	Address offset: 0x40 */
    VolatileRegister<MACA0LR, AccessType::ReadWrite> maca0lr; /*!< Ethernet MAC address 0 low register	Address offset: 0x44 */
    VolatileRegister<MACAxHR, AccessType::ReadWrite> maca1hr; /*!< Ethernet MAC address 1 high register (ETH_MACA1HR)	Address offset: 0x48 */
    VolatileRegister<MACAxLR, AccessType::ReadWrite> maca1lr; /*!< Ethernet MAC address1 low register	Address offset: 0x4C */
    VolatileRegister<MACAxHR, AccessType::ReadWrite> maca2hr; /*!< Ethernet MAC address 2 high register (ETH_MACA2HR)	Address offset: 0x50 */
    VolatileRegister<MACAxLR, AccessType::ReadWrite> maca2lr; /*!< Ethernet MAC address 2 low register	Address offset: 0x54 */
    VolatileRegister<MACAxHR, AccessType::ReadWrite> maca3hr; /*!< Ethernet MAC address 3 high register (ETH_MACA3HR)	Address offset: 0x58 */
    VolatileRegister<MACAxLR, AccessType::ReadWrite> maca3lr; /*!< Ethernet MAC address 3 low register	Address offset: 0x5C */
};

#if defined(_MICROHAL_ETHERNET_MAC_BASE_ADDRESS)
[[maybe_unused]] static ETHERNET_MAC *const ethernetMac = reinterpret_cast<ETHERNET_MAC *>(_MICROHAL_ETHERNET_MAC_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_MAC
