/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 7-10-2020
 *
 * @copyright Copyright (c) 2020, Pawel Okas
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

#ifndef MICROHAL_CANOVERHOSTCOMM_H_
#define MICROHAL_CANOVERHOSTCOMM_H_

#include <list>
#include <queue>
#include "can/canInterface.h"
#include "canPacket.h"
#include "hostComm.h"

namespace microhal {
namespace communication {
class CanOverHostcomm;

class HostcommCAN : public microhal::can::CAN_Interface {
 public:
    using Message = microhal::can::Message;
    using Filter = microhal::can::Filter;

    HostcommCAN(microhal::HostComm &hostComm) : hostComm(hostComm) {}

    bool isProtocolSupported(Protocol protocol) final { return false; }

    bool transmit(const Message &message) final {
        CANPacket packet;
        packet.payload() = message;
        return hostComm.send(packet);
    }
    bool receive(Message &message) final {
        std::lock_guard<decltype(messagesMutex)> g(messagesMutex);
        if (!messages.empty()) {
            message = messages.front();
            messages.pop();
            return true;
        }
        return false;
    }

    bool waitForTransmitFinish(std::chrono::milliseconds timeout) const noexcept final { return transmitFinishSemaphore.wait(timeout); }
    bool waitForMessage(std::chrono::milliseconds timeout) const noexcept final { return semaphore.wait(timeout); }

    bool addFilter(const Filter &filter) final {
        std::lock_guard<decltype(filtersMutex)> g(filtersMutex);
        filters.push_back(filter);
        return true;
    }
    bool removeFilter(const Filter &filter) final {
        std::lock_guard<decltype(filtersMutex)> g(filtersMutex);
        auto size = filters.size();
        filters.remove(filter);
        return size > filters.size();
    }

    uint32_t receiveErrorCount() const final { return receiveErrors; }
    uint32_t transmitErrorCount() const final { return transmitErrors; }

 private:
    Protocol supportedProtocol{};
    uint32_t receiveErrors = 0;
    uint32_t transmitErrors = 0;
    std::list<Filter> filters;
    std::queue<Message> messages;
    microhal::HostComm &hostComm;
    mutable microhal::os::Semaphore semaphore;
    mutable microhal::os::Semaphore transmitFinishSemaphore;
    mutable std::mutex messagesMutex;
    mutable std::mutex filtersMutex;

    friend CanOverHostcomm;
};

class CanOverHostcomm {
 public:
    using Message = microhal::can::Message;
    using Filter = microhal::can::Filter;

    CanOverHostcomm(microhal::IODevice &hostcommPort, microhal::IODevice &log, const char *logHeader = "CAN over Hostcomm");
    virtual ~CanOverHostcomm();

    bool ping() { return hostComm.ping(true); }

    HostcommCAN &getCan() {
        canInterfaces.emplace_back(hostComm);
        return canInterfaces.back();
    }

 private:
    microhal::can::CAN_Interface::Protocol supportedProtocol{};
    microhal::HostComm hostComm;
    std::list<HostcommCAN> canInterfaces;

    void proceedPacket(microhal::HostCommPacket &packet);
    microhal::Slot_1<CanOverHostcomm, microhal::HostCommPacket &, &CanOverHostcomm::proceedPacket> proceedPacket_slot;
};

}  // namespace communication
}  // namespace microhal

#endif /* MICROHAL_CANOVERHOSTCOMM_H_ */
