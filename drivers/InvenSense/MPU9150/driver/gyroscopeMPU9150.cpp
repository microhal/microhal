/* =========================================================================================== *//**
 @file        GyroscopeMPU9150.cpp
 @authors     Paweł Okas
 @version     $Id$
 @package     HALFRED gyroscope implementation for MCP9150
 @brief       MPU9150 driver
 @hardware    none
 @copyright   $Copyright$
 @details     Interface for 3-axis gyroscope.

 *//* ============================================================================================ */

#include "gyroscopeMPU9150.h"
#include "mpu9150.h"

/**
 * \brief Default constructor of gyroscope class
 * @param parent MPU9150 instance.
 */
GyroscopeMPU9150::GyroscopeMPU9150(MPU9150 *parent) :
		mpu(*parent) {
	getRange();
}
/** This function return actual sampling frequency of gyroscope.
 * @return Actual sampling frequency in Hz.
 */
uint32_t GyroscopeMPU9150::getSamplingFrequency() {
	return mpu.getSamplingFrequency();
}

/**
 * \brief   function set sampling range
 *
 * \param   range sampling range
 * \return  true if new setting was applied or false if something went wrong
 */
bool GyroscopeMPU9150::setRange(FullScaleRange range) {
	uint8_t configRegister = 0;

	if (range == UNKNOWN_RANGE)
		return false;
	//read actual range
	if (mpu.readRegister(MPU9150::GYRO_CONFIG, configRegister) == false) {
		return false;
	}
	uint8_t tmpRange = range << 3;
	//if new range is difrent then actual
	if ((configRegister & 0x18) != tmpRange) {
		configRegister &= ~0x18;
		configRegister |= tmpRange;
		//set new range
		if (mpu.writeRegister(MPU9150::GYRO_CONFIG, configRegister) == true) {
			actualRange = range;
		} else {
			return false;
		}
	}
	return true;
}

/**
 * \brief   function return actual sampling range
 *
 * \return  actual scale range
 */
GyroscopeMPU9150::FullScaleRange GyroscopeMPU9150::getRange() {
	uint8_t configRegister;
	if (mpu.readRegister(MPU9150::GYRO_CONFIG, configRegister) == false) {
		return UNKNOWN_RANGE;
	}
	actualRange = (FullScaleRange) ((configRegister & 0x18) >> 3);
	return actualRange;
}

/**
 * \brief Update data
 *
 * \param   buffer - pointer to buffer with data
 */
void GyroscopeMPU9150::update(uint8_t *buffer) {
	const float cSensitivity[] = { 131, 65.5, 32.8, 16.4 };
	int16_t tmp[3];

	//convert endianness
	tmp[0] = (buffer[0] << 8) | buffer[1];
	tmp[1] = (buffer[2] << 8) | buffer[3];
	tmp[2] = (buffer[4] << 8) | buffer[5];
	//storage data in vector
//	angularVelocity.set((float) tmp[0] / cSensitivity[actualRange],
//			(float) tmp[1] / cSensitivity[actualRange],
//			(float) tmp[2] / cSensitivity[actualRange]);

	angularVelocity[0] = static_cast<float>(tmp[0]) / cSensitivity[actualRange];
	angularVelocity[1] = static_cast<float>(tmp[1]) / cSensitivity[actualRange];
	angularVelocity[2] = static_cast<float>(tmp[2]) / cSensitivity[actualRange];
}
/**
 * \brief   This function reset gyroscope
 *
 * \return  true if success or false if failed
 */
bool GyroscopeMPU9150::reset(void) {
	return mpu.writeRegister(MPU9150::SIGNAL_PATH_RESET, static_cast<uint8_t>(MPU9150::GYRO_RESET));
}


