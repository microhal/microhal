/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      hostComm unit test packet

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

#ifndef TESTPACKET_H_
#define TESTPACKET_H_

#include "diagnostic/diagnostic.h"
#include "hostComm/hostCommPacket.h"

namespace microhal {

class testData{
public:
	template <diagnostic::LogLevel level>
	void log(diagnostic::Diagnostic<level> &log = diagnostic::diagChannel);

	void setCounter(uint32_t cnt) { counter = cnt; }

	uint8_t getCounter() { return counter; }

private:
	uint32_t counter;
public:
	uint8_t data[490];
};


class testPacket_noACK : public HostCommDataPacket<testData, 0x21>{
public:
	testPacket_noACK() : HostCommDataPacket(false, false) {
	}

	enum {
		Request = 0x20
	};
};

class testPacket_ACK : public HostCommDataPacket<testData, 0x31>{
public:
	testPacket_ACK() : HostCommDataPacket(true, false) {
	}

	enum {
		Request = 0x30
	};
};

class testPacket_noACK_CRC : public HostCommDataPacket<testData, 0x41>{
public:
	testPacket_noACK_CRC() : HostCommDataPacket(false, true) {
	}

	enum {
		Request = 0x40
	};
};

class testPacket_ACK_CRC : public HostCommDataPacket<testData, 0x51>{
public:
	testPacket_ACK_CRC() : HostCommDataPacket(true, true) {
	}

	enum {
		Request = 0x50
	};
};

} // namespace microhal

#endif // TESTPACKET_H_
