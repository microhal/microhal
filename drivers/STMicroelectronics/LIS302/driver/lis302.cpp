/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      Driver for LIS302DL accelerometr

 * @authors    Pawel
 * created on: 10-11-2015
 * last modification: <DD-MM-YYYY>

 * @copyright Copyright (c) 2015, microHAL
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 * conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "lis302.h"


bool LIS302DL::init() {
    if (getID() != WHO_AM_I_VALUE) {
        microhal::diagnostic::diagChannel << microhal::diagnostic::Error <<
            "LIS302DL: error in init, ID mismatch."
            << microhal::diagnostic::endl;
        return false;
    }

    return true;
}

bool LIS302DL::setMode(Mode mode) {
    bool result = false;
    uint8_t ctrl_reg1;

    do {
        if (false == readRegister(READ | CTRL_REG1, ctrl_reg1)) {
            break;
        }
        if (((LIS302DL::Mode) (ctrl_reg1 & (PowerDown | Active)) == mode)) {
            result = true;
            break;
        }
        // in other case set new mode
        // clear actual setting
        ctrl_reg1 &= ~(PowerDown | Active);
        // set new data
        ctrl_reg1 |= mode;
        if (false == writeRegister(CTRL_REG1, ctrl_reg1)) {
            break;
        }

        result = true;
    } while (0);

    return result;
}

LIS302DL::Mode LIS302DL::getMode() {
    uint8_t ctrl_reg1;

    if (readRegister(READ | CTRL_REG1, ctrl_reg1) == true) {
        return (LIS302DL::Mode) (ctrl_reg1 & (PowerDown | Active));
    }
    return ModeUnknown;
}

bool LIS302DL::setODR(LIS302DL::Odr odr) {
    bool result = false;
    uint8_t ctrl_reg1;

    do {
        if (false == readRegister(READ | CTRL_REG1, ctrl_reg1)) {
            break;
        }

        // if odr to set is equal actual odr return true
        if ((LIS302DL::Odr) ((ctrl_reg1 & (ODR_100Hz | ODR_400Hz)) == odr)) {
            result = true;
            break;
        }
        // in other case set new odr
        // clear actual setting
        ctrl_reg1 &= ~(ODR_100Hz | ODR_400Hz);
        // set new data
        ctrl_reg1 |= odr;
        if (false == writeRegister(CTRL_REG1, ctrl_reg1)) {
            break;
        }
        result = true;
    } while (0);

    return result;
}

LIS302DL::Odr LIS302DL::getODR() {
    uint8_t ctrl_reg1;

    if (readRegister(READ | CTRL_REG1, ctrl_reg1) == true) {
        return (LIS302DL::Odr) (ctrl_reg1 & (ODR_100Hz | ODR_400Hz));
    }
    return ODR_Unknown;
}
bool LIS302DL::enableAxis(Axis axis) {
    bool result = false;
    uint8_t ctrl_reg1;

    do {
        if (false == readRegister(READ | CTRL_REG1, ctrl_reg1)) {
            break;
        }

        // if axis to set is equal actual axis return true
        if (static_cast<Axis>(ctrl_reg1 & XYZ) == axis) {
            m_axis = axis;
            result = true;
            break;
        }
        // in other case set new Axis configuration
        // clear actual setting
        ctrl_reg1 &= ~XYZ;
        // set new data
        ctrl_reg1 |= axis;
        if (false == writeRegister(CTRL_REG1, ctrl_reg1)) {
            break;
        }
        m_axis = axis;
        result = true;
    } while (0);

    return result;
}

LIS302DL::Axis LIS302DL::getActiveAxis() {
    uint8_t ctrl_reg1;

    if (readRegister(READ | CTRL_REG1, ctrl_reg1) == true) {
        return (Axis) (ctrl_reg1 & XYZ);
    }
    return AxisUnknown;
}

bool LIS302DL::setRange(Range range) {
    bool result = false;
    uint8_t ctrl_reg1;

    do {
        if (false == readRegister(READ | CTRL_REG1, ctrl_reg1)) {
            break;
        }
        if (((Range) (ctrl_reg1 & (Range_2g | Range_8g)) == range)) {
            m_range = range;
            result = true;
            break;
        }
        // in other case set new mode
        // clear actual setting
        ctrl_reg1 &= ~(Range_2g | Range_8g);
        // set new data
        ctrl_reg1 |= range;
        if (false == writeRegister(CTRL_REG1, ctrl_reg1)) {
            break;
        }
        m_range = range;
        result = true;
    } while (0);

    return result;
}

LIS302DL::Range LIS302DL::getRange() {
    uint8_t ctrl_reg1;

    if (readRegister(READ | CTRL_REG1, ctrl_reg1) == true) {
        return (Range) (ctrl_reg1 & (Range_2g | Range_8g));
    }
    return Range_Unknown;
}

uint8_t LIS302DL::getID() {
    uint8_t id;
    if (readRegister(READ | WHO_AM_I, id) == true) {
        return id;
    }
    return 0;
}

bool LIS302DL::getAcceleration(AccelerationVector* acc) {
    bool result = false;
    uint8_t status;
    do {
        if (false == readRegister(READ | STATUS_REG, status)) {
            break;
        }

        int8_t x = 0;
        int8_t y = 0;
        int8_t z = 0;

        if ((m_axis & Axis::X) == (status & STATUS_REG_XDA) &&
        (m_axis & Axis::Y) == (status & STATUS_REG_YDA) &&
        (m_axis & Axis::Z) == (status & STATUS_REG_ZDA)) {
            if ((m_axis & Axis::X)) {  // all axis requested
                x = getX();
            }
            if ((m_axis & Axis::Y)) {  // all axis requesteed
                y = getY();
            }
            if ((m_axis & Axis::Z)) {  // all axis requested
                z = getZ();
            }
        } else {
            break;
        }

        convertToAcceleration(acc, x, y, z);
        result = true;
    } while (0);
    return result;
}

void LIS302DL::convertToAcceleration(AccelerationVector* acc,
                                     int8_t x, int8_t y, int8_t z) {
    float scale = 1.0;
    if (Range::Range_2g ==  m_range) {
        scale = (static_cast<float>(SENSIVITY_RAGNE2_MULTIPLIER)) /
                (static_cast<float>(SENSIVITY_RAGNE2_DIVIDER));
    } else if (Range::Range_8g ==  m_range) {
        scale = (static_cast<float>(SENSIVITY_RAGNE8_MULTIPLIER)) /
                (static_cast<float>(SENSIVITY_RAGNE8_DIVIDER));
    }

    (*acc)[0] = microhal::Acceleration(static_cast<float>(x) * scale);
    (*acc)[1] = microhal::Acceleration(static_cast<float>(y) * scale);
    (*acc)[2] = microhal::Acceleration(static_cast<float>(z) * scale);
}

int8_t LIS302DL::getX() {
    uint8_t x;
    readRegister(READ | OUTX, x);
    return static_cast<int8_t>(x);
}

int8_t LIS302DL::getY() {
    uint8_t y;
    readRegister(READ | OUTY, y);
    return static_cast<int8_t>(y);
}

int8_t LIS302DL::getZ() {
    uint8_t z;
    readRegister(READ | OUTZ, z);
    return static_cast<int8_t>(z);
}
