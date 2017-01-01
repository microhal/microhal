/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Pawel Okas
 * created on: 31-12-2016
 * last modification: 31-12-2016
 *
 * @copyright Copyright (c) 2016, Pawel Okas
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

#ifndef _MICROHAL_ONEWIRE_H_
#define _MICROHAL_ONEWIRE_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "serialPort.h"
#include "diagnostic/diagnostic.h"

// NOTICE: This driver use SerialPort do comunicate with 1-Wire device. This driver was written based on:
// https://www.maximintegrated.com/en/app-notes/index.mvp/id/214

namespace microhal {
/* **************************************************************************************************************************************************
 * CLASS
 */
class OneWire {
 public:
	enum class Command {
		MatchROM = 0x55,
		SearchROM =	0xF0,
		SkipROM = 0xCC,
		AlarmSearch = 0xEC,
		ReadRom = 0x33,
	};

	OneWire(SerialPort &serial) noexcept : serial(serial) {
	}

	bool write(uint8_t data) {
		uint8_t tmp[8];
		for (uint8_t bitPos = 0; bitPos < 8; bitPos++) {
			if (data & (1 << (bitPos))) {
				tmp[bitPos] = 0xFF;
			} else {
				tmp[bitPos] = 0x00;
			}
		}
		return serial.write(reinterpret_cast<char*>(tmp), sizeof(tmp)) == sizeof(tmp);
	}

	bool read(uint8_t *data, size_t length) {
		if (serial.waitForWriteFinish(std::chrono::milliseconds {100})) {
			serial.clear(SerialPort::Direction::Input);
			const uint8_t write[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
			static_assert(sizeof(write) == 8,"");
			for(size_t byte = 0; byte < length; byte++) {
				serial.write(reinterpret_cast<const char*>(write), sizeof(write));
				uint8_t read[8];
				serial.read(reinterpret_cast<char*>(read), sizeof(read), std::chrono::milliseconds {100});
				uint8_t tmp = 0;
				for (uint8_t bitPos = 0; bitPos < 8; bitPos++) {
					if (read[bitPos] == 0xFF) {
						tmp |= 1 << (bitPos);
					}
				}
				data[byte] = tmp;
			}
			return true;
		}
		return false;
	}

	bool sendResetPulse(void) {
		if (serial.waitForWriteFinish(std::chrono::milliseconds::max())) {
			serial.clear(SerialPort::Direction::Input);
			serial.setBaudRate(SerialPort::Baud9600);
			serial.putChar(0xF0);
			char response;
			serial.read(&response, 1, std::chrono::milliseconds {1});
			if (response != 0xF0) {
				serial.setBaudRate(SerialPort::Baud115200);
				return true;
			} else {
				//diagChannel << lock << MICROHAL_ERROR << "1-wire device not found" << unlock;
			}
		}
		return false;
	}

 private:
	SerialPort &serial;
};

}  // namespace microhal

#endif  // _MICROHAL_ONEWIRE_H_
