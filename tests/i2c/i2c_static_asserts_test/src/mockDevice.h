/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 19-10-2017
 * last modification: 19-10-2017
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

#ifndef _MICROHAL_MOCKDEVICE_H_
#define _MICROHAL_MOCKDEVICE_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <algorithm>
#include <cstdint>
#include "microhal.h"

/* **************************************************************************************************************************************************
 * CLASS
 */
class Device {
 public:
    Device(microhal::I2C::DeviceAddress address) : i2cAddress(address) {}

    void write(const uint8_t *data, size_t size) {
        if (size == 0) return;
        address = data[0];
        if (size == 1) return;
        if (address < sizeof(data)) {
            auto toCopy = (size - 1 < sizeof(this->data)) ? size - 1 : sizeof(this->data) - address;
            std::copy_n(data + 1, toCopy, &this->data[address]);
            address += size - 1;
        }
    }

    void write(const uint8_t *dataA, size_t sizeA, const uint8_t *dataB, size_t sizeB) {
        write(dataA, sizeA);
        if (sizeB == 0) return;
        if (address < sizeof(data)) {
            auto toCopy = (address + sizeB < sizeof(this->data)) ? sizeB : sizeof(this->data) - address;
            std::copy_n(dataB, toCopy, &this->data[address]);
        }
        address += sizeB;
    }

    void read(uint8_t *data, size_t size) {
        if (size == 0) return;
        if (address < sizeof(this->data)) {
            auto toCopy = (address + size < sizeof(this->data)) ? size : sizeof(this->data) - address;
            std::copy_n(&this->data[address], toCopy, data);
            std::fill_n(&data[toCopy], size - toCopy, 0xFF);
            address += size;
        } else {
            std::fill_n(data, size, 0xFF);
        }
    }

    void writeRead(const uint8_t *write, size_t write_size, uint8_t *read, size_t read_size) {
        this->write(write, write_size);
        this->read(read, read_size);
    }

    uint8_t addr() const { return i2cAddress; }
    const auto operator[](size_t i) const {
        if (i > sizeof(data)) std::terminate();
        return data[i];
    }

    const auto dataPtr() const { return data; }

 private:
    uint8_t data[10];
    uint8_t address = 0;
    const microhal::I2C::DeviceAddress i2cAddress;
};

#endif  // _MICROHAL_MOCKDEVICE_H_
