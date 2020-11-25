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

#ifndef MICROHAL_CANOVERHOSTCOMM_CAN_PACKET_H_
#define MICROHAL_CANOVERHOSTCOMM_CAN_PACKET_H_

#include "can/canMessage.h"
#include "diagnostic/diagnostic.h"
#include "hostCommPacket.h"

#ifndef MICROHAL_CAN_OVER_HOSTCOMM_MESSAGE_ID
#define MICROHAL_CAN_OVER_HOSTCOMM_MESSAGE_ID 0x21
#endif

namespace microhal {
namespace communication {

class CANPacket : public microhal::HostCommDataPacket<microhal::can::Message, MICROHAL_CAN_OVER_HOSTCOMM_MESSAGE_ID> {
 public:
    CANPacket() : microhal::HostCommDataPacket<microhal::can::Message, MICROHAL_CAN_OVER_HOSTCOMM_MESSAGE_ID>::HostCommDataPacket(false, false) {}
    ~CANPacket() {}

    enum { Request = MICROHAL_CAN_OVER_HOSTCOMM_MESSAGE_ID - 1 };
};

}  // namespace communication
}  // namespace microhal

#endif /* MICROHAL_CANOVERHOSTCOMM_CAN_PACKET_H_ */
