/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for LSM330DL gyroscope

 @authors    Pawel
 created on: 14-12-2014
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
#ifndef LSM330DLGYROSCOPE_H_
#define LSM330DLGYROSCOPE_H_

#include "microhal.h"
#include "I2CDevice/I2CDevice.h"
#include "diagnostic/diagnostic.h"

namespace lsm330 {

class Gyroscope : protected microhal::I2CDevice{
	enum Registers : uint8_t {
		CTRL_REG1_G = 0x20,
		CTRL_REG2_G,
		CTRL_REG3_G,
		CTRL_REG4_G,
		CTRL_REG5_G,
		DATACAPTURE_G,
		OUT_TEMP_G,
		STATUS_REG_G,
		OUT_X_L_G,
		OUT_X_H_G,
		OUT_Y_L_G,
		OUT_Y_H_G,
		OUT_Z_L_G,
		OUT_Z_H_G,
		FIFO_CTRL_REG_G,
		FIFO_SRC_REG_G,
		INT1_CFG_G,
		INT1_SRC_G,
		INT1_THS_XH_G,
		INT1_THS_XL_G,
		INT1_THS_YH_G,
		INT1_THS_YL_G,
		INT1_THS_ZH_G,
		INT1_THS_ZL_G,
		INT1_DURATION_G
	};

	enum SpecialCommends : uint8_t {
		ENABLE_AUTOINCREMENT = (1<<7)
	};

	enum Bitmasks{
		CTRL_REG1_G_ODR_mask = 0b11000000,
		CTRL_REG1_G_Cutoff_mask = 0b00110000,

		CTRL_REG4_G_FS_mask = 0b00110000,
	};
public:
	enum class Axis : uint8_t {
		X = 0x01,
		Y = 0x02,
		Z = 0x04
	};

	enum class Mode : uint8_t {
		PowerDown = 0x00,
		NormalOrSleep = 0x08	///<! sleep mode only when all axis will be disabled
	};

	typedef enum {
		ODR_100Hz_cutoff_12_5Hz = 0b0000, ///<! output frequency 100Hz cut off frequency 12.5Hz
		ODR_100Hz_cutoff_25Hz = 0b0001, ///<! output frequency 100Hz cut off frequency 25
		//ODR_100Hz_cutoff_25Hz = 0b0010, ///<! output frequency 100Hz cut off frequency 25
		//ODR_100Hz_cutoff_25Hz = 0b0011, ///<! output frequency 100Hz cut off frequency 25
		ODR_200Hz_cutoff_12_5Hz = 0b0100, ///<! output frequency 200Hz cut off frequency 12.5
		ODR_200Hz_cutoff_25Hz = 0b0101, ///<! output frequency 200Hz cut off frequency 25
		ODR_200Hz_cutoff_50Hz = 0b0110, ///<! output frequency 200Hz cut off frequency 50
		ODR_200Hz_cutoff_70Hz = 0b0111, ///<! output frequency 200Hz cut off frequency 70
		ODR_400Hz_cutoff_20Hz = 0b1000, ///<! output frequency 400Hz cut off frequency 20
		ODR_400Hz_cutoff_25Hz = 0b1001, ///<! output frequency 400Hz cut off frequency 25
		ODR_400Hz_cutoff_50Hz = 0b1010, ///<! output frequency 400Hz cut off frequency 50
		ODR_400Hz_cutoff_110Hz = 0b1011, ///<! output frequency 400Hz cut off frequency 110
		ODR_800Hz_cutoff_30Hz = 0b1100, ///<! output frequency 800Hz cut off frequency 30
		ODR_800Hz_cutoff_35Hz = 0b1101, ///<! output frequency 800Hz cut off frequency 35
		ODR_800Hz_cutoff_50Hz = 0b1110, ///<! output frequency 800Hz cut off frequency 50
		ODR_800Hz_cutoff_110Hz = 0b1111, ///<! output frequency 800Hz cut off frequency 110
	} OdrCutoff;

	typedef enum {
		FSR_250dps = 0x00 << 4,
		FSR_500dps = 0x01 << 4,
		FSR_2000dps = 0x02 << 4,
	} FullScaleRange;

	enum class FIFOmode : uint8_t {
		Bypas,
		FIFO,
		Stream,
		StreamToFIFO,
		BypasToStream
	};

	typedef enum : uint8_t {
		I2C_ADDRESS_0 = 0xD0,
		I2C_ADDRESS_1 = 0xD2
	} PossibleI2CAddress;

	constexpr Gyroscope(microhal::I2C &i2c, PossibleI2CAddress address)
		: I2CDevice(i2c, address) {
	}
	~Gyroscope() {
	}

	bool init(Mode mode, OdrCutoff odr, Axis activeAxis){
		if(setMode(mode)){
			if(setODR(odr)){
				return enableAxis(activeAxis);
			}
		}
		return false;
	}

	bool enableAxis(Axis axis){
		return setBitsInRegister(CTRL_REG1_G, static_cast<uint8_t>(axis));
	}

	bool disableAxis(Axis axis){
		return clearBitsInRegister(CTRL_REG1_G, static_cast<uint8_t>(axis));
	}

	bool setMode(Mode mode){
		if(mode == Mode::PowerDown){
			return clearBitsInRegister(CTRL_REG1_G, static_cast<uint8_t>(Mode::NormalOrSleep));
		}else{
			return setBitsInRegister(CTRL_REG1_G, static_cast<uint8_t>(mode));
		}
		return false;
	}

	bool setODR(OdrCutoff odr){
		return writeRegisterWithMask(CTRL_REG1_G, odr, CTRL_REG1_G_ODR_mask | CTRL_REG1_G_Cutoff_mask);
	}

	bool setFullScaleRange(FullScaleRange fsr){
		return writeRegisterWithMask(CTRL_REG4_G, fsr, CTRL_REG4_G_FS_mask);
	}

	bool isNewDataAvailable(Axis axis){
		uint8_t status;
		if(readRegister(STATUS_REG_G, status) == true){
			return status & static_cast<uint8_t>(axis);
		}
		return false;
	}

	bool readSamples(int16_t &x, int16_t &y, int16_t &z) {
		int16_t data[3];
		const bool status = readRegisters(OUT_X_L_G | ENABLE_AUTOINCREMENT,
				(uint16_t*)data, 3, microhal::LittleEndian);

		x = data[0];
		y = data[1];
		z = data[2];

		return status;
	}

	bool readTemperature(int8_t &temp){
		return readRegister(OUT_TEMP_G, (uint8_t&)temp);
	}

	template <microhal::diagnostic::LogLevel level>
	void registerDump(microhal::diagnostic::Diagnostic<level> &log) {
		uint8_t regVal;

		log << microhal::diagnostic::Warning << "----------- LSM330 gyroscope register dump -----------" <<  microhal::diagnostic::endl;

		for(uint8_t reg = CTRL_REG1_G; reg <= INT1_DURATION_G; reg++){
			log << microhal::diagnostic::Warning << microhal::diagnostic::toHex(reg) << " ";

			if(readRegister(reg, regVal)){
				log << microhal::diagnostic::Warning << regVal << microhal::diagnostic::endl;
			} else {
				log << microhal::diagnostic::Warning << "Error while reading." << microhal::diagnostic::endl;
			}
		}

		log << microhal::diagnostic::Warning <<	"----------- End of register dump -----------" <<  microhal::diagnostic::endl;
	}
};

constexpr Gyroscope::Axis operator|(Gyroscope::Axis a, Gyroscope::Axis b){
	return static_cast<Gyroscope::Axis>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

} // namespace lsm330


#endif /* LSM330DLGYROSCOPE_H_ */
