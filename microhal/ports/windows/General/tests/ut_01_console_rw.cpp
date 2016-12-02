/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      cyclicBuffer unit test

 @authors    Pawel Okas
 created on: 01-10-2015
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
#include "consoleIODevice.h"
#include "microhal.h"
#include "microhal_bsp.h"
#include "catch.hpp"


using namespace microhal;
using namespace std::chrono;

TEST_CASE ("Console IO test") {
	bool closeAtEnd = false;
	if(!debugPort.isOpen()) {
		closeAtEnd = true;
		CHECK(true == debugPort.open(IODevice::OpenMode::ReadWrite));
	}

	char tester[] = "Console IO test\n";
	SECTION("Writing to console")
	{
		CHECK(strlen(tester) == debugPort.write(tester, strlen(tester)));
	}

	char testChar = 't';
	char readData = 0;
	SECTION("Reading console with not data")
	{
		// should be empty
		((microhal::windows::console_IODevice*)(&debugPort))->flushInput();
		DWORD t =  GetLastError();
		CHECK(0 == debugPort.getAvailableBytes());
		CHECK(false == debugPort.getChar(readData));
		CHECK(0 == debugPort.read(&readData, 1));
		CHECK(0 == readData);
	}

	SECTION("Reading console with some data")
	{
		CHECK(true == ((microhal::windows::console_IODevice*)(&debugPort))->addCharToInput(testChar));

		CHECK(1 == debugPort.getAvailableBytes());

		CHECK(true == debugPort.getChar(readData));
		CHECK(testChar == readData);
		readData = 0;
		CHECK(0 == debugPort.getAvailableBytes());
		CHECK(false == debugPort.getChar(readData));
		CHECK(0 == debugPort.read(&readData, 1));
		CHECK(0 == readData);
	}

	SECTION("Reading console with line")
	{
		size_t testDataLen = strlen(tester);
		char* testData = tester;

		while(testDataLen--) {
			CHECK(true == ((microhal::windows::console_IODevice*)(&debugPort))->addCharToInput(*testData++));
		}

		CHECK(strlen(tester) == debugPort.getAvailableBytes());
		char readBuffer[5];
		CHECK(5 == debugPort.read(readBuffer, 5));
		CHECK((strlen(tester) - 5) == debugPort.getAvailableBytes());
		CHECK(0 == memcmp(tester, readBuffer, 5));

		char lineBuffer[10];
		CHECK(10 == debugPort.readLine(lineBuffer, 11));
		CHECK(0 == debugPort.getAvailableBytes());
		CHECK(0 == memcmp(lineBuffer, &tester[5], 10));

		CHECK(true == ((microhal::windows::console_IODevice*)(&debugPort))->addCharToInput('\n'));
		CHECK(true == ((microhal::windows::console_IODevice*)(&debugPort))->addCharToInput('\n'));
		CHECK(true == ((microhal::windows::console_IODevice*)(&debugPort))->addCharToInput('a'));
		CHECK(true == ((microhal::windows::console_IODevice*)(&debugPort))->addCharToInput('\n'));

		CHECK(0 == debugPort.readLine(lineBuffer, 11));
		CHECK(0 == debugPort.readLine(lineBuffer, 11));
		CHECK(1 == debugPort.readLine(lineBuffer, 11));
		CHECK('a' == lineBuffer[0]);
		CHECK(0 == debugPort.getAvailableBytes());
	}
	if(closeAtEnd) {
		debugPort.close();
	}
}
microhal::windows::console_IODevice testDevice;
microhal::IODevice &testConsole = testDevice;

TEST_CASE ("open_write_problem") {

	SECTION("write to closed console")
	{
		char tester[] = "Console is close\n";
		CHECK_FALSE(testConsole.isOpen());
		CHECK_FALSE(strlen(tester) == testConsole.write(tester, strlen(tester)));
	}

	SECTION("open console and write")
	{
		char tester[] = "Console is open\n";
		CHECK(testConsole.open(IODevice::OpenMode::ReadWrite));
		CHECK(testConsole.isOpen());
		CHECK(strlen(tester) == testConsole.write(tester, strlen(tester)));
	}

	SECTION("close console and write")
	{
		char tester[] = "Console is close\n";
		testConsole.close();
		CHECK_FALSE(testConsole.isOpen());
		CHECK_FALSE(strlen(tester) == testConsole.write(tester, strlen(tester)));
	}
}


