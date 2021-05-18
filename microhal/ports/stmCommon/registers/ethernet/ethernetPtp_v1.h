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

#ifndef _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_PTP
#define _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_PTP

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32F107, STM32F103, STM32F101
namespace microhal {
namespace registers {
/**
 *
 */
struct ETHERNET_PTP {
    // Ethernet PTP time stamp control register (ETH_PTPTSCR)
    union PTPTSCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> TSE;   /*!< Time stamp enable */
            microhal::Bitfield<uint32_t, 1, 1> TSFCU; /*!< Time stamp fine or coarse update */
            microhal::Bitfield<uint32_t, 2, 1> TSSTI; /*!< Time stamp system time initialize */
            microhal::Bitfield<uint32_t, 3, 1> TSSTU; /*!< Time stamp system time update */
            microhal::Bitfield<uint32_t, 4, 1> TSITE; /*!< Time stamp interrupt trigger enable */
            microhal::Bitfield<uint32_t, 5, 1> TSARU; /*!< Time stamp addend register update */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PTPTSCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PTPTSCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PTPTSCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PTPTSCR operator&(uint32_t value) const {
            PTPTSCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PTPTSCR operator|(uint32_t value) const {
            PTPTSCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PTPTSCR, AccessType::ReadOnly>;
        friend class VolatileRegister<PTPTSCR, AccessType::WriteOnly>;
        friend class VolatileRegister<PTPTSCR, AccessType::ReadWrite>;
    };

    // Ethernet PTP subsecond increment register
    union PTPSSIR {
        union {
            microhal::Bitfield<uint32_t, 0, 8> STSSI; /*!< System time subsecond increment */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PTPSSIR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PTPSSIR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PTPSSIR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PTPSSIR operator&(uint32_t value) const {
            PTPSSIR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PTPSSIR operator|(uint32_t value) const {
            PTPSSIR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PTPSSIR, AccessType::ReadOnly>;
        friend class VolatileRegister<PTPSSIR, AccessType::WriteOnly>;
        friend class VolatileRegister<PTPSSIR, AccessType::ReadWrite>;
    };

    // Ethernet PTP time stamp high register
    union PTPTSHR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> STS; /*!< System time second */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PTPTSHR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PTPTSHR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PTPTSHR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PTPTSHR operator&(uint32_t value) const {
            PTPTSHR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PTPTSHR operator|(uint32_t value) const {
            PTPTSHR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PTPTSHR, AccessType::ReadOnly>;
        friend class VolatileRegister<PTPTSHR, AccessType::WriteOnly>;
        friend class VolatileRegister<PTPTSHR, AccessType::ReadWrite>;
    };

    // Ethernet PTP time stamp low register (ETH_PTPTSLR)
    union PTPTSLR {
        union {
            microhal::Bitfield<uint32_t, 0, 31> STSS;  /*!< System time subseconds */
            microhal::Bitfield<uint32_t, 31, 1> STPNS; /*!< System time positive or negative sign */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PTPTSLR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PTPTSLR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PTPTSLR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PTPTSLR operator&(uint32_t value) const {
            PTPTSLR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PTPTSLR operator|(uint32_t value) const {
            PTPTSLR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PTPTSLR, AccessType::ReadOnly>;
        friend class VolatileRegister<PTPTSLR, AccessType::WriteOnly>;
        friend class VolatileRegister<PTPTSLR, AccessType::ReadWrite>;
    };

    // Ethernet PTP time stamp high update register
    union PTPTSHUR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> TSUS; /*!< Time stamp update second */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PTPTSHUR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PTPTSHUR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PTPTSHUR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PTPTSHUR operator&(uint32_t value) const {
            PTPTSHUR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PTPTSHUR operator|(uint32_t value) const {
            PTPTSHUR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PTPTSHUR, AccessType::ReadOnly>;
        friend class VolatileRegister<PTPTSHUR, AccessType::WriteOnly>;
        friend class VolatileRegister<PTPTSHUR, AccessType::ReadWrite>;
    };

    // Ethernet PTP time stamp low update register (ETH_PTPTSLUR)
    union PTPTSLUR {
        union {
            microhal::Bitfield<uint32_t, 0, 31> TSUSS;  /*!< Time stamp update subseconds */
            microhal::Bitfield<uint32_t, 31, 1> TSUPNS; /*!< Time stamp update positive or negative sign */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PTPTSLUR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PTPTSLUR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PTPTSLUR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PTPTSLUR operator&(uint32_t value) const {
            PTPTSLUR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PTPTSLUR operator|(uint32_t value) const {
            PTPTSLUR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PTPTSLUR, AccessType::ReadOnly>;
        friend class VolatileRegister<PTPTSLUR, AccessType::WriteOnly>;
        friend class VolatileRegister<PTPTSLUR, AccessType::ReadWrite>;
    };

    // Ethernet PTP time stamp addend register
    union PTPTSAR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> TSA; /*!< Time stamp addend */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PTPTSAR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PTPTSAR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PTPTSAR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PTPTSAR operator&(uint32_t value) const {
            PTPTSAR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PTPTSAR operator|(uint32_t value) const {
            PTPTSAR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PTPTSAR, AccessType::ReadOnly>;
        friend class VolatileRegister<PTPTSAR, AccessType::WriteOnly>;
        friend class VolatileRegister<PTPTSAR, AccessType::ReadWrite>;
    };

    // Ethernet PTP target time high register
    union PTPTTHR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> TTSH; /*!< Target time stamp high */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PTPTTHR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PTPTTHR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PTPTTHR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PTPTTHR operator&(uint32_t value) const {
            PTPTTHR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PTPTTHR operator|(uint32_t value) const {
            PTPTTHR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PTPTTHR, AccessType::ReadOnly>;
        friend class VolatileRegister<PTPTTHR, AccessType::WriteOnly>;
        friend class VolatileRegister<PTPTTHR, AccessType::ReadWrite>;
    };

    // Ethernet PTP target time low register
    union PTPTTLR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> TTSL; /*!< Target time stamp low */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        PTPTTLR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        PTPTTLR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        PTPTTLR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        PTPTTLR operator&(uint32_t value) const {
            PTPTTLR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        PTPTTLR operator|(uint32_t value) const {
            PTPTTLR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<PTPTTLR, AccessType::ReadOnly>;
        friend class VolatileRegister<PTPTTLR, AccessType::WriteOnly>;
        friend class VolatileRegister<PTPTTLR, AccessType::ReadWrite>;
    };

    VolatileRegister<PTPTSCR, AccessType::ReadWrite> ptptscr;   /*!< Ethernet PTP time stamp control register (ETH_PTPTSCR)	Address offset: 0x0 */
    VolatileRegister<PTPSSIR, AccessType::ReadWrite> ptpssir;   /*!< Ethernet PTP subsecond increment register	Address offset: 0x4 */
    VolatileRegister<PTPTSHR, AccessType::ReadWrite> ptptshr;   /*!< Ethernet PTP time stamp high register	Address offset: 0x8 */
    VolatileRegister<PTPTSLR, AccessType::ReadWrite> ptptslr;   /*!< Ethernet PTP time stamp low register (ETH_PTPTSLR)	Address offset: 0xC */
    VolatileRegister<PTPTSHUR, AccessType::ReadWrite> ptptshur; /*!< Ethernet PTP time stamp high update register	Address offset: 0x10 */
    VolatileRegister<PTPTSLUR, AccessType::ReadWrite>
        ptptslur;                                             /*!< Ethernet PTP time stamp low update register (ETH_PTPTSLUR)	Address offset: 0x14 */
    VolatileRegister<PTPTSAR, AccessType::ReadWrite> ptptsar; /*!< Ethernet PTP time stamp addend register	Address offset: 0x18 */
    VolatileRegister<PTPTTHR, AccessType::ReadWrite> ptptthr; /*!< Ethernet PTP target time high register	Address offset: 0x1C */
    VolatileRegister<PTPTTLR, AccessType::ReadWrite> ptpttlr; /*!< Ethernet PTP target time low register	Address offset: 0x20 */
};
}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_PTP
