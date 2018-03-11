/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      driver for LSM330DL gyroscope
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

#ifndef LSM330DLGYROSCOPE_H_
#define LSM330DLGYROSCOPE_H_

#include <array>

#include "I2CDevice/I2CDevice.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"

namespace lsm330 {

class Gyroscope : protected microhal::I2CDevice {
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    using Error = microhal::I2C::Error;

    // create alias to microhal::Address, we just want to type less
    template <typename T, T i>
    using Address = microhal::Address<T, i>;

    enum SpecialCommends : uint8_t { ENABLE_AUTOINCREMENT = (1 << 7) };

    static constexpr auto CTRL_REG1_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x20 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto CTRL_REG2_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x21 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto CTRL_REG3_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x22 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto CTRL_REG4_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x23 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto CTRL_REG5_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x24 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto DATACAPTURE_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x25 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto OUT_TEMP_G = microhal::makeRegister<int8_t, Access::ReadWrite>(Address<uint8_t, 0x26 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto STATUS_REG_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x27 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto OUT_X_L_G =
        microhal::makeRegister<int16_t, Access::ReadWrite, Endianness::Little>(Address<uint8_t, 0x28 | ENABLE_AUTOINCREMENT>{});
    // static constexpr auto OUT_X_H_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x29 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto OUT_Y_L_G =
        microhal::makeRegister<int16_t, Access::ReadWrite, Endianness::Little>(Address<uint8_t, 0x2A | ENABLE_AUTOINCREMENT>{});
    // static constexpr auto OUT_Y_H_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2B | ENABLE_AUTOINCREMENT>{});
    static constexpr auto OUT_Z_L_G =
        microhal::makeRegister<int16_t, Access::ReadWrite, Endianness::Little>(Address<uint8_t, 0x2C | ENABLE_AUTOINCREMENT>{});
    // static constexpr auto OUT_Z_H_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2D | ENABLE_AUTOINCREMENT>{});
    static constexpr auto FIFO_CTRL_REG_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2E | ENABLE_AUTOINCREMENT>{});
    static constexpr auto FIFO_SRC_REG_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x2F | ENABLE_AUTOINCREMENT>{});
    static constexpr auto INT1_CFG_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x30 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto INT1_SRC_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x31 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto INT1_THS_XH_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x32 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto INT1_THS_XL_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x33 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto INT1_THS_YH_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x34 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto INT1_THS_YL_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x35 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto INT1_THS_ZH_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x36 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto INT1_THS_ZL_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x37 | ENABLE_AUTOINCREMENT>{});
    static constexpr auto INT1_DURATION_G = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x38 | ENABLE_AUTOINCREMENT>{});

    enum Bitmasks {
        CTRL_REG1_G_ODR_mask = 0b11000000,
        CTRL_REG1_G_Cutoff_mask = 0b00110000,

        CTRL_REG4_G_FS_mask = 0b00110000,
    };

 public:
    enum class Axis : uint8_t { X = 0x01, Y = 0x02, Z = 0x04 };

    enum class Mode : uint8_t {
        PowerDown = 0x00,
        NormalOrSleep = 0x08  ///<! sleep mode only when all axis will be disabled
    };

    typedef enum {
        ODR_100Hz_cutoff_12_5Hz = 0b0000,  ///<! output frequency 100Hz cut off frequency 12.5Hz
        ODR_100Hz_cutoff_25Hz = 0b0001,    ///<! output frequency 100Hz cut off frequency 25
        // ODR_100Hz_cutoff_25Hz = 0b0010, ///<! output frequency 100Hz cut off frequency 25
        // ODR_100Hz_cutoff_25Hz = 0b0011, ///<! output frequency 100Hz cut off frequency 25
        ODR_200Hz_cutoff_12_5Hz = 0b0100,  ///<! output frequency 200Hz cut off frequency 12.5
        ODR_200Hz_cutoff_25Hz = 0b0101,    ///<! output frequency 200Hz cut off frequency 25
        ODR_200Hz_cutoff_50Hz = 0b0110,    ///<! output frequency 200Hz cut off frequency 50
        ODR_200Hz_cutoff_70Hz = 0b0111,    ///<! output frequency 200Hz cut off frequency 70
        ODR_400Hz_cutoff_20Hz = 0b1000,    ///<! output frequency 400Hz cut off frequency 20
        ODR_400Hz_cutoff_25Hz = 0b1001,    ///<! output frequency 400Hz cut off frequency 25
        ODR_400Hz_cutoff_50Hz = 0b1010,    ///<! output frequency 400Hz cut off frequency 50
        ODR_400Hz_cutoff_110Hz = 0b1011,   ///<! output frequency 400Hz cut off frequency 110
        ODR_800Hz_cutoff_30Hz = 0b1100,    ///<! output frequency 800Hz cut off frequency 30
        ODR_800Hz_cutoff_35Hz = 0b1101,    ///<! output frequency 800Hz cut off frequency 35
        ODR_800Hz_cutoff_50Hz = 0b1110,    ///<! output frequency 800Hz cut off frequency 50
        ODR_800Hz_cutoff_110Hz = 0b1111,   ///<! output frequency 800Hz cut off frequency 110
    } OdrCutoff;

    typedef enum {
        FSR_250dps = 0x00 << 4,
        FSR_500dps = 0x01 << 4,
        FSR_2000dps = 0x02 << 4,
    } FullScaleRange;

    enum class FIFOmode : uint8_t { Bypas, FIFO, Stream, StreamToFIFO, BypasToStream };

    typedef enum : uint8_t { I2C_ADDRESS_0 = 0xD0, I2C_ADDRESS_1 = 0xD2 } PossibleI2CAddress;

    constexpr Gyroscope(microhal::I2C &i2c, PossibleI2CAddress address) : I2CDevice(i2c, address) {}
    ~Gyroscope() {}

    bool init(Mode mode, OdrCutoff odr, Axis activeAxis) {
        if (setMode(mode)) {
            if (setODR(odr) == Error::None) {
                return enableAxis(activeAxis) == Error::None;
            }
        }
        return false;
    }

    Error enableAxis(Axis axis) { return setBitsInRegister(CTRL_REG1_G, static_cast<uint8_t>(axis)); }

    Error disableAxis(Axis axis) { return clearBitsInRegister(CTRL_REG1_G, static_cast<uint8_t>(axis)); }

    bool setMode(Mode mode) {
        if (mode == Mode::PowerDown) {
            return clearBitsInRegister(CTRL_REG1_G, static_cast<uint8_t>(Mode::NormalOrSleep)) == Error::None;
        } else {
            return setBitsInRegister(CTRL_REG1_G, static_cast<uint8_t>(mode)) == Error::None;
        }
        return false;
    }

    Error setODR(OdrCutoff odr) { return modifyBitsInRegister(CTRL_REG1_G, odr, CTRL_REG1_G_ODR_mask | CTRL_REG1_G_Cutoff_mask); }

    Error setFullScaleRange(FullScaleRange fsr) { return modifyBitsInRegister(CTRL_REG4_G, fsr, CTRL_REG4_G_FS_mask); }

    bool isNewDataAvailable(Axis axis) {
        uint8_t status;
        if (readRegister(STATUS_REG_G, status) == Error::None) {
            return status & static_cast<uint8_t>(axis);
        }
        return false;
    }

    Error readSamples(int16_t &x, int16_t &y, int16_t &z) {
        std::array<int16_t, 3> data;
        const auto status = readMultipleRegisters(data, OUT_X_L_G, OUT_Y_L_G, OUT_Z_L_G);

        x = data[0];
        y = data[1];
        z = data[2];

        return status;
    }

    Error readTemperature(int8_t &temp) { return readRegister(OUT_TEMP_G, temp); }

    template <microhal::diagnostic::LogLevel level>
    void registerDump(microhal::diagnostic::Diagnostic<level> &log) {
        uint8_t regVal;

        log << microhal::diagnostic::Warning << "----------- LSM330 gyroscope register dump -----------" << microhal::diagnostic::endl;

        //        for (uint8_t reg = CTRL_REG1_G; reg <= INT1_DURATION_G; reg++) {
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

constexpr Gyroscope::Axis operator|(Gyroscope::Axis a, Gyroscope::Axis b) {
    return static_cast<Gyroscope::Axis>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

}  // namespace lsm330

#endif /* LSM330DLGYROSCOPE_H_ */
