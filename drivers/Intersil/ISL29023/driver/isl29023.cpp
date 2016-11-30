/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 22-02-2014
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

#include "isl29023.h"

bool ISL29023::reset(){
	return writeRegister(COMMAND_1, static_cast<uint16_t>(0), microhal::LittleEndian);
}

/** @brief This function set operating mode.
 *
 * @param mode new mode to set.
 * @return
 */
bool ISL29023::setMode(OperatimgModes mode) {
    if (mode == Mode_Unknown) return false;
    uint8_t command;

    if (readRegister(COMMAND_1, command) == true) {
        //clear actuall mode bits
        command &= ~(Mode_PowerDown | Mode_AlsOnce | Mode_IrOnce | Mode_AlsContinuous
                | Mode_IrContinuous);
        //apply new mode
        command |= mode;
        //write regiser
        return writeRegister(COMMAND_1, command);
    }
    return false;
}
/**@brief This function read actual operating mode.
 *
 * @return actual operating mode or UNKNOWN if an error occurred.
 */
ISL29023::OperatimgModes ISL29023::getMode() {
    uint8_t command;

    if (readRegister(COMMAND_1, command) == true) {
        command &= Mode_PowerDown | Mode_AlsOnce | Mode_IrOnce | Mode_AlsContinuous
                | Mode_IrContinuous;
        return (OperatimgModes) command;
    } else {
        return Mode_Unknown;
    }
}
/**@brief This function set full scale range.
 *
 * @param range - new range to set.
 * @retval true if new range was set.
 * @retval false if an error occurred.
 */
bool ISL29023::setRange(FullScalleRange range) {
    if (range == RangeUnknown) return false;
    uint8_t command;

    if (readRegister(COMMAND_2, command) == true) {
        //clear actuall mode bits
        command &= ~(Range1 | Range2 | Range3 | Range4);
        //apply new mode
        command |= range;
        //write regiser
        if (writeRegister(COMMAND_2, command) == true) {
            return calibrate();
        }
    }
    return false;
}
/**@brief This function read actual full scale range.
 *
 * @return actual full scale range or UNKNOWN_RANGE if an error occurred.
 */
ISL29023::FullScalleRange ISL29023::getRange() {
    uint8_t command;

    if (readRegister(COMMAND_2, command) == true) {
        command &= Range1 | Range2 | Range3 | Range4;
        return (FullScalleRange) command;
    } else {
        return RangeUnknown;
    }
}
/**@brief This function set new resolution.
 *
 * @param[in] resolution
 * @return
 */
bool ISL29023::setResolution(Resolution resolution) {
    if (resolution == Resolution_Unknown) return false;
    uint8_t command;

    if (readRegister(COMMAND_2, command) == true) {
        //clear actuall mode bits
        command &= ~(Resolution_16bit | Resolution_12bit | Resolution_8bit | Resolution_4bit);
        //apply new mode
        command |= resolution;
        //write regiser
        if (writeRegister(COMMAND_2, command) == true) {
            return calibrate();
        }
    }
    return false;
}
/**
 *
 * @return actual resolution or UNKNOWN if an error occurred.
 */
ISL29023::Resolution ISL29023::getResolution() {
    uint8_t command;
    if (readRegister(COMMAND_2, command) == true) {
        //clear other bits than resolution
        command &= Resolution_16bit | Resolution_12bit | Resolution_8bit | Resolution_4bit;
        return (Resolution) command;
    } else {
        return Resolution_Unknown;
    }
}
/**
 *
 * @return
 */
bool ISL29023::calibrate() {
    float count;
    float range_k;

    const Resolution resolution = getResolution();
    const FullScalleRange range = getRange();

    switch (resolution) {
    case Resolution_16bit:
        count = 65536.0;
        break;
    case Resolution_12bit:
        count = 4096.0;
        break;
    case Resolution_8bit:
        count = 256.0;
        break;
    case Resolution_4bit:
        count = 16.0;
        break;
    default:
        return false;
    }

    switch (range) {
    case Range1:
        range_k = 1000.0;
        break;
    case Range2:
        range_k = 4000.0;
        break;
    case Range3:
        range_k = 16000.0;
        break;
    case Range4:
        range_k = 64000.0;
        break;
    default:
        return false;
    }

    alfa = range_k / count;

    return true;
}
