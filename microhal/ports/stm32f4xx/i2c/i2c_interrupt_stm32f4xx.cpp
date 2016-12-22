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

I2C::Error I2C_interrupt::write(uint8_t deviceAddress, const uint8_t *data, size_t length) {
    transfer.deviceAddress = deviceAddress;
    transfer.bufferA.ptr = const_cast<uint8_t*>(data);
    transfer.bufferA.length = length;
    transfer.mode = Transmit;

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
I2C::Error I2C_interrupt::write(DeviceAddress deviceAddress, const void *write_data, size_t write_data_size, const void *write_dataB,
               size_t write_data_sizeB) noexcept {
    transfer.deviceAddress = deviceAddress;
    transfer.bufferA.ptr = (uint8_t*)const_cast<void*>(write_data);
    transfer.bufferA.length = write_data_size;
    transfer.bufferB.ptr = (uint8_t*)const_cast<void*>(write_dataB);
    transfer.bufferB.length = write_data_sizeB;
    transfer.mode = TransmitDoubleBuffer;

    ErrorSemaphore = UnknownError;
    // send start
    while (i2c.SR2 & I2C_SR2_BUSY) {
    }
    i2c.CR2 |= I2C_CR2_ITBUFEN;
    i2c.CR1 |= I2C_CR1_START;

    while (ErrorSemaphore == UnknownError) {
    }

    return ErrorSemaphore;
};
I2C::Error I2C_interrupt::read(uint8_t deviceAddress, uint8_t *data, size_t length) {
    transfer.deviceAddress = deviceAddress;
    transfer.bufferA.ptr = data;
    transfer.bufferA.length = length;
    transfer.mode = Receive;

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

I2C::Error I2C_interrupt::read(uint8_t deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept {
    transfer.deviceAddress = deviceAddress;
    transfer.bufferA.ptr = data;
    transfer.bufferA.length = dataLength;
    transfer.bufferB.ptr = dataB;
    transfer.bufferB.length = dataBLength;
    transfer.mode = ReceiveDoubleBuffer;

    i2c.CR2 &= ~I2C_CR2_ITBUFEN;

    ErrorSemaphore = UnknownError;
    // send start
    while (i2c.SR2 & I2C_SR2_BUSY) {
    }
    i2c.CR1 |= I2C_CR1_START;

    while (ErrorSemaphore == UnknownError) {
    }

    return ErrorSemaphore;
};

I2C::Error I2C_interrupt::writeRead(DeviceAddress address, const void *write, size_t write_size, void *read, size_t read_size) noexcept {
    transfer.deviceAddress = address;
    transfer.bufferA.ptr = const_cast<uint8_t*>(static_cast<const uint8_t*>(write));
    transfer.bufferA.length = write_size;
    transfer.bufferB.ptr = static_cast<uint8_t*>(read);
    transfer.bufferB.length = read_size;
    transfer.mode = WriteRead;

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

    if (status1 == I2C_SR1_SB) {
    	// start sequence was sent, now we need to send device address
        i2c->DR = obj.transfer.deviceAddress | (obj.transfer.mode & 0x01); // set last bit of addres depending on active mode
    } else if (status1 & I2C_SR1_ADDR) {
    	// address was sent, now we are working in master mode
        __attribute__((unused)) volatile uint16_t tmp = i2c->SR2;  // to clear interrupt flag register SR2 read is necessarily

        if (obj.transfer.mode != I2C_interrupt::Receive) {
        	i2c->DR = *obj.transfer.bufferA.ptr;
        	obj.transfer.bufferA.ptr++;
        	obj.transfer.bufferA.length--;
        }

        if (obj.transfer.mode == I2C_interrupt::WriteRead) {
        	obj.transfer.bufferA = obj.transfer.bufferB;
            obj.transfer.mode = I2C_interrupt::Receive;

            if (obj.transfer.bufferA.length == 1) {
                i2c->CR1 = (i2c->CR1 & (~I2C_CR1_ACK & ~I2C_CR1_POS)) | I2C_CR1_START;
                i2c->CR2 |= I2C_CR2_ITBUFEN;
            } else if (obj.transfer.bufferA.length == 2) {
                i2c->CR1 = (i2c->CR1 & ~I2C_CR1_ACK) | I2C_CR1_POS | I2C_CR1_START;
            } else {
                i2c->CR2 |= I2C_CR2_ITBUFEN;
                i2c->CR1 |= I2C_CR1_ACK /* | I2C_CR1_POS */ | I2C_CR1_START;
            }
        } else {
            if (obj.transfer.mode != I2C_interrupt::Receive /*|| obj.transfer.length != 2*/) i2c->CR2 |= I2C_CR2_ITBUFEN;
        }
    } else {
        if (obj.transfer.mode == I2C_interrupt::Receive || obj.transfer.mode == I2C_interrupt::ReceiveDoubleBuffer) {
            if (status1 == (I2C_SR1_RXNE | I2C_SR1_BTF)) {
            	// here we are receiving last two bytes
                i2c->CR1 |= I2C_CR1_STOP;

                if (obj.transfer.mode == I2C_interrupt::Receive) {
                	*obj.transfer.bufferA.ptr++ = i2c->DR;
                	*obj.transfer.bufferA.ptr = i2c->DR;
                } else {
                	// we are in double buffer mode
                	if (obj.transfer.bufferA.length == 0) {
                		// this means that both bytes have to go to buffer B
                		*obj.transfer.bufferB.ptr++ = i2c->DR;
                		*obj.transfer.bufferB.ptr = i2c->DR;
                	} else {
                		// first byte have to go to buffer A, and second to buffer B
                		*obj.transfer.bufferA.ptr = i2c->DR;
                		*obj.transfer.bufferB.ptr = i2c->DR;
                	}
                }

                obj.ErrorSemaphore = I2C_interrupt::NoError;
            } else if (status1 & I2C_SR1_RXNE) {
            //	auto& activeBuffer = obj.transfer.bufferA.length != 0 ? obj.transfer.bufferA : obj.transfer.bufferB;

            	if (obj.transfer.mode == I2C_interrupt::Receive) {
					if (obj.transfer.bufferA.length == 1) {
						i2c->CR1 |= I2C_CR1_STOP;
						//*obj.transfer.buffer_ptr = i2c->DR;
						i2c->CR2 &= ~I2C_CR2_ITBUFEN;
						obj.ErrorSemaphore = I2C_interrupt::NoError;
					} else {
						obj.transfer.bufferA.length--;
						if (obj.transfer.bufferA.length == 2) {
							i2c->CR1 = (i2c->CR1 & ~I2C_CR1_ACK) | I2C_CR1_POS;
							i2c->CR2 &= ~I2C_CR2_ITBUFEN;
						}
					}
					*obj.transfer.bufferA.ptr = i2c->DR;
					obj.transfer.bufferA.ptr++;
            	} else {
            		obj.transfer.bufferA.length--;
            		if (obj.transfer.bufferA.length == 1 && obj.transfer.bufferB.length == 1) {
            			i2c->CR1 = (i2c->CR1 & ~I2C_CR1_ACK) | I2C_CR1_POS;
            			i2c->CR2 &= ~I2C_CR2_ITBUFEN;
            		} else if (obj.transfer.bufferA.length == 0) {
            			if (obj.transfer.bufferB.length == 2) {
            				i2c->CR1 = (i2c->CR1 & ~I2C_CR1_ACK) | I2C_CR1_POS;
            				i2c->CR2 &= ~I2C_CR2_ITBUFEN;
            			}
            			*obj.transfer.bufferA.ptr = i2c->DR;
            			//obj.transfer.bufferA.ptr++;

            			obj.transfer.bufferA = obj.transfer.bufferB;
            			obj.transfer.mode = I2C_interrupt::Receive;
            			return;
            		}
            		*obj.transfer.bufferA.ptr = i2c->DR;
            		obj.transfer.bufferA.ptr++;
            	}
            }
        } else {
        	if (obj.transfer.mode == I2C_interrupt::Transmit) {
				if (status1 == I2C_SR1_TXE) {
					if (obj.transfer.bufferA.length) {
						i2c->DR = *obj.transfer.bufferA.ptr++;
						obj.transfer.bufferA.length--;
					} else {
						i2c->CR1 |= I2C_CR1_STOP;
						i2c->CR2 &= ~I2C_CR2_ITBUFEN;
						obj.ErrorSemaphore = I2C_interrupt::NoError;
					}
				} else {
					i2c->DR = *obj.transfer.bufferA.ptr++;
					obj.transfer.bufferA.length--;
					if (obj.transfer.bufferA.length == 0) {
						obj.transfer.bufferA = obj.transfer.bufferB;
					}
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
