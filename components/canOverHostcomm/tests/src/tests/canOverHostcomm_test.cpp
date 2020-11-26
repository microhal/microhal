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

using namespace microhal;
using namespace std::literals;

namespace microhal {
namespace communication {

TEST_CASE("Testing CanOverHostcomm class") {
    CanOverHostcomm canOverHostcomm1(bsp::communicationPortA, bsp::debugPort);
    CanOverHostcomm canOverHostcomm2(bsp::communicationPortB, bsp::debugPort);

    HostcommCAN& canA1 = canOverHostcomm1.getCan();
    HostcommCAN& canB = canOverHostcomm2.getCan();
    can::Filter filter(can::Message::StandardID(8),
                       can::Filter::Match::DataFrame | can::Filter::Match::RemoteFrame | can::Filter::Match::StandardIDAndExtendedID);
    canB.addFilter(filter);

    can::Message message(can::Message::StandardID(8));
    CHECK(canA1.transmit(message));
    std::this_thread::sleep_for(500ms);
    can::Message recMessage;
    CHECK(canB.receive(recMessage));
    CHECK(message == recMessage);
    CHECK_FALSE(canB.receive(recMessage));

    message.setStandardID(9);
    CHECK(canA1.transmit(message));
    std::this_thread::sleep_for(500ms);
    CHECK_FALSE(canB.receive(recMessage));

    message.setExtendedID(8);
    CHECK(canA1.transmit(message));
    std::this_thread::sleep_for(500ms);
    CHECK_FALSE(canB.receive(recMessage));

    SUBCASE("") {
        can::Filter filter(can::Message::StandardID(8),
                           can::Filter::Match::DataFrame | can::Filter::Match::RemoteFrame | can::Filter::Match::StandardIDAndExtendedID);
        can::Filter filter2(can::Message::StandardID(9),
                            can::Filter::Match::DataFrame | can::Filter::Match::RemoteFrame | can::Filter::Match::StandardIDAndExtendedID);
        canB.addFilter(filter);
        canB.addFilter(filter2);

        can::Message message(can::Message::StandardID(8));
        HostcommCAN& canA2 = canOverHostcomm1.getCan();
        CHECK(canA1.transmit(message));
        message.setStandardID(9);
        CHECK(canA2.transmit(message));
        std::this_thread::sleep_for(500ms);

        CHECK(canB.receive(recMessage));
        CHECK(canB.receive(recMessage));
    }
}

}  // namespace communication
}  // namespace microhal
