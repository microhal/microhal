/* =========================================================================================== *//**
 @file        ds2786.h
 @authors     Michal Karczewski
 @version     v 0.1
 @package
 @brief       DS2786 driver
 @hardware    STM32F4xx
 @copyright   $Copyright$
 @details     Implementation of the DS2786

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
 *  @class DS2786
 *  @}
 *
 * @brief DS2786 stand-alone ocv-based fuel gauge 2-wire interface
 */

namespace ds2786 {

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
	enum ConfigBits
	: uint8_t {
		PORF = 0x40,
		SMOD = 0x20,
		LDIS = 0x10,
		VODIS = 0x08,
		ITEMP = 0x04,
		AIN1 = 0x02,
		AIN0 = 0x01
	};

	enum CommandRegister
	: uint8_t {
		POR = 0x80,
		POCV = 0x08,
		SOCV = 0x04,
		RCALL = 0x02,
		COPY = 0x01
	};

public:
	typedef enum
		: uint8_t {
			DS2786_ADDR = 0x6C
	} PossibleI2CAddress;

	constexpr DS2786(microhal::I2C &i2c, PossibleI2CAddress address) :
			I2CDevice(i2c, address) {
	}

	/**
	 *@brief Read config register.
	 *
	 *@param [OUT] data - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readConfig(uint8_t *data) {
		const bool status = readRegister(CONFIG, *data);
		return status;
	}

	/**
	 *@brief Check power on reset flag bit on config register.
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool checkPowerOnResetFlag() {
		uint8_t data;
		bool status;
		readRegister(CONFIG, data);
		status = data & PORF;
		return status;
	}

	/**
	 *@brief clear power on reset flag bit on config register.
	 *
	 *@retval   true if clearing bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool clearPowerOnResetFlag(){
		return clearBitsInRegister(CONFIG, static_cast<uint8_t>(PORF));
	}

	/**
	 *@brief Check sleep mode bit on config register.
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool checkSleepMode() {
		uint8_t data;
		bool status;
		readRegister(CONFIG, data);
		status = data & SMOD;
		return status;
	}

	/**
	 *@brief Set sleep mode bit on config register.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool enableSleepMode(){
		return setBitsInRegister(CONFIG, static_cast<uint8_t>(SMOD));
	}

	/**
	 *@brief clear sleep mode bit on config register.
	 *
	 *@retval   true if clearing bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool disableSleepMode(){
		return clearBitsInRegister(CONFIG, static_cast<uint8_t>(SMOD));
	}

	/**
	 *@brief Check capacity learning bit on config register.
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool checkCapacityLearning() {
		uint8_t data;
		bool status;
		readRegister(CONFIG, data);
		status = data & LDIS;
		return status;
	}

	/**
	 *@brief Set capacity learning bit on config register.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool enableCapacityLearning(){
		return setBitsInRegister(CONFIG, static_cast<uint8_t>(LDIS));
	}

	/**
	 *@brief clear capacity learning bit on config register.
	 *
	 *@retval   true if clearing bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool disableCapacityLearning(){
		return clearBitsInRegister(CONFIG, static_cast<uint8_t>(LDIS));
	}

	/**
	 *@brief Check Vout disable bit on config register.
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool checkVoutDisable() {
		uint8_t data;
		bool status;
		readRegister(CONFIG, data);
		status = data & VODIS;
		return status;
	}

	/**
	 *@brief Set Vout disable bit on config register.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool enableVoutDisable(){
		return setBitsInRegister(CONFIG, static_cast<uint8_t>(VODIS));
	}

	/**
	 *@brief Clear Vout disable bit on config register.
	 *
	 *@retval   true if clearing bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool disableVoutDisable(){
		return clearBitsInRegister(CONFIG, static_cast<uint8_t>(VODIS));
	}

	/**
	 *@brief Check temperature interior sensor bit on config register.
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool checkTempInteriorSensor() {
		uint8_t data;
		bool status;
		readRegister(CONFIG, data);
		status = data & ITEMP;
		return status;
	}

	/**
	 *@brief Set temperature interior sensor bit on config register.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool enableTempInteriorSensor(){
		return setBitsInRegister(CONFIG, static_cast<uint8_t>(ITEMP));
	}

	/**
	 *@brief Clear temperature interior sensor bit on config register.
	 *
	 *@retval   true if clearing bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool disableTempInteriorSensor(){
		return clearBitsInRegister(CONFIG, static_cast<uint8_t>(ITEMP));
	}

	/**
	 *@brief Check AIN1 conversion vaild bit on config register.
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool checkAIN1ConversionValid() {
		uint8_t data;
		bool status;
		readRegister(CONFIG, data);
		status = data & AIN1;
		return status;
	}

	/**
	 *@brief Check AIN0 conversion vaild bit on config register.
	 *
	 *@retval   true if read register is successful
	 *@retval   false if an error occurred
	 */

	bool checkAIN0ConversionValid() {
		uint8_t data;
		bool status;
		readRegister(CONFIG, data);
		status = data & AIN0;
		return status;
	}

	/**
	 *@brief Read command register.
	 *
	 *@param [OUT] data - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readCommand(uint8_t *data) {
		const bool status = readRegister(COMMAND, *data);
		return status;
	}

	/**
	 *@brief Check power on reset bit on command register.
	 *
	 *@retval   true if read register is successful
	 *@retval   false if an error occurred
	 */

	bool checkPowerOnReset() {
		uint8_t data;
		bool status;
		readRegister(COMMAND, data);
		status = data & POR;
		return status;
	}

	/**
	 *@brief Set power on reset bit on command register.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool enablePowerOnReset(){
		return setBitsInRegister(COMMAND, static_cast<uint8_t>(POR));
	}

	/**
	 *@brief Check present ocv bit on command register.
	 *
	 *@retval   true if read register is successful
	 *@retval   false if an error occurred
	 */

	bool checkPresentOCV() {
		uint8_t data;
		bool status;
		readRegister(COMMAND, data);
		status = data & POCV;
		return status;
	}

	/**
	 *@brief Set present ocv bit on command register.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool enablePresentOCV(){
		return setBitsInRegister(COMMAND, static_cast<uint8_t>(POCV));
	}

	/**
	 *@brief Check stored ocv bit on command register.
	 *
	 *@retval   true if read register is successful
	 *@retval   false if an error occurred
	 */

	bool checkStoredOCV() {
		uint8_t data;
		bool status;
		readRegister(COMMAND, data);
		status = data & SOCV;
		return status;
	}

	/**
	 *@brief Set stored ocv bit on command register.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool enableStoredOCV(){
		return setBitsInRegister(COMMAND, static_cast<uint8_t>(SOCV));
	}

	/**
	 *@brief Check recall bit on command register.
	 *
	 *@retval   true if read register is successful
	 *@retval   false if an error occurred
	 */

	bool checkRecall() {
		uint8_t data;
		bool status;
		readRegister(COMMAND, data);
		status = data & RCALL;
		return status;
	}

	/**
	 *@brief Set recall bit on command register.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool enableRecall(){
		return setBitsInRegister(COMMAND, static_cast<uint8_t>(RCALL));
	}

	/**
	 *@brief Clear recall bit on command register.
	 *
	 *@retval   true if clearing bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool disableRecall(){
			return clearBitsInRegister(COMMAND, static_cast<uint8_t>(RCALL));
		}

	/**
	 *@brief Check copy bit on command register.
	 *
	 *@retval   true if read register is successful
	 *@retval   false if an error occurred
	 */

	bool checkCopy() {
		uint8_t data;
		bool status;
		readRegister(COMMAND, data);
		status = data & COPY;
		return status;
	}

	/**
	 *@brief Set copy bit on command register.
	 *
	 *@retval   true if seting bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool enableCopy(){
		return setBitsInRegister(COMMAND, static_cast<uint8_t>(COPY));
	}

	/**
	 *@brief Clear copy bit on command register.
	 *
	 *@retval   true if clearing bit in register is successful
	 *@retval   false if an error occurred
	 */

	bool disableCopy(){
		return clearBitsInRegister(COMMAND, static_cast<uint8_t>(COPY));
	}

	/**
	 *@brief Read MSB and LSB axuiliary 0 registers.
	 *
	 *@param [OUT] axuiliaryValue - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readAxuiliary0(uint8_t *axuiliaryValue) {
		const bool status = readRegisters(AUXILARY_MSB_0, axuiliaryValue, 2);
		return status;
	}

	/**
	 *@brief Read MSB and LSB axuiliary 1 registers.
	 *
	 *@param [OUT] axuiliaryValue - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readAxuiliary1(uint8_t *axuiliaryValue) {
		const bool status = readRegisters(AUXILARY_MSB_1, axuiliaryValue, 2);
		return status;
	}

	/**
	 *@brief Read MSB and LSB voltage registers.
	 *
	 *@param [OUT] voltageValue - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readVoltage(uint8_t *voltageValue) {
		const bool status = readRegisters(VOLTAGE_MSB, voltageValue, 2);
		return status;
	}

	/**
	 *@brief Read MSB and LSB current registers.
	 *
	 *@param [OUT] currentValue - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readCurrent(uint8_t *currentValue) {
		const bool status = readRegisters(CURRENT_MSB, currentValue, 2);
		return status;
	}

	/**
	 *@brief Read initial voltage registers.
	 *
	 *@param [OUT] initialVoltageValue - array of two values. Variable for registers
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readInitialVoltage(uint8_t *initialVoltageValue) {
		const bool status = readRegisters(INITIAL_VOLTAGE_MSB, initialVoltageValue, 2);
		return status;
	}

	/**
	 *@brief Read initial voltage register.
	 *
	 *@param [OUT] relativeCapacityValue - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readRelativeCapacity(uint8_t *relativeCapacityValue) {
		const bool status = readRegister(RELATIVE_CAPACITY, *relativeCapacityValue);
		return status;
	}

	/**
	 *@brief Read last ocv capacity register.
	 *
	 *@param [OUT] lastOCVCapacityValue - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readLastOCVCapacity(uint8_t *lastOCVCapacityValue) {
		const bool status = readRegister(LAST_OCV_CAPACITY, *lastOCVCapacityValue);
		return status;
	}

	/**
	 *@brief Read capacity scaling factor register.
	 *
	 *@param [OUT] capacityScalingFactorValue - variable for register
	 *
	 *@retval   true if reading register is successful
	 *@retval   false if an error occurred
	 */

	bool readCapacityScalingFactor(uint8_t *capacityScalingFactorValue) {
		const bool status = readRegister(CAPACITY_SCALING_FACTOR, *capacityScalingFactorValue);
		return status;
	}

	/**
	 *@brief Read current offset from eeprom register.
	 *
	 *@param [OUT] data - variable for register
	 *
	 *@retval   true if reading from register is successful
	 *@retval   false if an error occurred
	 */

	bool readEEPROMCurrentOffset(uint8_t *data) {
		enableRecall();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		const bool status = readRegister(EEPROM_CURRENT_OFFSET, *data);
		disableRecall();
		return status;
	}

	/**
	 *@brief Write current offset to eeprom register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeEEPROMCurrentOffset(uint8_t data) {
		const bool status = writeRegister(EEPROM_CURRENT_OFFSET, data);
		enableCopy();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		disableCopy();
		return status;
	}

	/**
	 *@brief Read remaining capacity breakpoint value from eeprom registers.
	 *
	 *@param [OUT] data - variable for register
	 *@param [IN] number - number of capacity register (from 1 to 7)
	 *
	 *@retval   true if reading from register is successful
	 *@retval   false if an error occurred
	 */

	bool readEEPROMCapacity(uint8_t *data, uint8_t number) {
		bool status;
		if (number > 0 && number < 8) {
			enableRecall();
			std::this_thread::sleep_for(std::chrono::milliseconds {500});
			status = readRegister(EEPROM_CAPACITY_1 + number - 1, *data);
			disableRecall();
		}
		else
			status = 0;
		return status;
	}

	/**
	 *@brief Write remaining capacity breakpoint value to eeprom registers.
	 *
	 *@param [IN] data - variable to register
	 *@param [IN] number - number of capacity register (from 1 to 7)
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeEEPROMCapacity(uint8_t data, uint8_t number) {
		bool status;
		if (number > 0 && number < 8) {
			status = writeRegister(EEPROM_CAPACITY_1 + number - 1, data);
			enableCopy();
			std::this_thread::sleep_for(std::chrono::milliseconds {500});
			disableCopy();
		}
		else
			status = 0;
		return status;
	}

	/**
	 *@brief Read voltage breakpoint value from eeprom registers.
	 *
	 *@param [OUT] data - variable for register
	 *@param [IN] number - number of voltage register (from 0 to 8)
	 *
	 *@retval   true if reading from register is successful
	 *@retval   false if an error occurred
	 */

	bool readEEPROMVoltageBreakpoint(uint8_t *data, uint8_t number) {
		bool status;
		if (number >= 0 && number <= 8) {
			enableRecall();
			std::this_thread::sleep_for(std::chrono::milliseconds {500});
			status = readRegisters(EEPROM_VOLTAGE_BREAKPOINT_MSB_0 + number, data, 2);
			disableRecall();
		}
		else
			status = 0;
		return status;
	}

	/**
	 *@brief Write voltage breakpoint value to eeprom registers.
	 *
	 *@param [IN] data - variable to register
	 *@param [IN] number - number of capacity register (from 0 to 8)
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeEEPROMVoltageBreakpoint(uint8_t* data, uint8_t number) {
		bool status;
		if (number >= 0 && number <= 8) {
			status = writeRegisters(EEPROM_VOLTAGE_BREAKPOINT_MSB_0 + number, data, 2);
			enableCopy();
			std::this_thread::sleep_for(std::chrono::milliseconds {500});
			disableCopy();
		}
		else
			status = 0;
		return status;
	}

	/**
	 *@brief Read initial capacity scaling from eeprom register.
	 *
	 *@param [OUT] data - variable for register
	 *
	 *@retval   true if reading from register is successful
	 *@retval   false if an error occurred
	 */

	bool readEEPROMInitialCapacityScaling(uint8_t *data) {
		enableRecall();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		const bool status = readRegister(EEPROM_INITIAL_CAPACITY_SCALING, *data);
		disableRecall();
		return status;
	}

	/**
	 *@brief Write initial capacity scaling to eeprom register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeEEPROMInitialCapacityScaling(uint8_t data) {
		const bool status = writeRegister(EEPROM_INITIAL_CAPACITY_SCALING, data);
		enableCopy();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		disableCopy();
		return status;
	}

	/**
	 *@brief Read current threshold from eeprom register.
	 *
	 *@param [OUT] data - variable for register
	 *
	 *@retval   true if reading from register is successful
	 *@retval   false if an error occurred
	 */

	bool readEEPROMCurrentThreshold(uint8_t *data) {
		enableRecall();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		const bool status = readRegister(EEPROM_CURRENT_THRESHOLD, *data);
		disableRecall();
		return status;
	}

	/**
	 *@brief Write current threshold to eeprom register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeEEPROMCurrentThreshold(uint8_t data) {
		const bool status = writeRegister(EEPROM_CURRENT_THRESHOLD, data);
		enableCopy();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		disableCopy();
		return status;
	}

	/**
	 *@brief Read dv/dt threshold from eeprom register.
	 *
	 *@param [OUT] data - variable for register
	 *
	 *@retval   true if reading from register is successful
	 *@retval   false if an error occurred
	 */

	bool readEEPROMDvDtThreshold(uint8_t *data) {
		enableRecall();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		const bool status = readRegister(EEPROM_DV_DT_THRESHOLD, *data);
		disableRecall();
		return status;
	}

	/**
	 *@brief Write dv/dt threshold to eeprom register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeEEPROMDvDtThreshold(uint8_t data) {
		const bool status = writeRegister(EEPROM_DV_DT_THRESHOLD, data);
		enableCopy();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		disableCopy();
		return status;
	}

	/**
	 *@brief Read i2c address from eeprom register.
	 *
	 *@param [OUT] data - variable for register
	 *
	 *@retval   true if reading from register is successful
	 *@retval   false if an error occurred
	 */

	bool readEEPROMI2CAddress(uint8_t *data) {
		enableRecall();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		const bool status = readRegister(EEPROM_I2C_ADDRESS, *data);
		disableRecall();
		return status;
	}

	/**
	 *@brief Write i2c address to eeprom register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeEEPROMI2CAddress(uint8_t data) {
		const bool status = writeRegister(EEPROM_I2C_ADDRESS, data);
		enableCopy();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		disableCopy();
		return status;
	}

	/**
	 *@brief Read learn threshold from eeprom register.
	 *
	 *@param [OUT] data - variable for register
	 *
	 *@retval   true if reading from register is successful
	 *@retval   false if an error occurred
	 */

	bool readEEPROMLearnThreshold(uint8_t *data) {
		enableRecall();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		const bool status = readRegister(EEPROM_LEARN_THRESHOLD, *data);
		disableRecall();
		return status;
	}

	/**
	 *@brief Write learn threshold to eeprom register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeEEPROMLearnThreshold(uint8_t data) {
		const bool status = writeRegister(EEPROM_LEARN_THRESHOLD, data);
		enableCopy();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		disableCopy();
		return status;
	}

	/**
	 *@brief Read user value from eeprom register.
	 *
	 *@param [OUT] data - variable for register
	 *
	 *@retval   true if reading from register is successful
	 *@retval   false if an error occurred
	 */

	bool readEEPROMUserValue(uint8_t *data) {
		enableRecall();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		const bool status = readRegister(EEPROM_USER_VALUE, *data);
		disableRecall();
		return status;
	}

	/**
	 *@brief Write user value to eeprom register.
	 *
	 *@param [IN] data - variable to register
	 *
	 *@retval   true if writing to register is successful
	 *@retval   false if an error occurred
	 */

	bool writeEEPROMUserValue(uint8_t *data) {
		const bool status = writeRegister(EEPROM_USER_VALUE, *data);
		enableCopy();
		std::this_thread::sleep_for(std::chrono::milliseconds {500});
		disableCopy();
		return status;
	}
};


} // namespace ds2786

#endif
