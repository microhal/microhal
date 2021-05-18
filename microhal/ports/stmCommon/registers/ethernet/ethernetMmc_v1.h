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

#ifndef _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_MMC
#define _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_MMC

#include "registersBaseAddressDefinitions.h"
#include "utils/bitfield.h"
#include "utils/volatileRegister.h"

// Supported MCU: STM32F107, STM32F103, STM32F101
namespace microhal {
namespace registers {
/**
 *
 */
struct ETHERNET_MMC {
    // Ethernet MMC control register (ETH_MMCCR)
    union MMCCR {
        union {
            microhal::Bitfield<uint32_t, 0, 1> CR;   /*!< Counter reset */
            microhal::Bitfield<uint32_t, 1, 1> CSR;  /*!< Counter stop rollover */
            microhal::Bitfield<uint32_t, 2, 1> ROR;  /*!< Reset on read */
            microhal::Bitfield<uint32_t, 31, 1> MCF; /*!< MMC counter freeze */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MMCCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MMCCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MMCCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MMCCR operator&(uint32_t value) const {
            MMCCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MMCCR operator|(uint32_t value) const {
            MMCCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MMCCR, AccessType::ReadOnly>;
        friend class VolatileRegister<MMCCR, AccessType::WriteOnly>;
        friend class VolatileRegister<MMCCR, AccessType::ReadWrite>;
    };

    // Ethernet MMC receive interrupt register (ETH_MMCRIR)
    union MMCRIR {
        union {
            microhal::Bitfield<uint32_t, 5, 1> RFCES;  /*!< Received frames CRC error status */
            microhal::Bitfield<uint32_t, 6, 1> RFAES;  /*!< Received frames alignment error status */
            microhal::Bitfield<uint32_t, 17, 1> RGUFS; /*!< Received Good Unicast Frames Status */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MMCRIR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MMCRIR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MMCRIR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MMCRIR operator&(uint32_t value) const {
            MMCRIR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MMCRIR operator|(uint32_t value) const {
            MMCRIR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MMCRIR, AccessType::ReadOnly>;
        friend class VolatileRegister<MMCRIR, AccessType::WriteOnly>;
        friend class VolatileRegister<MMCRIR, AccessType::ReadWrite>;
    };

    // Ethernet MMC transmit interrupt register (ETH_MMCTIR)
    union MMCTIR {
        union {
            microhal::Bitfield<uint32_t, 14, 1> TGFSCS;  /*!< Transmitted good frames single collision status */
            microhal::Bitfield<uint32_t, 15, 1> TGFMSCS; /*!< Transmitted good frames more single collision status */
            microhal::Bitfield<uint32_t, 21, 1> TGFS;    /*!< Transmitted good frames status */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MMCTIR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MMCTIR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MMCTIR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MMCTIR operator&(uint32_t value) const {
            MMCTIR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MMCTIR operator|(uint32_t value) const {
            MMCTIR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MMCTIR, AccessType::ReadOnly>;
        friend class VolatileRegister<MMCTIR, AccessType::WriteOnly>;
        friend class VolatileRegister<MMCTIR, AccessType::ReadWrite>;
    };

    // Ethernet MMC receive interrupt mask register (ETH_MMCRIMR)
    union MMCRIMR {
        union {
            microhal::Bitfield<uint32_t, 5, 1> RFCEM;  /*!< Received frame CRC error mask */
            microhal::Bitfield<uint32_t, 6, 1> RFAEM;  /*!< Received frames alignment error mask */
            microhal::Bitfield<uint32_t, 17, 1> RGUFM; /*!< Received good unicast frames mask */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MMCRIMR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MMCRIMR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MMCRIMR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MMCRIMR operator&(uint32_t value) const {
            MMCRIMR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MMCRIMR operator|(uint32_t value) const {
            MMCRIMR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MMCRIMR, AccessType::ReadOnly>;
        friend class VolatileRegister<MMCRIMR, AccessType::WriteOnly>;
        friend class VolatileRegister<MMCRIMR, AccessType::ReadWrite>;
    };

    // Ethernet MMC transmit interrupt mask register (ETH_MMCTIMR)
    union MMCTIMR {
        union {
            microhal::Bitfield<uint32_t, 14, 1> TGFSCM;  /*!< Transmitted good frames single collision mask */
            microhal::Bitfield<uint32_t, 15, 1> TGFMSCM; /*!< Transmitted good frames more single collision mask */
            microhal::Bitfield<uint32_t, 21, 1> TGFM;    /*!< Transmitted good frames mask */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MMCTIMR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MMCTIMR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MMCTIMR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MMCTIMR operator&(uint32_t value) const {
            MMCTIMR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MMCTIMR operator|(uint32_t value) const {
            MMCTIMR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MMCTIMR, AccessType::ReadOnly>;
        friend class VolatileRegister<MMCTIMR, AccessType::WriteOnly>;
        friend class VolatileRegister<MMCTIMR, AccessType::ReadWrite>;
    };

    // Ethernet MMC transmitted good frames after a single collision counter
    union MMCTGFSCCR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> TGFSCC; /*!< Transmitted good frames after a single collision counter */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MMCTGFSCCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MMCTGFSCCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MMCTGFSCCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MMCTGFSCCR operator&(uint32_t value) const {
            MMCTGFSCCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MMCTGFSCCR operator|(uint32_t value) const {
            MMCTGFSCCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MMCTGFSCCR, AccessType::ReadOnly>;
        friend class VolatileRegister<MMCTGFSCCR, AccessType::WriteOnly>;
        friend class VolatileRegister<MMCTGFSCCR, AccessType::ReadWrite>;
    };

    // Ethernet MMC transmitted good frames after more than a single collision
    union MMCTGFMSCCR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> TGFMSCC; /*!< Transmitted good frames after more than a single collision counter */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MMCTGFMSCCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MMCTGFMSCCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MMCTGFMSCCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MMCTGFMSCCR operator&(uint32_t value) const {
            MMCTGFMSCCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MMCTGFMSCCR operator|(uint32_t value) const {
            MMCTGFMSCCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MMCTGFMSCCR, AccessType::ReadOnly>;
        friend class VolatileRegister<MMCTGFMSCCR, AccessType::WriteOnly>;
        friend class VolatileRegister<MMCTGFMSCCR, AccessType::ReadWrite>;
    };

    // Ethernet MMC transmitted good frames counter register
    union MMCTGFCR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> TGFC; /*!< Transmitted good frames counter */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MMCTGFCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MMCTGFCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MMCTGFCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MMCTGFCR operator&(uint32_t value) const {
            MMCTGFCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MMCTGFCR operator|(uint32_t value) const {
            MMCTGFCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MMCTGFCR, AccessType::ReadOnly>;
        friend class VolatileRegister<MMCTGFCR, AccessType::WriteOnly>;
        friend class VolatileRegister<MMCTGFCR, AccessType::ReadWrite>;
    };

    // Ethernet MMC received frames with CRC error counter register
    union MMCRFCECR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> RFCFC; /*!< Received frames with CRC error counter */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MMCRFCECR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MMCRFCECR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MMCRFCECR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MMCRFCECR operator&(uint32_t value) const {
            MMCRFCECR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MMCRFCECR operator|(uint32_t value) const {
            MMCRFCECR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MMCRFCECR, AccessType::ReadOnly>;
        friend class VolatileRegister<MMCRFCECR, AccessType::WriteOnly>;
        friend class VolatileRegister<MMCRFCECR, AccessType::ReadWrite>;
    };

    // Ethernet MMC received frames with alignment error counter register
    union MMCRFAECR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> RFAEC; /*!< Received frames with alignment error counter */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MMCRFAECR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MMCRFAECR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MMCRFAECR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MMCRFAECR operator&(uint32_t value) const {
            MMCRFAECR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MMCRFAECR operator|(uint32_t value) const {
            MMCRFAECR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MMCRFAECR, AccessType::ReadOnly>;
        friend class VolatileRegister<MMCRFAECR, AccessType::WriteOnly>;
        friend class VolatileRegister<MMCRFAECR, AccessType::ReadWrite>;
    };

    // MMC received good unicast frames counter register
    union MMCRGUFCR {
        union {
            microhal::Bitfield<uint32_t, 0, 32> RGUFC; /*!< Received good unicast frames counter */
        };

        operator uint32_t() const { return raw; }
        operator bool() const { return raw; }

        MMCRGUFCR &operator=(uint32_t value) {
            raw = value;
            return *this;
        }
        MMCRGUFCR &operator|=(uint32_t value) {
            raw |= value;
            return *this;
        }
        MMCRGUFCR &operator&=(uint32_t value) {
            raw &= value;
            return *this;
        }

        bool operator==(uint32_t value) const { return raw == value; }
        bool operator!=(uint32_t value) const { return raw != value; }
        bool operator>(uint32_t value) const { return raw > value; }
        bool operator<(uint32_t value) const { return raw < value; }
        bool operator>=(uint32_t value) const { return raw >= value; }
        bool operator<=(uint32_t value) const { return raw <= value; }
        MMCRGUFCR operator&(uint32_t value) const {
            MMCRGUFCR tmp;
            tmp.raw = raw & value;
            return tmp;
        }
        MMCRGUFCR operator|(uint32_t value) const {
            MMCRGUFCR tmp;
            tmp.raw = raw | value;
            return tmp;
        }

     private:
        uint32_t raw;
        friend class VolatileRegister<MMCRGUFCR, AccessType::ReadOnly>;
        friend class VolatileRegister<MMCRGUFCR, AccessType::WriteOnly>;
        friend class VolatileRegister<MMCRGUFCR, AccessType::ReadWrite>;
    };

    VolatileRegister<MMCCR, AccessType::ReadWrite> mmccr;     /*!< Ethernet MMC control register (ETH_MMCCR)	Address offset: 0x0 */
    VolatileRegister<MMCRIR, AccessType::ReadWrite> mmcrir;   /*!< Ethernet MMC receive interrupt register (ETH_MMCRIR)	Address offset: 0x4 */
    VolatileRegister<MMCTIR, AccessType::ReadWrite> mmctir;   /*!< Ethernet MMC transmit interrupt register (ETH_MMCTIR)	Address offset: 0x8 */
    VolatileRegister<MMCRIMR, AccessType::ReadWrite> mmcrimr; /*!< Ethernet MMC receive interrupt mask register (ETH_MMCRIMR)	Address offset: 0xC */
    VolatileRegister<MMCTIMR, AccessType::ReadWrite>
        mmctimr;            /*!< Ethernet MMC transmit interrupt mask register (ETH_MMCTIMR)	Address offset: 0x10 */
    uint32_t reserved0[14]; /*!< Reserved register	Address offset: 0x14 */
    VolatileRegister<MMCTGFSCCR, AccessType::ReadWrite>
        mmctgfsccr; /*!< Ethernet MMC transmitted good frames after a single collision counter	Address offset: 0x4C */
    VolatileRegister<MMCTGFMSCCR, AccessType::ReadWrite>
        mmctgfmsccr;       /*!< Ethernet MMC transmitted good frames after more than a single collision	Address offset: 0x50 */
    uint32_t reserved1[5]; /*!< Reserved register	Address offset: 0x54 */
    VolatileRegister<MMCTGFCR, AccessType::ReadWrite> mmctgfcr; /*!< Ethernet MMC transmitted good frames counter register	Address offset: 0x68
                                                                 */
    uint32_t reserved2[10];                                     /*!< Reserved register	Address offset: 0x6c */
    VolatileRegister<MMCRFCECR, AccessType::ReadWrite>
        mmcrfcecr; /*!< Ethernet MMC received frames with CRC error counter register	Address offset: 0x94 */
    VolatileRegister<MMCRFAECR, AccessType::ReadWrite>
        mmcrfaecr;          /*!< Ethernet MMC received frames with alignment error counter register	Address offset: 0x98 */
    uint32_t reserved3[10]; /*!< Reserved register	Address offset: 0x9c */
    VolatileRegister<MMCRGUFCR, AccessType::ReadWrite> mmcrgufcr; /*!< MMC received good unicast frames counter register	Address offset: 0xC4
                                                                   */
};
}  // namespace registers
}  // namespace microhal
#endif  // _MICROHAL_PERIPHERAL_REGISTER_ETHERNET_MMC
