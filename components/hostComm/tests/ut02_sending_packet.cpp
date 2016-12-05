/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      hostComm unit test

 @authors    Pawel Okas
 created on: 14-09-2015
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2015, microHAL
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

 *//* ========================================================================================================================== */

#include "microhal.h"
#include "hostComm/hostComm.h"
#include "diagnostic/diagnostic.h"
#include "microhal_bsp.h"

#include "ut_common.h"

#include <thread>

#include "catch.hpp"
#include "testPacket.h"


using namespace microhal;
using namespace diagnostic;
using namespace std::chrono_literals;


static void proceedPacket(HostCommPacket &packet) {
	diagChannel << lock << Debug << "Received packet, type = " << toHex(packet.getType()) << endl << unlock;
}

TEST_CASE ("Packet sending", "[packet sending]") {

	// ports should be open
	REQUIRE(communicationPortA.isOpen());
	REQUIRE(communicationPortB.isOpen());

	//clear ports
	REQUIRE(communicationPortA.clear());
	REQUIRE(communicationPortB.clear());

	HostComm hostCommA(communicationPortA, debugPort, "HostComm A: ");
	HostComm hostCommB(communicationPortB, debugPort, "HostComm B: ");

	enum {
		PacketType = 100
	};

	INFO ( "Starting timeProc thread.");

	//create and run hostComm proc task
	hostCommA.startHostCommThread();
	hostCommB.startHostCommThread();

	INFO ( "Connecting incomming packet slot.");
	//connect function that will be called when new packet will be received
	hostCommB.incommingPacket.connect(proceedPacket);

	REQUIRE(hostCommA.ping(true));
	REQUIRE(hostCommB.ping(true));

	SECTION("Sending no ACK packet")
	{
		HostCommPacket packet(PacketType, false);

		CHECK (hostCommA.send(packet));
	}

	SECTION("Sending ACK packet")
	{
		HostCommPacket packet(PacketType, true);

		CHECK (hostCommA.send(packet));
	}

	SECTION("Sending no ACK packet with data")
	{
		testPacket_noACK packet;
		packet.payload().setCounter(0xCCDDEEFF);

		uint32_t i = 2;
		while (i--) {
			CHECK (hostCommA.send(packet));
		}
	}

	SECTION("Sending ACK packet with data")
	{
		testPacket_ACK packet;
		packet.payload().setCounter(0xAABBCCDD);

		uint32_t i = 2;
		while (i--) {
			INFO ( "Sending packet");
			CHECK (hostCommA.send(packet));
		}
	}

	SECTION("Sending no ACK packet with data CRC check")
	{
		testPacket_noACK_CRC packet;
		packet.payload().setCounter(0xCCDDEEFF);

		uint32_t i = 2;
		while (i--) {
			CHECK (hostCommA.send(packet));
		}
	}

	SECTION("Sending ACK packet with data CRC check")
	{
		testPacket_ACK_CRC packet;
		packet.payload().setCounter(0xAABBCCDD);

		uint32_t i = 2;
		while (i--) {
			INFO ( "Sending packet");
			CHECK (hostCommA.send(packet));
		}
	}

	INFO ( "End of test, join proc thread.");
	hostCommA.stopHostCommThread();
	hostCommB.stopHostCommThread();
}
