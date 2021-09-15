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

static_assert(sizeof(FrameInfo) == 1);

constexpr auto linFrameInit() {
    std::array<FrameInfo, 64> framesInfo{};
    framesInfo[60] = lin::FrameInfo{.isRequest = 0, .isEnhancedChacksum = 0, .isInUse = 0, .length = 8};
    framesInfo[61] = lin::FrameInfo{.isRequest = 1, .isEnhancedChacksum = 0, .isInUse = 0, .length = 8};
    return framesInfo;
}

std::array<FrameInfo, 64> linFramesInfo = linFrameInit();

bool Frame::configureFrame(uint8_t id, const FrameInfo &info) {
    if (id < linFramesInfo.size()) {
        linFramesInfo[id] = info;
        return true;
    }
    return false;
}

bool Frame::isRequest(const Header &header) {
    return linFramesInfo[header.id()].isRequest;
}

bool Frame::isEnhancedChecksum(const Header &header) {
    return linFramesInfo[header.id()].isEnhancedChacksum;
}

uint_fast8_t Frame::dataLength(const Header &header) {
    return linFramesInfo[header.id()].length;
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
