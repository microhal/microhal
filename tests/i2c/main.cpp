/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    pawel
 * created on: 22-12-2016
 * last modification: 22-12-2016
 *
 * @copyright Copyright (c) 2016, microHAL
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */

#include "microhal.h"
#include "bsp.h"
#include <algorithm>

using namespace microhal;
using namespace diagnostic;

int main(void) {
	debugPort.setDataBits(SerialPort::Data8);
	debugPort.setStopBits(SerialPort::OneStop);
	debugPort.setParity(SerialPort::NoParity);
	debugPort.open(SerialPort::ReadWrite);
	debugPort.setBaudRate(SerialPort::Baud115200);

	debugPort.write("\n\r------------------- I2C Test -------------------------\n\r");
	diagChannel.setOutputDevice(debugPort);

	uint8_t page[] = "ala ma kot";
	uint8_t bufferA[sizeof(page)];
	uint8_t bufferB[sizeof(page)];

	// read I2C speed
	diagChannel << MICROHAL_NOTICE << "i2c.speed() = " << sensorI2C.speed();

/////////////////////////////////
	sensorI2C.write(0xA0, 0);

	sensorI2C.read(0xA0, bufferA, sizeof(bufferA));
	if (std::equal(std::begin(page), std::end(page) - 1, bufferA)) {
		diagChannel << MICROHAL_NOTICE << "i2c.read OK";
	} else {
		diagChannel << MICROHAL_NOTICE << "i2c.read FAILED";
	}
///////////////////////////////
	for (size_t len = 1; len<7; len++) {
		sensorI2C.write(0xA0, 0);
		std::fill(std::begin(bufferA), std::end(bufferA), 0);
		sensorI2C.read(0xA0, bufferA, len);
		if (std::equal(std::begin(page), std::begin(page) + len, bufferA)) {
			diagChannel << MICROHAL_NOTICE << "i2c.read OK";
		} else {
			diagChannel << MICROHAL_NOTICE << "i2c.read FAILED, len = " << (uint8_t)len;
		}
	}
//////////////////////////////////
	for (uint8_t ratio = 1; ratio < sizeof(page); ratio++) {
		sensorI2C.write(0xA0, 0);
		std::fill(std::begin(bufferA), std::end(bufferA), 0);
		std::fill(std::begin(bufferB), std::end(bufferB), 0);

		sensorI2C.read(0xA0, bufferA, ratio, bufferB, sizeof(page) - ratio);
		if (std::equal(std::begin(page), std::begin(page) + ratio, bufferA) &&
			std::equal(std::begin(page) + ratio, std::end(page) - 1, bufferB)) {
			diagChannel << MICROHAL_NOTICE << "i2c.read OK" << " B size = " << (uint32_t)(sizeof(page) - ratio);
		} else {
			diagChannel << MICROHAL_NOTICE << "i2c.read FAILED, ratio: " << ratio;
			diagChannel << MICROHAL_NOTICE << endl << (const char*)bufferA;
			diagChannel << MICROHAL_NOTICE << endl << (const char*)bufferB;
		}
	}
/////////////////////////////////
	diagChannel << MICROHAL_NOTICE << "writeRead function tests";
/////////////////////////////////
	uint8_t address = 0;
	sensorI2C.writeRead(0xA0, &address, sizeof(address), bufferA, sizeof(bufferA));
	if (std::equal(std::begin(page), std::end(page) - 1, bufferA)) {
		diagChannel << MICROHAL_NOTICE << "i2c.writeRead OK";
	} else {
		diagChannel << MICROHAL_NOTICE << "i2c.writeRead FAILED";
	}
/////////////////////////////////
	for (size_t len = 1; len<8; len++) {
		std::fill(std::begin(bufferA), std::end(bufferA), 0);
		sensorI2C.writeRead(0xA0, &address, sizeof(address), bufferA, len);
		if (std::equal(std::begin(page), std::begin(page) + len, bufferA)) {
			diagChannel << MICROHAL_NOTICE << "i2c.writeRead OK";
		} else {
			diagChannel << MICROHAL_NOTICE << "i2c.writeRead FAILED, len = " << (uint8_t)len;
		}
	}

/////////////////////////////////
	diagChannel << MICROHAL_NOTICE << "write function tests";
/////////////////////////////////
	const uint8_t data[] = "Page test data.";
	static_assert(sizeof(data) == 16, "");
	uint8_t readBuffer[sizeof(data)];
	uint8_t txBuff[sizeof(data) + 1];
	uint8_t addr = 0;

	txBuff[0] = addr;
	std::copy(std::begin(data), std::end(data), &txBuff[1]);

	///////////////////////////////
	sensorI2C.write(0xA2, txBuff, sizeof(txBuff));
	// we need to wait for data write
	std::this_thread::sleep_for(std::chrono::milliseconds {10});
	sensorI2C.writeRead(0xA2, &addr, 1, readBuffer, sizeof(readBuffer));
	if (std::equal(std::begin(data), std::end(data), readBuffer)) {
		diagChannel << MICROHAL_NOTICE << "i2c.write OK";
	} else {
		diagChannel << MICROHAL_NOTICE << "i2c.write FAILED";
	}
	///////////////////////////////
	for (uint8_t ratio = 0; ratio < sizeof(data); ratio++) {
		txBuff[0] = addr;
		std::copy(std::begin(data), std::begin(data) + ratio, &txBuff[1]);

		sensorI2C.write(0xA2, txBuff, ratio + 1, data + ratio, sizeof(data) - ratio);
		std::this_thread::sleep_for(std::chrono::milliseconds {10});

		std::fill(std::begin(readBuffer), std::end(readBuffer), 0);
		sensorI2C.writeRead(0xA2, &addr, 1, readBuffer, sizeof(readBuffer));
		if (std::equal(std::begin(data), std::end(data), readBuffer)) {
			diagChannel << MICROHAL_NOTICE << "i2c.write OK" << " B size = " << (uint32_t)(sizeof(data) - ratio);
		} else {
			diagChannel << MICROHAL_NOTICE << "i2c.write FAILED, ratio: " << ratio;
			diagChannel << MICROHAL_NOTICE << endl << (const char*)bufferA;
			diagChannel << MICROHAL_NOTICE << endl << (const char*)bufferB;
		}
	}


	diagChannel << MICROHAL_NOTICE << "End of tests";
	while(1) {

	}
	return 0;
}
