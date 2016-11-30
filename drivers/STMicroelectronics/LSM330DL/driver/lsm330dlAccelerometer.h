/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for LSM330DL accelerometer

 @authors    Pawel
 created on: 16-10-2014
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

#ifndef LSM330DLACCELEROMETER_H_
#define LSM330DLACCELEROMETER_H_

#include "microhal.h"
#include "I2CDevice/I2CDevice.h"
#include "diagnostic/diagnostic.h"

namespace lsm330 {

class Accelerometer: protected microhal::I2CDevice {
	enum Registers : uint8_t{
		CTRL_REG1_A = 0x20,
		CTRL_REG2_A,
		CTRL_REG3_A,
		CTRL_REG4_A,
		CTRL_REG5_A,
		CTRL_REG6_A,
		DATACAPTURE_A,
		STATUS_REG_A,
		OUT_X_L_A,
		OUT_X_H_A,
		OUT_Y_L_A,
		OUT_Y_H_A,
		OUT_Z_L_A,
		OUT_Z_H_A,
		FIFO_CTRL_REG_A,
		FIFO_SRC_REG_A,
		INT1_CFG_A,
		INT1_SRC_A,
		INT1_THS_A,
		INT1_DURATION_A,
		INT2_CFG_A,
		INT2_SOURCE_A,
		INT2_THS_A,
		INT2_DURATION_A,
		CLICK_CFG_A,
		CLICK_SRC_A,
		CLICK_THS_A,
		TIME_LIMIT_A,
		TIME_LATENCY_A,
		TIME_WINDOW_A
	};

	enum SpecialCommends : uint8_t {
		ENABLE_AUTOINCREMENT = (1<<7)
	};

	enum Bitmasks{
		CTRL_REG1_G_ODR_mask = 0b11000000,
		CTRL_REG1_G_Cutoff_mask = 0b00110000,

		CTRL_REG4_A_FS_mask = 0b00110000,
	};

	enum Bitfields{
		CTRL_REG1_A_LPen = 0x08,
	};

public:
	enum class Axis : uint8_t {
		X = 0x01,
		Y = 0x02,
		Z = 0x04
	};

	typedef enum : uint8_t {
		ODR_0Hz_PowerDown = 0x00,
		ODR_1Hz = 0x10,
		ODR_10Hz,
		ODR_25Hz,
		ODR_50Hz,
		ODR_100Hz,
		ODR_200Hz,
		ODR_400Hz,
		ODR_LowPowerMode_1620Hz,	///set only when low power mode is enabled
		ODR_Normal_1344Hz_LowPowerMode_5376Hz
	} OutputDataRate;


	typedef enum {
		FSR_2g = 0x00 << 4,
		FSR_4g = 0x01 << 4,
		FSR_8g = 0x02 << 4,
		FSR_16g = 0x03 << 4,
	} FullScaleRange;

	typedef enum : uint8_t {
		I2C_ADDRESS_0 = 0x30,
		I2C_ADDRESS_1 = 0x32
	} PossibleI2CAddress;

	constexpr Accelerometer(microhal::I2C &i2c, PossibleI2CAddress address)
		: I2CDevice(i2c, address) {
	}

	bool init(bool lowPowerMode, OutputDataRate odr, Axis activeAxis){
		if(lowPowerMode){
			if(enableLowPowerMode() == false) return false;
		} else {
			if(disableLowPowerMode() == false) return false;
		}

		if(setODR(odr)){
			return enableAxis(activeAxis);
		}
		return false;
	}

	bool enableAxis(Axis axis){
		return setBitsInRegister(CTRL_REG1_A, static_cast<uint8_t>(axis));
	}

	bool disableAxis(Axis axis){
		return clearBitsInRegister(CTRL_REG1_A, static_cast<uint8_t>(axis));
	}

	bool enableLowPowerMode(){
		return setBitsInRegister(CTRL_REG1_A, static_cast<uint8_t>(CTRL_REG1_A_LPen));
	}

	bool disableLowPowerMode(){
		return clearBitsInRegister(CTRL_REG1_A, static_cast<uint8_t>(CTRL_REG1_A_LPen));
	}

	bool setODR(OutputDataRate odr){
		return writeRegisterWithMask(CTRL_REG1_A, odr, CTRL_REG1_G_ODR_mask | CTRL_REG1_G_Cutoff_mask);
	}

	bool setFullScaleRange(FullScaleRange fsr){
		return writeRegisterWithMask(CTRL_REG4_A, fsr, CTRL_REG4_A_FS_mask);
	}

	bool isNewDataAvailable(Axis axis){
		uint8_t status;
		if(readRegister(STATUS_REG_A, status) == true){
			return status & static_cast<uint8_t>(axis);
		}
		return false;
	}

	bool readSamples(int16_t &x, int16_t &y, int16_t &z) {
		int16_t data[3];
		const bool status = readRegisters(OUT_X_L_A | ENABLE_AUTOINCREMENT,
				(uint16_t*)data, 3, microhal::LittleEndian);

		x = data[0];
		y = data[1];
		z = data[2];

		return status;
	}

	template <microhal::diagnostic::LogLevel level>
	void registerDump(microhal::diagnostic::Diagnostic<level> &log){
		uint8_t regVal;

		log << microhal::diagnostic::Warning <<	"----------- LSM330 accelerometer register dump -----------" <<  microhal::diagnostic::endl;

		for(uint8_t reg = CTRL_REG1_A; reg <= TIME_WINDOW_A; reg++){
			log << microhal::diagnostic::Warning << microhal::diagnostic::toHex(reg) << " ";

			if(readRegister(reg, regVal)){
				log << microhal::diagnostic::Warning << regVal
					<< microhal::diagnostic::endl;
			} else {
				log << microhal::diagnostic::Warning << "Error while reading."
					<< microhal::diagnostic::endl;
			}
		}

		log << microhal::diagnostic::Warning <<	"----------- End of register dump -----------"
			<<  microhal::diagnostic::endl;
	}
};

constexpr Accelerometer::Axis operator|(Accelerometer::Axis a, Accelerometer::Axis b){
	return static_cast<Accelerometer::Axis>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
} // namespace lsm330
#endif /* LSM330DLACCELEROMETER_H_ */
