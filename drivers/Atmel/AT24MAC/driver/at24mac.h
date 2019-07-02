/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-03-2019
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

#ifndef _AT24MAC_H_
#define _AT24MAC_H_

#include <charconv>
#include <gsl/span>
#include <optional>
#include <string_view>
#include "I2CDevice/I2CDevice.h"
#include "microhal.h"

/**
 * \addtogroup Devices
 * @{
 * @class AT24MAC
 * @}
 */
class AT24MAC {
 public:
    using I2C = microhal::I2C;
    enum class Error {
        AcknowledgeFailure = static_cast<int>(I2C::Error::AcknowledgeFailure),
        ArbitrationLost = static_cast<int>(I2C::Error::ArbitrationLost),
        Bus = static_cast<int>(I2C::Error::Bus),
        None = static_cast<int>(I2C::Error::None),
        Overrun = static_cast<int>(I2C::Error::Overrun),
        Timeout = static_cast<int>(I2C::Error::Timeout),
        Unknown = static_cast<int>(I2C::Error::Unknown),
        DataOverflow,
        Addres
    };

    struct SerialNumber {
        uint8_t serial[128 / 8];

        bool operator!=(SerialNumber b) { return std::equal(std::begin(serial), std::begin(serial) + sizeof(serial), std::begin(b.serial)); }
        std::string toString() const {
            std::string str;
            str.reserve(18);
            for (uint_fast8_t i = 0; i < sizeof(serial); i++) {
                std::array<char, 2> buff;
                auto [p, ec] = std::to_chars(buff.data(), buff.data() + buff.size(), serial[i], 16);
                str.append(std::string_view(buff.data(), p - str.data()));
                if (i + 1 != sizeof(serial)) {
                    str.append(":");
                }
            }
            return str;
        }
    };

    static constexpr const size_t pageSize = 16;
    static constexpr const size_t memorySizeInBytes = 256;

 private:
    using Endianness = microhal::Endianness;
    using Access = microhal::Access;
    using span = gsl::span<uint8_t>;  // Todo change to std::span when it will be available in gcc

    // create alias to microhal::Address, we just want to type less
    template <typename T, T i>
    using Address = microhal::Address<T, i>;

    struct Register {
        static constexpr auto SerialNumberReg =
            microhal::makeRegister<SerialNumber, Access::ReadOnly, Endianness::Little>(Address<uint8_t, 0b1000'0000>{});
        static constexpr auto EUIAddress = microhal::makeRegister<uint64_t, Access::ReadOnly, Endianness::Big>(Address<uint8_t, 0b1001'1000>{});
    };

    static constexpr uint8_t getMACi2cAddrFromMemoryI2cAddr(uint8_t memoryAddress) { return (memoryAddress & 0x0F) | 0xB0; }

 public:
    AT24MAC(I2C &i2c, uint8_t address) : memory(i2c, address), mac(i2c, getMACi2cAddrFromMemoryI2cAddr(address)) {}

    static std::string_view toString(Error error);

    Error readEUI(uint64_t &eui) { return static_cast<Error>(mac.readRegister(Register::EUIAddress, eui)); }
    Error readSerialNumber(SerialNumber &serial) { return static_cast<Error>(mac.readRegister(Register::SerialNumberReg, serial)); }

    // Memory access functions
    Error readByte(uint8_t address, uint8_t &data) { return static_cast<Error>(memory.read(address, data)); }
    Error read(uint8_t address, span data) { return static_cast<Error>(memory.read(address, data)); }

    Error writeByte(uint8_t address, uint8_t data) { return static_cast<Error>(memory.write(address, data)); }
    Error writePage(uint8_t pageAddress, span data) {
        if (data.size_bytes() > 16) return Error::DataOverflow;
        if ((pageAddress % 16) != 0) return Error::Addres;
        return static_cast<Error>(memory.write(pageAddress, data));
    }
    Error write(uint8_t address, span data) {
        if (data.size_bytes() > memorySizeInBytes) return Error::DataOverflow;

        uint_fast8_t bytesInFirstPage = pageSize - (address % pageSize);
        span firstPage(data.data(), bytesInFirstPage);
        writePage(address, firstPage);

        address += bytesInFirstPage;
        uint_fast8_t bytesToWrite = (data.size_bytes() - bytesInFirstPage);
        uint8_t *dataPtr = data.data() + bytesInFirstPage;
        while (bytesToWrite / pageSize) {
            span page(dataPtr, pageSize);
            writePage(address, page);
            address += pageSize;
            dataPtr += pageSize;
            bytesToWrite -= pageSize;
        }

        uint_fast8_t lastPageSize = bytesToWrite % pageSize;
        if (lastPageSize) {
            span page(dataPtr, lastPageSize);
            writePage(address, page);
        }
        return Error::None;
    }

    void writeWait() { std::this_thread::sleep_for(std::chrono::milliseconds{5}); }

 private:
    microhal::I2CDevice memory;
    microhal::I2CDevice mac;
};

#endif /* _AT24MAC_H_ */
