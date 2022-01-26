/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      I2C interface
 *
 * @authors    Pawel Okas
 * created on: 02-04-2017
 * last modification: 02-04-2017
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

#ifndef _MICROHAL_I2C_LINUX_H_
#define _MICROHAL_I2C_LINUX_H_

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "interfaces/i2c_interface.h"

namespace microhal {
namespace linux {

class I2C : public microhal::I2C {
 public:
    I2C(const char *filename) {
        file = ::open(filename, O_RDWR);

        if (file < 0) {
            std::cout << "unable to open I2C interface.";
            std::abort();
        }
    }
    ~I2C() { close(file); }

    bool speed(Speed speed, Mode mode) noexcept final { std::terminate(); }
    Speed speed() noexcept final { std::abort(); }
    void busReset() noexcept final { std::abort(); }

    Error write(DeviceAddress deviceAddress, const uint8_t *data, size_t length) noexcept final {
        if (::ioctl(file, I2C_SLAVE, deviceAddress >> 1) < 0) {
            return Error::Unknown;
        }
        auto writeStatus = ::write(file, data, length);
        if (writeStatus >= 0) {
            // we know that writeStatus is positive so it is safe to cast it into unsigned data
            if (static_cast<size_t>(writeStatus) == length) return Error::None;
        } else {
        }
        return Error::Unknown;
    }
    Error write(DeviceAddress deviceAddress, const uint8_t *data, size_t dataLength, const uint8_t *dataB, size_t dataBLength) noexcept final {
        if (::ioctl(file, I2C_SLAVE, deviceAddress >> 1) < 0) return Error::Unknown;
        uint8_t *buffer = new uint8_t[dataLength + dataBLength];
        std::copy_n(data, dataLength, buffer);
        std::copy_n(dataB, dataBLength, buffer + dataLength);
        auto writeStatus = ::write(file, buffer, dataLength + dataBLength);
        if (writeStatus >= 0) {
            // we know that writeStatus is positive so it is safe to cast it into unsigned data
            if (static_cast<size_t>(writeStatus) == dataLength + dataBLength) return Error::None;
        } else {
        }
        return Error::Unknown;
    }
    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t size) noexcept final {
        if (::ioctl(file, I2C_SLAVE, deviceAddress >> 1) < 0) return Error::Unknown;
        auto readStatus = ::read(file, data, size);
        if (readStatus >= 0) {
            // we know that writeStatus is positive so it is safe to cast it into unsigned data
            if (static_cast<size_t>(readStatus) == size) return Error::None;
        } else {
        }
        return Error::Unknown;
    }
    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept final;
    Error writeRead(DeviceAddress deviceAddress, const uint8_t *write, size_t writeLength, uint8_t *read, size_t readLength) noexcept final;

    bool addSlave(microhal::I2CSlave &) final { std::terminate(); }

 private:
    int file{};
};

}  // namespace linux
}  // namespace microhal

#endif  // _MICROHAL_I2C_LINUX_H_
