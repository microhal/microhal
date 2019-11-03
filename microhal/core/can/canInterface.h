/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-03-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#ifndef _MICROHAL_CANINTERFACE_H_
#define _MICROHAL_CANINTERFACE_H_

#include <chrono>
#include <cstdint>
#include "canFilter.h"
#include "canMessage.h"

namespace microhal {
namespace can {

class CAN_Interface {
 public:
    enum class Protocol { v2_0A, V2_0B };

    virtual ~CAN_Interface() {}

    virtual bool isProtocolSupported(Protocol protocol) = 0;

    virtual bool transmit(const Message &message) = 0;
    virtual bool receive(Message &message) = 0;

    virtual bool waitForTransmitFinish(std::chrono::milliseconds timeout) const noexcept = 0;
    virtual bool waitForMessage(std::chrono::milliseconds timeout) const noexcept = 0;

    virtual bool addFilter(const Filter &filter) = 0;
    virtual bool removeFilter(const Filter &filter) = 0;
    virtual uint32_t receiveErrorCount() const = 0;
    virtual uint32_t transmitErrorCount() const = 0;
};

}  // namespace can
}  // namespace microhal

#endif /* _MICROHAL_CANINTERFACE_H_ */
