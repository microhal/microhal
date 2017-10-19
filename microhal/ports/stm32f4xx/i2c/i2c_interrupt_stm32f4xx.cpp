/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 17-04-2014
 * last modification: 29-06-2016
 *
 * @copyright Copyright (c) 2014-2016, Pawel Okas
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
/* ************************************************************************************************
 *                                            FUNCTIONS
 * ***********************************************************************************************/
I2C::Error I2C_interrupt::write(uint8_t deviceAddress, const uint8_t *data, size_t length) noexcept {
    transfer.deviceAddress = deviceAddress;
    transfer.bufferA.ptr = const_cast<uint8_t *>(data);
    transfer.bufferA.length = length;
    transfer.mode = Mode::Transmit;

    // send start
    while (i2c.SR2 & I2C_SR2_BUSY) {
    }
    i2c.CR2 |= I2C_CR2_ITBUFEN;
    i2c.CR1 |= I2C_CR1_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}
I2C::Error I2C_interrupt::write(DeviceAddress deviceAddress, const uint8_t *write_data, size_t write_data_size, const uint8_t *write_dataB,
                                size_t write_data_sizeB) noexcept {
    transfer.deviceAddress = deviceAddress;
    transfer.bufferA.ptr = const_cast<uint8_t *>(write_data);
    transfer.bufferA.length = write_data_size;
    transfer.bufferB.ptr = const_cast<uint8_t *>(write_dataB);
    transfer.bufferB.length = write_data_sizeB;
    transfer.mode = Mode::TransmitDoubleBuffer;

    // send start
    while (i2c.SR2 & I2C_SR2_BUSY) {
    }
    i2c.CR2 |= I2C_CR2_ITBUFEN;
    i2c.CR1 |= I2C_CR1_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
};
I2C::Error I2C_interrupt::read(uint8_t deviceAddress, uint8_t *data, size_t length) noexcept {
    transfer.deviceAddress = deviceAddress;
    transfer.bufferA.ptr = data;
    transfer.bufferA.length = length;
    transfer.mode = Mode::Receive;

    i2c.CR2 &= ~I2C_CR2_ITBUFEN;

    // send start
    while (i2c.SR2 & I2C_SR2_BUSY) {
    }
    i2c.CR1 |= I2C_CR1_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}

I2C::Error I2C_interrupt::read(uint8_t deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept {
    transfer.deviceAddress = deviceAddress;
    transfer.bufferA.ptr = data;
    transfer.bufferA.length = dataLength;
    transfer.bufferB.ptr = dataB;
    transfer.bufferB.length = dataBLength;
    transfer.mode = Mode::ReceiveDoubleBuffer;

    i2c.CR2 &= ~I2C_CR2_ITBUFEN;
    // send start
    while (i2c.SR2 & I2C_SR2_BUSY) {
    }
    i2c.CR1 |= I2C_CR1_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
};

I2C::Error I2C_interrupt::writeRead(DeviceAddress address, const uint8_t *write, size_t write_size, uint8_t *read, size_t read_size) noexcept {
    transfer.deviceAddress = address;
    transfer.bufferA.ptr = const_cast<uint8_t *>(write);
    transfer.bufferA.length = write_size;
    transfer.bufferB.ptr = read;
    transfer.bufferB.length = read_size;
    transfer.mode = Mode::TransmitReceive;

    i2c.CR2 &= ~I2C_CR2_ITBUFEN;
    while (i2c.SR2 & I2C_SR2_BUSY) {
    }
    // send start
    i2c.CR1 |= I2C_CR1_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}
//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void I2C_interrupt::IRQFunction(I2C_interrupt &obj, I2C_TypeDef *i2c) {
    using Mode = I2C_interrupt::Mode;

    uint16_t status1 = i2c->SR1;

    if (status1 == I2C_SR1_SB) {
        // start sequence was sent, now we need to send device address
        i2c->DR = obj.transfer.deviceAddress | (static_cast<uint8_t>(obj.transfer.mode) & 0x01);  // set last bit of addres depending on active mode
    } else if (status1 & I2C_SR1_ADDR) {
        // address was sent, now we are working in master mode
        __attribute__((unused)) volatile uint16_t tmp = i2c->SR2;  // to clear interrupt flag register SR2 read is necessarily

        uint32_t restart = 0;
        if (obj.transfer.mode == Mode::Transmit || obj.transfer.mode == Mode::TransmitReceive || obj.transfer.mode == Mode::TransmitDoubleBuffer) {
            i2c->DR = *obj.transfer.bufferA.ptr;
            obj.transfer.bufferA.ptr++;
            obj.transfer.bufferA.length--;

            if (obj.transfer.bufferA.length == 0 && obj.transfer.mode == Mode::TransmitReceive) {
                obj.transfer.bufferA = obj.transfer.bufferB;
                obj.transfer.mode = Mode::Receive;
                i2c->CR1 |= I2C_CR1_START;
                return;
            }
        }

        if (obj.transfer.mode == Mode::Receive || obj.transfer.mode == Mode::ReceiveDoubleBuffer) {
            auto toReceive =
                obj.transfer.mode == Mode::Receive ? obj.transfer.bufferA.length : obj.transfer.bufferA.length + obj.transfer.bufferB.length;

            if (toReceive == 1) {
                // i2c->CR2 &= ~I2C_CR2_ITBUFEN;
                i2c->CR2 |= I2C_CR2_ITBUFEN;
                i2c->CR1 = (i2c->CR1 & (~I2C_CR1_ACK & ~I2C_CR1_POS)) | restart;
            } else if (toReceive == 2) {
                i2c->CR2 &= ~I2C_CR2_ITBUFEN;
                i2c->CR1 = (i2c->CR1 & ~I2C_CR1_ACK) | I2C_CR1_POS | restart;
            } else {
                i2c->CR2 |= I2C_CR2_ITBUFEN;
                i2c->CR1 |= I2C_CR1_ACK /* | I2C_CR1_POS */ | restart;
            }
        } else {
            if (obj.transfer.mode != Mode::Receive || obj.transfer.bufferA.length >= 2) i2c->CR2 |= I2C_CR2_ITBUFEN;
        }
    } else {
        if (status1 == (I2C_SR1_RXNE | I2C_SR1_BTF)) {
            // here we are receiving last two bytes
            i2c->CR1 |= I2C_CR1_STOP;
            auto toReceive =
                obj.transfer.mode == Mode::Receive ? obj.transfer.bufferA.length : obj.transfer.bufferA.length + obj.transfer.bufferB.length;
            if (toReceive == 1) {
                *obj.transfer.bufferA.ptr = i2c->DR;
            } else if (obj.transfer.mode == Mode::Receive) {
                *obj.transfer.bufferA.ptr++ = i2c->DR;
                *obj.transfer.bufferA.ptr = i2c->DR;
            } else {
                *obj.transfer.bufferA.ptr = i2c->DR;
                *obj.transfer.bufferB.ptr = i2c->DR;
            }
            auto shouldYeld = obj.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
            portYIELD_FROM_ISR(shouldYeld);
#else
            (void)shouldYeld;
#endif
        } else if (status1 & I2C_SR1_RXNE) {
            auto toReceive =
                obj.transfer.mode == Mode::Receive ? obj.transfer.bufferA.length : obj.transfer.bufferA.length + obj.transfer.bufferB.length;
            if (toReceive > 3) {
                *obj.transfer.bufferA.ptr = i2c->DR;
                obj.transfer.bufferA.ptr++;
                obj.transfer.bufferA.length--;
                if (obj.transfer.bufferA.length == 0) {
                    // we have to be in double buffer mode
                    obj.transfer.bufferA = obj.transfer.bufferB;
                    obj.transfer.mode = Mode::Receive;
                }
            } else if (toReceive == 3) {
                *obj.transfer.bufferA.ptr = i2c->DR;
                obj.transfer.bufferA.ptr++;
                obj.transfer.bufferA.length--;
                if (obj.transfer.bufferA.length == 0) {
                    // we have to be in double buffer mode
                    obj.transfer.bufferA = obj.transfer.bufferB;
                    obj.transfer.mode = Mode::Receive;
                }
                i2c->CR2 &= ~I2C_CR2_ITBUFEN;
                i2c->CR1 = (i2c->CR1 & ~I2C_CR1_ACK) | I2C_CR1_POS;
            } else if (toReceive == 1) {
                i2c->CR1 |= I2C_CR1_STOP;
                *obj.transfer.bufferA.ptr = i2c->DR;
                auto shouldYeld = obj.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
                portYIELD_FROM_ISR(shouldYeld);
#else
                (void)shouldYeld;
#endif
            }
        } else if (status1 == I2C_SR1_TXE) {
            if (obj.transfer.bufferA.length) {
                i2c->DR = *obj.transfer.bufferA.ptr++;
                obj.transfer.bufferA.length--;
            } else {
                if (obj.transfer.mode == Mode::TransmitDoubleBuffer) {
                    obj.transfer.bufferA = obj.transfer.bufferB;
                    obj.transfer.mode = Mode::Transmit;
                } else if (obj.transfer.mode == Mode::TransmitReceive) {
                    obj.transfer.bufferA = obj.transfer.bufferB;
                    obj.transfer.mode = Mode::Receive;

                    if (obj.transfer.bufferA.length == 1) {
                        i2c->CR2 &= ~I2C_CR2_ITBUFEN;
                        i2c->CR1 = (i2c->CR1 & (~I2C_CR1_ACK & ~I2C_CR1_POS)) | I2C_CR1_START;
                    } else if (obj.transfer.bufferA.length == 2) {
                        i2c->CR2 &= ~I2C_CR2_ITBUFEN;
                        i2c->CR1 = I2C_CR1_ACK | I2C_CR1_POS | I2C_CR1_START;
                    } else {
                        i2c->CR1 |= I2C_CR1_ACK | I2C_CR1_START;
                    }
                } else {
                    i2c->CR1 |= I2C_CR1_STOP;
                    i2c->CR2 &= ~I2C_CR2_ITBUFEN;
                    auto shouldYeld = obj.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
                    portYIELD_FROM_ISR(shouldYeld);
#else
                    (void)shouldYeld;
#endif
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
    I2C_interrupt::i2c1.error = I2C::errorCheckAndClear(I2C1, I2C1->SR1);
    auto shouldYeld = I2C_interrupt::i2c1.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#else
    (void)shouldYeld;
#endif
}
#endif
#ifdef MICROHAL_USE_I2C2_INTERRUPT
void I2C2_ER_IRQHandler(void) {
    I2C2->CR1 |= I2C_CR1_STOP;
    I2C_interrupt::i2c2.error = I2C::errorCheckAndClear(I2C2, I2C2->SR1);
    auto shouldYeld = I2C_interrupt::i2c2.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#else
    (void)shouldYeld;
#endif
}
#endif
#ifdef MICROHAL_USE_I2C3_INTERRUPT
void I2C3_ER_IRQHandler(void) {
    I2C3->CR1 |= I2C_CR1_STOP;
    I2C_interrupt::i2c3.error = I2C::errorCheckAndClear(I2C3, I2C3->SR1);
    auto shouldYeld = I2C_interrupt::i2c3.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#else
    (void)shouldYeld;
#endif
}
#endif

}  // namespace stm32f4xx
}  // namespace microhal
