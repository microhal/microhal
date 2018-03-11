/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 12-02-2017
 * last modification: 12-02-2017
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

#ifndef _MICROHAL_MPL115A2_H_
#define _MICROHAL_MPL115A2_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <chrono>
#include <experimental/optional>
#include "I2CDevice/I2CDevice.h"
#include "microhal.h"
#include "units/pressure.h"
/* **************************************************************************************************************************************************
 * CLASS
 */
class MPL115A2 : microhal::I2CDevice {
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    using Error = microhal::I2C::Error;

    struct Registers {
        static constexpr auto Padc = microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Big>(
            microhal::Address<uint8_t, 0x00>{});  ///< 10-bit Pressure ADC output value MSB
        static constexpr auto Tadc = microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Big>(
            microhal::Address<uint8_t, 0x02>{});  ///< 10-bit Temperature ADC output value MSB
        static constexpr auto a0 =
            microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(microhal::Address<uint8_t, 0x04>{});  ///< a0 coefficient MSB
        static constexpr auto b1 =
            microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(microhal::Address<uint8_t, 0x06>{});  ///< b1 coefficient MSB
        static constexpr auto b2 =
            microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(microhal::Address<uint8_t, 0x08>{});  ///< b2 coefficient MSB
        static constexpr auto c12 =
            microhal::makeRegister<int16_t, Access::ReadOnly, Endianness::Big>(microhal::Address<uint8_t, 0x0A>{});  ///< c12 coefficient MSB
        static constexpr auto CONVERT =
            microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x12>{});  ///< Start Pressure and Temperature Conversion
    };

 public:
    using Pressure = microhal::Pressure<float>;

    MPL115A2(microhal::I2C &i2c) : I2CDevice(i2c, 0xC0) {}

    bool init() { return readCoefficient(); }

    Error startConversion() { return writeRegister(Registers::CONVERT, 0x00); }

    std::experimental::optional<Pressure> pressure() {
        std::experimental::optional<Pressure> pressure;
        Pressure tmp;
        if (readData(tmp)) {
            pressure = tmp;
        }

        return pressure;
    }

    void debug();

    static constexpr auto maxConversionTime() noexcept { return std::chrono::milliseconds{3}; }

 private:
    struct Coefficient {
        float a0;
        float b1;
        float b2;
        float c12;
    } coefficient = {};

    bool readCoefficient() {
        std::array<int16_t, 4> tmp;
        if (readMultipleRegisters(tmp, Registers::a0, Registers::b1, Registers::b2, Registers::c12) == Error::None) {
            float a0 = tmp[0];
            a0 /= 1 << 3;
            coefficient.a0 = a0;
            float b1 = tmp[1];
            b1 /= 1 << 13;
            coefficient.b1 = b1;
            float b2 = tmp[2];
            b2 /= 1 << 14;
            coefficient.b2 = b2;
            float c12 = tmp[3];
            c12 /= 1 << 23;
            coefficient.c12 = c12;
            return true;
        }
        return false;
    }

    Pressure compensate(uint16_t rawPressure, uint16_t rawTemperature) const noexcept {
        rawPressure >>= 6;
        rawTemperature >>= 6;
        float Pcomp = coefficient.a0 + (coefficient.b1 + coefficient.c12 * rawTemperature) * rawPressure + coefficient.b2 * rawTemperature;
        return Pressure{Pcomp * ((115.0f - 50.0f) / 1023.0f) + 50.0f};
    }

    bool readData(Pressure &compensatedPressure) {
        std::array<uint16_t, 2> tmp;
        if (readMultipleRegisters(tmp, Registers::Padc, Registers::Tadc) == Error::None) {
            compensatedPressure = compensate(tmp[0], tmp[1]);
            return true;
        }
        return false;
    }
};

#endif  // _MICROHAL_MPL115A2_H_
