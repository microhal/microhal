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

#include "linFrame.h"
#include <array>
#include <bitset>

namespace microhal {
namespace lin {

extern const std::bitset<64> requestFlags;
extern const std::bitset<64> enhancedChecksumFlags;
extern std::array<uint8_t, 64> linFramesDataLengths;

bool Frame::isRequest(const Header &header) {
    return requestFlags[header.id()];
}

bool Frame::isEnhancedChecksum(const Header &header) {
    return enhancedChecksumFlags[header.id()];
}

uint_fast8_t Frame::dataLength(const Header &header) {
    return linFramesDataLengths[header.id()];
}

bool Frame::isChecksumValid() const {
    return data[dataLen] == calculateChecksum();
}

void Frame::updateChecksum() {
    data[dataLen] = calculateChecksum();
}

uint8_t Frame::calculateClasicChecksum() const {
    uint16_t checksum = 0;

    for (size_t i = 0; i < dataLen; i++) {
        checksum += data[i];
        if (checksum >= 256) checksum -= 255;
    }
    return ~checksum;
}

uint8_t Frame::calculateEnhancedChecksum() const {
    uint16_t checksum = 0;  // id.protectedIdentifier;

    for (size_t i = 0; i < dataLen; i++) {
        checksum += data[i];
        if (checksum >= 256) checksum -= 255;
    }
    return ~checksum;
}

uint8_t Frame::calculateChecksum() const {
    if (isEnhancedChecksum(header)) {
        return calculateEnhancedChecksum();
    } else {
        return calculateClasicChecksum();
    }
}

}  // namespace lin
}  // namespace microhal
