/* =========================================================================================== *//**
 @file        accelerometerMPU9150.cpp
 @authors     Paweł Okas
 @version     $Id$
 @package     HALFRED accelerometer implementation for MCP9150
 @brief       MPU9150 driver
 @hardware    none
 @copyright   $Copyright$
 @details     Interface for 3-axis accelerometer.

 *//* ============================================================================================ */

/*  Created on: 2 gru 2013 */

#include "accelerometerMPU9150.h"
#include "mpu9150.h"

/**
 * \brief Default constructor of Accelerometer class
 * @param parent MPU9150 instance.
 */
AccelerometerMPU9150::AccelerometerMPU9150(MPU9150 *parent) :
        mpu(*parent) {
    actualRange = getRange();
}

/** This function return actual sampling frequency of accelerometer.
 * @return Actual sampling frequency in Hz.
 */
uint32_t AccelerometerMPU9150::getSamplingFrequency() {
    return mpu.getSamplingFrequency();
}

/**
 *\brief This function set new full scale range for accelerometer.
 * @param range new range to set
 * @retval true if new range was set.
 * @retval false if an error occurred.
 */
bool AccelerometerMPU9150::setRange(FullScaleRange range) {
    uint8_t accelConfReg = 0;

    if (range == UNKNOWN_RANGE) return false;
    //read actual range
    if (mpu.readRegister(MPU9150::ACCEL_CONFIG, accelConfReg) == false) {
        return false;
    }
    uint8_t tmpRange = range << 3;
    //if new range is different then actual
    if ((accelConfReg & 0x18) != tmpRange) {
        accelConfReg &= ~0x18;
        accelConfReg |= tmpRange;
        //set new range
        if (mpu.writeRegister(MPU9150::ACCEL_CONFIG, accelConfReg) == true) {
            actualRange = range;
        } else {
            return false;
        }
    }
    return true;
}
/**
 * \brief This function reset accelerometer registers.
 * @retval true if accelerometer was reset.
 * @retval false if an error occurred.
 */
bool AccelerometerMPU9150::reset(void) {
    return mpu.writeRegister(MPU9150::SIGNAL_PATH_RESET, static_cast<uint8_t>(MPU9150::ACCEL_RESET));
}
/**
 * \brief This function return actual accelerometer full scale range.
 * @return Actual accelerometer range.
 */
AccelerometerMPU9150::FullScaleRange AccelerometerMPU9150::getRange() {
    uint8_t accelConfReg;
    if (mpu.readRegister(MPU9150::ACCEL_CONFIG, accelConfReg) == false) {
        return UNKNOWN_RANGE;
    }
    actualRange = (FullScaleRange) ((accelConfReg & 0x18) >> 3);
    return actualRange;
}
/**
 * \brief This function load new data to acceleration Vector.
 */
void AccelerometerMPU9150::update(uint8_t *buffer) {
    static const uint32_t cSensitivity[4] = { 16384, 8192, 4096, 2048 };
    int16_t tmp[3];

    //convert endianness
    tmp[0] = (buffer[0] << 8) | buffer[1];
    tmp[1] = (buffer[2] << 8) | buffer[3];
    tmp[2] = (buffer[4] << 8) | buffer[5];
    //storage data in vector
//    acceleration.set((float) tmp[0] / cSensitivity[actualRange],
//            (float) tmp[1] / cSensitivity[actualRange], (float) tmp[2] / cSensitivity[actualRange]);
    acceleration[0] = static_cast<float>(tmp[0]) / cSensitivity[actualRange];
    acceleration[1] = static_cast<float>(tmp[1]) / cSensitivity[actualRange];
    acceleration[2] = static_cast<float>(tmp[2]) / cSensitivity[actualRange];
}

