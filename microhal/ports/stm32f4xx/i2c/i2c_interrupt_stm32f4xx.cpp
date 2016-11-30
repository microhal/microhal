/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 17-04-2014
 * last modification: 27-06-2016
 *
 * @copyright Copyright (c) 2016, Pawel Okas
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

#include "i2c_interrupt_stm32f4xx.h"
#include "microhalPortConfig_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 *                                   STATIC VARIABLES
 * ***********************************************************************************************/
#ifdef MICROHAL_USE_I2C1_INTERRUPT
I2C_interrupt I2C_interrupt::i2c1(*I2C1);
I2C &I2C::i2c1 = I2C_interrupt::i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_INTERRUPT
I2C_interrupt I2C_interrupt::i2c2(*I2C2);
I2C &I2C::i2c2 = I2C_interrupt::i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_INTERRUPT
I2C_interrupt I2C_interrupt::i2c3(*I2C3);
I2C &I2C::i2c3 = I2C_interrupt::i2c3;
#endif

I2C::Error I2C_interrupt::write(uint8_t deviceAddress, uint8_t data) {
    transfer.deviceAddress = deviceAddress;
    transfer.registerAddress = data;
    transfer.length = 0;
    transfer.mode = TRANSMIT;

    ErrorSemaphore = UnknownError;
    while (i2c.SR2 & I2C_SR2_BUSY) {
    }
    // send start
    i2c.CR1 |= I2C_CR1_START;

    while (ErrorSemaphore == UnknownError) {
    }

    return ErrorSemaphore;
}
I2C::Error I2C_interrupt::write(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data) {
    return write(deviceAddress, registerAddress, &data, 1);
}
I2C::Error I2C_interrupt::write(uint8_t deviceAddress, uint8_t registerAddress, const void *data, size_t length) {
    transfer.deviceAddress = deviceAddress;
    transfer.registerAddress = registerAddress;
    transfer.buffer_ptr = const_cast<uint8_t *>(static_cast<const uint8_t *>(data));
    transfer.length = length;
    transfer.mode = TRANSMIT;

    ErrorSemaphore = UnknownError;
    // send start
    while (i2c.SR2 & I2C_SR2_BUSY) {
    }
    i2c.CR2 |= I2C_CR2_ITBUFEN;
    i2c.CR1 |= I2C_CR1_START;

    while (ErrorSemaphore == UnknownError) {
    }

    return ErrorSemaphore;
}
I2C::Error I2C_interrupt::read(uint8_t deviceAddress, uint8_t &data) {
    transfer.deviceAddress = deviceAddress;
    transfer.buffer_ptr = &data;
    transfer.length = 1;
    transfer.mode = RECEIVE;

    i2c.CR2 &= ~I2C_CR2_ITBUFEN;

    ErrorSemaphore = UnknownError;
    // send start
    while (i2c.SR2 & I2C_SR2_BUSY) {
    }
    i2c.CR1 |= I2C_CR1_START;

    while (ErrorSemaphore == UnknownError) {
    }

    return ErrorSemaphore;
}
I2C::Error I2C_interrupt::read(uint8_t deviceAddress, uint8_t registerAddress, uint8_t &data) {
    return read(deviceAddress, registerAddress, &data, 1);
}
I2C::Error I2C_interrupt::read(uint8_t deviceAddress, uint8_t registerAddress, void *data, size_t length) {
    transfer.deviceAddress = deviceAddress;
    transfer.registerAddress = registerAddress;
    transfer.buffer_ptr = static_cast<uint8_t *>(data);
    transfer.length = length;
    transfer.mode = RECEIVE_FROM_REGISTER;

    i2c.CR2 &= ~I2C_CR2_ITBUFEN;
    ErrorSemaphore = UnknownError;
    while (i2c.SR2 & I2C_SR2_BUSY) {
    }
    // send start
    i2c.CR1 |= I2C_CR1_START;

    while (ErrorSemaphore == UnknownError) {
    }

    return ErrorSemaphore;
}
//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void I2C_interrupt::IRQFunction(I2C_interrupt &obj, I2C_TypeDef *i2c) {
    uint16_t status1 = i2c->SR1;

    if (status1 == I2C_SR1_SB) {  // sent start sequence
        i2c->DR = obj.transfer.deviceAddress | (obj.transfer.mode & 0x01);
    } else if (status1 & I2C_SR1_ADDR) {                           // adres(MASTER MODE) was sent
        __attribute__((unused)) volatile uint16_t tmp = i2c->SR2;  // to clear interrupt flag register SR2 read is necessarily

        if (obj.transfer.mode != I2C_interrupt::RECEIVE) i2c->DR = obj.transfer.registerAddress;

        if (obj.transfer.mode == I2C_interrupt::RECEIVE_FROM_REGISTER) {
            obj.transfer.mode = I2C_interrupt::RECEIVE;

            if (obj.transfer.length == 1) {
                i2c->CR1 = (i2c->CR1 & (~I2C_CR1_ACK & ~I2C_CR1_POS)) | I2C_CR1_START;
                i2c->CR2 |= I2C_CR2_ITBUFEN;
            } else if (obj.transfer.length == 2) {
                i2c->CR1 = (i2c->CR1 & ~I2C_CR1_ACK) | I2C_CR1_POS | I2C_CR1_START;
            } else {
                i2c->CR2 |= I2C_CR2_ITBUFEN;
                i2c->CR1 |= I2C_CR1_ACK /* | I2C_CR1_POS */ | I2C_CR1_START;
            }
        } else {
            if (obj.transfer.mode != I2C_interrupt::RECEIVE /*|| obj.transfer.length != 2*/) i2c->CR2 |= I2C_CR2_ITBUFEN;
        }
    } else {
        if (obj.transfer.mode == I2C_interrupt::RECEIVE) {
            if (status1 == (I2C_SR1_RXNE | I2C_SR1_BTF)) {
                i2c->CR1 |= I2C_CR1_STOP;

                *obj.transfer.buffer_ptr++ = i2c->DR;
                *obj.transfer.buffer_ptr = i2c->DR;

                obj.ErrorSemaphore = I2C_interrupt::NoError;
            } else if (status1 & I2C_SR1_RXNE) {
                if (obj.transfer.length == 1) {
                    i2c->CR1 |= I2C_CR1_STOP;
                    *obj.transfer.buffer_ptr = i2c->DR;
                    i2c->CR2 &= ~I2C_CR2_ITBUFEN;
                    obj.ErrorSemaphore = I2C_interrupt::NoError;
                } else {
                    obj.transfer.length--;

                    if (obj.transfer.length == 2) {
                        i2c->CR1 = (i2c->CR1 & ~I2C_CR1_ACK) | I2C_CR1_POS;
                        i2c->CR2 &= ~I2C_CR2_ITBUFEN;
                    }

                    *obj.transfer.buffer_ptr++ = i2c->DR;
                }
            }
        } else {
            if (status1 == I2C_SR1_TXE) {
                if (obj.transfer.length) {
                    i2c->DR = *obj.transfer.buffer_ptr++;
                    obj.transfer.length--;
                } else {
                    i2c->CR1 |= I2C_CR1_STOP;
                    i2c->CR2 &= ~I2C_CR2_ITBUFEN;
                    obj.ErrorSemaphore = I2C_interrupt::NoError;
                }
            }
        }
    }
}
//***********************************************************************************************//
//                                          IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_I2C1_INTERRUPT
void I2C1_EV_IRQHandler(void) {
    I2C_interrupt::IRQFunction(I2C_interrupt::i2c1, I2C1);
}
#endif
#ifdef MICROHAL_USE_I2C2_INTERRUPT
void I2C2_EV_IRQHandler(void) {
    I2C_interrupt::IRQFunction(I2C_interrupt::i2c2, I2C2);
}
#endif
#ifdef MICROHAL_USE_I2C3_INTERRUPT
void I2C3_EV_IRQHandler(void) {
    I2C_interrupt::IRQFunction(I2C_interrupt::i2c3, I2C3);
}
#endif
//***********************************************************************************************//
//                                         error IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_I2C1_INTERRUPT
void I2C1_ER_IRQHandler(void) {
    I2C1->CR1 |= I2C_CR1_STOP;
    I2C_interrupt::i2c1.ErrorSemaphore = I2C::errorCheckAndClear(I2C1, I2C1->SR1);
}
#endif
#ifdef MICROHAL_USE_I2C2_INTERRUPT
void I2C2_ER_IRQHandler(void) {
    I2C2->CR1 |= I2C_CR1_STOP;
    I2C_interrupt::i2c2.ErrorSemaphore = I2C::errorCheckAndClear(I2C2, I2C2->SR1);
}
#endif
#ifdef MICROHAL_USE_I2C3_INTERRUPT
void I2C3_ER_IRQHandler(void) {
    I2C3->CR1 |= I2C_CR1_STOP;
    I2C_interrupt::i2c3.ErrorSemaphore = I2C::errorCheckAndClear(I2C3, I2C3->SR1);
}
#endif

}  // namespace stm32f4xx
}  // namespace microhal
