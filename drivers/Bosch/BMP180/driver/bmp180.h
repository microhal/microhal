/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      driver for BMP180 pressure sensor
 *
 * @authors    Pawel Okas
 * created on: 16-04-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014-2017, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BMP180_H_
#define BMP180_H_

#include "I2CDevice/I2CDevice.h"
#include "microhal.h"

/**
 * \addtogroup Devices
 * @{
 * @class BMP180
 * @}
 */
class BMP180 : public microhal::I2CDevice {
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    using Error = microhal::I2C::Error;
    // create alias to microhal::Address, we just want to type less
    template <typename T, T i>
    using Address = microhal::Address<T, i>;

    enum Flags : uint8_t {
        CTRL_MEAS_SCO = 0x20,
    };
    /**
     *
     */
    struct Register {
        static constexpr auto OUT_MSB = microhal::makeRegister<microhal::uint24_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xF6>{});
        static constexpr auto OUT = microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xF6>{});
        static constexpr auto CTRL_MEAS = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0xF4>{});
        static constexpr auto SOFT_RESET = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0xE0>{});
        static constexpr auto ID = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0xD0>{});
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
        DEFAULT_ADDRESS = 0xEE  //!< DEFAULT_ADDRESS
    };
    /**
     *  Value of fixed registers.
     */
    enum ConstRegisterValues {
        ID_VALUE = 0x55,  //!< ID VALUE
    };
    /**
     *
     * @param i2cNo
     */
    BMP180(microhal::I2C &i2c) : I2CDevice(i2c, DEFAULT_ADDRESS) {}
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
    inline Error reset();
    inline Error startConversion();
    inline bool isNewDataRedy();

 private:
    CalibrationCoefficients calibrationCoefs;

    bool readCalibrationData();
    inline bool update();
};

uint8_t BMP180::getDeviceID() {
    uint8_t id;

    if (readRegister(Register::ID, id) == Error::None) {
        return id;
    } else {
        return 0;
    }
}
/**
 *
 * @return
 */
microhal::I2C::Error BMP180::reset() {
    return writeRegister(Register::SOFT_RESET, static_cast<uint8_t>(0xB6));
}
/**
 *
 * @return
 */
microhal::I2C::Error BMP180::startConversion() {
    return setBitsInRegister(Register::CTRL_MEAS, CTRL_MEAS_SCO);
}
/**
 *
 * @return
 */
bool BMP180::isNewDataRedy() {
    uint8_t ctrl;
    static bool lastConversionInRun = false;

    if (readRegister(Register::CTRL_MEAS, ctrl) == Error::None) {
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
    microhal::uint24_t buff;
    // read uncompressed temperature value
    if (readRegister(Register::OUT, ut) == Error::None) {
        // read uncompressed pressure value
        if (readRegister(Register::OUT_MSB, buff) == Error::None) {
            up = buff;
            // calculate true temperature
            int32_t x1 = (ut - calibrationCoefs.AC6) * calibrationCoefs.AC5 / 2 << 15;
            int32_t x2 = calibrationCoefs.MC * 2 << 11 / (x1 + calibrationCoefs.MD);
            int32_t b5 = x1 + x2;
            int32_t temp = (b5 + 8) / 2 << 4;
        }
    }
}
#endif /* BMP180_H_ */
