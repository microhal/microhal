/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      FLIR Lepton IR camera driver file
 *
 * @authors    Pawel Okas
 * created on: 7-07-2016
 * last modification: <DD-MM-YYYY>
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

#ifndef LEPTON_H_
#define LEPTON_H_

#include "I2CDevice/I2CDevice.h"
#include "microhal.h"
#include "spi.h"
#include "crc/crc16.h"

class Lepton {
	class VoSPIPacket {
		uint16_t id;
		uint16_t crc;
		uint8_t data[160];

		uint16_t getCRC() const {
			return microhal::byteswap(crc);
		}
	public:
		uint16_t getNumber() const {
			return microhal::byteswap(id) & 0x0FFF;
		}

		bool isDiscard() const {
			//return (id & 0x0F00) == 0x0F00;
			return (id & 0x000F) == 0x000F;
		}

		bool verifyCRC() const {
			const uint16_t tmp = 0xFF0F;
			const uint16_t IdCrcField[2] = {id & tmp, 0};
			uint16_t crc = microhal::CRC16::calculate(&IdCrcField, sizeof(IdCrcField), 0);
			crc = microhal::CRC16::calculate(data, sizeof(data), crc);
			if (crc != getCRC()) {
				microhal::diagnostic::diagChannel << microhal::diagnostic::lock << microhal::diagnostic::Debug <<
						"Frame number = " << getNumber() << ", calculated CRC: " << microhal::diagnostic::toHex(crc) << " expected: " << microhal::diagnostic::toHex(getCRC())
						<< microhal::diagnostic::endl << microhal::diagnostic::unlock;
//				asm volatile("BKPT #01");
			}
			return crc == getCRC();
		}

		constexpr const uint8_t* getImageDataPtr() const noexcept { return data; }

		static constexpr size_t size() noexcept { return 160; }
	};
	static_assert(sizeof(VoSPIPacket) == 164,"");
public:
	Lepton(microhal::SPI &spi, microhal::I2C &i2c, microhal::GPIO::IOPin SPIChipSelect, microhal::GPIO::IOPin power, microhal::GPIO::IOPin reset)
					: spi(spi), i2c(i2c, 0xFF), cs(SPIChipSelect, microhal::GPIO::Direction::Output) {
		cs.set();
	}

	void startup() {
		cs.set();
		cs.reset();
		cs.set();

		std::this_thread::sleep_for(std::chrono::milliseconds {185});
	}

	bool shutdown() {
		return false;
	}

	static constexpr size_t getPictureSize() {
		return 80*60*2;
	}

	void timeProc();

	bool readImagePacket() {
		cs.reset();
		microhal::SPI::Error status = spi.readBuffer(&imagePacket, sizeof(imagePacket)) ;
		cs.set();
		return status == microhal::SPI::NoError;
	}

	bool isNewPictureAvailable() {
		bool tmp = pictureReady;
		pictureReady = false;
		return tmp;
	}

	const uint8_t* getPicture() const { return pictureBuffer;}
private:
	microhal::SPI &spi;
	microhal::I2CDevice i2c;
	microhal::GPIO cs;
	VoSPIPacket imagePacket;
	uint16_t lastImageNumber = 0xFFFF;
	static constexpr size_t picture_size = 80*60*2;
	uint8_t pictureBuffer[picture_size];
	bool pictureReady = false;
};

#endif  // LEPTON_H_
