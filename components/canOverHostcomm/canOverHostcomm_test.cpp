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

#include "bsp.h"
#include "canOverHostcomm.h"
#include "doctest.h"

#include "IODevice/virtualIODevice.h"

using namespace microhal;
using namespace std::literals;

namespace microhal {
namespace communication {

TEST_CASE("Testing CanOverHostcomm class") {
    VirtualIODevice pcDevice;
    VirtualIODevice rotorDevice;
    CanOverHostcomm canOnPC(pcDevice, bsp::debugPort);
    CanOverHostcomm canOnRotor(rotorDevice, bsp::debugPort);

    REQUIRE(pcDevice.connect(rotorDevice));
    REQUIRE(rotorDevice.connect(pcDevice));
    REQUIRE(pcDevice.open(IODevice::ReadWrite));
    REQUIRE(rotorDevice.open(IODevice::ReadWrite));

    CANOver& can1 = canOnPC.getCan();
    CANOver& rotorCan = canOnRotor.getCan();
    can::Filter filter(can::Message::StandardID(8),
                       can::Filter::Match::DataFrame | can::Filter::Match::RemoteFrame | can::Filter::Match::StandardIDAndExtendedID);
    rotorCan.addFilter(filter);

    can::Message message(can::Message::StandardID(8));
    CHECK(can1.transmit(message));
    std::this_thread::sleep_for(500ms);
    can::Message recMessage;
    CHECK(rotorCan.receive(recMessage));
    CHECK(message == recMessage);
    CHECK_FALSE(rotorCan.receive(recMessage));

    message.setStandardID(9);
    CHECK(can1.transmit(message));
    std::this_thread::sleep_for(500ms);
    CHECK_FALSE(rotorCan.receive(recMessage));

    message.setExtendedID(8);
    CHECK(can1.transmit(message));
    std::this_thread::sleep_for(500ms);
    CHECK_FALSE(rotorCan.receive(recMessage));

    SUBCASE("") {
        can::Filter filter(can::Message::StandardID(8),
                           can::Filter::Match::DataFrame | can::Filter::Match::RemoteFrame | can::Filter::Match::StandardIDAndExtendedID);
        can::Filter filter2(can::Message::StandardID(9),
                            can::Filter::Match::DataFrame | can::Filter::Match::RemoteFrame | can::Filter::Match::StandardIDAndExtendedID);
        rotorCan.addFilter(filter);
        rotorCan.addFilter(filter2);

        can::Message message(can::Message::StandardID(8));
        CANOver& can2 = canOnPC.getCan();
        CHECK(can1.transmit(message));
        message.setStandardID(9);
        CHECK(can2.transmit(message));
        std::this_thread::sleep_for(500ms);

        CHECK(rotorCan.receive(recMessage));
        CHECK(rotorCan.receive(recMessage));
    }
}

}  // namespace communication
}  // namespace microhal
