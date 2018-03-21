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

#include <optional>
#include "I2CDevice/I2CDevice.h"
#include "microhal.h"

/**
 * \addtogroup Devices
 * @{
 * @class BMP180
 * @}
 */
class BMP180 : public microhal::I2CDevice {
 public:
    using Error = microhal::I2C::Error;

 private:
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;

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
        static constexpr auto SOFT_RESET = microhal::makeRegister<uint8_t, Access::WriteOnly>(Address<uint8_t, 0xE0>{});
        static constexpr auto ID = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0xD0>{});
        static constexpr auto AC1 = microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xAA>{});
        static constexpr auto AC2 = microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xAC>{});
        static constexpr auto AC3 = microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xAE>{});
        static constexpr auto AC4 = microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xB0>{});
        static constexpr auto AC5 = microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xB2>{});
        static constexpr auto AC6 = microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xB4>{});
        static constexpr auto B1 = microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xB6>{});
        static constexpr auto B2 = microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xB8>{});
        static constexpr auto MB = microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xBA>{});
        static constexpr auto MC = microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xBC>{});
        static constexpr auto MD = microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0xBE>{});
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
    enum class Oversampling { None = 0b00, TwoTimes = 0b01, FourTimes = 0b10, EightTimes = 0b11 };

    BMP180(microhal::I2C &i2c) : I2CDevice(i2c, DEFAULT_ADDRESS) {}

    bool init() {
        if (getDeviceID() == ID_VALUE) {
            if (readCalibrationData() == Error::None) {
                return true;
            }
        }
        return false;
    }

    uint8_t getDeviceID() {
        uint8_t id;

        if (readRegister(Register::ID, id) == Error::None) {
            return id;
        } else {
            return 0;
        }
    }

    Error reset() { return writeRegister(Register::SOFT_RESET, static_cast<uint8_t>(0xB6)); }

    struct TemperatureAndPressure {
        int32_t temperature;
        uint32_t pressure;
    };
    std::optional<TemperatureAndPressure> getTemperatureAndPressure(Oversampling oversampling) {
        if (auto ut = getUncompensatedTemperature()) {
            if (auto up = getUncompensatedPressure(oversampling)) {
                auto b5 = calculateB5(*ut);
                auto temp = calculateTrueTemperature(b5);
                auto pressure = calculateTruePressure(*up, oversampling, b5);
                return TemperatureAndPressure{temp, pressure};
            }
        }
        return {};
    }

 private:
    CalibrationCoefficients calibrationCoefs;

    bool waitForDataReady() {
        uint16_t i = 0;
        while (isNewDataRedy() == false) {
            i++;
            if (i > 2000) return false;
        }
        return true;
    }

    Error startTemperatureConversion() { return writeRegister(Register::CTRL_MEAS, CTRL_MEAS_SCO | 0x0E); }
    Error startPressureConversion(Oversampling oversampling) {
        return writeRegister(Register::CTRL_MEAS, CTRL_MEAS_SCO | (static_cast<uint8_t>(oversampling) << 6) | 0x14);
    }

    bool isNewDataRedy() {
        uint8_t ctrl;
        if (readRegister(Register::CTRL_MEAS, ctrl) == Error::None) {
            return (ctrl & CTRL_MEAS_SCO) == 0;
        }
        return false;
    }

    std::optional<int16_t> getUncompensatedTemperature() {
        if (startTemperatureConversion() == Error::None) {
            std::this_thread::sleep_for(std::chrono::milliseconds{5});
            if (waitForDataReady()) {
                int16_t ut;
                if (readRegister(Register::OUT, ut) == Error::None) {
                    return ut;
                }
            }
        }
        return {};
    }

    std::optional<int32_t> getUncompensatedPressure(Oversampling oversampling) {
        const std::chrono::milliseconds conversionTime[] = {std::chrono::milliseconds{5}, std::chrono::milliseconds{8}, std::chrono::milliseconds{14},
                                                            std::chrono::milliseconds{26}};
        if (startPressureConversion(oversampling) == Error::None) {
            std::this_thread::sleep_for(conversionTime[static_cast<uint8_t>(oversampling)]);
            if (waitForDataReady()) {
                microhal::uint24_t buff;
                if (readRegister(Register::OUT_MSB, buff) == Error::None) {
                    uint32_t up = buff;
                    up >>= 8 - static_cast<uint8_t>(oversampling);
                    return up;
                }
            }
        }
        return {};
    }

    constexpr int32_t calculateB5(int32_t uncompensatedTemperature) {
        int32_t x1 = (static_cast<int32_t>(uncompensatedTemperature - calibrationCoefs.AC6) * calibrationCoefs.AC5) / (1 << 15);
        int32_t x2 = calibrationCoefs.MC * (1 << 11) / (x1 + calibrationCoefs.MD);
        int32_t b5 = x1 + x2;
        return b5;
    }
    constexpr int32_t calculateTrueTemperature(int32_t b5) const {
        int32_t temp = (b5 + 8) / (1 << 4);
        return temp;
    }

    constexpr uint32_t calculateTruePressure(int32_t uncompensatedPressure, Oversampling oversampling, int32_t b5) const {
        uint8_t oss = static_cast<uint8_t>(oversampling);
        int32_t b6 = b5 - 4000;
        int32_t x1 = (calibrationCoefs.B2 * (b6 * b6 / (1 << 12))) / (1 << 11);
        int32_t x2 = calibrationCoefs.AC2 * b6 / (1 << 11);
        int32_t x3 = x1 + x2;
        int32_t ac1 = calibrationCoefs.AC1;
        int32_t b3 = (((ac1 * 4 + x3) << oss) + 2) / 4;
        x1 = calibrationCoefs.AC3 * b6 / (1 << 13);
        x2 = (calibrationCoefs.B1 * (b6 * b6 / (1 << 12))) / (1 << 16);
        x3 = ((x1 + x2) + 2) / (1 << 2);
        uint32_t b4 = calibrationCoefs.AC4 * static_cast<uint32_t>(x3 + 32768) / (1 << 15);
        uint32_t b7 = static_cast<uint32_t>(uncompensatedPressure - b3) * (50000 >> oss);
        int32_t p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;
        x1 = (p / (1 << 8)) * (p / (1 << 8));
        x1 = (x1 * 3038) / (1 << 16);
        x2 = (-7357 * p) / (1 << 16);
        p = p + (x1 + x2 + 3791) / (1 << 4);
        return p;
    }

    Error readCalibrationData();
};

#endif /* BMP180_H_ */
