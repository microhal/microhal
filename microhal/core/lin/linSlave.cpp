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

#include <lin/linSlave.h>
#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
#include "diagnostic/diagnostic.h"

using namespace microhal::diagnostic;
#endif

namespace microhal {
namespace lin {

Result<Header, LINSlave::Error, LINSlave::Error::None> LINSlave::readHeader(std::chrono::milliseconds timeout) {
    Header header;
    Error status = readHeader_to(header, timeout);
    if (status == Error::None) {
        if (header.sync != 0x55) return Error::IncorrectSync;
        auto tmp = header.protectedIdentifier.raw;
        header.protectedIdentifier.calculateParity();
        if (header.protectedIdentifier.raw != tmp) return Error::IncorrectHeaderParity;
        return header;
    }
    return status;
}

microhal::Result<Frame *, LINSlave::Error, LINSlave::Error::None> LINSlave::readFrame(Frame &frame, std::chrono::milliseconds timeout) {
    Error status = read_to((uint8_t *)&frame.header, frame.size(), timeout);
    if (status == Error::None) {
        if (frame.header.sync != 0x55) return Error::IncorrectSync;
        Header header = frame.header;
        header.protectedIdentifier.calculateParity();
        if (header.protectedIdentifier.raw != frame.header.protectedIdentifier.raw) return Error::IncorrectHeaderParity;

        if (frame.isChecksumValid()) {
            return &frame;
        }
        return Error::IncorrectChecksum;
    }
    return status;
}

LINSlave::Error LINSlave::sendResponse(Frame &frame, std::chrono::milliseconds timeout) {
    frame.updateChecksum();
#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
    diagChannel << lock << MICROHAL_DEBUG << "Sending LIN response: " << toHex(frame.data, frame.dataLen + 1) << unlock;
#endif
    return write({frame.data, frame.dataLen + 1}, timeout);
}

} /* namespace lin */
} /* namespace microhal */
