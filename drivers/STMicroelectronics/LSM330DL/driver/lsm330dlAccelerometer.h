/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      driver for LSM330DL accelerometer
 *
 * @authors    Pawel Okas
 * created on: 16-10-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014-2018, Pawel Okas
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

#ifndef LSM330DLACCELEROMETER_H_
#define LSM330DLACCELEROMETER_H_

#include "I2CDevice/I2CDevice.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"

namespace lsm330 {

class Accelerometer : protected microhal::I2CDevice {
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    using Error = microhal::I2C::Error;

    // create alias to microhal::Address, we just want to type less
    template <typename T, T i>
    using Address = microhal::Address<T, i>;

    enum SpecialCommends : uint8_t { ENABLE_AUTOINCREMENT = (1 << 7) };

    static constexpr auto CTRL_REG1_A = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x20 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto CTRL_REG2_A = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x21 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto CTRL_REG3_A = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x22 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto CTRL_REG4_A = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x23 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto CTRL_REG5_A = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x24 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto CTRL_REG6_A = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x25 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto DATACAPTURE_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x26 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto STATUS_REG_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x27 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto OUT_X_L_A =
        microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Little>(Address<uint8_t, 0x28 | ENABLE_AUTOINCREMENT>{});
    // static constexpr auto OUT_X_H_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x29>{});
    static constexpr auto OUT_Y_L_A =
        microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Little>(Address<uint8_t, 0x2A | ENABLE_AUTOINCREMENT>{});
    // static constexpr auto OUT_Y_H_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x2B>{});
    static constexpr auto OUT_Z_L_A =
        microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Little>(Address<uint8_t, 0x2C | ENABLE_AUTOINCREMENT>{});
    // static constexpr auto OUT_Z_H_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x2D>{});
    static constexpr auto FIFO_CTRL_REG_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x2E>{});
    static constexpr auto FIFO_SRC_REG_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x2F>{});
    static constexpr auto INT1_CFG_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x30>{});
    static constexpr auto INT1_SRC_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x31>{});
    static constexpr auto INT1_THS_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x32>{});
    static constexpr auto INT1_DURATION_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x33>{});
    static constexpr auto INT2_CFG_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x34>{});
    static constexpr auto INT2_SOURCE_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x35>{});
    static constexpr auto INT2_THS_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x36>{});
    static constexpr auto INT2_DURATION_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x37>{});
    static constexpr auto CLICK_CFG_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x38>{});
    static constexpr auto CLICK_SRC_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x39>{});
    static constexpr auto CLICK_THS_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x3A>{});
    static constexpr auto TIME_LIMIT_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x3B>{});
    static constexpr auto TIME_LATENCY_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x3C>{});
    static constexpr auto TIME_WINDOW_A = microhal::makeRegister<uint8_t, Access::ReadOnly>(Address<uint8_t, 0x3D>{});

    enum Bitmasks {
        CTRL_REG1_G_ODR_mask = 0b11000000,
        CTRL_REG1_G_Cutoff_mask = 0b00110000,

        CTRL_REG4_A_FS_mask = 0b00110000,
    };

    enum Bitfields {
        CTRL_REG1_A_LPen = 0x08,
    };

 public:
    enum class Axis : uint8_t { X = 0x01, Y = 0x02, Z = 0x04 };

    typedef enum : uint8_t {
        ODR_0Hz_PowerDown = 0x00,
        ODR_1Hz = 0x10,
        ODR_10Hz,
        ODR_25Hz,
        ODR_50Hz,
        ODR_100Hz,
        ODR_200Hz,
        ODR_400Hz,
        ODR_LowPowerMode_1620Hz,  /// set only when low power mode is enabled
        ODR_Normal_1344Hz_LowPowerMode_5376Hz
    } OutputDataRate;

    typedef enum {
        FSR_2g = 0x00 << 4,
        FSR_4g = 0x01 << 4,
        FSR_8g = 0x02 << 4,
        FSR_16g = 0x03 << 4,
    } FullScaleRange;

    typedef enum : uint8_t { I2C_ADDRESS_0 = 0x30, I2C_ADDRESS_1 = 0x32 } PossibleI2CAddress;

    constexpr Accelerometer(microhal::I2C &i2c, PossibleI2CAddress address) : I2CDevice(i2c, address) {}

    bool init(bool lowPowerMode, OutputDataRate odr, Axis activeAxis) {
        if (lowPowerMode) {
            if (enableLowPowerMode() != Error::None) return false;
        } else {
            if (disableLowPowerMode() != Error::None) return false;
        }

        if (setODR(odr) == Error::None) {
            return enableAxis(activeAxis) == Error::None;
        }
        return false;
    }

    Error enableAxis(Axis axis) { return setBitsInRegister(CTRL_REG1_A, static_cast<uint8_t>(axis)); }

    Error disableAxis(Axis axis) { return clearBitsInRegister(CTRL_REG1_A, static_cast<uint8_t>(axis)); }

    Error enableLowPowerMode() { return setBitsInRegister(CTRL_REG1_A, static_cast<uint8_t>(CTRL_REG1_A_LPen)); }

    Error disableLowPowerMode() { return clearBitsInRegister(CTRL_REG1_A, static_cast<uint8_t>(CTRL_REG1_A_LPen)); }

    Error setODR(OutputDataRate odr) { return modifyBitsInRegister(CTRL_REG1_A, odr, CTRL_REG1_G_ODR_mask | CTRL_REG1_G_Cutoff_mask); }

    Error setFullScaleRange(FullScaleRange fsr) { return modifyBitsInRegister(CTRL_REG4_A, fsr, CTRL_REG4_A_FS_mask); }

    bool isNewDataAvailable(Axis axis) {
        uint8_t status;
        if (readRegister(STATUS_REG_A, status) == Error::None) {
            return status & static_cast<uint8_t>(axis);
        }
        return false;
    }

    Error readSamples(int16_t &x, int16_t &y, int16_t &z) {
        std::array<int16_t, 3> data;
        const auto status = readMultipleRegisters(data, OUT_X_L_A, OUT_Y_L_A, OUT_Z_L_A);

        x = data[0];
        y = data[1];
        z = data[2];

        return status;
    }

    template <microhal::diagnostic::LogLevel level>
    void registerDump(microhal::diagnostic::Diagnostic<level> &log) {
        uint8_t regVal;

        log << microhal::diagnostic::Warning << "----------- LSM330 accelerometer register dump -----------" << microhal::diagnostic::endl;

        //        for (uint8_t reg = CTRL_REG1_A; reg <= TIME_WINDOW_A; reg++) {
        //            log << microhal::diagnostic::Warning << microhal::diagnostic::toHex(reg) << " ";
        //
        //            if (readRegister(reg, regVal)) {
        //                log << microhal::diagnostic::Warning << regVal << microhal::diagnostic::endl;
        //            } else {
        //                log << microhal::diagnostic::Warning << "Error while reading." << microhal::diagnostic::endl;
        //            }
        //        }

        log << microhal::diagnostic::Warning << "----------- End of register dump -----------" << microhal::diagnostic::endl;
    }
};

constexpr Accelerometer::Axis operator|(Accelerometer::Axis a, Accelerometer::Axis b) {
    return static_cast<Accelerometer::Axis>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}
}  // namespace lsm330
#endif /* LSM330DLACCELEROMETER_H_ */
