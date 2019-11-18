/*
 * i2c_pulling_stm32f4xx.cpp
 *
 *  Created on: 17 kwi 2014
 *      Author: pawel
 */

#include "i2c_polling_stmCommon.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
/* ************************************************************************************************
 *                                   STATIC VARIABLES
 * ***********************************************************************************************/
#ifdef MICROHAL_USE_I2C1_POLLING
I2C_polling I2C_polling::i2c1(registers::i2c1);
I2C &I2C::i2c1 = I2C_polling::i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_POLLING
I2C_polling I2C_polling::i2c2(registers::i2c2);
I2C &I2C::i2c2 = I2C_polling::i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_POLLING
I2C_polling I2C_polling::i2c3(registers::i2c3);
I2C &I2C::i2c3 = I2C_polling::i2c3;
#endif
/* ************************************************************************************************
 *                                   FUNCTIONS IMPLEMENTATION
 * ***********************************************************************************************/
static I2C::Error sendStart(registers::I2C &i2c) {
    // Generate the Start condition
    auto cr1 = i2c.cr1.volatileLoad();
    cr1.START.set();
    i2c.cr1.volatileStore(cr1);
    // wait for start sent
    registers::I2C::SR1 status;
    do {
        status = i2c.sr1.volatileLoad();
        const I2C::Error error = I2C::errorCheckAndClear(&i2c, status);
        if (error != I2C::Error::None) return error;
    } while (!(status.SB));

    return I2C::Error::None;
}

static I2C::Error sendDeviceAddress(registers::I2C &i2c, uint8_t deviceAddress) {
    i2c.dr.volatileStore({deviceAddress});
    registers::I2C::SR1 status;
    do {
        status = i2c.sr1.volatileLoad();
        const I2C::Error error = I2C::errorCheckAndClear(&i2c, status);
        if (error != I2C::Error::None) return error;
    } while (!(status.ADDR));

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
    I2C::Error error = write_implementation(deviceAddress, write, write_size, write, 0);
    if (error == Error::None) {
        registers::I2C::SR1 status;
        do {
            status = i2c.sr1.volatileLoad();
            error = errorCheckAndClear(&i2c, status);
            if (error != Error::None) return error;
        } while (!(status.BTF));

        error = read(deviceAddress, (uint8_t *)data_read, read_size);
    }
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

    const I2C::Error error = write_implementation(deviceAddress, write, write_size, write, 0);
    if (error == Error::None) {
        registers::I2C::SR1 status;
        do {
            status = i2c.sr1.volatileLoad();
            const I2C::Error error = errorCheckAndClear(&i2c, status);
            if (error != Error::None) return error;
        } while (!(status.BTF));
    }
    // Send I2Cx STOP Condition
    auto cr1 = i2c.cr1.volatileLoad();
    cr1.STOP.set();
    i2c.cr1.volatileStore(cr1);

    return error;
}

I2C::Error I2C_polling::write(DeviceAddress deviceAddress, const uint8_t *write_data, size_t write_data_size, const uint8_t *write_dataB,
                              size_t write_data_sizeB) noexcept {
    const I2C::Error error = write_implementation(deviceAddress, write_data, write_data_size, write_dataB, write_data_sizeB);
    if (error == Error::None) {
        registers::I2C::SR1 status;
        do {
            status = i2c.sr1.volatileLoad();
            const I2C::Error error = errorCheckAndClear(&i2c, status);
            if (error != Error::None) return error;
        } while (!(status.BTF));
    }
    // Send I2Cx STOP Condition
    auto cr1 = i2c.cr1.volatileLoad();
    cr1.STOP.set();
    i2c.cr1.volatileStore(cr1);

    return error;
}

I2C::Error I2C_polling::write(uint8_t data) {
    Error error;
    registers::I2C::SR1 status;
    do {
        status = i2c.sr1.volatileLoad();
        error = errorCheckAndClear(&i2c, status);
        if (error != Error::None) return error;
    } while (!(status.TxE));
    i2c.dr.volatileStore(data);

    return Error::None;
}

I2C::Error I2C_polling::write_implementation(DeviceAddress deviceAddress, const void *write_data, size_t write_data_size, const void *write_dataB,
                                             size_t write_data_sizeB) {
    if (write_data_size == 0) return Error::None;

    I2C::Error error;

    // Generate the Start condition
    const I2C::Error startError = sendStart(i2c);
    if (startError != I2C::Error::None) return startError;
    // Send I2Cx slave Address for write
    const I2C::Error deviceAddressError = sendDeviceAddress(i2c, deviceAddress);
    if (deviceAddressError != I2C::Error::None) return startError;

    i2c.sr2.volatileLoad();  // do not delete, read sr2 for clear addr flag,

    const uint8_t *ptr = static_cast<const uint8_t *>(write_data);
    while (write_data_size--) {
        error = write(*ptr++);
        if (error != Error::None) return error;
    }

    const uint8_t *ptrB = static_cast<const uint8_t *>(write_dataB);
    while (write_data_sizeB--) {
        error = write(*ptrB++);
        if (error != Error::None) return error;
    }

    return Error::None;
}

I2C::Error I2C_polling::read_implementation(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB,
                                            size_t dataBLength) noexcept {
    // Generate the Start condition
    const I2C::Error restartError = sendStart(i2c);
    if (restartError != I2C::Error::None) return restartError;
    // Send I2Cx slave Address for read
    const I2C::Error deviceReadAddressError = sendDeviceAddress(i2c, deviceAddress + 1);
    if (deviceReadAddressError != I2C::Error::None) return deviceReadAddressError;

    i2c.sr2.volatileLoad();  // do not delete,read sr2 for clear addr flag

    I2C::Error error;
    // buffer A
    auto cr1 = i2c.cr1.volatileLoad();
    cr1.ACK.set();
    i2c.cr1.volatileStore(cr1);
    registers::I2C::SR1 status;
    for (size_t i = 0; i < dataLength; i++) {
        do {
            status = i2c.sr1.volatileLoad();
            error = errorCheckAndClear(&i2c, status);
            if (error != Error::None) return error;
        } while (!(status.RxNE));
        ((uint8_t *)data)[i] = i2c.dr.volatileLoad().DATA;
    }
    // buffer B
    size_t i = 0;
    if (dataBLength > 1) {
        for (; i < dataBLength - 1; i++) {
            do {
                status = i2c.sr1.volatileLoad();
                error = errorCheckAndClear(&i2c, status);
                if (error != Error::None) return error;
            } while (!(status.RxNE));
            ((uint8_t *)dataB)[i] = i2c.dr.volatileLoad().DATA;
        }
    }

    cr1.ACK.clear();
    // Generate the Stop condition
    cr1.STOP.set();
    i2c.cr1.volatileStore(cr1);

    // wait until one byte has been received
    do {
        status = i2c.sr1.volatileLoad();
        error = errorCheckAndClear(&i2c, status);
        if (error != Error::None) return error;
    } while (!(status.RxNE));
    // read data from I2C data register
    ((uint8_t *)dataB)[i] = i2c.dr.volatileLoad().DATA;
    return Error::None;
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
