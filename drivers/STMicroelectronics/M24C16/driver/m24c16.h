/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      M24C16 memory driver
 *
 * @authors    Pawel Okas
 * created on: 7-06-2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 	   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * 	      documentation and/or other materials provided with the distribution.
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

#ifndef M24C16_H_
#define M24C16_H_

#include <cstdint>
#include <experimental/optional>

#include "I2CDevice/I2CDevice.h"
#include "gsl/gsl"
#include "microhal.h"

class M24C16 {
    using Endianness = microhal::Endianness;
    using Error = microhal::I2C::Error;
    using Access = microhal::Access;

 public:
    /**
    * @brief This enum contain I2C address
    */
    enum : microhal::I2C::DeviceAddress { I2CAddress = 0b1010'000'0 };
    static constexpr size_t pageSize = 16;
    static constexpr size_t memorySize = 2048;  // in Bytes
    static constexpr size_t beginAddress = 0;
    static constexpr size_t endAddress = memorySize;

    M24C16(microhal::I2C &i2c) : i2c(i2c) {}
    virtual ~M24C16();

    bool write(size_t address, const gsl::not_null<const void *> data, size_t data_size);

    bool read(size_t address, gsl::not_null<void *> data, size_t data_size) {
        Error error = sequentealRandomRead(address, data, data_size);
        if (error == Error::None) {
            return true;
        }
        return false;
    }

 private:
    microhal::I2C &i2c;
    // reading functions
    Error currentAddressRead(uint8_t &data) { return i2c.read(I2CAddress, &data, 1); }

    Error randomAddressRead(size_t address, gsl::not_null<void *> data, size_t data_size) {
        uint8_t lowAddress = getAddrLowByte(address);
        return i2c.writeRead(I2CAddress | getAddrHighByte(address), &lowAddress, sizeof(lowAddress), static_cast<uint8_t *>(data.get()), data_size);
    }

    Error sequentialCurrentRead(gsl::not_null<void *> data, size_t data_size) { return i2c.read(I2CAddress, (uint8_t *)data.get(), data_size); }

    Error sequentealRandomRead(size_t address, gsl::not_null<void *> data, size_t data_size) {
        uint8_t lowAddress = getAddrLowByte(address);
        return i2c.writeRead(I2CAddress | getAddrHighByte(address), &lowAddress, sizeof(lowAddress), static_cast<uint8_t *>(data.get()), data_size);
    }
    // writing functions
    Error byteWrite(size_t address, uint8_t data) {
        uint8_t tmp[] = {getAddrLowByte(address), data};
        return i2c.write(I2CAddress | getAddrHighByte(address), tmp, sizeof(tmp));
    }

    Error pageWrite(size_t address, gsl::not_null<const void *> data, size_t data_size) {
        uint8_t lowAddr = getAddrLowByte(address);
        return i2c.write(I2CAddress | getAddrHighByte(address), &lowAddr, sizeof(lowAddr), static_cast<const uint8_t *>(data.get()), data_size);
    }

    static constexpr uint8_t getAddrHighByte(size_t address) { return (address >> 8) & 0x0E; }

    static constexpr uint8_t getAddrLowByte(size_t address) { return address & 0xFF; }
};

#endif /* M24C16_H_ */
