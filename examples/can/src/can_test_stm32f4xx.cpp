/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
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

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "doctest.h"
#include "microhal.h"
#include "ports/stm32f4xx/can_stm32f4xx.h"

using namespace std::literals::chrono_literals;

using namespace microhal;
using namespace diagnostic;

using CAN = stm32f4xx::CAN;
CAN can1(CAN::Device::CAN1);

can::Message message;

TEST_CASE("Message test") {
    can::Message message;
    CHECK_FALSE(message.isExtendedID());
    CHECK_FALSE(message.isStandardID());

    message.setExtendedID(0xFFFF);
    CHECK_FALSE(message.isStandardID());
    CHECK(message.isExtendedID());
    CHECK(message.getID().getID() == 0xFFFF);

    message.setStandardID(0xFF);
    CHECK_FALSE(message.isExtendedID());
    CHECK(message.isStandardID());
    CHECK(message.getID().getID() == 0xFF);

    // try set remote request on message but with to big size
    CHECK_FALSE(message.setRemoteRequest(9));
    // try set remote request on message
    CHECK(message.setRemoteRequest(8));
    CHECK(message.isRemoteFrame());
    CHECK_FALSE(message.isDataFrame());

    std::array<uint8_t, 9> toBigData{1, 2, 3, 4, 5, 6, 7, 8, 9};
    CHECK_FALSE(message.setDataFrame(toBigData));

    std::array<uint8_t, 4> data{1, 2, 3, 4};
    CHECK(message.setDataFrame(data));
    CHECK(message.isDataFrame());
    CHECK_FALSE(message.isRemoteFrame());
}

TEST_CASE("Transmit test") {
    CHECK(can1.setBaudrate(30000) == 30000);
    can1.setMode(CAN::Mode::LoopbackAndSilent);
    can1.sleepMode(CAN::Sleep::AutoWakeup);

    CHECK(can1.emptyTxMailboxCount() == 3);
    message.setStandardID(10);
    uint8_t data[] = {'O', 'K'};
    CHECK(message.setDataFrame(data));
    CHECK(can1.transmit(message));
    CHECK(can1.emptyTxMailboxCount() == 2);
    CHECK(can1.waitForTransmitFinish(std::chrono::milliseconds{100}));
    CHECK(can1.emptyTxMailboxCount() == 3);
}

TEST_CASE("Loopback test") {
    can1.setBaudrate(20000);
    can1.setMode(CAN::Mode::LoopbackAndSilent);
    can1.setMode(CAN::Mode::Loopback);
    can1.sleepMode(CAN::Sleep::AutoWakeup);
    REQUIRE(can1.addFilter(can::Message::StandardID{10}, 0, false));
    message.setStandardID(10);
    uint8_t data[] = {'O', 'K'};
    message.setDataFrame(data);
    CHECK(can1.transmit(message));
    CHECK(can1.waitForTransmitFinish(std::chrono::milliseconds{100}));
    CHECK(can1.getLastError() == CAN::None);
    CHECK(can1.transmitErrorCount() == 0);

    REQUIRE(can1.waitForMessage(10s));
    can::Message recMessage;
    CHECK(can1.receive(recMessage));
    CHECK(can1.getLastError() == CAN::None);
    CHECK(can1.receiveErrorCount() == 0);
    CHECK(message == recMessage);
    CHECK(can1.removeFilter(can::Message::StandardID{10}, 0, false));
}

TEST_CASE("Filter test") {
    std::array<uint8_t, 4> data{1, 2, 3, 4};
    message.setDataFrame(data);
    message.setExtendedID(20);
    CHECK(can1.transmit(message));
    CHECK(can1.waitForTransmitFinish(std::chrono::milliseconds{100}));

    can::Message recMessage;
    CHECK_FALSE(can1.receive(recMessage));
    // set filter for standard id = 20
    can1.addFilter(can::Message::StandardID{20}, 20, false);
    CHECK(can1.transmit(message));
    CHECK(can1.waitForTransmitFinish(std::chrono::milliseconds{100}));
    CHECK(can1.receive(recMessage));
}
