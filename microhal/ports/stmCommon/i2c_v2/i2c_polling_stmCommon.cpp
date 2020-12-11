/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 20-02-2017
 * last modification: 24-02-2017
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

#include "i2c_polling_stmCommon.h"
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#if _MICROHAL_PORT_STM_I2C_DRIVER_VERSION == 2  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_I2C(POLLING)                    // Check if driver is enabled in microhal port config

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
/* ************************************************************************************************
 *                                   STATIC VARIABLES
 * ***********************************************************************************************/
#if MICROHAL_USE_I2C1_POLLING == 1
I2C_polling I2C_polling::i2c1(*registers::i2c1);
I2C &I2C::i2c1 = I2C_polling::i2c1;
#endif
#if MICROHAL_USE_I2C2_POLLING == 1
I2C_polling I2C_polling::i2c2(*registers::i2c2);
I2C &I2C::i2c2 = I2C_polling::i2c2;
#endif
#if MICROHAL_USE_I2C3_POLLING == 1
I2C_polling I2C_polling::i2c3(*registers::i2c3);
I2C &I2C::i2c3 = I2C_polling::i2c3;
#endif
/* ************************************************************************************************
 *                                   FUNCTIONS IMPLEMENTATION
 * ***********************************************************************************************/
static I2C::Error sendStart(registers::I2C &i2c) {
    // Generate the Start condition
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.START.set();
    i2c.cr2.volatileStore(cr2);
    // wait for start sent
    registers::I2C::ISR status;
    do {
        status = i2c.isr.volatileLoad();
        const I2C::Error error = I2C::errorCheckAndClear(&i2c, status);
        if (error != I2C::Error::None) return error;
    } while (i2c.cr2.volatileLoad().START);

    return I2C::Error::None;
}
/**
 *
 * @param deviceAddress address to I2C device connected to bus
 * @param registerAddress device address from data will be read
 * @param data pointer to buffer where data will be stored
 * @param length number of bytes to read
 *
 * @return Error::None if data was read successfully or error code if an error occurred, see @ref I2C::Error.
 */
I2C::Error I2C_polling::writeRead(DeviceAddress deviceAddress, const uint8_t *write, size_t write_size, uint8_t *data_read,
                                  size_t read_size) noexcept {
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.AUTOEND.clear();
    i2c.cr2.volatileStore(cr2);
    I2C::Error error = write_implementation(deviceAddress, write, write_size, nullptr, 0);
    if (error == Error::None) {
        registers::I2C::ISR status;
        do {
            status = i2c.isr.volatileLoad();
            error = errorCheckAndClear(&i2c, status);
            if (error != Error::None) return error;
        } while (!(status.transferComplete()));

        error = read_implementation(deviceAddress, data_read, read_size, nullptr, 0);
    }
    // waitForStopSend();
    return error;
}
/**
 *
 * @param deviceAddress address to I2C device connected to bus
 * @param registerAddress device register address where data will be stored
 * @param data pointer to data buffer
 * @param length number of bytes to write
 *
 * @return Error::None if data was send successfully or error code if an error occurred, see @ref I2C::Error.
 */
I2C::Error I2C_polling::write(DeviceAddress deviceAddress, const uint8_t *write, size_t write_size) noexcept {
    if (write_size == 0) return Error::None;
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.AUTOEND.set();
    i2c.cr2.volatileStore(cr2);
    Error error = write_implementation(deviceAddress, write, write_size, nullptr, 0);
    // waitForStopSend();
    return error;
}

I2C::Error I2C_polling::write(DeviceAddress deviceAddress, const uint8_t *write_data, size_t write_data_size, const uint8_t *write_dataB,
                              size_t write_data_sizeB) noexcept {
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.AUTOEND.set();
    i2c.cr2.volatileStore(cr2);
    Error error = write_implementation(deviceAddress, write_data, write_data_size, write_dataB, write_data_sizeB);
    // waitForStopSend();
    return error;
}

I2C::Error I2C_polling::write_implementation(DeviceAddress deviceAddress, const uint8_t *write_data, size_t write_data_size,
                                             const uint8_t *write_dataB, size_t write_data_sizeB) {
    size_t toRead = write_data_size + write_data_sizeB;
    auto cr2 = i2c.cr2.volatileLoad();
    // set new device addres, set number of bytes to transfer, set transfer direction to transmit
    cr2.SADD = deviceAddress;
    cr2.RD_WRN.clear();
    if (toRead > 255) {
        cr2.NBYTES = 255;
        cr2.RELOAD.set();
    } else {
        cr2.NBYTES = toRead;
        cr2.RELOAD.clear();
    }
    i2c.cr2.volatileStore(cr2);

    // Generate the Start condition
    if (I2C::Error error = sendStart(i2c); error != I2C::Error::None) return error;
    if (I2C::Error error = write(write_data, write_data_size, toRead); error != Error::None) return error;
    if (I2C::Error error = write(write_dataB, write_data_sizeB, toRead); error != Error::None) return error;
    return Error::None;
}

I2C::Error I2C_polling::read_implementation(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB,
                                            size_t dataBLength) noexcept {
    size_t toRead = dataLength + dataBLength;
    // set new device addres, set number of bytes to transfer, set transfer direction to transmit
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.SADD = deviceAddress;
    cr2.RD_WRN.set();
    cr2.AUTOEND.set();
    if (toRead > 255) {
        cr2.NBYTES = 255;
        cr2.RELOAD.set();
    } else {
        cr2.NBYTES = toRead;
        cr2.RELOAD.clear();
    }
    i2c.cr2.volatileStore(cr2);

    if (I2C::Error error = sendStart(i2c); error != I2C::Error::None) return error;
    if (I2C::Error error = readInto(data, dataLength, toRead); error != Error::None) return error;
    if (I2C::Error error = readInto(dataB, dataBLength, toRead); error != Error::None) return error;
    return Error::None;
}

I2C::Error I2C_polling::write(const uint8_t *data, size_t length, size_t &toRead) {
    registers::I2C::ISR status;
    for (size_t i = 0; i < length; i++) {
        do {
            status = i2c.isr.volatileLoad();
            Error error = errorCheckAndClear(&i2c, status);
            if (error != Error::None) return error;
        } while (!(status.TXIS));
        i2c.txdr.volatileStore(data[i]);
        toRead--;
        if (status.transferCompleteReload()) {
            auto cr2 = i2c.cr2.volatileLoad();
            if (toRead > 255) {
                cr2.NBYTES = 255;
                cr2.RELOAD.set();
            } else {
                cr2.NBYTES = toRead;
                cr2.RELOAD.clear();
            }
            i2c.cr2.volatileStore(cr2);
        }
    }
    return Error::None;
}

I2C::Error I2C_polling::readInto(uint8_t *data, size_t length, size_t &toRead) {
    registers::I2C::ISR status;
    for (size_t i = 0; i < length; i++) {
        do {
            status = i2c.isr.volatileLoad();
            I2C::Error error = errorCheckAndClear(&i2c, status);
            if (error != Error::None) return error;
        } while (!(status.receiveRegisterNotEmpty()));
        data[i] = (uint32_t)i2c.rxdr.volatileLoad();
        toRead--;
        if (status.transferCompleteReload()) {
            auto cr2 = i2c.cr2.volatileLoad();
            if (toRead > 255) {
                cr2.NBYTES = 255;
                cr2.RELOAD.set();
            } else {
                cr2.NBYTES = toRead;
                cr2.RELOAD.clear();
            }
            i2c.cr2.volatileStore(cr2);
        }
    }
    return Error::None;
}

void I2C_polling::waitForStopSend() {
    registers::I2C::ISR status;
    do {
        status = i2c.isr.volatileLoad();
        Error error = errorCheckAndClear(&i2c, status);
        // if (error != Error::None) return error;
    } while (!(status.stopDetected()));
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif
#endif
