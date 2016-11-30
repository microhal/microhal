/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 18-12-2014
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

#ifndef MPL3115_H_
#define MPL3115_H_

#include <experimental/optional>

#include "microhal.h"
#include "I2CDevice/I2CDevice.h"
#include "types/int.h"
#include "units/pressure.h"
#include "units/temperature.h"

class MPL3115 : protected microhal::I2CDevice {
	using Endianness = microhal::Endianness;
	using RegisterAccess = microhal::RegisterAccess;

	static constexpr auto STATUS = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x00>();
	static constexpr auto OUT_P_MSB = microhal::createDeviceRegister<RegisterAccess::ReadOnly, microhal::uint24_t, Endianness::BigEndian>().address<uint8_t, 0x01>();
	//static constexpr auto OUT_P_CSB = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x02>();
	//static constexpr auto OUT_P_LSB = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x03>();

	static constexpr auto OUT_T_MSB = microhal::createDeviceRegister<RegisterAccess::ReadOnly, int16_t, Endianness::BigEndian>().address<uint8_t, 0x04>();
	//static constexpr auto OUT_T_LSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x05>();
	static constexpr auto DR_STATUS = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x06>();
	static constexpr auto OUT_P_DELTA_MSB = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x07>();
	static constexpr auto OUT_P_DELTA_CSB = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x08>();
	static constexpr auto OUT_P_DELTA_LSB = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x09>();

	static constexpr auto OUT_T_DELTA_MSB = microhal::createDeviceRegister<RegisterAccess::ReadOnly, int16_t, Endianness::BigEndian>().address<uint8_t, 0x0A>();
	//static constexpr auto OUT_T_DELTA_LSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x0B>();
	static constexpr auto WHO_AM_I = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x0C>();
	static constexpr auto F_STATUS = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x0D>();
	static constexpr auto F_DATA = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x0E>();
	static constexpr auto F_SETUP = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x0F>();
	static constexpr auto TIME_DLY = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x10>();
	static constexpr auto SYSMOD = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x11>();
	static constexpr auto INT_SOURCE = microhal::createDeviceRegister<RegisterAccess::ReadOnly, uint8_t>().address<uint8_t, 0x12>();
	static constexpr auto PT_DATA_CFG = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x13>();
	static constexpr auto BAR_IN_MSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, int16_t, Endianness::BigEndian>().address<uint8_t, 0x14>();
	//static constexpr auto BAR_IN_LSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x15>();
	static constexpr auto P_TGT_MSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint16_t, Endianness::BigEndian>().address<uint8_t, 0x16>();
	//static constexpr auto P_TGT_LSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x17>();
	static constexpr auto T_TGT = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x18>();
	static constexpr auto P_WND_MSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint16_t, Endianness::BigEndian>().address<uint8_t, 0x19>();
	//static constexpr auto P_WND_LSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x1A>();
	static constexpr auto T_WND = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x1B>();
	static constexpr auto P_MIN_MSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x1C>();
	static constexpr auto P_MIN_CSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x1D>();
	static constexpr auto P_MIN_LSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x1E>();
	static constexpr auto T_MIN_MSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint16_t, Endianness::BigEndian>().address<uint8_t, 0x1F>();
	//static constexpr auto T_MIN_LSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x20>();
	static constexpr auto P_MAX_MSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x21>();
	static constexpr auto P_MAX_CSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x22>();
	static constexpr auto P_MAX_LSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x23>();
	static constexpr auto T_MAX_MSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x24>();
	//static constexpr auto T_MAX_LSB = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x25>();
	static constexpr auto CTRL_REG1 = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x26>();
	static constexpr auto CTRL_REG2 = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x27>();
	static constexpr auto CTRL_REG3 = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x28>();
	static constexpr auto CTRL_REG4 = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x29>();
	static constexpr auto CTRL_REG5 = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x2A>();
	static constexpr auto OFF_P = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x2B>();
	static constexpr auto OFF_T = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x2C>();
	static constexpr auto OFF_H = microhal::createDeviceRegister<RegisterAccess::ReadWrite, uint8_t>().address<uint8_t, 0x2D>();

public:
	/**
	 *  Value of fixed registers.
	 */
	enum ConstRegisterValues : uint8_t{
		WHO_AM_I_VALUE = 0xC4,                         //!< ID VALUE
	};

	enum class Mode : uint8_t {
		Standby = 0x00,
		Active = 0x01
	};

	enum class SensingMode : uint8_t {
		Barometer = 0b0000'0000,
		Altimeter = 0b1000'0000
	};

	enum class InterruptSource : uint8_t {
		DataReady,
		FIFO,
		AltitudePressureAlert,
		TemperatureAlert,
		AltitudePresureThreshold,
		TemperatureThreshold,
		DeltaTemperature,
		DeltaPressure
	};

	enum OutputSampleRate : uint8_t{
		OSR_6ms   = 0b0000'0000,
		OSR_10ms  = 0b0000'1000,
		OSR_18ms  = 0b0001'0000,
		OSR_34ms  = 0b0001'1000,
		OSR_66ms  = 0b0010'0000,
		OSR_130ms = 0b0010'1000,
		OSR_258ms = 0b0011'0000,
		OSR_512ms = 0b0011'1000
	};

	enum class Event {
		OnNewTemperature = 0x01,
		OnNewPressureOrAltitude = 0x02,
		DataChange = 0x04
	};

	constexpr explicit MPL3115(microhal::I2C &i2c) noexcept : I2CDevice(i2c, 0xC0) {
	}
	~MPL3115() {
	}

	bool init() {
		if(auto who = whoAmI()) {
			if(*who == WHO_AM_I_VALUE) return true;
		}
		return false;
	}



	std::experimental::optional<uint8_t> whoAmI() noexcept {
		return readRegister(WHO_AM_I);
	}

	std::experimental::optional<Mode> readMode() {
		std::experimental::optional<Mode> mode;
		if(const auto sysmode = readRegister(SYSMOD)) {
			mode = static_cast<Mode>(*sysmode);
		}
		return mode;
	}

	std::experimental::optional<InterruptSource> readInterruptSource() {
		std::experimental::optional<InterruptSource> intSource;
		if (const auto source = readRegister(INT_SOURCE)) {
			intSource = static_cast<InterruptSource>(*source);
		}
		return intSource;
	}

	bool setOSR(OutputSampleRate osr) {
		return modifyBitsInRegister(CTRL_REG1, static_cast<uint8_t>(osr), static_cast<uint8_t>(0x0C));
	}

	bool setSensingMode(SensingMode mode) {
		return modifyBitsInRegister(CTRL_REG1, static_cast<uint8_t>(mode), static_cast<uint8_t>(0b1000'0000));
	}

	bool setMode(Mode mode) {
		return setBitsInRegister(CTRL_REG1, static_cast<uint8_t>(mode));
	}

	bool enableEvent(Event event) {
		return setBitsInRegister(PT_DATA_CFG, static_cast<uint8_t>(event));
	}

	bool isPressureOrAltitudeReady() {
		if (const auto status = readRegister(STATUS)) {
			if (*status & 0x08) {
				return true;
			}
		}
		return false;
	}

	std::experimental::optional<std::pair<microhal::Pressure, microhal::Temperature>> read() {
		std::experimental::optional<std::pair<microhal::Pressure, microhal::Temperature>> data;
		if (const auto temp = readRegister(OUT_T_MSB)) {
			if (const auto pressure = readRegister(OUT_P_MSB)) {
				data = std::make_pair(microhal::Pressure (static_cast<float>(*pressure)/64.0f), microhal::Temperature::fromCelcius(static_cast<float>(*temp)/256.0f));
			}
		}
		return data;
	}
//	template <microhal::diagnostic::LogLevel level>
//	void registerDump(microhal::diagnostic::Diagnostic<level> &log) {
//		uint8_t regVal;
//
//		log << microhal::diagnostic::Warning <<	"----------- MPL3110 register dump -----------" <<  microhal::diagnostic::endl;
//
//		for (uint8_t reg = STATUS; reg <= OFF_H; reg++) {
//			log << microhal::diagnostic::Warning << microhal::diagnostic::toHex(reg) << " ";
//
//			if (readRegister(reg, regVal)) {
//				log << microhal::diagnostic::Warning << regVal << microhal::diagnostic::endl;
//			} else {
//				log << microhal::diagnostic::Warning << "Error while reading." << microhal::diagnostic::endl;
//			}
//		}
//
//		log << microhal::diagnostic::Warning <<	"----------- End of register dump -----------" <<  microhal::diagnostic::endl;
//	}
};

constexpr MPL3115::Event operator | (MPL3115::Event a, MPL3115::Event b) {
	return static_cast<MPL3115::Event>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

#endif /* MPL3115_H_ */
