/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-01-2018
 * last modification: 30-01-2018
 *
 * @copyright Copyright (c) 2018, Pawel Okas
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

#ifndef _MICROHAL_MCP23017_H_
#define _MICROHAL_MCP23017_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "I2CDevice/I2CDevice.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"
/* **************************************************************************************************************************************************
 * CLASS
 */
class MCP23017 {
 public:
    using LogLevel = microhal::diagnostic::LogLevel;
    using Error = microhal::I2C::Error;
    using Pin = uint8_t;

 private:
    using Access = microhal::Access;
    using Endianness = microhal::Endianness;
    // create alias to microhal::Address, we just want to type less
    template <typename T, T i>
    using Address = microhal::Address<T, i>;

    static constexpr auto IODIR =
        microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Little>(Address<uint8_t, 0x00>{});  ///< I/O DIRECTION REGISTER (ADDR 0x00)
    static constexpr auto IPOL = microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Little>(
        Address<uint8_t, 0x02>{});  ///< INPUT POLARITY PORT REGISTER (ADDR 0x01)
    static constexpr auto GPINTEN =
        microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Little>(Address<uint8_t, 0x04>{});  ///< INTERRUPT-ON-CHANGE PINS (ADDR 0x02)
    static constexpr auto DEFVAL =
        microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Little>(Address<uint8_t, 0x06>{});  ///< DEFAULT VALUE REGISTER (ADDR 0x03)
    static constexpr auto INTCON = microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Little>(
        Address<uint8_t, 0x08>{});  ///< INTERRUPT-ON-CHANGE CONTROL REGISTER (ADDR 0x04)
    static constexpr auto IOCON = microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Little>(
        Address<uint8_t, 0x0A>{});  ///< I/O EXPANDER CONFIGURATION REGISTER (ADDR 0x05)
    static constexpr auto GPPU = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Little>(
        Address<uint8_t, 0x0C>{});  ///< GPIO PULL-UP RESISTOR REGISTER (ADDR 0x06)
    static constexpr auto INTF =
        microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Little>(Address<uint8_t, 0x0E>{});  ///< INTERRUPT FLAG REGISTER (ADDR 0x07)
    static constexpr auto INTCAP = microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Little>(
        Address<uint8_t, 0x10>{});  ///< INTERRUPT CAPTURED VALUE FOR PORT REGISTER (ADDR 0x08)
    static constexpr auto GPIO = microhal::makeRegister<uint16_t, Access::ReadWrite, Endianness::Little>(
        Address<uint8_t, 0x12>{});  ///< GENERAL PURPOSE I / O PORT REGISTER(ADDR 0x09)
    static constexpr auto OLAT =
        microhal::makeRegister<uint16_t, Access::ReadOnly, Endianness::Little>(Address<uint8_t, 0x14>{});  ///< OUTPUT LATCH REGISTER 0 (ADDR 0x0A)

 public:
    enum Address : uint8_t {
        Addr_0x40 = 0b01000000,
        Addr_0x42 = 0b01000010,
        Addr_0x44 = 0b01000100,
        Addr_0x46 = 0b01000110,
        Addr_0x48 = 0b01001000,
        Addr_0x4A = 0b01001010,
        Addr_0x4C = 0b01001100,
        Addr_0x4E = 0b01001110,
    };

    enum class PullConfiguration {
        PullUp = 1,  ///< Pin have pull-up enabled
        None = 0,    ///< Pin is floating
    };

    enum class Direction {
        Output = 0,  ///< Pin configured as output
        Input = 1    ///< Pin configured as input
    };

    MCP23017(microhal::I2C &i2c, uint8_t address) : i2c(i2c, address) {
        if (address > Addr_0x4E || address < Addr_0x40) {
            log << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "I2C address out of range." << microhal::diagnostic::unlock;
            std::terminate();  // critical error, provided I2C address out of range
        }
    }
    virtual ~MCP23017();

    Error setPort(uint16_t values) { return i2c.setBitsInRegister(GPIO, values); }
    Error resetPort(uint16_t values) { return i2c.clearBitsInRegister(GPIO, values); }
    Error getPort(uint16_t &values) { return i2c.readRegister(GPIO, values); }
    Error getPortDirection(uint16_t &dir) { return i2c.readRegister(IODIR, dir); }

    Error setPin(Pin pin) { return setPort(1 << pin); }
    Error resetPin(Pin pin) { return resetPort(1 << pin); }
    Error getPin(Pin pin, bool &state) {
        uint16_t tmp;
        auto status = getPort(tmp);
        state = tmp & (1 << pin) == (1 << pin);
        return status;
    }
    /** @brief Function for pin initialization, this function allow to configure only one pin if you want to configure multiple pins at single
     * function call
     * please use @ref congifurePort method.
     *
     * @param pinNumber from 0 to 15
     * @param direction desired pin direction @ref Direction
     * @param pull configuration of pull-up @ref PullConfiguration
     * @return
     */
    Error configurePin(Pin pinNumber, Direction direction, PullConfiguration pull) {
        if (pinNumber > pinCount - 1) return Error::Unknown;

        auto status = i2c.modifyBitsInRegister(IODIR, uint16_t{static_cast<uint16_t>(direction) << pinNumber}, 1 << pinNumber);
        if (status == Error::None) {
            status = i2c.modifyBitsInRegister(GPPU, uint16_t{static_cast<uint16_t>(pull) << pinNumber}, 1 << pinNumber);
        }

        return status;
    }
    /** @brief This function allow you to configure all pins of MCP23017 in single function call.
     *
     * Notice: All previous configuration will be overwritten by this function.
     *
     * @param direction - 16bit data representing pin direction, when bit is set to 1 then corresponding pin will be configured as output, when
     * bit is equal to 0 then corresponding pin will be configured as input; LSB is corresponding to PORTA.0, MSB is corresponding to PORTB.7. So
     * if you want to configure PORTA.1 and PORTB.0 as output you have to pass 0b0000'0001'0000'0010 as direction value. All other pins will be
     * configured as input.
     * @param pullUp - 16bit data representing pin pull-up state. If bit is set to 1 then corresponding pin will have enabled pull-up. Bits are
     * organized same as in direction parameter. Example: if you want to enable pull-up on PORTA.0 and PORTB.1 you have pass 0b0000'0010'0000'0001
     * @return
     */
    Error configurePort(uint16_t direction, uint16_t pullUp) {
        auto status = i2c.writeRegister(IODIR, direction);
        if (status == Error::None) {
            status = i2c.writeRegister(GPPU, pullUp);
        }

        return status;
    }

    Error getIOCon(uint16_t &val) { return i2c.readRegister(IOCON, val); }

    Error init() {
        static constexpr auto Config =
            microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x05>{});  ///< I/O DIRECTION REGISTER (ADDR 0x00)
        return i2c.writeRegister(Config, uint8_t{0b00000000});
    }

 private:
    microhal::I2CDevice i2c;
    microhal::diagnostic::Diagnostic<LogLevel::Disabled> log;
    static constexpr unsigned int pinCount = 16;
};

#endif  // _MICROHAL_MCP23017_H_
