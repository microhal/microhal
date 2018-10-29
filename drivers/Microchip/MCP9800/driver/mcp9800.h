/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 07-03-2017
 * last modification: 07-03-2017
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

#ifndef _MICROHAL_MCP9800_H_
#define _MICROHAL_MCP9800_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "I2CDevice/I2CDevice.h"
#include "microhal.h"
/* **************************************************************************************************************************************************
 * CLASS
 */
class MCP9800 : private microhal::I2CDevice {
    using Access = microhal::Access;
    using Endianness = microhal::Endianness;

    static constexpr auto AmbientTemperature =
        microhal::makeRegister<uint16_t, Access::ReadWrite, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x00>{});
    static constexpr auto SensorConfiguration = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x01>{});
    static constexpr auto TemperatureHysteresis =
        microhal::makeRegister<int16_t, Access::ReadWrite, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x02>{});
    static constexpr auto TemperatureLimitSet =
        microhal::makeRegister<int16_t, Access::ReadWrite, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x03>{});

 public:
    using Error = microhal::I2C::Error;

    enum class Resolution : uint8_t {
        NineBits = 0b00000000,    //! 0.5 Celsius degree
        TenBits = 0b00100000,     //! 0.25 Celsius degree
        ElevenBits = 0b01000000,  //! 0.125 Celsius degree
        TwelveBits = 0b01100000   //! 0.0625 Celsius degree
    };

    enum class AlertPolarity { ActiveLow = 0b000, ActiveHigh = 0b100 };

    enum class AlertMode {
        Comparator = 0b00,
        Interrupt = 0b10,
    };

    enum class Address : uint8_t {
        Addr_0x90 = 0x90,
        Addr_0x92 = 0x92,
        Addr_0x94 = 0x94,
        Addr_0x96 = 0x96,
        Addr_0x98 = 0x98,
        Addr_0x9A = 0x9A,
        Addr_0x9C = 0x9C,
        Addr_0x9E = 0x9E,
    };

    MCP9800(microhal::I2C &i2c, Address address) : I2CDevice(i2c, static_cast<uint8_t>(address)) {}

    Error temperature(float *temperature) {
        uint16_t tmp;
        auto status = readRegister(AmbientTemperature, tmp);
        float temp = static_cast<int16_t>(tmp);
        *temperature = temp / 256;
        return status;
    }

    Error resolution(Resolution resolution) { return modifyBitsInRegister(SensorConfiguration, static_cast<uint8_t>(resolution), 0b01100000); }

    Error configureAlertPin(AlertMode mode, AlertPolarity polarity) {
        return modifyBitsInRegister(SensorConfiguration, static_cast<uint8_t>(mode) | static_cast<uint8_t>(polarity),
                                    static_cast<uint8_t>(AlertMode::Interrupt) | static_cast<uint8_t>(AlertPolarity::ActiveHigh));
    }

    Error enableShutdown() { return setBitsInRegister(SensorConfiguration, 1); }

    Error disableShutdown() { return clearBitsInRegister(SensorConfiguration, 1); }

    Error enableOneShot() { return setBitsInRegister(SensorConfiguration, 1); }

    Error disableOneShot() { return clearBitsInRegister(SensorConfiguration, 1); }

    Error faultQueue(uint8_t *bits) {
        const uint8_t lookup[] = {1, 2, 4, 6};
        uint8_t tmp;
        auto status = readRegister(SensorConfiguration, tmp);
        *bits = lookup[(tmp >> 3) & 0b11];
        return status;
    }

    Error faultQueue(uint8_t bits) {
        uint8_t tmp;
        switch (bits) {
            case 1:
                tmp = 0;
                break;
            case 2:
                tmp = 1;
                break;
            case 4:
                tmp = 2;
                break;
            case 6:
                tmp = 3;
                break;
            default:
                return Error::Unknown;
        }
        return modifyBitsInRegister(SensorConfiguration, tmp, 0b00011000);
    }
    /**
     *
     * @param Tset upper temperature limit
     * @param Thyst lower temperature limit
     * @return true if temperature limit was set
     * @return false if an error occurred
     */
    Error congigureTemperatureAlert(float Tset, float Thyst) {
        Tset *= 256;
        auto status = writeRegister(TemperatureLimitSet, static_cast<int16_t>(Tset));
        if (status == Error::None) {
            Thyst *= 256;
            status = writeRegister(TemperatureHysteresis, static_cast<int16_t>(Thyst));
        }
        return status;
    }
};

#endif  // _MICROHAL_MCP9800_H_
