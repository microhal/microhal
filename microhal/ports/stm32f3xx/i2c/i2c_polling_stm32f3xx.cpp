/*
 * i2c_pulling_stm32f4xx.cpp
 *
 *  Created on: 17 kwi 2014
 *      Author: pawel
 */

#include "i2c_polling_stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {
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
    i2c.CR2 |= I2C_CR2_START;
    // wait for start sent
    uint16_t status;
    do {
        status = i2c.ISR;
        const I2C::Error error = I2C::errorCheckAndClear(&i2c, status);
        if (error != I2C::NoError) return error;
    } while (i2c.CR2 & I2C_CR2_START);

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
            status = i2c.ISR;
            error = errorCheckAndClear(&i2c, status);
            if (error != NoError) return error;
        } while (!(status & I2C_ISR_TC));

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
            status = i2c.ISR;
            const I2C::Error error = errorCheckAndClear(&i2c, status);
            if (error != NoError) return error;
        } while (!(status & I2C_ISR_TC));
    }
    // Send I2Cx STOP Condition
    i2c.CR2 |= I2C_CR2_STOP;

    return error;
}

I2C::Error I2C_polling::write(DeviceAddress deviceAddress, const uint8_t *write_data, size_t write_data_size, const uint8_t *write_dataB,
                              size_t write_data_sizeB) noexcept {
    const I2C::Error error = write_implementation(deviceAddress, write_data, write_data_size, write_dataB, write_data_sizeB);
    if (error == NoError) {
        uint16_t status;
        do {
            status = i2c.ISR;
            const I2C::Error error = errorCheckAndClear(&i2c, status);
            if (error != NoError) return error;
        } while (!(status & I2C_ISR_TC));
    }
    // Send I2Cx STOP Condition
    i2c.CR2 |= I2C_CR2_STOP;

    return error;
}

I2C::Error I2C_polling::write(uint8_t data) {
    Error error;
    uint16_t status;

    do {
        status = i2c.ISR;
        error = errorCheckAndClear(&i2c, status);
        if (error != NoError) return error;
    } while (!(status & I2C_ISR_TXE));
    i2c.TXDR = data;

    return NoError;
}

I2C::Error I2C_polling::write_implementation(DeviceAddress deviceAddress, const void *write_data, size_t write_data_size, const void *write_dataB,
                                             size_t write_data_sizeB) {
    if (write_data_size == 0) return NoError;

    I2C::Error error;

    uint32_t cr2 = i2c.CR2;
    // clear device address and number of bytes
    cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk & ~I2C_CR2_RD_WRN;
    // set new device addres, set number of bytes to transfer, set transfer direction to transmit
    cr2 |= deviceAddress | ((write_data_size + write_data_sizeB) << I2C_CR2_NBYTES_Pos);
    i2c.CR2 = cr2;

    // Generate the Start condition
    const I2C::Error startError = sendStart(i2c);
    if (startError != I2C::NoError) return startError;

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
    uint32_t cr2 = i2c.CR2;
    // clear device address and number of bytes
    cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk;
    // set new device addres, set number of bytes to transfer, set transfer direction to transmit
    cr2 |= deviceAddress | I2C_CR2_RD_WRN | ((dataLength + dataBLength) << I2C_CR2_NBYTES_Pos);
    i2c.CR2 = cr2;

	// Generate the Start condition
    const I2C::Error restartError = sendStart(i2c);
    if (restartError != I2C::NoError) return restartError;

    I2C::Error error;
    volatile uint16_t status;

    for (size_t i = 0; i < dataLength; i++) {
    	do {
    		status = i2c.ISR;
    		error = errorCheckAndClear(&i2c, status);
    		if (error != NoError) return error;
    	} while (!(status & I2C_ISR_RXNE));
    	((uint8_t *)data)[i] = i2c.RXDR;
    }
    // buffer B
    size_t i = 0;
    if (dataBLength > 1) {
    	for (; i < dataBLength - 1; i++) {
            do {
                status = i2c.ISR;
                error = errorCheckAndClear(&i2c, status);
                if (error != NoError) return error;
            } while (!(status & I2C_ISR_RXNE));
            ((uint8_t *)dataB)[i] = i2c.RXDR;
        }
    }

    // Generate the Stop condition
    i2c.CR2 |= I2C_CR2_STOP;
    // wait until one byte has been received
    do {
        status = i2c.ISR;
        error = errorCheckAndClear(&i2c, status);
        if (error != NoError) return error;
    } while (!(status & I2C_ISR_RXNE));
    // read data from I2C data register
    ((uint8_t *)dataB)[i] = i2c.RXDR;
    return NoError;
};

}  // namespace stm32f3xx
}  // namespace microhal
