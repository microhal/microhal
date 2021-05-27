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

#include "canOverHostcomm.h"
#include "canPacket.h"
#include "microhal.h"
#include "os.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals;

namespace microhal {
namespace communication {

CanOverHostcomm::CanOverHostcomm(microhal::IODevice &hostcommPort, microhal::IODevice &log, const char *logHeader)
    : hostComm(hostcommPort, log, logHeader) {
    hostComm.incommingPacket.connect(proceedPacket_slot, *this);
    hostComm.startHostCommThread();
}

CanOverHostcomm::~CanOverHostcomm() {
    hostComm.stopHostCommThread();
}

void CanOverHostcomm::proceedPacket(microhal::HostCommPacket &packet) {
    if (packet.getType() == CANPacket::PacketType) {
        diagChannel << Debug << "Got CANPacket.";
        CANPacket &packetC = static_cast<CANPacket &>(packet);
        auto message = packetC.payload();
        diagChannel << lock << MICROHAL_DEBUG << "Received packet." << message << endl << unlock;

        for (auto &interface : canInterfaces) {
            for (auto filter : interface.filters) {
                if (filter.match(message)) {
                    {
                        std::lock_guard<std::mutex> g(interface.messagesMutex);
                        interface.messages.push(message);
                    }
                    interface.semaphore.give();
                }
            }
        }
    }
}

}  // namespace communication
}  // namespace microhal
