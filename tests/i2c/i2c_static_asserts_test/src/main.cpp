/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 18-10-2017
 * last modification: 18-10-2017
 *
 * @copyright Copyright (c) 2017-2018, Pawel Okas
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */

#include <algorithm>
#include "I2CDevice/I2CDevice.h"
#include "i2c_linux_mock.h"
#include "microhal.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

microhal::linux::I2C i2cmocked;
microhal::I2C &microhal::linux::I2C::mocked = i2cmocked;
static microhal::I2C &i2c = microhal::linux::I2C::mocked;

void testRegisterCreation() {
#if defined TEST_REGISTER_ONE_BYTE_CREATION_SHOULD_FAIL || defined TEST_ALL
    {
        // Build should fail because we are adding endianness parameter into 1 byte data
        static constexpr auto reg0 =
            microhal::makeRegister<uint8_t, Access::WriteOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x00>{});
    }
#endif
#if defined TEST_REGISTER_TWO_BYTE_CREATION_SHOULD_FAIL || defined TEST_ALL
    {
        // Build should fail because we forget to add endianness parameter into 2 byte data
        static constexpr auto reg0 = microhal::makeRegister<uint16_t, Access::WriteOnly>(microhal::Address<uint8_t, 0x00>{});
    }
#endif
#if defined TEST_REGISTER_CREATION_SHOULD_PASS || defined TEST_ALL
    {
        // This should build succeed
        static constexpr auto reg0 = microhal::makeRegister<uint8_t, Access::WriteOnly>(microhal::Address<uint8_t, 0x00>{});
        static constexpr auto reg1 = microhal::makeRegister<int8_t, Access::WriteOnly>(microhal::Address<uint8_t, 0x01>{});
        static constexpr auto reg2 =
            microhal::makeRegister<uint16_t, Access::WriteOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x02>{});
        static constexpr auto reg3 =
            microhal::makeRegister<uint16_t, Access::WriteOnly, microhal::Endianness::Little>(microhal::Address<uint8_t, 0x03>{});
        static constexpr auto reg4 =
            microhal::makeRegister<int16_t, Access::WriteOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x04>{});
        static constexpr auto reg5 =
            microhal::makeRegister<int16_t, Access::WriteOnly, microhal::Endianness::Little>(microhal::Address<uint8_t, 0x05>{});
        static constexpr auto reg6 =
            microhal::makeRegister<uint32_t, Access::WriteOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x06>{});
        static constexpr auto reg7 =
            microhal::makeRegister<uint32_t, Access::WriteOnly, microhal::Endianness::Little>(microhal::Address<uint8_t, 0x07>{});
        static constexpr auto reg8 =
            microhal::makeRegister<int32_t, Access::WriteOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x08>{});
        static constexpr auto reg9 =
            microhal::makeRegister<int32_t, Access::WriteOnly, microhal::Endianness::Little>(microhal::Address<uint8_t, 0x09>{});
    }
#endif
}

void testRegisterAccessDetection() {
#if defined TEST_REGISTER_ACCESS_WRITE_FAIL || defined TEST_ALL
    {
        // Build should fail because we are trying to write data into read only register
        I2CDevice dev(i2c, I2C::DeviceAddress{0x12});
        static constexpr auto reg0 =
            microhal::makeRegister<uint16_t, Access::ReadOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x00>{});
        uint16_t data;
        dev.writeRegister(reg0, data);
    }
#endif
#if defined TEST_REGISTER_ACCESS_READ_FAIL || defined TEST_ALL
    {
        I2CDevice dev(i2c, I2C::DeviceAddress{0x12});
        // Build should fail because we are trying to read data from write only register
        static constexpr auto reg0 =
            microhal::makeRegister<uint16_t, Access::WriteOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x00>{});
        uint16_t data;
        dev.readRegister(reg0, data);
    }
#endif
#if defined TEST_REGISTER_ACCESS_PASS || defined TEST_ALL
    {
        I2CDevice dev(i2c, I2C::DeviceAddress{0x12});
        // Build should succeed
        static constexpr auto reg0 = microhal::makeRegister<uint8_t, Access::WriteOnly>(microhal::Address<uint8_t, 0x00>{});
        static constexpr auto reg1 = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x00>{});
        static constexpr auto reg2 = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x00>{});
        uint8_t data;
        dev.writeRegister(reg0, data);
        dev.readRegister(reg1, data);
        dev.readRegister(reg2, data);
        dev.readRegister(reg2, data);
    }

#endif
}

void testWriteMultipleRegister() {
#if defined TEST_WRITE_MULTIPLE_REGISTERS_ARRAY_FAIL || defined TEST_ALL
    {
        // Build should fail because we are trying to write 2 byte data into 1 byte registers
        I2CDevice dev(i2c, I2C::DeviceAddress{0x12});
        static constexpr auto reg0 = microhal::makeRegister<uint8_t, Access::WriteOnly>(microhal::Address<uint8_t, 0x00>{});
        static constexpr auto reg1 = microhal::makeRegister<uint8_t, Access::WriteOnly>(microhal::Address<uint8_t, 0x01>{});
        std::array<uint16_t, 2> data;
        dev.writeMultipleRegisters(data, reg0, reg1);
    }
#endif
#if defined TEST_WRITE_MULTIPLE_REGISTERS_TUPLE_FAIL || defined TEST_ALL
    {
        // Build should fail because we are trying to write data with incompatible type
        I2CDevice dev(i2c, I2C::DeviceAddress{0x12});
        static constexpr auto reg0 = microhal::makeRegister<uint8_t, Access::WriteOnly>(microhal::Address<uint8_t, 0x00>{});
        static constexpr auto reg1 =
            microhal::makeRegister<uint16_t, Access::WriteOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x01>{});
        std::tuple<uint8_t, uint8_t> data;
        dev.writeMultipleRegisters(data, reg0, reg1);
    }
#endif
#if defined TEST_WRITE_MULTIPLE_REGISTERS_PASS || defined TEST_ALL
    {
        I2CDevice dev(i2c, I2C::DeviceAddress{0x12});
        static constexpr auto reg0 = microhal::makeRegister<uint8_t, Access::WriteOnly>(microhal::Address<uint8_t, 0x00>{});
        static constexpr auto reg1 =
            microhal::makeRegister<uint16_t, Access::WriteOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x01>{});
        std::tuple<uint8_t, uint16_t> data;
        dev.writeMultipleRegisters(data, reg0, reg1);
    }
    {
        I2CDevice dev(i2c, I2C::DeviceAddress{0x12});
        static constexpr auto reg0 =
            microhal::makeRegister<uint16_t, Access::WriteOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x00>{});
        static constexpr auto reg1 =
            microhal::makeRegister<uint16_t, Access::WriteOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x02>{});
        std::array<uint16_t, 2> data;
        dev.writeMultipleRegisters(data, reg0, reg1);
    }
#endif
}

void testReadMultipleRegister() {
#if defined TEST_READ_MULTIPLE_REGISTERS_ARRAY_FAIL || defined TEST_ALL
    {
        // Build should fail because we are trying to write 2 byte data into 1 byte registers
        I2CDevice dev(i2c, I2C::DeviceAddress{0x12});
        static constexpr auto reg0 = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x00>{});
        static constexpr auto reg1 = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x01>{});
        std::array<uint16_t, 2> data;
        dev.readMultipleRegisters(data, reg0, reg1);
    }
#endif
#if defined TEST_READ_MULTIPLE_REGISTERS_TUPLE_FAIL || defined TEST_ALL
    {
        // Build should fail because we are trying to write data with incompatible type
        I2CDevice dev(i2c, I2C::DeviceAddress{0x12});
        static constexpr auto reg0 = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x00>{});
        static constexpr auto reg1 =
            microhal::makeRegister<uint16_t, Access::ReadOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x01>{});
        std::tuple<uint8_t, uint8_t> data;
        dev.readMultipleRegisters(data, reg0, reg1);
    }
#endif
#if defined TEST_READ_MULTIPLE_REGISTERS_PASS || defined TEST_ALL
    {
        I2CDevice dev(i2c, I2C::DeviceAddress{0x12});
        static constexpr auto reg0 = microhal::makeRegister<uint8_t, Access::ReadOnly>(microhal::Address<uint8_t, 0x00>{});
        static constexpr auto reg1 =
            microhal::makeRegister<uint16_t, Access::ReadOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x01>{});
        std::tuple<uint8_t, uint16_t> data;
        dev.readMultipleRegisters(data, reg0, reg1);
    }
    {
        I2CDevice dev(i2c, I2C::DeviceAddress{0x12});
        static constexpr auto reg0 =
            microhal::makeRegister<uint16_t, Access::ReadOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x00>{});
        static constexpr auto reg1 =
            microhal::makeRegister<uint16_t, Access::ReadOnly, microhal::Endianness::Big>(microhal::Address<uint8_t, 0x02>{});
        std::array<uint16_t, 2> data;
        dev.readMultipleRegisters(data, reg0, reg1);
    }
#endif
}

int main(void) {
    return 0;
}
