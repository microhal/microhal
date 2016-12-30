/*
 * i2c_pulling_stm32f4xx.cpp
 *
 *  Created on: 17 kwi 2014
 *      Author: pawel
 */

#include "i2c_polling_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 *                                   STATIC VARIABLES
 * ***********************************************************************************************/
#ifdef MICROHAL_USE_I2C1_POLLING
I2C_polling I2C_polling::i2c1(*I2C1);
I2C &I2C::i2c1 = I2C_polling::i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_POLLING
I2C_polling I2C_polling::i2c2(*I2C2);
I2C &I2C::i2c2 = I2C_polling::i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_POLLING
I2C_polling I2C_polling::i2c3(*I2C3);
I2C &I2C::i2c3 = I2C_polling::i2c3;
#endif
/* ************************************************************************************************
 *                                   FUNCTIONS IMPLEMENTATION
 * ***********************************************************************************************/
static I2C::Error sendStart(I2C_TypeDef &i2c) {
    // Generate the Start condition
    i2c.CR1 |= I2C_CR1_START;
    // wait for start sent
    uint16_t status;
    do {
        status = i2c.SR1;
        const I2C::Error error = I2C::errorCheckAndClear(&i2c, status);
        if (error != I2C::NoError) return error;
    } while (!(status & I2C_SR1_SB));

    return I2C::NoError;
}

static I2C::Error sendDeviceAddress(I2C_TypeDef &i2c, uint8_t deviceAddress) {
    i2c.DR = deviceAddress;
    uint16_t status;
    do {
        status = i2c.SR1;
        const I2C::Error error = I2C::errorCheckAndClear(&i2c, status);
        if (error != I2C::NoError) return error;
    } while (!(status & I2C_SR1_ADDR));

    return I2C::NoError;
}
/**
 *
 * @param deviceAddress address to I2C device connected to bus
 * @param registerAddress device address from data will be read
 * @param data pointer to buffer where data will be stored
 * @param length number of bytes to read
 *
 * @return NoError if data was read successfully or error code if an error occurred, see @ref I2C::Error.
 */
I2C::Error I2C_polling::writeRead(DeviceAddress deviceAddress, const uint8_t *write, size_t write_size, uint8_t *data_read, size_t read_size) noexcept {
    I2C::Error error = write_implementation(deviceAddress, write, write_size, write, 0);
    if (error == NoError) {
        volatile uint16_t status;
        do {
            status = i2c.SR1;
            error = errorCheckAndClear(&i2c, status);
            if (error != NoError) return error;
        } while (!(status & I2C_SR1_BTF));

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
 * @return NoError if data was send successfully or error code if an error occurred, see @ref I2C::Error.
 */
I2C::Error I2C_polling::write(DeviceAddress deviceAddress, const uint8_t *write, size_t write_size) noexcept {
    if (write_size == 0) return NoError;

    const I2C::Error error = write_implementation(deviceAddress, write, write_size, write, 0);
    if (error == NoError) {
        uint16_t status;
        do {
            status = i2c.SR1;
            const I2C::Error error = errorCheckAndClear(&i2c, status);
            if (error != NoError) return error;
        } while (!(status & I2C_SR1_BTF));
    }
    // Send I2Cx STOP Condition
    i2c.CR1 |= I2C_CR1_STOP;

    return error;
}

I2C::Error I2C_polling::write(DeviceAddress deviceAddress, const uint8_t *write_data, size_t write_data_size, const uint8_t *write_dataB,
                              size_t write_data_sizeB) noexcept {
    const I2C::Error error = write_implementation(deviceAddress, write_data, write_data_size, write_dataB, write_data_sizeB);
    if (error == NoError) {
        uint16_t status;
        do {
            status = i2c.SR1;
            const I2C::Error error = errorCheckAndClear(&i2c, status);
            if (error != NoError) return error;
        } while (!(status & I2C_SR1_BTF));
    }
    // Send I2Cx STOP Condition
    i2c.CR1 |= I2C_CR1_STOP;

    return error;
}

I2C::Error I2C_polling::write(uint8_t data) {
    Error error;
    uint16_t status;

    do {
        status = i2c.SR1;
        error = errorCheckAndClear(&i2c, status);
        if (error != NoError) return error;
    } while (!(status & I2C_SR1_TXE));
    i2c.DR = data;

    return NoError;
}

I2C::Error I2C_polling::write_implementation(DeviceAddress deviceAddress, const void *write_data, size_t write_data_size, const void *write_dataB,
                                             size_t write_data_sizeB) {
    if (write_data_size == 0) return NoError;

    I2C::Error error;

    // Generate the Start condition
    const I2C::Error startError = sendStart(i2c);
    if (startError != I2C::NoError) return startError;
    // Send I2Cx slave Address for write
    const I2C::Error deviceAddressError = sendDeviceAddress(i2c, deviceAddress);
    if (deviceAddressError != I2C::NoError) return startError;

    __attribute__((unused)) volatile uint16_t tmp = i2c.SR2;  // do not delete, read sr2 for clear addr flag,

    const uint8_t *ptr = static_cast<const uint8_t *>(write_data);
    while (write_data_size--) {
        error = write(*ptr++);
        if (error != NoError) return error;
    }

    const uint8_t *ptrB = static_cast<const uint8_t *>(write_dataB);
    while (write_data_sizeB--) {
        error = write(*ptrB++);
        if (error != NoError) return error;
    }

    return NoError;
}

I2C::Error I2C_polling::read_implementation(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept {
    // Generate the Start condition
    const I2C::Error restartError = sendStart(i2c);
    if (restartError != I2C::NoError) return restartError;
    // Send I2Cx slave Address for read
    const I2C::Error deviceReadAddressError = sendDeviceAddress(i2c, deviceAddress + 1);
    if (deviceReadAddressError != I2C::NoError) return deviceReadAddressError;

    __attribute__((unused)) volatile uint16_t tmp = i2c.SR2;  // do not delete,read sr2 for clear addr flag

    I2C::Error error;
    volatile uint16_t status;

    // buffer A
    i2c.CR1 |= I2C_CR1_ACK;

    for (size_t i = 0; i < dataLength; i++) {
    	do {
    		status = i2c.SR1;
    		error = errorCheckAndClear(&i2c, status);
    		if (error != NoError) return error;
    	} while (!(status & I2C_SR1_RXNE));
    	((uint8_t *)data)[i] = i2c.DR;
    }
    // buffer B
    size_t i = 0;
    if (dataBLength > 1) {
    	for (; i < dataBLength - 1; i++) {
            do {
                status = i2c.SR1;
                error = errorCheckAndClear(&i2c, status);
                if (error != NoError) return error;
            } while (!(status & I2C_SR1_RXNE));
            ((uint8_t *)dataB)[i] = i2c.DR;
        }
    }

    i2c.CR1 &= ~I2C_CR1_ACK;
    // Generate the Stop condition
    i2c.CR1 |= I2C_CR1_STOP;
    // wait until one byte has been received
    do {
        status = i2c.SR1;
        error = errorCheckAndClear(&i2c, status);
        if (error != NoError) return error;
    } while (!(status & I2C_SR1_RXNE));
    // read data from I2C data register
    ((uint8_t *)dataB)[i] = i2c.DR;
    return NoError;
};

}  // namespace stm32f4xx
}  // namespace microhal
