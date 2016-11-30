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
/**
 * @brief This function write data to device connected to I2C bus.
 *
 * @param deviceAddress address to I2C device connected to bus
 * @param data to send
 *
 * @return NoError if data was send successfully or error code if an error
 * occurred, see @ref I2C::Error.
 */
I2C::Error I2C_polling::write(uint8_t deviceAddress, uint8_t data) {
  Error error;
  uint16_t status;
  // Generate the Start condition
  i2c.CR1 |= I2C_CR1_START;
  // wait for start sent
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_SB));
  // Send I2Cx slave Address for write
  i2c.DR = deviceAddress;
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_ADDR));

  __attribute__((unused)) volatile uint16_t tmp =
      i2c.SR2;  // do not delete, read sr2 for clear addr flag,

  error = write(data);
  if (error != NoError) return error;

  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_BTF));
  // Send I2Cx STOP Condition
  i2c.CR1 |= I2C_CR1_STOP;
  return NoError;
}
/**
 *
 * @param deviceAddress address to I2C device connected to bus
 * @param registerAddress device register address where data will be stored
 * @param data to write
 *
 * @return NoError if data was send successfully or error code if an error
 * occurred, see @ref I2C::Error.
 */
I2C::Error I2C_polling::write(uint8_t deviceAddress, uint8_t registerAddress,
                              uint8_t data) {
  Error error;
  uint16_t status;
  // Generate the Start condition
  i2c.CR1 |= I2C_CR1_START;
  // wait for start sent
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_SB));
  // Send I2Cx slave Address for write
  i2c.DR = deviceAddress;
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_ADDR));
  __attribute__((unused)) volatile uint16_t tmp =
      i2c.SR2;  // do not delete, read sr2 for clear addr flag,

  error = write(registerAddress);
  if (error != NoError) return error;
  error = write(data);
  if (error != NoError) return error;

  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_BTF));
  // Send I2Cx STOP Condition
  i2c.CR1 |= I2C_CR1_STOP;
  return NoError;
}
/**
 *
 * @param deviceAddress address to I2C device connected to bus
 * @param registerAddress device register address where data will be stored
 * @param data pointer to data buffer
 * @param length number of bytes to write
 *
 * @return NoError if data was send successfully or error code if an error
 * occurred, see @ref I2C::Error.
 */
I2C::Error I2C_polling::write(uint8_t deviceAddress, uint8_t registerAddress,
                              const void *data, size_t length) {
  if (length == 0) return NoError;
  if (length == 1) {
    return write(deviceAddress, registerAddress, *((uint8_t *)data));
  }
  I2C::Error error;
  uint16_t status;

  // Generate the Start condition
  i2c.CR1 |= I2C_CR1_START;
  // wait for start sent
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_SB));
  // Send I2Cx slave Address for write
  i2c.DR = deviceAddress;
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_ADDR));

  __attribute__((unused)) volatile uint16_t tmp =
      i2c.SR2;  // do not delete, read sr2 for clear addr flag,

  i2c.DR = registerAddress;
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_BTF));
  //    length--; TODO - why it was here?
  uint8_t *ptr = (uint8_t *)data;
  while (length--) {
    error = write(*ptr++);
    if (error != NoError) return error;
  }
  // write((uint8_t*)data);
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_BTF));
  // Send I2Cx STOP Condition
  i2c.CR1 |= I2C_CR1_STOP;
  return NoError;
}
/**
 *
 * @param deviceAddress address to I2C device connected to bus
 * @param data reference to memory where read data will be copy
 *
 * @return NoError if data was send successfully or error code if an error
 * occurred, see @ref I2C::Error.
 */
I2C::Error I2C_polling::read(uint8_t deviceAddress, uint8_t &data) {
  I2C::Error error;
  uint16_t status;

  // Generate the Start condition
  i2c.CR1 |= I2C_CR1_START;
  // wait for start sent
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_SB));
  i2c.DR = deviceAddress + 1;
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_ADDR));

  __attribute__((unused)) volatile uint16_t tmp =
      i2c.SR2;  // do not delete, read sr2 for clear addr flag,

  i2c.CR1 &= ~I2C_CR1_ACK;
  i2c.CR1 |= I2C_CR1_STOP;
  // wait until one byte has been received
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_RXNE));
  // read data from I2C data register
  data = i2c.DR;
  return NoError;
}
/**
 *
 * @param deviceAddress address to I2C device connected to bus
 * @param registerAddress device address from data will be read
 * @param data reference to memory where read data will be stored
 *
 * @return NoError if data was send successfully or error code if an error
 * occurred, see @ref I2C::Error.
 */
I2C::Error I2C_polling::read(uint8_t deviceAddress, uint8_t registerAddress,
                             uint8_t &data) {
  I2C::Error error;
  uint16_t status;
  // Generate the Start condition
  i2c.CR1 |= I2C_CR1_START;
  // wait for start sent
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_SB));

  // Send I2Cx slave Address for write
  i2c.DR = deviceAddress;
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_ADDR));

  __attribute__((unused)) volatile uint16_t tmp =
      i2c.SR2;  // do not delete, read sr2 for clear addr flag,
  i2c.DR = registerAddress;
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_BTF));
  return read(deviceAddress, data);
}
/**
 *
 * @param deviceAddress address to I2C device connected to bus
 * @param registerAddress device address from data will be read
 * @param data pointer to buffer where data will be stored
 * @param length number of bytes to read
 *
 * @return NoError if data was read successfully or error code if an error
 * occurred, see @ref I2C::Error.
 */
I2C::Error I2C_polling::read(uint8_t deviceAddress, uint8_t registerAddress,
                             void *data, size_t length) {
  I2C::Error error;
  volatile uint16_t status;

  // Generate the Start condition
  i2c.CR1 |= I2C_CR1_START;
  // wait for start sent
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_SB));
  // Send I2Cx slave Address for write
  i2c.DR = deviceAddress;
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_ADDR));
  __attribute__((unused)) volatile uint16_t tmp =
      i2c.SR2;  // do not delete, read sr2 for clear addr flag,
  // write register address
  i2c.DR = registerAddress;
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_BTF));
  // Generate the Start condition
  i2c.CR1 |= I2C_CR1_START;
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_SB));
  // Send I2Cx slave Address for write
  i2c.DR = deviceAddress + 1;
  do {
    status = i2c.SR1;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_SR1_ADDR));

  tmp = i2c.SR2;  // do not delete,read sr2 for clear addr flag

  size_t i = 0;
  if (length > 1) {
    i2c.CR1 |= I2C_CR1_ACK;

    for (; i < length - 1; i++) {
      do {
        status = i2c.SR1;
        error = errorCheckAndClear(&i2c, status);
        if (error != NoError) return error;
      } while (!(status & I2C_SR1_RXNE));
      ((uint8_t *)data)[i] = i2c.DR;
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
  ((uint8_t *)data)[i] = i2c.DR;
  return NoError;
}

}  // namespace stm32f4xx
}  // namespace microhal
