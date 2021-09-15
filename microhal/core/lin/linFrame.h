/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 13-03-2021
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

#ifndef _MICROHAL_LIN_LINFRAME_H_
#define _MICROHAL_LIN_LINFRAME_H_

#include <utils/bitfield.h>
#include <utils/packed.h>
#include <span>
#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
#include "diagnostic/diagnostic.h"
#endif

namespace microhal {
namespace lin {

struct FrameInfo {
    uint8_t isRequest : 1;
    uint8_t isEnhancedChacksum : 1;
    uint8_t isInUse : 1;
    uint8_t length : 4;
};

struct Header {
    uint8_t sync = 0x55;
    union {
        uint8_t raw;
        union {
            microhal::Bitfield<uint8_t, 0, 6> frameIdentifier;
            microhal::Bitfield<uint8_t, 0, 1> id0;
            microhal::Bitfield<uint8_t, 1, 1> id1;
            microhal::Bitfield<uint8_t, 2, 1> id2;
            microhal::Bitfield<uint8_t, 3, 1> id3;
            microhal::Bitfield<uint8_t, 4, 1> id4;
            microhal::Bitfield<uint8_t, 5, 1> id5;
            microhal::Bitfield<uint8_t, 6, 2> parity;
            microhal::Bitfield<uint8_t, 7, 1> p1;
            microhal::Bitfield<uint8_t, 6, 1> p0;
        };
        void calculateParity() {
            p0 = id0 ^ id1 ^ id2 ^ id4;
            p1 = ~(id1 ^ id3 ^ id4 ^ id5);
        }
    } protectedIdentifier;

    uint8_t id() const { return protectedIdentifier.frameIdentifier; }
};

static_assert(sizeof(Header) == 2);

struct Frame {
    Header header;
    uint8_t data[9];  // data + checksum
    uint8_t dataLen = 8;

    static bool isRequest(const Frame &frame) { return isRequest(frame.header); }
    static bool isRequest(const Header &header);
    static uint_fast8_t dataLength(const Header &header);
    static bool isEnhancedChecksum(const Header &header);
    static bool isClasicChecksum(const Header &header) { return !isEnhancedChecksum(header); }

    uint8_t id() const { return header.id(); }
    uint_fast8_t size() const { return dataLen + 3; /* dataLen + sizeof(Header) + sizeof(checksum) */ }

    std::span<const uint8_t> payload() const { return std::span<const uint8_t>(data, dataLen); }

    bool isUnconditionalFrame() const { return id() <= 59; }
    bool isDiagnosticFrame() const { return id() == 60 || id() == 61; }
    bool isReserved() const { return id() == 62 || id() == 63; }

    uint32_t headerDurationNominal() const { return 34 * tbit; }
    uint32_t responseDurationNominal() const { return 10 * (dataLen + 1) * tbit; }
    uint32_t frameDurationNominal() const { return headerDurationNominal() + frameDurationNominal(); }
    uint32_t headerDurationMax() const { return 1.4 * headerDurationNominal(); }
    uint32_t responseDurationMax() const { return 1.4 * responseDurationNominal(); }
    uint32_t frameDurationMax() const { return headerDurationMax() + responseDurationMax(); }

    uint8_t tbit = 1;

    void updateChecksum();
    bool isChecksumValid() const;
    uint8_t calculateChecksum() const;
    uint8_t calculateClasicChecksum() const;
    uint8_t calculateEnhancedChecksum() const;

    bool operator!=(const Frame &rhs) const {
        if (header.id() != rhs.header.id()) return true;
        if (dataLen != rhs.dataLen) return true;
        for (uint_fast8_t i = 0; i < dataLen; i++) {
            if (data[i] != rhs.data[i]) return true;
        }
        return false;
    }

    bool operator==(const Frame &rhs) const { return !this->operator!=(rhs); }
};

}  // namespace lin
}  // namespace microhal

#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> &operator<<(microhal::diagnostic::LogLevelChannel<level, B> &logChannel,
                                                                   const microhal::lin::Header &header) {
    return logChannel << "Id: " << header.id();
}

template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> &operator<<(microhal::diagnostic::LogLevelChannel<level, B> &logChannel,
                                                                   const microhal::lin::Frame &frame) {
    return logChannel << "Id: " << frame.id() << ", data: " << microhal::diagnostic::toHex(frame.data, frame.dataLen)
                      << ", checksum valid: " << frame.isChecksumValid() << ", checksum enhanced: " << frame.isEnhancedChecksum(frame.header);
}

template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> &operator<<(microhal::diagnostic::LogLevelChannel<level, B> &logChannel,
                                                                   const microhal::lin::Frame *frame) {
    if (frame)
        return logChannel << *frame;
    else
        return logChannel << "nullptr";
}
#endif

#endif  // _MICROHAL_LIN_LINFRAME_H_
