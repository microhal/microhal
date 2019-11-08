/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 21-02-2017
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

#include <ports/stmCommon/i2c_v2/i2c_interrupt_stmCommon.h>

#include "microhalPortConfig_stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {
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
I2C::Error I2C_interrupt::write(uint8_t deviceAddress, const uint8_t *data, size_t size) noexcept {
    uint32_t cr2 = i2c.CR2;
    // clear device address and number of bytes and read flag
    cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk & ~I2C_CR2_RD_WRN;
    // set new device address, set number of bytes to transfer, set transfer direction to transmit
    if (size > 255) {
        cr2 |= deviceAddress | (255 << I2C_CR2_NBYTES_Pos) | I2C_CR2_RELOAD;
        size -= 255;
    } else {
        cr2 |= deviceAddress | (size << I2C_CR2_NBYTES_Pos);
        size = 0;
    }

    transfer.bufferA.ptr = const_cast<uint8_t *>(data);
    transfer.bufferA.length = size;
    transfer.mode = Mode::Transmit;

    i2c.CR1 |= I2C_CR1_TXIE | I2C_CR1_TCIE;
    i2c.CR2 = cr2 | I2C_CR2_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}
I2C::Error I2C_interrupt::write(DeviceAddress deviceAddress, const uint8_t *write_data, size_t write_data_size, const uint8_t *write_dataB,
                                size_t write_data_sizeB) noexcept {
    uint32_t cr2 = i2c.CR2;
    // clear device address and number of bytes and read flag
    cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk & ~I2C_CR2_RD_WRN;
    // set new device address, set number of bytes to transfer, set transfer direction to transmit
    if (write_data_size > 255) {
        cr2 |= deviceAddress | (255 << I2C_CR2_NBYTES_Pos);
        write_data_size -= 255;
    } else {
        cr2 |= deviceAddress | (write_data_size << I2C_CR2_NBYTES_Pos);
        write_data_size = 0;
    }

    transfer.bufferA.ptr = const_cast<uint8_t *>(write_data);
    transfer.bufferA.length = write_data_size;
    transfer.bufferB.ptr = const_cast<uint8_t *>(write_dataB);
    transfer.bufferB.length = write_data_sizeB;
    transfer.mode = Mode::TransmitDoubleBuffer;

    i2c.CR1 |= I2C_CR1_TXIE | I2C_CR1_TCIE;
    i2c.CR2 = cr2 | I2C_CR2_RELOAD | I2C_CR2_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}
I2C::Error I2C_interrupt::read(uint8_t deviceAddress, uint8_t *data, size_t length) noexcept {
    uint32_t cr2 = i2c.CR2;
    // clear device address and number of bytes
    cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk;
    // set new device address, set number of bytes to transfer, set transfer direction to receive
    if (length > 255) {
        cr2 |= deviceAddress | (255 << I2C_CR2_NBYTES_Pos) | I2C_CR2_RD_WRN | I2C_CR2_RELOAD;
        length -= 255;
    } else {
        cr2 |= deviceAddress | (length << I2C_CR2_NBYTES_Pos) | I2C_CR2_RD_WRN;
        length = 0;
    }

    transfer.bufferA.ptr = data;
    transfer.bufferA.length = length;
    transfer.mode = Mode::Receive;

    i2c.CR1 |= I2C_CR1_RXIE | I2C_CR1_TCIE;
    i2c.CR2 = cr2 | I2C_CR2_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}

I2C::Error I2C_interrupt::read(uint8_t deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept {
    uint32_t cr2 = i2c.CR2;
    // clear device address and number of bytes and read flag
    cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk;
    // set new device address, set number of bytes to transfer, set transfer direction to receive
    if (dataLength > 255) {
        cr2 |= deviceAddress | (255 << I2C_CR2_NBYTES_Pos) | I2C_CR2_RD_WRN;
        dataLength -= 255;
    } else {
        cr2 |= deviceAddress | (dataLength << I2C_CR2_NBYTES_Pos) | I2C_CR2_RD_WRN;
        dataLength = 0;
    }

    transfer.bufferA.ptr = data;
    transfer.bufferA.length = dataLength;
    transfer.bufferB.ptr = dataB;
    transfer.bufferB.length = dataBLength;
    transfer.mode = Mode::ReceiveDoubleBuffer;

    i2c.CR1 |= I2C_CR1_RXIE | I2C_CR1_TCIE;
    i2c.CR2 = cr2 | I2C_CR2_RELOAD | I2C_CR2_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}

I2C::Error I2C_interrupt::writeRead(DeviceAddress address, const uint8_t *write, size_t write_size, uint8_t *read, size_t read_size) noexcept {
    uint32_t cr2 = i2c.CR2;
    // clear device address and number of bytes and read flag
    cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk & ~I2C_CR2_RD_WRN;
    // set new device address, set number of bytes to transfer, set transfer direction to transmit
    if (write_size > 255) {
        cr2 |= address | (255 << I2C_CR2_NBYTES_Pos) | I2C_CR2_RELOAD;
        write_size -= 255;
    } else {
        cr2 |= address | (write_size << I2C_CR2_NBYTES_Pos);
        write_size = 0;
    }

    transfer.bufferA.ptr = const_cast<uint8_t *>(write);
    transfer.bufferA.length = write_size;
    transfer.bufferB.ptr = read;
    transfer.bufferB.length = read_size;
    transfer.mode = Mode::TransmitReceive;

    // send start
    i2c.CR1 |= I2C_CR1_TXIE | I2C_CR1_TCIE | I2C_CR1_RXIE;  // | I2C_CR1_NACKIE | I2C_CR1_STOPIE;
    i2c.CR2 = cr2 | I2C_CR2_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}
//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void I2C_interrupt::IRQFunction(I2C_interrupt &obj, I2C_TypeDef *i2c) {
    using Mode = I2C_interrupt::Mode;

    enum Transfer { Write = 0, Read = 1 };

    struct ISR_st {
        constexpr ISR_st(uint32_t data) : isr(data) {}

        uint8_t slaveAddress() const { return (isr & I2C_ISR_ADDCODE_Msk) >> (I2C_ISR_ADDCODE_Pos - 1); }  //!< Only in slave mode
        Transfer direction() const { return isr & I2C_ISR_DIR ? Read : Write; }                            //!< Only in slave mode
        bool busIsBusy() const { return isr & I2C_ISR_BUSY; }
        bool timeout() const { return isr & I2C_ISR_TIMEOUT; }
        bool overrunOrUnderrunOccured() const { return isr & I2C_ISR_OVR; }  //!< Only in slave mode
        bool arbitrationLost() const { return isr & I2C_ISR_ARLO; }
        bool busErrorDetected() const { return isr & I2C_ISR_BERR; }
        bool transferCompleteReload() const { return isr & I2C_ISR_TCR; }
        bool transferComplete() const { return isr & I2C_ISR_TC; }
        bool stopDetected() const { return isr & I2C_ISR_STOPF; }
        bool nackReceived() const { return isr & I2C_ISR_NACKF; }
        bool slaveAddressMatched() const { return isr & I2C_ISR_ADDR; }
        bool receiveRegisterNotEmpty() const { return isr & I2C_ISR_RXNE; }
        bool transmitInterrupsStatus() const { return isr & I2C_ISR_TXIS; }
        bool transmitRegisterEmpty() const { return isr & I2C_ISR_TXE; }

        uint32_t isr;
    };

    struct ICR_st {
        ICR_st(uint32_t icr) : icr(icr) {}
        void clearAlertFlag() { icr |= I2C_ICR_ALERTCF; }
        void clearTimeoutFlag() { icr |= I2C_ICR_TIMOUTCF; }
        void clearArbitrationLostFlag() { icr |= I2C_ICR_ARLOCF; }
        void clearBusErrorFlag() { icr |= I2C_ICR_BERRCF; }
        void clearStopFlag() { icr |= I2C_ICR_STOPCF; }
        void clearNackFlag() { icr |= I2C_ICR_NACKCF; }
        void clearAddressMatchedFlag() { icr |= I2C_ICR_ADDRCF; }
        uint32_t icr;
    };

    struct CR1_st {};

    uint32_t isr = i2c->ISR;

    ISR_st interrupStatus(isr);
    ICR_st interruptClear(i2c->ICR);

    if (interrupStatus.slaveAddressMatched()) {
        if (obj.activeSlave) {
            // slave is already active, restart detected
            auto transfered = obj.transfer.bufferB.length - obj.transfer.bufferA.length;
            if (interrupStatus.direction() == Write)
                obj.activeSlave->onTransmitFinish(transfered);
            else
                obj.activeSlave->onReceiveFinish(transfered);
        } else {
            obj.setActiveSlave(interrupStatus.slaveAddress());
        }
        gsl::span span = (interrupStatus.direction() == Read) ? obj.activeSlave->prepareReceiveBuffer() : obj.activeSlave->prepareTransmitData();
        if (span.data() != nullptr) {
            obj.transfer.bufferA.length = span.length();
            obj.transfer.bufferA.ptr = span.data();
            obj.transfer.bufferB.length = span.length();
            interruptClear.clearAddressMatchedFlag();
        }
    }

    if (interrupStatus.stopDetected()) {
        // Used only in slave mode
        if (obj.activeSlave) {
            auto transfered = obj.transfer.bufferB.length - obj.transfer.bufferA.length;
            if (interrupStatus.direction() == Write)
                obj.activeSlave->onTransmitFinish(transfered);
            else
                obj.activeSlave->onReceiveFinish(transfered);
            obj.activeSlave = nullptr;
        }
        interruptClear.clearStopFlag();
    }

    if (interrupStatus.nackReceived()) {
        // Used only in slave mode. Master send NACK so no next data is requested.
        i2c->CR1 &= ~I2C_CR1_TXIE;
        interruptClear.clearNackFlag();
    }

    if (interrupStatus.receiveRegisterNotEmpty()) {
        if (obj.activeSlave) {
            if (obj.transfer.bufferA.length > 0) {
                obj.transfer.bufferA.length--;
                *obj.transfer.bufferA.ptr++ = i2c->RXDR;
            }
        } else {
            *obj.transfer.bufferA.ptr++ = i2c->RXDR;
        }
    }

    if (isr & I2C_ISR_TXIS) {
        if (obj.activeSlave) {
            if (obj.transfer.bufferA.length > 0) {
                i2c->TXDR = *obj.transfer.bufferA.ptr;
                obj.transfer.bufferA.ptr++;
                obj.transfer.bufferA.length--;
            } else {
                i2c->TXDR = 0x00;
            }
        } else {
            i2c->TXDR = *obj.transfer.bufferA.ptr;
            obj.transfer.bufferA.ptr++;
        }
    }

    if (isr & I2C_ISR_TC) {
        if (obj.transfer.mode == Mode::TransmitReceive) {
            obj.transfer.bufferA = obj.transfer.bufferB;
            obj.transfer.mode = Mode::Receive;

            uint32_t cr2 = i2c->CR2;
            // clear device address and number of bytes
            cr2 &= ~I2C_CR2_NBYTES_Msk;
            // set number of bytes to transfer, set transfer direction to transmit
            cr2 |= I2C_CR2_RD_WRN | (obj.transfer.bufferB.length << I2C_CR2_NBYTES_Pos);
            i2c->CR2 = cr2;

            i2c->CR2 |= I2C_CR2_START;
        } else {
            i2c->CR2 |= I2C_CR2_STOP;
            // maybe it is better to enable stop interrupt and give semaphore in when stop interrupt occurs
            auto shouldYeld = obj.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
            portYIELD_FROM_ISR(shouldYeld);
#else
            (void)shouldYeld;
#endif
        }
    }

    if (isr & I2C_ISR_TCR) {
        uint32_t cr2 = i2c->CR2;
        // clear device address and number of bytes
        cr2 &= ~I2C_CR2_NBYTES_Msk & ~I2C_CR2_RELOAD;

        size_t toWrite = obj.transfer.bufferA.length;
        if (toWrite != 0) {
            if (toWrite > 255) {
                toWrite = 255;
                cr2 |= I2C_CR2_RELOAD;
            }
            obj.transfer.bufferA.length -= toWrite;
        } else {
            // we are in double buffer mode
            // switch buffers
            obj.transfer.bufferA = obj.transfer.bufferB;
            toWrite = obj.transfer.bufferA.length;
            if (toWrite > 255) {
                toWrite = 255;
                cr2 |= I2C_CR2_RELOAD;
            }
            obj.transfer.bufferA.length -= toWrite;
        }

        // set number of bytes to transfer, set transfer direction to transmit
        cr2 |= (toWrite << I2C_CR2_NBYTES_Pos);
        i2c->CR2 = cr2;
    }
}  // namespace stm32f3xx
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
    I2C1->CR2 |= I2C_CR2_STOP;
    I2C_interrupt::i2c1.error = I2C::errorCheckAndClear(I2C1, I2C1->ISR);
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
    I2C2->CR2 |= I2C_CR2_STOP;
    I2C_interrupt::i2c2.error = I2C::errorCheckAndClear(I2C2, I2C2->ISR);
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
    I2C3->CR2 |= I2C_CR2_STOP;
    I2C_interrupt::i2c3.error = I2C::errorCheckAndClear(I2C3, I2C3->ISR);
    auto shouldYeld = I2C_interrupt::i2c3.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#else
    (void)shouldYeld;
#endif
}
#endif

}  // namespace stm32f3xx
}  // namespace microhal
