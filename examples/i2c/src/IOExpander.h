/*
 * IOExpander.h
 *
 *  Created on: Sep 25, 2018
 *      Author: pokas
 */

#ifndef _IOEXPANDER_H_
#define _IOEXPANDER_H_

#include <cstdint>

#include "I2CDevice/I2CDevice.h"
#include "microhal.h"

class IOExpander {
 public:
    using Error = microhal::I2C::Error;

 private:
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    // create alias to microhal::Address, we just want to type less
    template <typename T, T i>
    using Address = microhal::Address<T, i>;

    struct Register {
        static constexpr auto DIRECTION = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x00>{});
        static constexpr auto PULLUP = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x01>{});
        static constexpr auto VALUE = microhal::makeRegister<uint8_t, Access::ReadWrite>(Address<uint8_t, 0x02>{});
    };

 public:
    IOExpander(microhal::I2C &i2c) : i2c(i2c, 0b001010000) {}

    // if bit is set then GPIO will be configured as output
    Error setGPIODirection(uint8_t direction) { return i2c.writeRegister(Register::DIRECTION, direction); }
    // if bit is set then pull up will be enabled
    Error setGPIOPullup(uint8_t pullup) { return i2c.writeRegister(Register::PULLUP, pullup); }
    // value have effect only when GPIO is configured as output, in other case value is ignored
    Error setGPIOState(uint8_t value) { return i2c.writeRegister(Register::VALUE, value); }
    // read GPIO state
    Error getGPIOStateInto(uint8_t &value) { return i2c.readRegister(Register::VALUE, value); }

 private:
    microhal::I2CDevice i2c;
};

#endif /* _IOEXPANDER_H_ */
