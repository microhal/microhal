/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MAG3110

 @authors    Pawel
 created on: 14-10-2014
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014 - 2017, Pawel Okas
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

#include "mag3110.h"

using namespace microhal::diagnostic;

bool MAG3110::init() {
    uint8_t whoAmI;
    if (readRegister(WHO_AM_I, whoAmI) == Error::None) {
        if (whoAmI != WHO_AM_I_VALUE) {
            diagChannel << lock << MICROHAL_ERROR << "MAG3110: error in init - ID mismatch: expected: " << toHex(WHO_AM_I_VALUE)
                        << ", received: " << toHex(whoAmI) << endl
                        << unlock;
        } else {
            return true;
        }
    }
    return false;
}

bool MAG3110::setMode(Mode mode) {
    switch (mode) {
        case Mode::Standby:
            return clearBitsInRegister(CTRL_REG1, CTRL_REG1_AC) == Error::None;
        case Mode::ActiveRAW:
            // enable automatic reset
            if (setBitsInRegister(CTRL_REG2, CTRL_REG2_AUTO_MRST_EN) != Error::None) {
                return false;
            }
            // to change value of control registers device have to be in standby mode
            // change mode to standby
            if (clearBitsInRegister(CTRL_REG1, CTRL_REG1_AC) != Error::None) {
                return false;
            }
            // set RAW bit for enable data correction
            if (setBitsInRegister(CTRL_REG2, CTRL_REG2_RAW) != Error::None) {
                return false;
            }
            // set mode to active
            if (setBitsInRegister(CTRL_REG1, CTRL_REG1_AC) != Error::None) {
                return false;
            }
            break;
        case Mode::ActiveCorrected:
            // enable automatic reset
            if (setBitsInRegister(CTRL_REG2, CTRL_REG2_AUTO_MRST_EN) != Error::None) {
                return false;
            }
            // to change value of control registers device have to be in standby mode
            // change mode to standby
            if (clearBitsInRegister(CTRL_REG1, (uint8_t)CTRL_REG1_AC) != Error::None) {
                return false;
            }
            // clear RAW bit for enable data correction
            if (clearBitsInRegister(CTRL_REG2, CTRL_REG2_RAW) != Error::None) {
                return false;
            }
            // set mode to active
            if (setBitsInRegister(CTRL_REG1, CTRL_REG1_AC) != Error::None) {
                return false;
            }
            break;
    }
    return true;
}

bool MAG3110::setCorrection(int16_t x, int16_t y, int16_t z) {
    const std::array<int16_t, 3> data = {x, y, z};
    return writeMultipleRegisters(data, OFF_X, OFF_Y, OFF_Z) == Error::None;
}

bool MAG3110::getCorrection(int16_t* x, int16_t* y, int16_t* z) {
    std::tuple<int16_t, int16_t, int16_t> data;
    const bool status = readMultipleRegisters(data, OFF_X, OFF_Y, OFF_Z) == Error::None;
    *x = std::get<0>(data);
    *y = std::get<1>(data);
    *z = std::get<2>(data);

    return status;
}

bool MAG3110::setODR_OSR(OutputDataRate_OverSamplingRate odr_osr) {
    // to change output data rate and output sampling rate we need switch Mode to STANDBY
    if (auto mode = getMode()) {
        if (setMode(Mode::Standby)) {
            uint8_t ctrl_reg = odr_osr;
            if (modifyBitsInRegister(CTRL_REG1, ctrl_reg, 0xF8) == Error::None) {
                return setMode(*mode);
            }
        }
    }
    return false;
}

std::experimental::optional<MAG3110::MagneticVector> MAG3110::getMagnetic() {
    std::experimental::optional<MagneticVector> mag;
    uint8_t status;
    if (readRegister(DR_STATUS, status) == Error::None) {
        std::array<int16_t, 3> data;
        if (readMultipleRegisters(data, OUT_X, OUT_Y, OUT_Z) == Error::None) {
            int16_t x = data[0];
            int16_t y = data[1];
            int16_t z = data[2];

            MagneticVector tmp;
            convertToMagnetic(&tmp, x, y, z);
            mag = tmp;
        }
    }
    return mag;
}
