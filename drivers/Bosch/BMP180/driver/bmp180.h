/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 16-04-2014
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

#ifndef BMP180_H_
#define BMP180_H_

#include "microhal.h"
#include "I2CDevice/I2CDevice.h"

/**
 * \addtogroup Devices
 * @{
 * @class BMP180
 * @}
 */
class BMP180: public microhal::I2CDevice {
    enum Flags : uint8_t{
        CTRL_MEAS_SCO = 0x20,
    };
    /**
     *
     */
    enum Registers {
        OUT_XLSB = 0xF8,  //!< OUT_XLSB
        OUT_LSB = 0xF7,   //!< OUT_LSB
        OUT_MSB = 0xF6,   //!< OUT_MSB
        CTRL_MEAS = 0xF4, //!< CTRL_MEAS
        SOFT_RESET = 0xE0, //!< SOFT_RESET
        ID = 0xD0,        //!< ID
    };
    /**
     *
     */
    typedef struct {
        int16_t AC1;
        int16_t AC2;
        int16_t AC3;
        uint16_t AC4;
        uint16_t AC5;
        uint16_t AC6;
        int16_t B1;
        int16_t B2;
        int16_t MB;
        int16_t MC;
        int16_t MD;
    } CalibrationCoefficients;
public:
    /**
     *
     */
    enum PossibleI2CAddress {
        DEFAULT_ADDRESS = 0xEE        //!< DEFAULT_ADDRESS
    };
    /**
     *  Value of fixed registers.
     */
    enum ConstRegisterValues {
        ID_VALUE = 0x55,                         //!< ID VALUE
    };
    /**
     *
     * @param i2cNo
     */
    BMP180(microhal::I2C &i2c) :
            I2CDevice(i2c, DEFAULT_ADDRESS) {

    }
    /** @brief This function read calibration coefficient
     *
     * @return
     */
    bool init() {
        if (getDeviceID() == ID_VALUE) {
            if (readCalibrationData() == true) {
                return true;
            }
        }
        return false;
    }
    inline uint8_t getDeviceID();
    inline bool reset();
    inline bool startConversion();
    inline bool isNewDataRedy();
private:
    CalibrationCoefficients calibrationCoefs;

    bool readCalibrationData();
    inline bool update();
};

uint8_t BMP180::getDeviceID() {
    uint8_t id;

    if (readRegister(ID, id) == true) {
        return id;
    } else {
        return 0;
    }
}
/**
 *
 * @return
 */
bool BMP180::reset() {
    return writeRegister(SOFT_RESET, static_cast<uint8_t>(0xB6));
}
/**
 *
 * @return
 */
bool BMP180::startConversion() {
    return setBitsInRegister(CTRL_MEAS, CTRL_MEAS_SCO);
}
/**
 *
 * @return
 */
bool BMP180::isNewDataRedy() {
    uint8_t ctrl;
    static bool lastConversionInRun = false;

    if (readRegister(CTRL_MEAS, ctrl) == true) {
        if (ctrl & CTRL_MEAS_SCO) {
            lastConversionInRun = true;
        } else {
            if (lastConversionInRun == true) {
                lastConversionInRun = false;
                return true;
            }
        }
    }
    return false;
}

bool BMP180::update() {
    int16_t ut;
    int32_t up;
    uint8_t buff[3];
    //read uncompressed temperature value
    if (readRegister(OUT_MSB, (uint16_t&) ut, microhal::BigEndian) == true) {
        //read uncompressed pressure value
        if (readRegisters(OUT_MSB, buff, 3) == true) {
            up = buff[0] << 16 | buff[1] << 8 | buff[0];
            //calculate true temperature
            int32_t x1 = (ut - calibrationCoefs.AC6) * calibrationCoefs.AC5 / 2 << 15;
            int32_t x2 = calibrationCoefs.MC * 2 << 11 / (x1 + calibrationCoefs.MD);
            int32_t b5 = x1 + x2;
            int32_t temp = (b5 + 8) / 2 << 4;
        }
    }
}
#endif /* BMP180_H_ */
