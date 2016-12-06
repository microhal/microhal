/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      hostComm

 @authors    Pawel Okas
 created on: 13-06-2014
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 in the documentation and/or other materials provided with the distribution.
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */ /* ==========================================================================================================================
                                                                                                                                         */

#ifndef HOSTCOMMPACKETACK_H_
#define HOSTCOMMPACKETACK_H_

#include <stdint.h>

#include "diagnostic/diagnostic.h"
#include "hostCommPacket.h"

namespace microhal {

class HostCommPacket_ACK : public HostCommDataPacket<HostCommPacket::PacketInfo, HostCommPacket::ACK> {
 public:
    void setPacketToACK(HostCommPacket &packet) noexcept { payload() = packet.packetInfo; }

    bool isAcknowledged(HostCommPacket &packet) const noexcept {
        if (packet.packetInfo != payload()) return false;

        return true;
    }

    template <diagnostic::LogLevel level>
    void log(diagnostic::Diagnostic<level> &log = diagnostic::diagChannel) const noexcept {
        const HostCommPacket::PacketInfo *ACKinfo = payloadPtr();
        log << diagnostic::Debug << "control: " << ACKinfo->control << diagnostic::endl
            << "type: " << ACKinfo->type << diagnostic::endl
            << "size: " << ACKinfo->size << diagnostic::endl;
    }
};

}  // namespace microhal

#endif  // HOSTCOMMPACKETACK_H_
