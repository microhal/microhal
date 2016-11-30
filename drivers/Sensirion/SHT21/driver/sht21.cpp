/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 21-02-2014
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

#include "sht21.h"

/**@brief This function reset device.
 *
 * @retval true if device was reset.
 * @retval false if an error occurred.
 */
bool SHT21::reset() {
    //return write(SOFT_RESET_CMD); //todo implement in I2Cdevice class
    return false;
}
/** @brief This function enable internal heater.
 *
 * @retval true if heater was on.
 * @retval false if an error occurred.
 */
bool SHT21::heaterEnable() {
    uint8_t userReg;

    if (readRegister(READ_USER_REGISTER_CMD, userReg) == true) {
        userReg |= USER_REGISTER_ENABLE_ON_CHIP_HEATER;
        return writeRegister(WRITE_USER_REGISTER_CMD, userReg);
    }
    return false;
}
/** @brief This function disable internal heater.
 *
 * @retval true if heater was off.
 * @retval false if an error occurred.
 */
bool SHT21::heaterDisable() {
    uint8_t userReg;

    if (readRegister(READ_USER_REGISTER_CMD, userReg) == true) {
        userReg &= ~USER_REGISTER_ENABLE_ON_CHIP_HEATER;
        return writeRegister(WRITE_USER_REGISTER_CMD, userReg);
    }
    return false;
}
/** @brief This function set new temperature and humidity resolution.
 *
 * @param[in] resolution
 * @retval true if resolution was set.
 * @retval false if an error occurred.
 */
bool SHT21::setResolution(Resolution resolution) {
    uint8_t userReg;

    if (readRegister(READ_USER_REGISTER_CMD, userReg) == true) {
        //clear actual resolution
        userReg &= ~(RESOLUTION_12_14 | RESOLUTION_8_12 | RESOLUTION_10_13 | RESOLUTION_11_11);
        //set new resolution
        userReg |= resolution;
        return writeRegister(WRITE_USER_REGISTER_CMD, userReg);
    }
    return false;
}
/**@brief This function read actual temperature and humidity resolution.
 *
 * @return actual resolution or UNKNOWN if an error occurred.
 */
SHT21::Resolution SHT21::getResolution() {
    uint8_t userReg;

    if (readRegister(READ_USER_REGISTER_CMD, userReg) == true) {
        //clear actual resolution
        return (Resolution) (userReg
                & (RESOLUTION_12_14 | RESOLUTION_8_12 | RESOLUTION_10_13 | RESOLUTION_11_11));
    }
    return UNKNOWN;
}
