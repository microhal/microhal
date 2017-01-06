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

#ifndef _MICROHAL_DS18B20_H_
#define _MICROHAL_DS18B20_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "oneWire.h"
#include "diagnostic/diagnostic.h"
/* **************************************************************************************************************************************************
 * CLASS
 */
class DS18B20 {
	enum class Command : uint8_t {
		Convert = 0x44,
		ReadScratchpad = 0xBE,
		WriteScratchpad = 0x4E,
		CopyScratchpad = 0x48,
		RecallE2 = 0xB8,
		ReadPowerSupply = 0xB4,
	};

 public:
	enum class Resolution {
		Bits_9 = 0x00, ///< 9 bit resolution, maximum conversion time = 93.75ms
		Bits_10 = 0x20, ///< 10 bit resolution, maximum conversion time = 187.5ms
		Bits_11 = 0x40, ///< 11 bit resolution, maximum conversion time = 375ms
		Bits_12 = 0x60 ///< 12 bit resolution, maximum conversion time = 750ms
	};

	constexpr DS18B20(microhal::OneWire &oneWire, uint64_t rom) : oneWire(oneWire), rom(rom) {}

	Resolution resolution(Resolution resolution) {
		uint8_t scratchpad[9];
		if (readScrathpad(scratchpad)) {
			if(oneWire.sendResetPulse()) {
				oneWire.write(microhal::OneWire::Command::MatchROM);
				oneWire.write(reinterpret_cast<uint8_t*>(&rom), sizeof(rom));
				oneWire.write(static_cast<uint8_t>(Command::WriteScratchpad));
				oneWire.write(scratchpad[2]);
				oneWire.write(scratchpad[3]);
				oneWire.write(static_cast<uint8_t>(resolution));
			}
		}
		return resolution;
	}

	Resolution resolution() const {
		uint8_t tmp[9];
		if (readScrathpad(tmp)) {
			uint8_t config = tmp[4];
			return static_cast<Resolution>(config);
		}
	}

	bool startConversion(bool waitForConversionEnd) {
		oneWire.sendResetPulse();
		oneWire.write(microhal::OneWire::Command::MatchROM);
		oneWire.write(reinterpret_cast<uint8_t*>(&rom), sizeof(rom));
		oneWire.write(static_cast<uint8_t>(Command::Convert));

		if (waitForConversionEnd) {				//if function should wait for conversion end
			//conversion will be ended when 1-wire data pin will go to high
		}
		return true;
	}

	bool temperature(float *temp) {
		uint8_t scratchpad[9];
		if (readScrathpad(scratchpad)) {
			int16_t temporary = scratchpad[1] << 8 | scratchpad[0];
			*temp = (float)temporary / 16;	//calculate actual temperature
			return true;
		}
		return false;
	}
 private:
	mutable microhal::OneWire oneWire;
	uint64_t rom;

	bool readScrathpad(uint8_t scratchpad[9]) const {
		if(oneWire.sendResetPulse()) {
			oneWire.write(microhal::OneWire::Command::MatchROM);
			oneWire.write(reinterpret_cast<const uint8_t*>(&rom), sizeof(rom));
			oneWire.write(static_cast<uint8_t>(Command::ReadScratchpad));
			oneWire.read(scratchpad, sizeof(scratchpad));
			//return scratchpad.crc == CRC<uint8_t, "x^8 + x^5 + x^4 + 1">::calculate(scratchpad, 8);
			return true;
		}
		return false;
	}

};

#endif  // _MICROHAL_DS18B20_H_
