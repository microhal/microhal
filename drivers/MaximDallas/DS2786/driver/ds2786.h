/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      DS2786 driver
 *
 * @authors    Michal Karczewski, Pawel Okas
 * created on: 2015
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015-2017, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 	   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * 	      documentation and/or other materials provided with the distribution.
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
#ifndef DS2786_H_
#define DS2786_H_

/******************************************************************************
 * INCLUDES
 */
#include "microhal.h"
#include "I2CDevice/I2CDevice.h"
#include "diagnostic/diagnostic.h"
#include <cstring>

/** @addtogroup Devices
 *  @{
 *  @class DS2786
 *  @}
 *
 * @brief DS2786 stand-alone ocv-based fuel gauge 2-wire interface
 */

template<typename Error, typename Data>
class Result {
 public:
	constexpr explicit Result(Data data) : _error(Error::None), data(data) {};
	//constexpr Result(Data &&data) : _error(Error::None), data(data) {};
	constexpr explicit Result(Error error) : _error(error) {};

	constexpr operator bool() const { return _error == Error::None; }

	Error error() const { return _error; }

	constexpr Data * operator->() { return &data; }
	constexpr const Data * operator->() const { return &data; }
	constexpr Data & operator*() { return data; }
	constexpr const Data & operator*() const { return data; }
	//constexpr Data && operator*() && { return std::forward(data); }
	//constexpr const Data && operator*() const && { return std::forward(data); }

 private:
	Error _error = !Error::None;
	Data data;
};

template<typename Data>
class Result<bool, Data> {
 public:
	constexpr explicit Result(Data data) : dataValid(true), data(data) {};
	constexpr explicit Result() : dataValid(false) {};
	constexpr explicit Result(bool dataValid, Data data) : dataValid(dataValid), data(data) {};

	constexpr operator bool() const { return dataValid; }

	constexpr Data * operator->() { return &data; }
	constexpr const Data * operator->() const { return &data; }
	constexpr Data & operator*() { return data; }
	constexpr const Data & operator*() const { return data; }
	//constexpr Data && operator*() && { return std::forward(data); }
	//constexpr const Data && operator*() const && { return std::forward(data); }

 private:
	bool dataValid;
	Data data;
};

class DS2786: protected microhal::I2CDevice {
	enum Registers
		: uint8_t {
		CONFIG = 0x01,
		RELATIVE_CAPACITY,
		AUXILARY_MSB_0 = 0x08,
		AUXILARY_LSB_0,
		AUXILARY_MSB_1,
		AUXILARY_LSB_1,
		VOLTAGE_MSB,
		VOLTAGE_LSB,
		CURRENT_MSB,
		CURRENT_LSB,
		INITIAL_VOLTAGE_MSB = 0x14,
		INITIAL_VOLTAGE_LSB,
		LAST_OCV_CAPACITY,
		CAPACITY_SCALING_FACTOR,
		EEPROM_CURRENT_OFFSET = 0x60,
		EEPROM_CAPACITY_1,
		EEPROM_CAPACITY_2,
		EEPROM_CAPACITY_3,
		EEPROM_CAPACITY_4,
		EEPROM_CAPACITY_5,
		EEPROM_CAPACITY_6,
		EEPROM_CAPACITY_7,
		EEPROM_VOLTAGE_BREAKPOINT_MSB_0,
		EEPROM_VOLTAGE_BREAKPOINT_LSB_0,
		EEPROM_VOLTAGE_BREAKPOINT_MSB_1,
		EEPROM_VOLTAGE_BREAKPOINT_LSB_1,
		EEPROM_VOLTAGE_BREAKPOINT_MSB_2,
		EEPROM_VOLTAGE_BREAKPOINT_LSB_2,
		EEPROM_VOLTAGE_BREAKPOINT_MSB_3,
		EEPROM_VOLTAGE_BREAKPOINT_LSB_3,
		EEPROM_VOLTAGE_BREAKPOINT_MSB_4,
		EEPROM_VOLTAGE_BREAKPOINT_LSB_4,
		EEPROM_VOLTAGE_BREAKPOINT_MSB_5,
		EEPROM_VOLTAGE_BREAKPOINT_LSB_5,
		EEPROM_VOLTAGE_BREAKPOINT_MSB_6,
		EEPROM_VOLTAGE_BREAKPOINT_LSB_6,
		EEPROM_VOLTAGE_BREAKPOINT_MSB_7,
		EEPROM_VOLTAGE_BREAKPOINT_LSB_7,
		EEPROM_VOLTAGE_BREAKPOINT_MSB_8,
		EEPROM_VOLTAGE_BREAKPOINT_LSB_8,
		EEPROM_INITIAL_CAPACITY_SCALING,
		EEPROM_CURRENT_THRESHOLD,
		EEPROM_DV_DT_THRESHOLD,
		EEPROM_I2C_ADDRESS,
		EEPROM_LEARN_THRESHOLD,
		EEPROM_USER_VALUE,
		COMMAND = 0x7E
	};
	enum ConfigBits : uint8_t {
		PORF = 0x40,
		SMOD = 0x20,
		LDIS = 0x10,
		VODIS = 0x08,
		ITEMP = 0x04,
		AIN1 = 0x02,
		AIN0 = 0x01
	};

	enum CommandRegister : uint8_t {
		POR = 0x80,
		POCV = 0x08,
		SOCV = 0x04,
		RCALL = 0x02,
		COPY = 0x01
	};

	typedef enum : uint8_t {
			DS2786_ADDR = 0x6C
	} PossibleI2CAddress;

	struct ParameterEEPROMMemoryBlock {
		uint8_t currentOffsetBiasRegister;
		uint8_t capacity[7];
		uint16_t voltageBreakpoint[9];
		uint8_t initialCapacityScallingFactor;
		uint8_t blankingOCVCurrentTreshold;
		uint8_t OCVdVdtThreshold;
		uint8_t i2cAddressConfiguration;
		uint8_t learnThreshold;
		uint8_t userEeprom;

		bool operator==(ParameterEEPROMMemoryBlock &b) const {
			return std::memcmp(this, &b, sizeof(ParameterEEPROMMemoryBlock)) == 0;
		}
	};

 public:
    struct CellModel {
	    int8_t capacity1; //! in [%] stored as constant comma values with resolution 0.5%
		uint8_t capacity2; //! in [%]
		uint8_t capacity3; //! in [%]
		uint8_t capacity4; //! in [%]
		uint8_t capacity5; //! in [%]
		uint8_t capacity6; //! in [%]
		uint8_t capacity7; //! in [%]
		uint32_t voltageBreakpoint0; //! in [uV]
		uint32_t voltageBreakpoint1; //! in [uV]
		uint32_t voltageBreakpoint2; //! in [uV]
		uint32_t voltageBreakpoint3; //! in [uV]
		uint32_t voltageBreakpoint4; //! in [uV]
		uint32_t voltageBreakpoint5; //! in [uV]
		uint32_t voltageBreakpoint6; //! in [uV]
		uint32_t voltageBreakpoint7; //! in [uV]
		uint32_t voltageBreakpoint8; //! in [uV]
		constexpr bool isValid() const noexcept {
			if (capacity1 == 0) return false;
			if (capacity7 >= 200) return false;
			if (capacity1 >= capacity2) return false;
			if (capacity2 >= capacity3) return false;
			if (capacity3 >= capacity4) return false;
			if (capacity4 >= capacity5) return false;
			if (capacity5 >= capacity6) return false;
			if (capacity6 >= capacity7) return false;
			if (voltageBreakpoint0 >= voltageBreakpoint1) return false;
			if (voltageBreakpoint1 >= voltageBreakpoint2) return false;
			if (voltageBreakpoint2 >= voltageBreakpoint3) return false;
			if (voltageBreakpoint3 >= voltageBreakpoint4) return false;
			if (voltageBreakpoint4 >= voltageBreakpoint5) return false;
			if (voltageBreakpoint5 >= voltageBreakpoint6) return false;
			if (voltageBreakpoint6 >= voltageBreakpoint7) return false;
			if (voltageBreakpoint7 >= voltageBreakpoint8) return false;
			if (voltageBreakpoint8 > 4996000) return false;
			return true;
		}
	};

    enum Error {
    	None,
		I2CCommunication,
		TemperatureNotEnabled,
		Aux1NotEnabled
    };

    enum class Command : uint8_t {
    	Rcall = 0b10,
		Copy = 1,
    };

	constexpr DS2786(microhal::I2C &i2c, uint32_t Rsns_uR) :
			I2CDevice(i2c, DS2786_ADDR), Rsns(Rsns_uR) {
	}

	bool init(const CellModel &cellModel, uint32_t initialCellCapacity_mAh, int32_t currentOffsetBias_uA, uint16_t OCVThreshold_uA, uint16_t OCVdVdtThreshold_uV);

	constexpr float currentResolution() const noexcept {
		const float voltageMeasurementResolution_uV = 25.0f;
		return voltageMeasurementResolution_uV/Rsns;
	}

	constexpr float currentMeasurementRange() const noexcept {
		const float voltageMeasurementRange_uV = 51200; //51.2mV
		return voltageMeasurementRange_uV/Rsns;
	}

	auto getUserEEPROM() {
		uint8_t data;
		if (readRegister(EEPROM_USER_VALUE, data)) {
			return Result<bool, uint8_t>(data);
		}
		return Result<bool, uint8_t>();
	}

	bool setUserEEPROM(uint8_t data) {
		if (auto tmp = getUserEEPROM()) {
			if (*tmp != data) {
				if (writeRegister(EEPROM_USER_VALUE, data)) {
					write(Command::Copy);
					std::this_thread::sleep_for(std::chrono::milliseconds {14});
					return clear(Command::Copy);
				}
			}
		}
		return true;
	}
	/**
	 *@brief Read battery voltage at Vin with respect to Vss. Voltage range is from 0V to 4.5V with resolution 1.22mV.
	 */
	auto readVoltage() {
		uint16_t voltageRAW;
		const bool status = readRegister(VOLTAGE_MSB, voltageRAW, microhal::Endianness::Big);
		voltageRAW = voltageRAW & 0x8000 ? 0xE000 | (voltageRAW >> 3) : 0x1FFF & (voltageRAW >> 3);
		return Result<bool, float>(status, 0.00122f * voltageRAW);
	}

	auto isAuxlinary0Valid() {
		uint8_t tmp;
		const bool status = readRegister(CONFIG, tmp);
		return Result<bool, bool>(status, tmp & AIN0);
	}

	bool isAuxlinary1Valid() {
		uint8_t tmp;
		const bool status = readRegister(CONFIG, tmp);
		return Result<bool, bool>(status, tmp & AIN1);
	}
	/**
	 *@brief Read Auxlinary voltage at input 0.
	 */
	auto readAxuiliary0() {
		uint16_t tmp;
		const bool status = readRegister(AUXILARY_MSB_0, tmp, microhal::Endianness::Big);
		int16_t aux = static_cast<int16_t>(tmp & 0x8000 ? 0xF000 | (tmp >> 4) : 0x0FFF & (tmp >> 4));
		return Result<bool, int16_t>(status, aux);
	}

	auto readAxuiliary1() {
		uint8_t status;
		if (readRegister(CONFIG, status)) {
			if (status & ConfigBits::ITEMP) return Result<Error, int16_t>(Aux1NotEnabled);
			uint16_t tmp;
			if (readRegister(AUXILARY_MSB_1, tmp, microhal::Endianness::Big)) {
				int16_t aux = static_cast<int16_t>(tmp & 0x8000 ? 0xF000 | (tmp >> 4) : 0x0FFF & (tmp >> 4));
				return Result<Error, int16_t>(aux);
			}
		}
		return Result<Error, int16_t>(I2CCommunication);
	}

	auto isInternalThermometerEnabled() {
		uint8_t tmp;
		const bool status = readRegister(CONFIG, tmp);
		return Result<bool, bool>(status, tmp & ITEMP);
	}

	bool enableInternalThermometer() {
		return setBitsInRegister(CONFIG, ITEMP);
	}

	bool disableInternalThermometer() {
		return clearBitsInRegister(CONFIG, ITEMP);
	}
	/**
	 *@brief Function return temperature in Celsius degree.
	 */
	auto readTemperature() {
		uint8_t status;
		if (readRegister(CONFIG, status)) {
			if (!(status & ConfigBits::ITEMP)) return Result<Error, float>(TemperatureNotEnabled);
			uint16_t tmp;
			if (readRegister(AUXILARY_MSB_1, tmp, microhal::Endianness::Big)) { // temperature address register is sheared width auxlinary
				float temperature = 0.125f * static_cast<int16_t>(tmp & 0x8000 ? 0xF100 | (tmp >> 5) : 0x0EFF & (tmp >> 5));
				return Result<Error, float>(temperature);
			}
		}
		return Result<Error, float>(I2CCommunication);
	}
	/**
	 *@brief Return current in uA
	 */
	auto readCurrent() {
		uint16_t tmp;
		const bool status = readRegister(CURRENT_MSB, tmp, microhal::Endianness::Big);
		float current = 25000 * static_cast<int16_t>(tmp & 0x8000 ? 0xF000 | (tmp >> 4) : 0x0FFF & (tmp >> 4));
		float currentValue = current * 1000.0f / Rsns;
		return Result<bool, float>(status, currentValue);
	}

	auto relativeCapacity() {
		uint8_t tmp;
		const bool status = readRegister(RELATIVE_CAPACITY, tmp);
		float capacity = 0.5f * tmp;
		return Result<bool, float>(status, capacity);
	}

	auto isCapacityLearningEnabled() {
		uint8_t tmp;
		const bool status = readRegister(CONFIG, tmp);
		return Result<bool, bool>(status, tmp & LDIS);
	}

	bool enableCapacityLearning() {
		return setBitsInRegister(CONFIG, LDIS);
	}

	bool disableCapacityLearning() {
		return clearBitsInRegister(CONFIG, LDIS);
	}

	auto readInitialVoltage() {
		uint16_t tmp;
		if (readRegister(INITIAL_VOLTAGE_MSB, tmp, microhal::Endianness::Big)) {
			float voltage = 0.00122f * static_cast<int16_t>(tmp & 0x8000 ? 0xE000 | (tmp >> 3) : tmp >> 3);
			return Result<bool, float>(voltage);
		}
		return Result<bool, float>();
	}

	bool isSleepModeEnabled() {
		uint8_t data;
		const bool status = readRegister(CONFIG, data);
		return Result<bool, bool>(status, data & SMOD);
	}

	bool enableSleepMode(){
		return setBitsInRegister(CONFIG, static_cast<uint8_t>(SMOD));
	}

	bool disableSleepMode(){
		return clearBitsInRegister(CONFIG, static_cast<uint8_t>(SMOD));
	}

	auto isVoutDisabled() {
		uint8_t data;
		const bool status = readRegister(CONFIG, data);
		return Result<bool, bool>(status, data & VODIS);
	}

	bool enableVout() {
		return clearBitsInRegister(CONFIG, VODIS);
	}

	bool disableVout() {
		return setBitsInRegister(CONFIG, VODIS);
	}

 private:
	const uint32_t Rsns; // resistance of Rsns in uOhm

	uint8_t calculateInitialCapacity(uint32_t cellCapacity_mAh) {
		uint32_t mVh = cellCapacity_mAh * Rsns / 1000;
		return 100000000 / (mVh * 78125);
	}

	bool write(Command command) {
		return setBitsInRegister(COMMAND, static_cast<uint8_t>(command));
	}

	bool clear(Command command) {
		return clearBitsInRegister(COMMAND, static_cast<uint8_t>(command));
	}
};

template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> operator<< (microhal::diagnostic::LogLevelChannel<level, B> logChannel, const DS2786::Error error) {
	const char* errorTxt[] = {"None", "I2C Communication", "Temperature not enabled", "Auxlinary input 1 not enabled"};
    return logChannel << errorTxt[static_cast<uint8_t>(error)];
}

#endif
