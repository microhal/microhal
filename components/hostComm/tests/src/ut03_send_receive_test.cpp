#include "doctest.h"

#include <thread>
#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "hostComm.h"
#include "microhal.h"
#include "testPacket.h"
#include "ut_common.h"

using namespace microhal;
using namespace diagnostic;
using namespace std::chrono_literals;

HostCommPacket *received = nullptr;

static void proceedPacket(HostCommPacket &packet) {
    received = &packet;
}

TEST_CASE("Packet sending") {
    // ports should be open
    REQUIRE(communicationPortA.isOpen());
    REQUIRE(communicationPortB.isOpen());

    // clear ports
    REQUIRE(communicationPortA.clear());
    REQUIRE(communicationPortB.clear());

    HostComm hostCommA(communicationPortA, debugPort, "HostComm A: ");
    HostComm hostCommB(communicationPortB, debugPort, "HostComm B: ");

    enum { PacketType = 100 };

    INFO("Starting timeProc thread.");

    // create and run hostComm proc task
    hostCommA.startHostCommThread();
    hostCommB.startHostCommThread();

    INFO("Connecting incoming packet slot.");
    // connect function that will be called when new packet will be received
    hostCommB.incommingPacket.connect(proceedPacket);

    REQUIRE(hostCommA.ping(true));
    REQUIRE(hostCommB.ping(true));

    SUBCASE("Sending no ACK packet") {
        HostCommPacket packet(PacketType, false);

        CHECK(hostCommA.send(packet));
        std::this_thread::sleep_for(100ms);
        CHECK(received->getType() == packet.getType());
        CHECK(received->getSize() == packet.getSize());
    }

    SUBCASE("Sending ACK packet") {
        HostCommPacket packet(PacketType, true);

        CHECK(hostCommA.send(packet));
        std::this_thread::sleep_for(100ms);
        CHECK(received->getType() == packet.getType());
        CHECK(received->getSize() == packet.getSize());
    }

    SUBCASE("Sending no ACK packet with data") {
        testPacket_noACK packet;
        packet.payload().setCounter(0xCCDDEEFF);

        REQUIRE(hostCommA.send(packet));

        std::this_thread::sleep_for(100ms);
        CHECK(received->getType() == packet.getType());
        CHECK(received->getSize() == packet.getSize());
        const testPacket_noACK *ptr = static_cast<testPacket_noACK *>(received);
        CHECK(ptr->payload().getCounter() == packet.payload().getCounter());
    }

    SUBCASE("Sending ACK packet with data") {
        testPacket_ACK packet;
        packet.payload().setCounter(0xAABBCCDD);

        REQUIRE(hostCommA.send(packet));

        std::this_thread::sleep_for(100ms);
        CHECK(received->getType() == packet.getType());
        CHECK(received->getSize() == packet.getSize());
        const testPacket_ACK *ptr = static_cast<testPacket_ACK *>(received);
        CHECK(ptr->payload().getCounter() == packet.payload().getCounter());
    }

    SUBCASE("Sending no ACK packet with data CRC check") {
        testPacket_noACK_CRC packet;
        packet.payload().setCounter(0xCCDDEEFF);

        uint32_t i = 2;
        while (i--) {
            CHECK(hostCommA.send(packet));
        }
    }

    SUBCASE("Sending ACK packet with data CRC check") {
        testPacket_ACK_CRC packet;
        packet.payload().setCounter(0xAABBCCDD);

        uint32_t i = 2;
        while (i--) {
            INFO("Sending packet");
            CHECK(hostCommA.send(packet));
        }
    }

    INFO("End of test, join proc thread.");
    hostCommA.stopHostCommThread();
    hostCommB.stopHostCommThread();
}
/*
 * ut03_send_receive_test.cpp
 *
 *  Created on: Nov 26, 2020
 *      Author: pokas
 */
