/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 17-09-2016
 * last modification: 17-09-2016
 *
 * @copyright Copyright (c) 2016, microHAL
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "i2c_polling_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {
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
  i2c.CR2 |= I2C_CR2_START;
  // wait for start sent
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (i2c.CR2 & I2C_CR2_START);
  // Send I2Cx slave Address for write
  i2c.TXDR = deviceAddress;
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_ADDR));

  i2c.ICR |= I2C_ICR_ADDRCF;
//  __attribute__((unused)) volatile uint16_t tmp =
//      i2c.SR2;  // do not delete, read sr2 for clear addr flag,

  error = write(data);
  if (error != NoError) return error;

  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_TC));
  // Send I2Cx STOP Condition
  i2c.CR2 |= I2C_CR2_STOP;
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
  i2c.CR2 |= I2C_CR2_START;
  // wait for start sent
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (i2c.CR2 & I2C_CR2_START);
  // Send I2Cx slave Address for write
  i2c.TXDR = deviceAddress;
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_ADDR));

  i2c.ICR |= I2C_ICR_ADDRCF;
//  __attribute__((unused)) volatile uint16_t tmp =
//      i2c.SR2;  // do not delete, read sr2 for clear addr flag,

  error = write(registerAddress);
  if (error != NoError) return error;
  error = write(data);
  if (error != NoError) return error;

  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_TC));
  // Send I2Cx STOP Condition
  i2c.CR2 |= I2C_CR2_STOP;
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
  i2c.CR2 |= I2C_CR2_START;
  // wait for start sent
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (i2c.CR2 & I2C_CR2_START);
  // Send I2Cx slave Address for write
  i2c.TXDR = deviceAddress;
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_ADDR));

  i2c.ICR |= I2C_ICR_ADDRCF;
//  __attribute__((unused)) volatile uint16_t tmp =
//      i2c.SR2;  // do not delete, read sr2 for clear addr flag,

  i2c.TXDR = registerAddress;
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_TC));
  //    length--; TODO - why it was here?
  uint8_t *ptr = (uint8_t *)data;
  while (length--) {
    error = write(*ptr++);
    if (error != NoError) return error;
  }
  // write((uint8_t*)data);
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_TC));
  // Send I2Cx STOP Condition
  i2c.CR2 |= I2C_CR2_STOP;
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
  i2c.CR2 |= I2C_CR2_START;
  // wait for start sent
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (i2c.CR2 & I2C_CR2_START);
  i2c.TXDR = deviceAddress + 1;
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_ADDR));

  i2c.ICR |= I2C_ICR_ADDRCF;
//  __attribute__((unused)) volatile uint16_t tmp =
//      i2c.SR2;  // do not delete, read sr2 for clear addr flag,

  i2c.CR2 &= ~I2C_CR2_NACK;
  i2c.CR2 |= I2C_CR2_STOP;
  // wait until one byte has been received
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_RXNE));
  // read data from I2C data register
  data = i2c.RXDR;
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
  i2c.CR2 |= I2C_CR2_START;
  // wait for start sent
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (i2c.CR2 & I2C_CR2_START);

  // Send I2Cx slave Address for write
  i2c.TXDR = deviceAddress;
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_ADDR));

  i2c.ICR |= I2C_ICR_ADDRCF;
//  __attribute__((unused)) volatile uint16_t tmp =
//      i2c.SR2;  // do not delete, read sr2 for clear addr flag,
  i2c.TXDR = registerAddress;
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_TC));
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
  i2c.CR2 |= I2C_CR2_START;
  // wait for start sent
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (i2c.CR2 & I2C_CR2_START);
  // Send I2Cx slave Address for write
  i2c.TXDR = deviceAddress;
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_ADDR));

  i2c.ICR |= I2C_ICR_ADDRCF;
//  __attribute__((unused)) volatile uint16_t tmp =
//      i2c.SR2;  // do not delete, read sr2 for clear addr flag,
  // write register address
  i2c.TXDR = registerAddress;
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_TC));
  // Generate the Start condition
  i2c.CR2 |= I2C_CR2_START;
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (i2c.CR2 & I2C_CR2_START);
  // Send I2Cx slave Address for write
  i2c.TXDR = deviceAddress + 1;
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_ADDR));

  i2c.ICR |= I2C_ICR_ADDRCF;
//  tmp = i2c.SR2;  // do not delete,read sr2 for clear addr flag

  size_t i = 0;
  if (length > 1) {
    i2c.CR2 |= I2C_CR2_NACK;

    for (; i < length - 1; i++) {
      do {
        status = i2c.ISR;
        error = errorCheckAndClear(&i2c, status);
        if (error != NoError) return error;
      } while (!(status & I2C_ISR_RXNE));
      ((uint8_t *)data)[i] = i2c.RXDR;
    }
  }

  i2c.CR2 &= ~I2C_CR2_NACK;
  // Generate the Stop condition
  i2c.CR2 |= I2C_CR2_STOP;
  // wait until one byte has been received
  do {
    status = i2c.ISR;
    error = errorCheckAndClear(&i2c, status);
    if (error != NoError) return error;
  } while (!(status & I2C_ISR_RXNE));
  // read data from I2C data register
  ((uint8_t *)data)[i] = i2c.RXDR;
  return NoError;
}

}  // namespace stm32l4xx
}  // namespace microhal


