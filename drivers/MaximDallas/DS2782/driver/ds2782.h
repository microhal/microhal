/* =========================================================================================== *//**
 @file        ds2782.h
 @authors     Michal Karczewski
 @version     v 0.1
 @package
 @brief       DS2782 driver
 @hardware    STM32F4xx
 @copyright   $Copyright$
 @details     Implementation of the DS2782

 *//* ============================================================================================ */

#ifndef DS2786_H_
#define DS2786_H_

/******************************************************************************
 * INCLUDES
 */
#include "microhal.h"
#include "I2CDevice/I2CDevice.h"
#include "diagnostic/diagnostic.h"

/** @addtogroup Devices
 *  @{
 *  @class DS2782
 *  @}
 *
 * @brief DS2782 stand-alone ocv-based fuel gauge 2-wire interface
 */

namespace ds2782 {

class DS2782: protected microhal::I2CDevice {
	enum Registers
		: uint8_t {
		STATUS = 0x01,
		RAAC_MSB,
		RAAC_LSB,
		RSAC_MSB,
		RSAC_LSB,
		RARC,
		RSRC,
		IAVG_MSB,
		IAVG_LSB,
		TEMP_MSB,
		TEMP_LSB,
		VOLT_MSB,
		VOLT_LSB,
		CURRENT_MSB,
		CURRENT_LSB,
		ACR_MSB,
		ACR_LSB,
		ACRL_MSB,
		ACRL_LSB,
		AS,
		SFR,
		FULL_MSB,
		FULL_LSB,
		AE_MSB,
		AE_LSB,
		SE_MSB,
		SE_LSB,
		EEPROM_REGISTER = 0x1F,
		USER_EEPROM_0,
		USER_EEPROM_1,
		USER_EEPROM_2,
		USER_EEPROM_3,
		USER_EEPROM_4,
		USER_EEPROM_5,
		USER_EEPROM_6,
		USER_EEPROM_7,
		USER_EEPROM_8,
		USER_EEPROM_9,
		USER_EEPROM_10,
		USER_EEPROM_11,
		USER_EEPROM_12,
		USER_EEPROM_13,
		USER_EEPROM_14,
		USER_EEPROM_15,
		ADD_USER_EEPROM_0,
		ADD_USER_EEPROM_1,
		ADD_USER_EEPROM_2,
		ADD_USER_EEPROM_3,
		ADD_USER_EEPROM_4,
		ADD_USER_EEPROM_5,
		ADD_USER_EEPROM_6,
		ADD_USER_EEPROM_7,
		ADD_USER_EEPROM_8,
		ADD_USER_EEPROM_9,
		ADD_USER_EEPROM_10,
		ADD_USER_EEPROM_11,
		ADD_USER_EEPROM_12,
		ADD_USER_EEPROM_13,
		ADD_USER_EEPROM_14,
		ADD_USER_EEPROM_15,
		EEPROM_CONTROL = 0x60,
		EEPROM_AB,
		EEPROM_AC_MSB,
		EEPROM_AC_LSB,
		EEPROM_VCHG,
		EEPROM_IMIN,
		EEPROM_VAE,
		EEPROM_IAE,
		EEPROM_ACTIVE_EMPTY,
		EEPROM_RSNSP,
		EEPROM_FULL_40_MSB,
		EEPROM_FULL_40_LSB,
		EEPROM_FULL_3040,
		EEPROM_FULL_2030,
		EEPROM_FULL_1020,
		EEPROM_FULL_0010,
		EEPROM_AE_3040,
		EEPROM_AE_2030,
		EEPROM_AE_1020,
		EEPROM_AE_0010,
		EEPROM_SE_3040,
		EEPROM_SE_2030,
		EEPROM_SE_1020,
		EEPROM_SE_0010,
		EEPROM_RSGAIN_MSB,
		EEPROM_RSGAIN_LSB,
		EEPROM_RSTC,
		EEPROM_FRSGAIN_MSB,
		EEPROM_FRSGAIN_LSB,
		EEPROM_2WIRE_ADDRESS = 0x7E,
		UNIQUE_ID_0 = 0xF0,
		UNIQUE_ID_1,
		UNIQUE_ID_2,
		UNIQUE_ID_3,
		UNIQUE_ID_4,
		UNIQUE_ID_5,
		UNIQUE_ID_6,
		UNIQUE_ID_7,
		FUNCTION_COMMAND_REGISTER = 0xFE
	};

	enum StatusBits
	: uint8_t {
		CHGTF = 0x80,
		AEF = 0x40,
		SEF = 0x20,
		LEARNF = 0x10,
		UVF = 0x04,
		PORF = 0x02
	};

	enum ControlBits
	: uint8_t {
		UVEN = 0x40,
		PMOD = 0x20,
	};

	enum SpecialFeatureRegister
	: uint8_t {
		SAWE = 0x02,
		PIOSC = 0x01
	};

	enum EEPROMRegister
	: uint8_t {
		EEC = 0x40,
		LOCK = 0x20,
		BL1 = 0x02,
		BL0 = 0x01
	};

	enum FunctionCommand
	: uint8_t {
		COPY_DATA_0 = 0x42,
		COPY_DATA_1 = 0x44,
		RECALL_DATA_0 = 0xB2,
		RECALL_DATA_1 = 0xB4,
		LOCK_0 = 0x63,
		LOCK_1 = 0x66
	};

public:
	typedef enum
		: uint8_t {
			DS2782_ADDR = 0x68
	} PossibleI2CAddress;

	constexpr DS2782(microhal::I2C &i2c, PossibleI2CAddress address) :
			I2CDevice(i2c, address) {
	}

	/**
	 *@brief Read eeprom control register.
	 *
	 *@param [OUT] data - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readEEPROMControl(uint8_t *data) {
		const bool status = readRegister(EEPROM_CONTROL, *data);
		return status;
	}

	/**
	 *@brief Check under voltage SLEEP enable bit on eeprom control register.
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool checkUnderVoltageSleep() {
		uint8_t data;
		bool status;
		readRegister(EEPROM_CONTROL, data);
		status = data & UVEN;
		return status;
	}

	/**
	 *@brief clear under voltage SLEEP enable bit on eeprom control register.
	 *
	 *@retval   true if clearing bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool clearUnderVoltageSleep(){
		return clearBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(UVEN));
	}

	/**
	 *@brief Set  under voltage SLEEP enable bit on eeprom control bit on config register.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool enableUnderVoltageSleep(){
		return setBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(UVEN));
	}

	/**
	 *@brief Check power mode enable bit on eeprom control register.
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool checkPowerMode() {
		uint8_t data;
		bool status;
		readRegister(EEPROM_CONTROL, data);
		status = data & PMOD;
		return status;
	}

	/**
	 *@brief clear power mode enable bit on eeprom control register.
	 *
	 *@retval   true if clearing bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool clearPowerMode(){
		return clearBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(PMOD));
	}

	/**
	 *@brief Set power mode enable bit on eeprom control bit on config register.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool enablePowerMode(){
		return setBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(PMOD));
	}

	/**
	 *@brief Read voltage registers.
	 *
	 *@param [OUT] voltageValue - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readVoltage(uint8_t *voltageValue) {
		const bool status = readRegisters(VOLT_MSB,
				voltageValue, 2);
		return status;
	}

	/**
	 *@brief Read temperature registers.
	 *
	 *@param [OUT] temperatureValue - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readTemperature(uint8_t *temperatureValue) {
		const bool status = readRegisters(TEMP_MSB,
				temperatureValue, 2);
		return status;
	}

	/**
	 *@brief Read current registers.
	 *
	 *@param [OUT] currentValue - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readCurrent(uint8_t *currentValue) {
		const bool status = readRegisters(CURRENT_MSB,
				currentValue, 2);
		return status;
	}

	/**
	 *@brief Read average current registers.
	 *
	 *@param [OUT] averageColtageValue - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readAverageCurrent(uint8_t *averageColtageValue) {
		const bool status = readRegisters(IAVG_MSB,
				averageColtageValue, 2);
		return status;
	}

	/**
	 *@brief Read current offset correction registers.
	 *
	 *@param [OUT] currentOffsetCorrection - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readCurrentOffsetCorrection(uint8_t *currentOffsetCorrection) {
		const bool status = readRegisters(ACR_MSB,
				currentOffsetCorrection, 2);
		return status;
	}

	/**
	 *@brief Read current measurement calibration register.
	 *
	 *@param [OUT] currentCalibrationRegister - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readCurrentCalibrationCalibrationRegister(uint8_t *currentCalibrationCalibrationRegister) {
		const bool status = readRegisters(EEPROM_RSGAIN_MSB,
				currentCalibrationCalibrationRegister, 2);
		return status;
	}

	/**
	 *@brief Write current measurement calibration register.
	 *
	 *@param [IN] data - variable to register (array of two 8 bit values)
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeCurrentCalibrationCalibrationRegister(uint8_t* data) {
		const bool status = writeRegisters(EEPROM_RSGAIN_MSB, data, 2);
		return status;
	}

	/**
	 *@brief Read current measurement calibration factory value.
	 *
	 *@param [OUT] currentCalibrationRegisterFactoryValue - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readCurrentOffsetCorrectionFactoryValue(uint8_t *currentCalibrationRegisterFactoryValue) {
		const bool status = readRegisters(ACR_MSB,
				currentCalibrationRegisterFactoryValue, 2);
		return status;
	}

	/**
	 *@brief Write sense resistor temperature compensation.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeSenseResistorTemperatureCompensation(uint8_t data) {
		const bool status = writeRegister(EEPROM_RSTC, data);
		return status;
	}

	/**
	 *@brief Read sense resistor temperature compensation.
	 *
	 *@param [OUT] senseResistorTemperatureCompensation - variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readSenseResistorTemperatureCompensation(uint8_t* senseResistorTemperatureCompensation) {
		const bool status = readRegister(EEPROM_RSTC,
				*senseResistorTemperatureCompensation);
		return status;
	}

	/**
	 *@brief Write current accumulation register.
	 *
	 *@param [IN] data - variable to register (array of two 8 bit values)
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeCurrentAccumulation(uint8_t* data) {
		const bool status = writeRegisters(ACR_MSB, data, 2);
		return status;
	}

	/**
	 *@brief Read current accumulation register.
	 *
	 *@param [OUT] senseResistorTemperatureCompensation - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readCurrentAccumulation(uint8_t *currentAcculumation) {
		const bool status = readRegisters(ACR_MSB,
				currentAcculumation, 2);
		return status;
	}

	/**
	 *@brief Read current accumulation fractional results register.
	 *
	 *@param [OUT] currentAcculumationFractionalResults - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readCurrentAccumulationFractionalResults(uint8_t *currentAcculumationFractionalResults) {
		const bool status = readRegisters(ACRL_MSB,
				currentAcculumationFractionalResults, 2);
		return status;
	}

	/**
	 *@brief Read accumulation bias register.
	 *
	 *@param [OUT] accumulationBias - variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readAccumulationBiasRegister(uint8_t* accumulationBias) {
		const bool status = readRegister(EEPROM_AB,
				*accumulationBias);
		return status;
	}

	/**
	 *@brief Read accumulation bias register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeAccumulationBiasRegister(uint8_t data) {
		const bool status = writeRegister(EEPROM_AB, data);
		return status;
	}

	/**
	 *@brief Read sense resistor prime register.
	 *
	 *@param [OUT] senseResistorPrime - variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readSenseResistorPrimeRegister(uint8_t* senseResistorPrime) {
		const bool status = readRegister(EEPROM_RSNSP,
				*senseResistorPrime);
		return status;
	}

	/**
	 *@brief Write sense resistor prime register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeSenseResistorPrimeRegister(uint8_t data) {
		const bool status = writeRegister(EEPROM_RSNSP, data);
		return status;
	}


	/**
	 *@brief Read charge voltage register.
	 *
	 *@param [OUT] chargeVoltage - variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readChargeVoltageRegister(uint8_t* chargeVoltage) {
		const bool status = readRegister(EEPROM_VCHG,
				*chargeVoltage);
		return status;
	}

	/**
	 *@brief Write charge voltage register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeChargeVoltageRegister(uint8_t data) {
		const bool status = writeRegister(EEPROM_VCHG, data);
		return status;
	}

	/**
	 *@brief Read minimum charge current register.
	 *
	 *@param [OUT] minimumChargeCurrent - variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readMinimumChargeCurrentRegister(uint8_t* minimumChargeCurrent) {
		const bool status = readRegister(EEPROM_IMIN,
				*minimumChargeCurrent);
		return status;
	}

	/**
	 *@brief Write charge voltage register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeMinimumChargeCurrentRegister(uint8_t data) {
		const bool status = writeRegister(EEPROM_IMIN, data);
		return status;
	}

	/**
	 *@brief Read active empty voltage register.
	 *
	 *@param [OUT] activeEmptyVoltage - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readActiveEmptyVoltage(uint8_t* activeEmptyVoltage) {
		const bool status = readRegister(EEPROM_VAE,
				*activeEmptyVoltage);
		return status;
	}

	/**
	 *@brief Write active empty voltage register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeActiveEmptyVoltage(uint8_t data) {
		const bool status = writeRegister(EEPROM_VAE, data);
		return status;
	}

	/**
	 *@brief Read active empty current register.
	 *
	 *@param [OUT] activeChargeCurrent - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readActiveEmptyCurrent(uint8_t* activeChargeCurrent) {
		const bool status = readRegister(EEPROM_IAE,
				*activeChargeCurrent);
		return status;
	}

	/**
	 *@brief Write active empty current register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeActiveEmptyCurrent(uint8_t data) {
		const bool status = writeRegister(EEPROM_IAE, data);
		return status;
	}

	/**
	 *@brief Read aging capacity register.
	 *
	 *@param [OUT] agingCapacity - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readAgingCapacity(uint8_t* agingCapacity) {
		const bool status = readRegisters(EEPROM_AC_MSB,
				agingCapacity, 2);
		return status;
	}

	/**
	 *@brief Write aging capacity register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeAgingCapacity(uint8_t *data) {
		const bool status = writeRegisters(EEPROM_AC_MSB, data, 2);
		return status;
	}

	/**
	 *@brief Read age scalar register.
	 *
	 *@param [OUT] ageScalar - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readAgeScalar(uint8_t* ageScalar) {
		const bool status = readRegister(AS,
				*ageScalar);
		return status;
	}

	/**
	 *@brief Write age scalar register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeAgeScalar(uint8_t data) {
		const bool status = writeRegister(AS, data);
		return status;
	}

	/**
	 *@brief Read remaining active absolute capacity register.
	 *
	 *@param [OUT] remainingActiveAbsoluteCapacity - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readRemainingActiveAbsoluteCapacityRegister(uint8_t* remainingActiveAbsoluteCapacity) {
		const bool status = readRegisters(RAAC_MSB,
				remainingActiveAbsoluteCapacity, 2);
		return status;
	}


	/**
	 *@brief Read remaining standby absolute capacity register.
	 *
	 *@param [OUT] remainingStandbyAbsoluteCapacity - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readRemainingStandbyAbsoluteCapacityRegister(uint8_t* remainingStandbyAbsoluteCapacity) {
		const bool status = readRegisters(RSAC_MSB,
				remainingStandbyAbsoluteCapacity, 2);
		return status;
	}

	/**
	 *@brief Read remaining active relative capacity register.
	 *
	 *@param [OUT] remainingActiveRelativeCapacity - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readRemainingActiveRelativeCapacityRegister(uint8_t* remainingActiveRelativeCapacity) {
		const bool status = readRegister(RARC,
				*remainingActiveRelativeCapacity);
		return status;
	}

	/**
	 *@brief Read remaining standby relative capacity register.
	 *
	 *@param [OUT] remainingStandbyRelativeCapacity - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readRemainingStandbyRelativeCapacityRegister(uint8_t* remainingStandbyRelativeCapacity) {
		const bool status = readRegister(RSRC,
				*remainingStandbyRelativeCapacity);
		return status;
	}

	/**
	 *@brief Read status register.
	 *
	 *@param [OUT] data - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readStatus(uint8_t *data) {
		const bool status = readRegister(STATUS, *data);
		return status;
	}

	/**
	 *@brief Check charge termination flag.
	 *
	 *@retval   true if ( VOLT > VCHG ) AND ( 0 < IAVG < IMIN ) continuously for a period between two IAVG register updates ( 28s to 56s ).
	 *@retval   false if RARC < 90%
	 */

	bool checkChargeTerminationFlag() {
		uint8_t data;
		bool status;
		readRegister(STATUS, data);
		status = data & CHGTF;
		return status;
	}

	/**
	 *@brief Check active empty flag.
	 *
	 *@retval   true if VOLT < VAE
	 *@retval   false if RARC > 5%
	 */

	bool checkActiveEmptyFlag() {
		uint8_t data;
		bool status;
		readRegister(STATUS, data);
		status = data & AEF;
		return status;
	}


	/**
	 *@brief Check standby empty flag.
	 *
	 *@retval   true if RSRC < 10%
	 *@retval   false if RSRC > 15%
	 */

	bool checkStandbyEmptyFlag() {
		uint8_t data;
		bool status;
		readRegister(STATUS, data);
		status = data & SEF;
		return status;
	}

	/**
	 *@brief Check learn flag.
	 *
	 *@retval   true if ( VOLT falls from above VAE to below VAE ) AND ( CURRENT > IAE )
	 *@retval   false if ( CHGTF = 1 ) OR ( CURRENT < +100μV ) OR ( ACR = 0 **) OR ( ACR written or recalled from EEPROM) OR ( SLEEP Entered )
	 */

	bool checkLearnFlag() {
		uint8_t data;
		bool status;
		readRegister(STATUS, data);
		status = data & LEARNF;
		return status;
	}

	/**
	 *@brief Check under voltage flag.
	 *
	 *@retval   true if VOLT < VSLEEP
	 *@retval   false if user set
	 */

	bool checkUnderVoltageFlag() {
		uint8_t data;
		bool status;
		readRegister(STATUS, data);
		status = data & UVF;
		return status;
	}

	/**
	 *@brief Set 0 to under voltage flag.
	 *
	 *@retval   true if setting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool resetUnderVoltageFlag(){
		return clearBitsInRegister(STATUS, static_cast<uint8_t>(UVF));
	}

	/**
	 *@brief Check power on reset flag.
	 *
	 *@retval   true if Power-Up by hardware.
	 *@retval   false if user set
	 */

	bool checkPowerOnResetFlag() {
		uint8_t data;
		bool status;
		readRegister(STATUS, data);
		status = data & PORF;
		return status;
	}

	/**
	 *@brief Set 0 to power on reset flag.
	 *
	 *@retval   true if setting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool resetPowerOnResetFlag(){
		return clearBitsInRegister(STATUS, static_cast<uint8_t>(PORF));
	}

	/**
	 *@brief Check under voltage sleep flag.
	 *
	 *@retval   true if enable transition.
	 *@retval   false if disabled transition.
	 */

	bool checkUnderVoltageSleepFlag() {
		uint8_t data;
		bool status;
		readRegister(EEPROM_CONTROL, data);
		status = data & UVEN;
		return status;
	}

	/**
	 *@brief Set 0 to under voltage sleep flag.
	 *
	 *@retval   true if setting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool resetUnderVoltageSleepFlag(){
		return clearBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(UVEN));
	}

	/**
	 *@brief Set 1 to under voltage sleep flag.
	 *
	 *@retval   true if setting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool setUnderVoltageSleepFlag(){
		return setBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(UVEN));
	}

	/**
	 *@brief Check power mode flag.
	 *
	 *@retval   true if transition is enabled.
	 *@retval   false if transition is disabled.
	 */

	bool checkPowerModeFlag() {
		uint8_t data;
		bool status;
		readRegister(EEPROM_CONTROL, data);
		status = data & PMOD;
		return status;
	}

	/**
	 *@brief Set 0 to power mode flag.
	 *
	 *@retval   true if setting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool resetPowerModeFlag(){
		return clearBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(PMOD));
	}

	/**
	 *@brief Set 1 to power mode flag.
	 *
	 *@retval   true if setting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool setPowerModeFlag(){
		return setBitsInRegister(EEPROM_CONTROL, static_cast<uint8_t>(PMOD));
	}

	/**
	 *@brief Check slave address write flag.
	 *
	 *@retval   true if Slave Address Write Enable is enabled to write .
	 *@retval   false if Slave Address Write Enable is disabled to write.
	 */

	bool checkSlaveAddressWriteFlag() {
		uint8_t data;
		bool status;
		readRegister(SFR, data);
		status = data & SAWE;
		return status;
	}

	/**
	 *@brief Set 0 to Slave Address Write Enable flag.
	 *
	 *@retval   true if setting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool resetSlaveAddressWriteFlag(){
		return clearBitsInRegister(SFR, static_cast<uint8_t>(SAWE));
	}

	/**
	 *@brief Set 1 to Slave Address Write Enable flag.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool setSlaveAddressWriteFlag(){
		return setBitsInRegister(SFR, static_cast<uint8_t>(SAWE));
	}

	/**
	 *@brief Check PIO Sense and Control flag.
	 *
	 *@retval   true if PIO pin ≥ Vih.
	 *@retval   false if PIO pin ≤ Vil.
	 */

	bool checkPIOSenseControlFlag() {
		uint8_t data;
		bool status;
		readRegister(SFR, data);
		status = data & PIOSC;
		return status;
	}

	/**
	 *@brief Set 0 to PIO Sense and Control flag.
	 *
	 *@retval   true if setting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool resetPIOSenseControlFlag(){
		return clearBitsInRegister(SFR, static_cast<uint8_t>(PIOSC));
	}

	/**
	 *@brief Set 1 to PIO Sense and Control flag.
	 *
	 *@retval   true if setting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool setPIOSenseControlFlag(){
		return setBitsInRegister(SFR, static_cast<uint8_t>(PIOSC));
	}

	/**
	 *@brief Check EEPROM Copy flag.
	 *
	 *@retval   true if Copy Data command executed.
	 *@retval   false if Copy Data command completes.
	 */

	bool checkEEPROMCopyFlag() {
		uint8_t data;
		bool status;
		readRegister(EEPROM_REGISTER, data);
		status = data & EEC;
		return status;
	}

	/**
	 *@brief Check EEPROM Lock flag.
	 *
	 *@retval   true if Lock command enabled.
	 *@retval   false if Lock command disabled.
	 */

	bool checkEEPROMLockFlag() {
		uint8_t data;
		bool status;
		readRegister(EEPROM_REGISTER, data);
		status = data & LOCK;
		return status;
	}

	/**
	 *@brief Set 1 to EEPROM Lock flag.
	 *
	 *@retval   true if setting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool setEEPROMLockFlag(){
		return setBitsInRegister(EEPROM_REGISTER, static_cast<uint8_t>(LOCK));
	}

	/**
	 *@brief Check EEPROM Block 1 Lock flag.
	 *
	 *@retval   true if Lock command enabled.
	 *@retval   false if Lock command disabled.
	 */

	bool checkEEPROMBlock1LockFlag() {
		uint8_t data;
		bool status;
		readRegister(EEPROM_REGISTER, data);
		status = data & BL1;
		return status;
	}

	/**
	 *@brief Check EEPROM Block 0 Lock flag.
	 *
	 *@retval   true if Lock command enabled.
	 *@retval   false if Lock command disabled.
	 */

	bool checkEEPROMBlock0LockFlag() {
		uint8_t data;
		bool status;
		readRegister(EEPROM_REGISTER, data);
		status = data & BL0;
		return status;
	}

	/**
	 *@brief Write slave address register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeSlaveAddress(uint8_t data) {
		const bool status = writeRegister(EEPROM_2WIRE_ADDRESS, data);
		return status;
	}

};

} // namespace ds2782

#endif
