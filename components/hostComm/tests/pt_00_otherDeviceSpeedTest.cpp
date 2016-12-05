/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      hostComm unit test, ping pong test

 @authors    Pawel Okas
 created on: 21-09-2015
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

 *//* ========================================================================================================================== */

#include "microhal.h"
#include "diagnostic/ticToc.h"
#include "hostComm/hostComm.h"

#include "microhal_bsp.h"

#include "ut_common.h"
#include "testPacket.h"

#include "catch.hpp"

using namespace microhal;
using namespace diagnostic;
using namespace std::chrono_literals;

template<typename _Rep, typename _Period>
uint32_t calculateSpeed(size_t size, const std::chrono::duration<_Rep, _Period>& sendTime) {
	std::chrono::microseconds time = std::chrono::duration_cast<std::chrono::microseconds>(sendTime);
	float byteSpeed = static_cast<float>(size) * (1000000.0f / time.count());

	diagChannel << lock << diagnostic::Informational << "Average connection speed = " << byteSpeed << "B/s" << endl << unlock;

	return static_cast<uint32_t>(byteSpeed);
}

TEST_CASE ("Connection with device speed test") {
	enum {
		PacketType = 100
	};

	// ports should be open
	REQUIRE(communicationPortA.isOpen());

	//clear communicationPorts
	REQUIRE(communicationPortA.clear());

	//create hostComm device
	HostComm hostCommA(communicationPortA, debugPort);

	INFO ( "Starting timeProc thread.");

	hostCommA.startHostCommThread();

	REQUIRE(hostCommA.ping(true));

	constexpr int reapeat = 10;
	std::chrono::microseconds responseTime[reapeat];

	diagnostic::TicToc duration;
	duration.callibrate();

	SECTION("Sending no data packet")
	{
		HostCommPacket packet(PacketType, true);

		for(uint8_t i=0; i<reapeat; i++) {
			duration.tic();
			CHECK (hostCommA.send(packet));
			duration.toc();

			responseTime[i] = duration.getDuration();
		}

		std::chrono::microseconds average = {};

		for (const std::chrono::microseconds &response : responseTime) {
			average += response;
			diagChannel << lock << diagnostic::Informational << "No data packet response time = " << response.count() << "us" << endl << unlock;
		}

		average /= reapeat;

		calculateSpeed(sizeof(packet), average);
	}

	SECTION("Sending no CRC calculation data packet")
	{
		testPacket_ACK packet;

		for(uint8_t i=0; i<reapeat; i++) {
			duration.tic();
			CHECK (hostCommA.send(packet));
			duration.toc();

			responseTime[i] = duration.getDuration();
		}

		std::chrono::microseconds average = {};

		for (const std::chrono::microseconds &response : responseTime) {
			average += response;
			diagChannel << lock << diagnostic::Informational << "Data packet response time = " << response.count() << "us" << endl << unlock;
		}

		average /= reapeat;

		calculateSpeed(packet.getSize(), average);
	}

	SECTION("Sending CRC calculation data packet")
	{
		testPacket_ACK_CRC packet;

		for(uint8_t i=0; i<reapeat; i++) {
			duration.tic();
			CHECK (hostCommA.send(packet));
			duration.toc();

			responseTime[i] = duration.getDuration();
		}

		std::chrono::microseconds average = {};

		for (const std::chrono::microseconds &response : responseTime) {
			average += response;
			diagChannel << lock << diagnostic::Debug << "Data packet with CRC response time = " << response.count() << "us" << endl << unlock;
		}

		average /= reapeat;

		calculateSpeed(packet.getSize(), average);
	}

	// close hostComm proc thread
	hostCommA.stopHostCommThread();
}


