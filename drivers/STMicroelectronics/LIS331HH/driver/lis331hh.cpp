/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MAG3110

 @authors    Kubaszek Mateusz
 created on: 19.01.2017
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

 */ /* ==========================================================================================================================
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      */

#include "lis331hh.h"
#include "I2CDevice.h"

using namespace microhal::diagnostic;
using namespace microhal;

bool LIS331HH::init(Axis axisEnable, PowerMode powerMode, Sensitivity sensitivity, DataRate dataRate) {
    bool returnValue = (microhal::I2C::Error::None == writeRegister(CTRL_REG2, CTRL_REG2_Flags::boot));
    uint8_t tmp;
    do {
        if (microhal::I2C::Error::None != readRegister(CTRL_REG2, tmp)) return false;
        if (CTRL_REG2_Flags::boot != (tmp & CTRL_REG2_Flags::boot)) break;
    } while (true);
    returnValue &= (microhal::I2C::Error::None == writeRegister(CTRL_REG4, CTRL_REG4_Flags::BDU));
    returnValue &= setAxis(axisEnable);
    returnValue &= setSpeed(powerMode, dataRate);
    returnValue &= setSensitivity(sensitivity);
    return returnValue;
}

bool LIS331HH::setAxis(Axis axis) {
    return I2C::Error::None == modifyBitsInRegister(CTRL_REG1, static_cast<uint8_t>(axis), CTRL_REG1_Masks::axis_mask);
}

bool LIS331HH::setSpeed(PowerMode powerMode, DataRate dataRate) {
    if (PowerMode::normalMode == powerMode) {
        if (I2C::Error::None !=
            modifyBitsInRegister(CTRL_REG1, static_cast<uint8_t>(dataRate) << CTRL_REG1_Offsets::DR_offset, CTRL_REG1_Masks::DR_mask)) {
            return false;
        }
    }
    return I2C::Error::None ==
           modifyBitsInRegister(CTRL_REG1, static_cast<uint8_t>(powerMode) << CTRL_REG1_Offsets::PM_offset, CTRL_REG1_Masks::PM_mask);
}

bool LIS331HH::setSensitivity(Sensitivity sensitivity) {
    if (I2C::Error::None ==
        modifyBitsInRegister(CTRL_REG4, static_cast<uint8_t>(sensitivity) << CTRL_REG4_Offset::FS_offset, CTRL_REG4_Masks::FS_mask)) {
        switch (sensitivity) {
            case Sensitivity::sensitivity6g:
                sensitivityMultiplyer = 6.0 / static_cast<float>(INT16_MAX);
                break;
            case Sensitivity::sensitivity12g:
                sensitivityMultiplyer = 12.0 / static_cast<float>(INT16_MAX);
                break;
            case Sensitivity::sensitivity24g:
                sensitivityMultiplyer = 24.0 / static_cast<float>(INT16_MAX);
                break;
            default:
                return false;
        }
        return true;
    }
    return false;
}

bool LIS331HH::dataAvailable(Axis &axis) {
    uint8_t statusRegisterValue;
    axis = Axis::none;
    if (I2C::Error::None == readRegister(STATUS_REG, statusRegisterValue)) {
        if (STATUS_REG_Flags::ZYXDA == (statusRegisterValue & STATUS_REG_Flags::ZYXDA)) {
            axis = Axis::all;
        } else {
            if (STATUS_REG_Flags::XDA == (statusRegisterValue & STATUS_REG_Flags::XDA)) {
                axis |= Axis::X;
            }
            if (STATUS_REG_Flags::YDA == (statusRegisterValue & STATUS_REG_Flags::YDA)) {
                axis |= Axis::Y;
            }
            if (STATUS_REG_Flags::ZDA == (statusRegisterValue & STATUS_REG_Flags::ZDA)) {
                axis |= Axis::Z;
            }
        }
        return true;
    }
    return false;
}

bool LIS331HH::dataOverrwritten(Axis &axis) {
    uint8_t statusRegisterValue;
    if (I2C::Error::None == readRegister(STATUS_REG, statusRegisterValue)) {
        if (STATUS_REG_Flags::ZYXOR == (statusRegisterValue & STATUS_REG_Flags::ZYXOR)) {
            axis = Axis::all;
        } else {
            if (STATUS_REG_Flags::XOR == (statusRegisterValue & STATUS_REG_Flags::XOR)) {
                axis |= Axis::X;
            }
            if (STATUS_REG_Flags::YOR == (statusRegisterValue & STATUS_REG_Flags::YOR)) {
                axis |= Axis::Y;
            }
            if (STATUS_REG_Flags::ZOR == (statusRegisterValue & STATUS_REG_Flags::ZOR)) {
                axis |= Axis::Z;
            }
        }
        return true;
    }
    return false;
}

std::experimental::optional<Acceleration> LIS331HH::getAcceleration(Axis axis) {
    int16_t accelerationData;
    std::experimental::optional<Acceleration> returnObject;
    switch (axis) {
        case Axis::X:
            if (I2C::Error::None != readRegister(OUT_X, accelerationData)) {
                return returnObject;
            }
            break;
        case Axis::Y:
            if (I2C::Error::None != readRegister(OUT_Y, accelerationData)) {
                return returnObject;
            }
            break;
        case Axis::Z:
            if (I2C::Error::None != readRegister(OUT_Z, accelerationData)) {
                return returnObject;
            }
            break;
        default:
            return returnObject;
    }
    returnObject = Acceleration(static_cast<float>(accelerationData) * sensitivityMultiplyer);
    return returnObject;
}

bool LIS331HH::getAcceleration(Acceleration &xAcceleration, Acceleration &yAcceleration, Acceleration &zAcceleration) {
    int16_t accelerationDataX;
    int16_t accelerationDataY;
    int16_t accelerationDataZ;
    uint8_t tmpL, tmpH;
    if (I2C::Error::None != readRegister(OUT_X_L, tmpL)) {
        return false;
    }
    if (I2C::Error::None != readRegister(OUT_X_H, tmpH)) {
        return false;
    }
    accelerationDataX = tmpL | (tmpH << 8);
    if (I2C::Error::None != readRegister(OUT_Y_L, tmpL)) {
        return false;
    }
    if (I2C::Error::None != readRegister(OUT_Y_H, tmpH)) {
        return false;
    }
    accelerationDataY = tmpL | (tmpH << 8);
    if (I2C::Error::None != readRegister(OUT_Z_L, tmpL)) {
        return false;
    }
    if (I2C::Error::None != readRegister(OUT_Z_H, tmpH)) {
        return false;
    }
    accelerationDataZ = tmpL | (tmpH << 8);
    // does not resets STATUS REGISTER
    //    read(OUT_X_L, tmp);
    //    read(OUT_X_H, tmp);
    //    read(OUT_Y_L, tmp);
    //    read(OUT_Y_H, tmp);
    //    read(OUT_Z_L, tmp);
    //    read(OUT_Z_H, tmp);
    //    if (I2C::Error::None != read(OUT_X, accelerationDataX)) {
    //        return false;
    //    }
    //    if (I2C::Error::None != read(OUT_Y, accelerationDataY)) {
    //        return false;
    //    }
    //    if (I2C::Error::None != read(OUT_Z, accelerationDataZ)) {
    //        return false;
    //    }
    // only if all transactions succeed fill the data variables
    xAcceleration = Acceleration(static_cast<float>(accelerationDataX) * sensitivityMultiplyer);
    yAcceleration = Acceleration(static_cast<float>(accelerationDataY) * sensitivityMultiplyer);
    zAcceleration = Acceleration(static_cast<float>(accelerationDataZ) * sensitivityMultiplyer);
    return true;
}

bool LIS331HH::test(void) {
    uint8_t data;
    bool result = true;
    diagChannel << lock << MICROHAL_NOTICE << "Reading registers:" << unlock;
    result &= (I2C::Error::None == readRegister(CTRL_REG1, data));
    diagChannel << lock << Notice << "\n\r\tCTRL_REG1: " << toHex(data) << unlock;
    result &= (I2C::Error::None == readRegister(CTRL_REG2, data));
    diagChannel << lock << Notice << "\n\r\tCTRL_REG2: " << toHex(data) << unlock;
    result &= (I2C::Error::None == readRegister(CTRL_REG3, data));
    diagChannel << lock << Notice << "\n\r\tCTRL_REG3: " << toHex(data) << unlock;
    result &= (I2C::Error::None == readRegister(CTRL_REG4, data));
    diagChannel << lock << Notice << "\n\r\tCTRL_REG4: " << toHex(data) << unlock;
    result &= (I2C::Error::None == readRegister(STATUS_REG, data));
    diagChannel << lock << Notice << "\n\r\tSTATUS_REG: " << toHex(data) << unlock;
    result &= (I2C::Error::None == readRegister(REFERENCE, data));
    diagChannel << lock << Notice << "\n\r\tREFERENCE: " << toHex(data) << unlock;
    return result;
}
