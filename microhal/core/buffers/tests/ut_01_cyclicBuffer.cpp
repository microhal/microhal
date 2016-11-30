/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      cyclicBuffer unit test

 @authors    Pawel Okas, Kubaszek Mateusz
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

#include "microhal.h"
#include "microhal_bsp.h"

#include "buffers/cyclicBuffer.h"

#include "catch.hpp"


using namespace microhal;
using namespace std::chrono;

TEST_CASE ("Cyclic buffer") {
	// minimum 40 elements
	constexpr size_t buffSize = 77;
	CyclicBuffer_data<uint16_t, buffSize> buffer;

	REQUIRE(buffer.getSize() == buffSize);
	REQUIRE(buffer.getLength() == 0);
	REQUIRE(buffer.getFreeSpace() == buffSize);

	for(size_t retry=0; retry < 1000; retry++) {
		for(size_t i=0; i < buffSize; i++) {
			CHECK(buffer.append(i));
		}

		// should return false, because buffer is full
		CHECK(!buffer.append(0xff));

		REQUIRE(buffer.getLength() == buffSize);
		REQUIRE(buffer.getFreeSpace() == 0);

		// check copyTo function
		uint16_t data[buffSize];

		CHECK(buffer.copyTo(data, buffSize) == buffSize);

		for(size_t i=0; i < buffSize; i++) {
			INFO("Iteration: " << i);
			INFO("Data value: " << data[i]);
			CHECK(data[i] == i);
		}
		// buffer length should be unchanged
		CHECK(buffer.getLength() == buffSize);

		for(size_t i=0; i < buffSize; i++) {
			INFO("Iteration: " << i);
			INFO("Data value: " << data[i]);
			CHECK(buffer.get() == i);
		}

		// buffer should be empty
		CHECK(buffer.getLength() == 0);
		REQUIRE(buffer.getFreeSpace() == buffSize);

		// adding data through write function
		for(size_t i = 0; i < buffSize; i++){
			data[i] = i+1000;
		}
		CHECK(buffer.write(data, buffSize-10));

		// buffer should have free 10 locations
		CHECK(buffer.getFreeSpace() == 10);

		// adding and getting data through append and get
		CHECK(buffer.append(123));
		CHECK(buffer.append(456));
		CHECK(buffer.get() == 1000);
		CHECK(buffer.get() == 1001);
		CHECK(buffer.getFreeSpace() == 10);

		// adding data through write function
		data[0] = 123;
		data[1] = 456;
		data[2] = 789;
		CHECK(buffer.write(data, 3));

		// buffer has to have 7 free locations
		CHECK(buffer.getFreeSpace() == 7);

		// checking data correctness
		for(size_t i=2; i < buffSize-10 ; i++){
			CHECK(buffer.get() == (1000+i));
			CHECK(buffer.getFreeSpace() == (7+i-1));
		}
		CHECK(buffer.get() == 123);
		CHECK(buffer.get() == 456);
		CHECK(buffer.get() == 123);
		CHECK(buffer.get() == 456);
		CHECK(buffer.get() == 789);

		// buffer should be free now
		CHECK(buffer.getFreeSpace() == buffSize);

		// filling table with new data
		for(size_t i = 0; i < buffSize; i++){
			data[i] = i+2000;
		}
		CHECK(buffer.write(data,buffSize-10));
		CHECK(buffer.getFreeSpace() == 10);
		CHECK(buffer.read(data, 10));
		for(size_t i = 0; i < 10; i++){
			CHECK(data[i] == (i+2000));
		}
		for(size_t i = 10; i < 20; i++){
			CHECK(buffer.get() == (i+2000));
		}
		CHECK(buffer.read(data, buffSize-30));
		for (size_t i = 0; i < buffSize-30; i++) {
			CHECK(data[i] == (i+2020));
		}

		// buffer should be free now
		CHECK(buffer.getFreeSpace() == buffSize);
	}
}
