/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      I2C device driver
 *
 * @authors    Pawel Okas
 * created on: 22-03-2015
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015-2016, microHAL
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

#include "I2CDevice.h"

namespace microhal {
/**
 * @brief This function read few registers from I2C device.
 *
 * @param[in] registerAddress - address of first register to read.
 * @param[out] buffer - pointer to buffer where read data will be stored.
 * @param[in] size - number of registers to read
 * @param[in] endianness - byte order in I2C device
 *
 * @retval true if data was read
 * @retval false if an error occurred.
 */
bool I2CDevice::readRegisters(uint8_t registerAddress, uint16_t *buffer, size_t size, Endianness endianness) {
    return readRegisters_impl(registerAddress, buffer, size, endianness);
}
/**
 * @brief This function read few registers from I2C device.
 *
 * @param[in] registerAddress - address of first register to read.
 * @param[out] buffer - pointer to buffer where read data will be stored.
 * @param[in] size - number of registers to read
 * @param[in] endianness - byte order in I2C device
 *
 * @retval true if data was read
 * @retval false if an error occurred.
 */
bool I2CDevice::readRegisters(uint8_t registerAddress, uint32_t *buffer, size_t size, Endianness endianness) {
    return readRegisters_impl(registerAddress, buffer, size, endianness);
}
/**
 * @brief This function set bits in 16bit unsigned register.
 *
 * @param[in] registerAddress - address of register.
 * @param[in] bitMask - if 1 then corresponding bit will be set.
 * @param[in] endianness - byte order in I2C device.
 *
 * @retval true if bits was set.
 * @retval false if an error occurred.
 */
template <typename T>
bool I2CDevice::setBitsInRegister_impl(uint8_t registerAddress, T bitMask, Endianness endianness) {
    if (endianness != DeviceEndianness) {
        bitMask = byteswap(bitMask);
    }

    i2c.lock();

    T tmp;
    I2C::Error status = i2c.read(deviceAddress, registerAddress, &tmp, sizeof(tmp));
    if (status == I2C::NoError) {
        tmp |= bitMask;
        status = i2c.write(deviceAddress, registerAddress, &tmp, sizeof(tmp));
    }

    i2c.unlock();
    if (status == I2C::NoError) {
        return true;
    } else {
        lastError = status;
        return false;
    }
}

bool I2CDevice::setBitsInRegister(uint8_t registerAddress, uint16_t bitMask, Endianness endianness) {
    return setBitsInRegister(registerAddress, bitMask, endianness);
}

bool I2CDevice::setBitsInRegister(uint8_t registerAddress, uint32_t bitMask, Endianness endianness) {
    return setBitsInRegister(registerAddress, bitMask, endianness);
}

/**
 * @brief This function clear bits in 16 bit unsigned register.
 *
 * @param[in] registerAddress - address of register.
 * @param[in] bitMask - if 1 then corresponding bit will be cleared.
 * @param[in] endianness - byte order in I2C device
 *
 * @retval true if bits was cleared succesfull.
 * @retval false if an error occurred.
 */
template <typename T>
bool I2CDevice::clearBitsInRegister_impl(uint8_t registerAddress, T bitMask, Endianness endianness) {
    if (endianness != DeviceEndianness) {
        bitMask = ~byteswap(bitMask);
    }

    i2c.lock();

    T tmp;
    I2C::Error status = i2c.read(deviceAddress, registerAddress, &tmp, sizeof(tmp));
    if (status == I2C::NoError) {
        tmp &= bitMask;
        status = i2c.write(deviceAddress, registerAddress, &tmp, sizeof(tmp));
    }

    i2c.unlock();
    if (status == I2C::NoError) {
        return true;
    } else {
        lastError = status;
        return false;
    }
}

bool I2CDevice::clearBitsInRegister(uint8_t registerAddress, uint16_t bitMask, Endianness endianness) {
    return clearBitsInRegister_impl(registerAddress, bitMask, endianness);
}

bool I2CDevice::clearBitsInRegister(uint8_t registerAddress, uint32_t bitMask, Endianness endianness) {
    return clearBitsInRegister_impl(registerAddress, bitMask, endianness);
}

/**
 * @brief This function set bits in 16bit unsigned register.
 *
 * @param[in] address - address of register.
 * @param[in] bitMask - if 1 then corresponding bit will be set.
 * @param[in] endianness - byte order in I2C device.
 *
 * @retval true if bits was set.
 * @retval false if an error occurred.
 */
template <typename T>
bool I2CDevice::writeRegisterWithMask_impl(uint8_t address, T data, T mask, Endianness endianness) {
    if (endianness != DeviceEndianness) {
        mask = byteswap(mask);
        data = byteswap(data);
    }

    i2c.lock();

    T tmp;
    I2C::Error status = i2c.read(deviceAddress, address, &tmp, sizeof(tmp));
    if (status == I2C::NoError) {
        // clear bits
        tmp &= ~mask;
        // set new value
        tmp |= (data & mask);
        status = i2c.write(deviceAddress, address, &tmp, sizeof(tmp));
    }

    i2c.unlock();
    if (status == I2C::NoError) {
        return true;
    } else {
        lastError = status;
        return false;
    }
}

bool I2CDevice::writeRegisterWithMask(uint8_t address, uint16_t data, uint16_t mask, Endianness endianness) {
    return writeRegisterWithMask_impl(address, data, mask, endianness);
}

bool I2CDevice::writeRegisterWithMask(uint8_t address, uint32_t data, uint32_t mask, Endianness endianness) {
    return writeRegisterWithMask_impl(address, data, mask, endianness);
}

}  // namespace microhal
