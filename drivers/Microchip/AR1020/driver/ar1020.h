/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 20-04-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#ifndef _MICROHAL_AR1020_H_
#define _MICROHAL_AR1020_H_

#include <cstdint>
#include "I2CDevice/I2CDevice.h"

class AR1020 {
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    using span = gsl::span<uint8_t>;  // Todo change to std::span when it will be available in gcc

    // create alias to microhal::Address, we just want to type less
    template <typename T, T i>
    using Address = microhal::Address<T, i>;

    struct Command {
        enum class CommandType : uint8_t {
            GetVersion = 0x10,
            Enable = 0x12,
            Disable = 0x13,
            CalibrateMode = 0x14,
            RegisterRead = 0x20,
            RegistrWrite = 0x21,
            RegistrStartAddressRequest = 0x22,
            RegistersWriteToEeprom = 0x23,
            EepromRead = 0x28,
            EepromWrite = 0x29,
            EepromWriteToRegisters = 0x2B
        };

        struct Response {
            enum Status : uint8_t {
                Success = 0,
                Unrecognixed,
                HeaderUnrecoginized,
                TimeOut,  //!< Exceeded ~100ms
                CancelCalibrationMode
            };

            uint8_t header;
            uint8_t dataSize;
            Status response;
            CommandType command;

            bool operator==(Response &b) const {
                if (command != b.command) return false;
                if (response != b.response) return false;
                if (dataSize != b.dataSize) return false;
                if (header != b.header) return false;
                return true;
            }
        };

        struct Base {
            constexpr Base(CommandType command, uint8_t data) : zeros(0), header(0x55), dataSize(2), command(command) { this->data[0] = data; }
            constexpr Base(CommandType command) : zeros(0), header(0x55), dataSize(1), command(command) {}

            const uint8_t *getDataPtr() const { return reinterpret_cast<const uint8_t *>(this); }
            uint8_t size_bytes() const { return dataSize + 3; }

            uint8_t zeros;
            uint8_t header;
            uint8_t dataSize;
            CommandType command;
            uint8_t data[3] = {0, 0, 0};
        };

        class GetVersion : public Base {
         public:
            constexpr GetVersion() : Base(CommandType::GetVersion) {}
        };

        class Enable : public Base {
         public:
            constexpr Enable() : Base(CommandType::Enable) {}
        };

        class Disable : public Base {
         public:
            constexpr Disable() : Base(CommandType::Disable) {}
        };

        class Calibrate : public Base {
         public:
            constexpr Calibrate() : Base(CommandType::CalibrateMode, 0x04) {}
        };

        class EepromRead : public Base {
         public:
            constexpr EepromRead(uint8_t startAddress, uint8_t length) : Base(CommandType::EepromRead) {
                dataSize = 4;
                data[0] = 0x00;
                data[1] = startAddress;
                data[2] = length;
            }
        };
    };

    struct Register {
        static constexpr auto TouchThreshold = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x02>{});
        static constexpr auto SensitivityFilter = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x03>{});
        static constexpr auto SamplingFast = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x04>{});
        static constexpr auto SamplingSlow = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x05>{});
        static constexpr auto AccuracyFilterFast = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x06>{});
        static constexpr auto AccuracyFilterSlow = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x07>{});
        static constexpr auto SpeedThreshold = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x08>{});
        static constexpr auto SleepDelay = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0A>{});
        static constexpr auto PenUpDelay = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0B>{});
        static constexpr auto TouchMode = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0C>{});
        static constexpr auto TouchOptions = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0D>{});
        static constexpr auto CalibrationInset = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0E>{});
        static constexpr auto PenStateReportDelay = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x0F>{});
        static constexpr auto TouchReportDelay = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x11>{});
    };

 public:
    using Error = microhal::I2C::Error;
    struct Position {
        uint16_t x;
        uint16_t y;
        bool pressed;
    };

    AR1020(microhal::I2C &i2c) : i2cbase(i2c), i2c(i2c, 0x9A) {}

    bool init() { return i2c.writeRegister(Register::TouchOptions, 0) == microhal::I2C::Error::None; }

    bool enable() {
        Command::Enable cmd;
        return sendCMD(cmd);
    }
    bool disable() {
        Command::Disable cmd;
        return sendCMD(cmd);
    }

    bool getVersion(uint16_t &version);

    bool calibrationStart();
    bool callibrationWaitForTouch();
    bool callibrationStop();

    bool eepromRead();

    Position readPosition();

 private:
    microhal::I2C &i2cbase;
    microhal::I2CDevice i2c;

    bool sendCMD(Command::Base &cmd, uint8_t *data, size_t dataSize);
    bool sendCMD(Command::Base &cmd);
};

#endif /* _MICROHAL_AR1020_H_ */
