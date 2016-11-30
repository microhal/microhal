/* =========================================================================================== *//**
 @file        AK8975.cpp
 @authors     Paweł Okas
 @version     $Id$
 @package
 @brief       AK8975 driver
 @hardware    none
 @copyright   $Copyright$
 @details     Interface for 3-axis magnetometer.

 *//* ============================================================================================ */

#include "ak8975.h"

/**
 * \brief Default constructor for AK8975 class. Initialize I2C device number and AK8974 address.
 *
 * AK8975 class represent triple-axes magnetometer
 * @param i2cNr - I2C number
 * @param addr - address of device
 */
AK8975::AK8975(microhal::I2C &i2c, uint8_t addr) :
        I2CDevice(i2c, addr) {
    scallingFactor[0] = 1.0 / cSensitivity;
    scallingFactor[1] = 1.0 / cSensitivity;
    scallingFactor[2] = 1.0 / cSensitivity;
}
/**
 * \brief This function check is new data in the buffer.
 * @retval true if new data ready to read.
 * @retval false if no new data available.
 */
bool AK8975::isDataReady() {
    uint8_t tmp;
    //read status register
    if (readRegister(ST1, tmp) == true) {
        //if new data redy
        if (tmp == ST1_DRDY) {
            uint8_t buffer[6];
            //read data registers
            if (readRegisters(HXL, buffer, sizeof(buffer)) == true) {
                //update magnetic flux vector
                update(buffer);
                return true;
            }
        }
    }
    return false;
}
/**This function set operation mode of AKM magnetometer.
 *
 * @param[in] mode -
 * @retval true if mode was set successful.
 * @retval false if an error occurred.
 */
bool AK8975::setMode(Mode mode) {
    if (mode != UNKNOWN_MODE) {
        return writeRegister(CNTL, static_cast<uint8_t>(mode));
    }
    return false;
}
/**\brief This Function calibrate magnetometer data
 *
 * @retval true if calibrating was completed successfully.
 * @retval false if an error occurred.
 */
bool AK8975::calibrate() {
    uint8_t asaX, asaY, asaZ;

    if (readAdjustment(asaX, asaY, asaZ) == false) {
        return false;
    }
    //calculations according to datasheet
    scallingFactor[0] = ((((float) (asaX - 128) * 0.5) / 128.0) + 1.0) / cSensitivity;
    scallingFactor[1] = ((((float) (asaY - 128) * 0.5) / 128.0) + 1.0) / cSensitivity;
    scallingFactor[2] = ((((float) (asaZ - 128) * 0.5) / 128.0) + 1.0) / cSensitivity;
    return true;
}
/**This function run self test.
 *
 * @retval true if self test was completed successfully.
 * @retval false if an error occurred.
 */
bool AK8975::setSelfTest() {

    if (writeRegister(ASTC, static_cast<uint8_t>(ASTC_SELF)) == false) return false;
    //todo check response
    return false;
}
/**
 *
 * @param[out] x
 * @param[out] y
 * @param[out] z
 * @return
 */
bool AK8975::readAdjustment(uint8_t &x, uint8_t &y, uint8_t &z) {
    uint8_t buff[3];
    Mode mode;

    //get actual mode and if isn't FUSE_ROM_ACCESS wait for change mode to POWER_DOWN. todo timeout
    do {
        if (getMode(mode) == false) {
            return false;
        }
    } while (mode != POWER_DOWN && mode != FUSE_ROM_ACCESS);
    //set mode to fuse rom, only in this mode adjustment data is available.
    if (mode != FUSE_ROM_ACCESS) {
        if (setMode(FUSE_ROM_ACCESS) == false) {
            return false;
        }
    }
    //read adjustment value
    if (readRegisters(ASAX, buff, sizeof(buff)) == false) {
        return false;
    }
    //set power down mode
    if (setMode(POWER_DOWN) == false) {
        return false;
    }

    x = buff[0];
    y = buff[1];
    z = buff[2];

    return true;
}
/**
 *
 * @param range - unused parameter
 * @return false
 */
bool AK8975::setRange(FullScaleRange range) {
    range = range; //make compiler happy
    return false;
}
/**
 *
 * @return Full scale range of magnetic field.
 */
AK8975::FullScaleRange AK8975::getRange() {
    return RANGE_1229uT;
}
