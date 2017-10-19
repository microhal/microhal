/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Pawel Okas
 * created on: 12-02-2017
 * last modification: 12-02-2017
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

#ifndef _MICROHAL_MPL115A2_H_
#define _MICROHAL_MPL115A2_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "microhal.h"
#include "I2CDevice/I2CDevice.h"
#include "units/pressure.h"
#include <experimental/optional>
/* **************************************************************************************************************************************************
 * CLASS
 */
class MPL115A2 : microhal::I2CDevice {
	using Endianness = microhal::Endianness;

	enum Registers {
		Padc_MSB = 0x00, ///< 10-bit Pressure ADC output value MSB
		Padc_LSB, ///< 10-bit Pressure ADC output value LSB
		Tadc_MSB, ///< 10-bit Temperature ADC output value MSB
		Tadc_LSB, ///< 10-bit Temperature ADC output value LSB
		a0_MSB, ///< a0 coefficient MSB
		a0_LSB, ///< a0 coefficient LSB
		b1_MSB, ///< b1 coefficient MSB
		b1_LSB, ///< b1 coefficient LSB
		b2_MSB, ///< b2 coefficient MSB
		b2_LSB, ///< b2 coefficient LSB
		c12_MSB, ///< c12 coefficient MSB
		c12_LSB, ///< c12 coefficient LSB
		CONVERT = 0x12 ///< Start Pressure and Temperature Conversion
	};
 public:
	using Pressure = microhal::Pressure<float>;

	MPL115A2(microhal::I2C &i2c) : I2CDevice(i2c, 0xC0) {}

	bool init() {
		return readCoefficient();
	}

	bool startConversion() {
		return writeRegister(CONVERT, (uint8_t)0x00);
	}

	std::experimental::optional<Pressure> pressure() {
		std::experimental::optional<Pressure> pressure;
		Pressure tmp;
		if (readData(tmp)) {
			pressure = tmp;
		}

		return pressure;
	}

	void debug();

	static constexpr auto maxConversionTime() noexcept {
		return std::chrono::milliseconds {3};
	}

 private:
	struct Coefficient {
		float a0;
		float b1;
		float b2;
		float c12;
	} coefficient;

	bool readCoefficient() {
		uint16_t tmp[4];
		if (readRegisters(a0_MSB, tmp, 4, Endianness::Big)) {
			float a0 = static_cast<int16_t>(tmp[0]);
			a0 /= 1<<3;
			coefficient.a0 = a0;
			float b1 = static_cast<int16_t>(tmp[1]);
			b1 /= 1<<13;
			coefficient.b1 = b1;
			float b2 = static_cast<int16_t>(tmp[2]);
			b2 /= 1<<14;
			coefficient.b2 = b2;
			float c12 = static_cast<int16_t>(tmp[3]);
			c12 /= 1<<23;
			coefficient.c12 = c12;
			return true;
		}
		return false;
	}

	Pressure compensate(uint16_t rawPressure, uint16_t rawTemperature) const noexcept {
		rawPressure >>= 6;
		rawTemperature >>= 6;
		float Pcomp = coefficient.a0 + (coefficient.b1 + coefficient.c12 * rawTemperature) * rawPressure + coefficient.b2 * rawTemperature;
		return Pressure {Pcomp * ((115.0f-50.0f)/1023.0f) + 50.0f};
	}

	bool readData(Pressure & compensatedPressure) {
		uint16_t tmp[2];
		if (readRegisters(Padc_MSB, tmp, 2, Endianness::Big)) {
//			uint16_t pressure = tmp[0] >> 6;
//			uint16_t temp = tmp[1] >> 6;
//
//			float Pcomp = coefficient.a0 + (coefficient.b1 + coefficient.c12 * temp) * pressure + coefficient.b2 * temp;
//			Pressure tmp(Pcomp * ((115.0-50.0)/1023.0) + 50.0);
			compensatedPressure = compensate(tmp[0], tmp[1]);
			return true;
		}
		return false;
	}
};

#endif  // _MICROHAL_MPL115A2_H_
