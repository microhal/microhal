/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 18-10-2017
 * last modification: 18-10-2017
 *
 * @copyright Copyright (c) 2017, microHAL
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
#include "consoleIODevice_linux.h"
#include "diagnostic/diagnostic.h"
#include "i2c_linux_mock.h"
#include "microhal.h"
#include "mockDevice.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

// operator for writing I2C error
template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel, I2C::Error error) {
    if (error == I2C::Error::None) return logChannel << "I2C::Error::None";
    if (error == I2C::Error::Timeout) logChannel << "I2C::Error::Timeout";
    if (error == I2C::Error::Bus) logChannel << "I2C::Error::Bus";
    if (error == I2C::Error::AcknowledgeFailure) logChannel << "I2C::Error::AcknowledgeFailure";
    if (error == I2C::Error::ArbitrationLost) logChannel << "I2C::Error::ArbitrationLost";
    if (error == I2C::Error::Overrun) logChannel << "I2C::Error::Overrun";
    if (error == I2C::Error::Unknown) logChannel << "I2C::Error::Unknown";

    return logChannel;
}

IODevice &debugPort = linux::consoleIODev;
Diagnostic<LogLevel::Debug> loging("log", debugPort, EnableFileName | EnableLevelName);

microhal::linux::I2C i2cmocked;
microhal::I2C &microhal::linux::I2C::mocked = i2cmocked;
static microhal::I2C &i2c = microhal::linux::I2C::mocked;

// create mock device
Device device(0x12);

bool test() {
    uint8_t data[] = {0, 1, 2};

    loging << lock << MICROHAL_DEBUG << "Writing text to I2C device with address 0x10" << unlock;
    auto stat = i2c.write(I2C::DeviceAddress{0x10}, data, sizeof(data));
    if (stat == I2C::Error::None)
        loging << lock << MICROHAL_DEBUG << "data successfully written." << unlock;
    else
        loging << lock << MICROHAL_DEBUG << stat << unlock;

    loging << lock << MICROHAL_DEBUG << "Writing data to I2C device with address 0x12" << unlock;
    if (i2c.write(I2C::DeviceAddress{0x12}, data, sizeof(data)) == I2C::Error::None) {
        if (device[0] == 1 && device[1] == 2) loging << lock << MICROHAL_DEBUG << "data successfully written." << unlock;
    } else
        loging << lock << MICROHAL_DEBUG << stat << unlock;

    loging << lock << MICROHAL_DEBUG << "Reading data from I2C device with address 0x12" << unlock;
    uint8_t address = 0;
    if (i2c.writeRead(I2C::DeviceAddress{0x12}, &address, sizeof(address), data, sizeof(data)) == I2C::Error::None) {
        if (device[0] == data[0] && device[1] == data[1] && device[2] == data[2])
            loging << lock << MICROHAL_DEBUG << "data successfully read." << unlock;
    } else
        loging << lock << MICROHAL_DEBUG << stat << unlock;

    return false;
}

bool deviceTest() {
    I2CDevice dev(i2c, I2C::DeviceAddress{0x12});

    // crate registers
    static constexpr auto reg0 = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x00>{});
    static constexpr auto reg1 = microhal::makeRegister<uint8_t, Access::ReadWrite>(microhal::Address<uint8_t, 0x01>{});

    if (dev.write(reg0, uint8_t{0}) == I2C::Error::None) {
        if (device[0] == 0)
            loging << lock << MICROHAL_DEBUG << "data successfully written." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "written data corrupted. Reg value: " << device[0] << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to write register." << unlock;
        return false;
    }

    uint8_t tmp;
    if (dev.read(reg0, tmp) == I2C::Error::None) {
        if (tmp == device[0])
            loging << lock << MICROHAL_DEBUG << "data read successfully." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "read data corrupted. Reg value: " << device[0] << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to read register." << unlock;
        return false;
    }

    if (dev.bitsSet(reg0, uint8_t{12}) == I2C::Error::None) {
        if (device[0] == 12)
            loging << lock << MICROHAL_DEBUG << "Bits set correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Bits set incorrectly. Reg value: " << device[0] << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to set bits." << unlock;
        return false;
    }

    dev.write(reg0, uint8_t{0b10101010});
    if (device[0] == 0b10101010) loging << lock << MICROHAL_DEBUG << "data successfully written." << unlock;

    if (dev.bitsClear(reg0, uint8_t{0b00000010}) == I2C::Error::None) {
        if (device[0] == 0b10101000)
            loging << lock << MICROHAL_DEBUG << "Bits clear correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Bits clear incorrectly. Reg value: " << device[0] << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to clear bits." << unlock;
        return false;
    }

    if (dev.bitsModify(reg0, uint8_t{0b01000010}, uint8_t{0b11000010}) == I2C::Error::None) {
        if (device[0] == 0b01101010)
            loging << lock << MICROHAL_DEBUG << "Bits modified correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Bits modified incorrectly. Reg value: " << device[0] << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to modify bits." << unlock;
        return false;
    }

    std::array<uint8_t, 2> regs;
    if (dev.readRegisters(regs, reg0, reg1) == I2C::Error::None) {
        if (device[0] == regs[0] && device[1] == regs[1])
            loging << lock << MICROHAL_DEBUG << "Multiple register read correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Data mismatch while reading multiple registers." << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to read multiple registers." << unlock;
        return false;
    }

    regs[0] = 0xAA;
    regs[1] = 0xBB;
    if (dev.writeRegisters(regs, reg0, reg1) == I2C::Error::None) {
        if (device[0] == regs[0] && device[1] == regs[1])
            loging << lock << MICROHAL_DEBUG << "Multiple register write correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Data mismatch while writing multiple registers." << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to write multiple registers." << unlock;
        return false;
    }

    // crate registers
    static constexpr auto reg2 = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Little>(microhal::Address<uint8_t, 0x02>{});
    static constexpr auto reg3 = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Little>(microhal::Address<uint8_t, 0x04>{});

    if (dev.write(reg2, uint16_t{0}) == I2C::Error::None) {
        if (device[2] == 0 && device[3] == 0)
            loging << lock << MICROHAL_DEBUG << "data successfully written." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "written data corrupted. Reg value: " << device[0] << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to write register." << unlock;
        return false;
    }

    uint16_t tmp2;
    if (dev.read(reg2, tmp2) == I2C::Error::None) {
        if (tmp == (device[2] | (device[3] << 8)))
            loging << lock << MICROHAL_DEBUG << "data read successfully." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "read data corrupted. Reg value: " << device[0] << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to read register." << unlock;
        return false;
    }

    if (dev.bitsSet(reg2, uint16_t{12}) == I2C::Error::None) {
        if ((device[2] | (device[3] << 8)) == 12)
            loging << lock << MICROHAL_DEBUG << "Bits set correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Bits set incorrectly. Reg value: " << device[0] << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to set bits." << unlock;
        return false;
    }

    dev.write(reg2, uint16_t{0b1010101010101010});
    if ((device[2] | (device[3] << 8)) == 0b1010101010101010) loging << lock << MICROHAL_DEBUG << "data successfully written." << unlock;

    if (dev.bitsClear(reg2, uint16_t{0b00000010}) == I2C::Error::None) {
        if ((device[2] | (device[3] << 8)) == 0b1010101010101000)
            loging << lock << MICROHAL_DEBUG << "Bits clear correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Bits clear incorrectly. Reg value: " << device[0] << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to clear bits." << unlock;
        return false;
    }

    if (dev.bitsModify(reg2, uint8_t{0b01000010}, uint8_t{0b11000010}) == I2C::Error::None) {
        if ((device[2] | (device[3] << 8)) == 0b1010101001101010)
            loging << lock << MICROHAL_DEBUG << "Bits modified correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Bits modified incorrectly. Reg value: " << device[0] << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to modify bits." << unlock;
        return false;
    }

    std::array<uint16_t, 2> regs2;
    if (dev.readRegisters(regs2, reg2, reg3) == I2C::Error::None) {
        if ((device[2] | (device[3] << 8)) == regs2[0] && (device[4] | (device[5] << 8)) == regs2[1])
            loging << lock << MICROHAL_DEBUG << "Multiple register read correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Data mismatch while reading multiple registers." << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to read multiple registers." << unlock;
        return false;
    }

    regs[0] = 0xAA;
    regs[1] = 0xBB;
    if (dev.writeRegisters(regs2, reg2, reg3) == I2C::Error::None) {
        if ((device[2] | (device[3] << 8)) == regs2[0] && (device[4] | (device[5] << 8)) == regs2[1])
            loging << lock << MICROHAL_DEBUG << "Multiple register write correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Data mismatch while writing multiple registers." << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to write multiple registers." << unlock;
        return false;
    }

    //////////////////////////////////////////////////////////////////////////////////
    std::tuple<uint8_t, uint16_t> tupReg;
    if (dev.readRegisters(tupReg, reg1, reg2) == I2C::Error::None) {
        if (device[1] == std::get<0>(tupReg) && (device[2] | (device[3] << 8)) == std::get<1>(tupReg))
            loging << lock << MICROHAL_DEBUG << "Multiple register read correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Data mismatch while reading multiple registers." << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to read multiple registers." << unlock;
        return false;
    }

    std::get<0>(tupReg) = 0x85;
    std::get<1>(tupReg) = 0xAABB;
    if (dev.writeRegisters(tupReg, reg1, reg2) == I2C::Error::None) {
        if (device[1] == std::get<0>(tupReg) && (device[2] | (device[3] << 8)) == std::get<1>(tupReg))
            loging << lock << MICROHAL_DEBUG << "Multiple register write correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Data mismatch while writing multiple registers." << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to write multiple registers." << unlock;
        return false;
    }

    std::get<0>(tupReg) = 0x85;
    std::get<1>(tupReg) = 0xAABB;
    if (dev.writeRegisters(tupReg, reg1, reg2) == I2C::Error::None) {
        if (device[1] == std::get<0>(tupReg) && (device[2] | (device[3] << 8)) == std::get<1>(tupReg))
            loging << lock << MICROHAL_DEBUG << "Multiple register write correctly." << unlock;
        else {
            loging << lock << MICROHAL_DEBUG << "Data mismatch while writing multiple registers." << unlock;
            return false;
        }
    } else {
        loging << lock << MICROHAL_DEBUG << "Unable to write multiple registers." << unlock;
        return false;
    }

    return true;
}

int main(void) {
    debugPort.open(IODevice::ReadWrite);

    debugPort.write("\n\r------------------- I2C mock test  -------------------------\n\r");

    i2cmocked.addDevice(&device);

    test();
    if (deviceTest() == false) loging << lock << MICROHAL_DEBUG << "An error occurred." << unlock;

    return 0;
}
