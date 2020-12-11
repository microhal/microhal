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

#include "i2c_interrupt_stmCommon.h"
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#if _MICROHAL_PORT_STM_I2C_DRIVER_VERSION == 2  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_I2C(INTERRUPT)                  // Check if driver is enabled in microhal port config

#include "ports/stmCommon/clockManager/i2cClock.h"
#include _MICROHAL_INCLUDE_PORT_DEVICE

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
/* ************************************************************************************************
 *                                   STATIC VARIABLES
 * ***********************************************************************************************/
#ifdef MICROHAL_USE_I2C1_INTERRUPT
I2C_interrupt I2C_interrupt::i2c1(*registers::i2c1);
I2C &I2C::i2c1 = I2C_interrupt::i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_INTERRUPT
I2C_interrupt I2C_interrupt::i2c2(*registers::i2c2);
I2C &I2C::i2c2 = I2C_interrupt::i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_INTERRUPT
I2C_interrupt I2C_interrupt::i2c3(*registers::i2c3);
I2C &I2C::i2c3 = I2C_interrupt::i2c3;
#endif

I2C_interrupt::I2C_interrupt(registers::I2C &i2c) : I2C(i2c), transfer(), error(), semaphore() {
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::enableI2C(getNumber(), ClockManager::PowerMode::Normal);
#else
    ClockManager::enableI2C(getNumber());
#endif

    switch (getNumber()) {
        case 1:
            NVIC_EnableIRQ(I2C1_EV_IRQn);
            NVIC_SetPriority(I2C1_EV_IRQn, 0);
            NVIC_EnableIRQ(I2C1_ER_IRQn);
            NVIC_SetPriority(I2C1_ER_IRQn, 0);
            break;
#if defined(I2C2)
        case 2:
            NVIC_EnableIRQ(I2C2_EV_IRQn);
            NVIC_SetPriority(I2C2_EV_IRQn, 0);
            NVIC_EnableIRQ(I2C2_ER_IRQn);
            NVIC_SetPriority(I2C2_ER_IRQn, 0);
            break;
#endif
#if defined(I2C3)
        case 3:
            NVIC_EnableIRQ(I2C3_EV_IRQn);
            NVIC_SetPriority(I2C3_EV_IRQn, 0);
            NVIC_EnableIRQ(I2C3_ER_IRQn);
            NVIC_SetPriority(I2C3_ER_IRQn, 0);
            break;
#endif
    }
}
/* ************************************************************************************************
 *                                            FUNCTIONS
 * ***********************************************************************************************/
I2C::Error I2C_interrupt::write(uint8_t deviceAddress, const uint8_t *data, size_t size) noexcept {
    transfer.bufferA.ptr = const_cast<uint8_t *>(data);
    transfer.bufferA.length = size;
    transfer.mode = Mode::Transmit;

    // enable interrupts
    auto cr1 = i2c.cr1.volatileLoad();
    cr1.TXIE.set();
    cr1.STOPIE.set();
    i2c.cr1.volatileStore(cr1);

    // set new device address, set number of bytes to transfer, set transfer direction to transmit
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.SADD = deviceAddress;
    cr2.RD_WRN.clear();
    cr2.AUTOEND.set();
    if (size > 255) {
        cr2.NBYTES = 255;
        cr2.RELOAD.set();
        size -= 255;
    } else {
        cr2.NBYTES = size;
        cr2.RELOAD.clear();
        size = 0;
    }
    cr2.START.set();
    i2c.cr2.volatileStore(cr2);

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}
I2C::Error I2C_interrupt::write(DeviceAddress deviceAddress, const uint8_t *write_data, size_t write_data_size, const uint8_t *write_dataB,
                                size_t write_data_sizeB) noexcept {
    transfer.bufferA.ptr = const_cast<uint8_t *>(write_data);
    transfer.bufferA.length = write_data_size;
    transfer.bufferB.ptr = const_cast<uint8_t *>(write_dataB);
    transfer.bufferB.length = write_data_sizeB;
    transfer.mode = Mode::TransmitDoubleBuffer;

    // enable interrupts
    auto cr1 = i2c.cr1.volatileLoad();
    cr1.TXIE.set();
    cr1.TCIE.set();
    cr1.STOPIE.set();
    i2c.cr1.volatileStore(cr1);

    // set new device address, set number of bytes to transfer, set transfer direction to transmit
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.SADD = deviceAddress;
    cr2.RD_WRN.clear();
    cr2.AUTOEND.clear();  // allow to generate TC interrupt, in this interrupt we will swap buffers
    if (write_data_size > 255) {
        cr2.NBYTES = 255;
        cr2.RELOAD.set();
        write_data_size -= 255;
    } else {
        cr2.NBYTES = write_data_size;
        cr2.RELOAD.clear();
        write_data_size = 0;
    }
    cr2.START.set();
    i2c.cr2.volatileStore(cr2);

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}
I2C::Error I2C_interrupt::read(uint8_t deviceAddress, uint8_t *data, size_t length) noexcept {
    transfer.bufferA.ptr = data;
    transfer.bufferA.length = length;
    transfer.mode = Mode::Receive;

    // enable interrupts
    auto cr1 = i2c.cr1.volatileLoad();
    cr1.RXIE.set();
    cr1.STOPIE.set();
    i2c.cr1.volatileStore(cr1);

    // set new device address, set number of bytes to transfer, set transfer direction to receive
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.SADD = deviceAddress;
    cr2.RD_WRN.set();
    cr2.AUTOEND.set();
    if (length > 255) {
        cr2.NBYTES = 255;
        cr2.RELOAD.set();
        length -= 255;
    } else {
        cr2.NBYTES = length;
        cr2.RELOAD.clear();
        length = 0;
    }
    // send start
    cr2.START.set();
    i2c.cr2.volatileStore(cr2);

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}

I2C::Error I2C_interrupt::read(uint8_t deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept {
    transfer.bufferA.ptr = data;
    transfer.bufferA.length = dataLength;
    transfer.bufferB.ptr = dataB;
    transfer.bufferB.length = dataBLength;
    transfer.mode = Mode::ReceiveDoubleBuffer;

    // enable interrupts
    auto cr1 = i2c.cr1.volatileLoad();
    cr1.RXIE.set();
    cr1.TCIE.set();
    cr1.STOPIE.set();
    i2c.cr1.volatileStore(cr1);

    // set new device address, set number of bytes to transfer, set transfer direction to receive
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.SADD = deviceAddress;
    cr2.RD_WRN.set();
    cr2.AUTOEND.clear();  // allow to generate TC interrupt, in this interrupt we will swap buffers
    if (dataLength > 255) {
        cr2.NBYTES = 255;
        cr2.RELOAD.set();
        dataLength -= 255;
    } else {
        cr2.NBYTES = dataLength;
        cr2.RELOAD.clear();
        dataLength = 0;
    }
    // send start
    cr2.START.set();
    i2c.cr2.volatileStore(cr2);

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}

I2C::Error I2C_interrupt::writeRead(DeviceAddress address, const uint8_t *write, size_t write_size, uint8_t *read, size_t read_size) noexcept {
    transfer.bufferA.ptr = const_cast<uint8_t *>(write);
    transfer.bufferA.length = write_size;
    transfer.bufferB.ptr = read;
    transfer.bufferB.length = read_size;
    transfer.mode = Mode::TransmitReceive;

    // enable interrupts
    auto cr1 = i2c.cr1.volatileLoad();
    cr1.TXIE.set();
    cr1.TCIE.set();
    cr1.RXIE.set();
    cr1.STOPIE.set();
    i2c.cr1.volatileStore(cr1);

    // set new device address, set number of bytes to transfer, set transfer direction to transmit
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.SADD = address;
    cr2.RD_WRN.clear();
    cr2.AUTOEND.clear();  // allow to generate TC interrupt, in this interrupt we will swap buffers
    if (write_size > 255) {
        cr2.NBYTES = 255;
        cr2.RELOAD.set();
        write_size -= 255;
    } else {
        cr2.NBYTES = write_size;
        cr2.RELOAD.clear();
        write_size = 0;
    }
    // send start
    cr2.START.set();
    i2c.cr2.volatileStore(cr2);

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}
//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void I2C_interrupt::IRQFunction() {
    using Mode = I2C_interrupt::Mode;
    using Transfer = registers::I2C::ISR::Transfer;

    auto interrupStatus = i2c.isr.volatileLoad();
    registers::I2C::ICR interruptClear = {};

    if (interrupStatus.slaveAddressMatched()) {
        if (activeSlave) {
            // slave is already active, restart detected
            auto transfered = transfer.bufferB.length - transfer.bufferA.length;
            if (interrupStatus.direction() == Transfer::Write)
                activeSlave->onTransmitFinish(transfered);
            else
                activeSlave->onReceiveFinish(transfered);
        } else {
            setActiveSlave(interrupStatus.slaveAddress());
        }
        gsl::span span = (interrupStatus.direction() == Transfer::Read) ? activeSlave->prepareReceiveBuffer() : activeSlave->prepareTransmitData();
        if (span.data() != nullptr) {
            transfer.bufferA.length = span.length();
            transfer.bufferA.ptr = span.data();
            transfer.bufferB.length = span.length();
            interruptClear.clearAddressMatchedFlag();
        }
    }

    if (interrupStatus.stopDetected()) {
        if (activeSlave) {
            auto transfered = transfer.bufferB.length - transfer.bufferA.length;
            if (interrupStatus.direction() == Transfer::Write)
                activeSlave->onTransmitFinish(transfered);
            else
                activeSlave->onReceiveFinish(transfered);
            activeSlave = nullptr;
        } else {
            auto shouldYeld = semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
            portYIELD_FROM_ISR(shouldYeld);
#else
            (void)shouldYeld;
#endif
        }
        interruptClear.clearStopFlag();
    }

    if (interrupStatus.nackReceived()) {
        // Used only in slave mode. Master send NACK so no next data is requested.
        auto cr1 = i2c.cr1.volatileLoad();
        cr1.TXIE.clear();
        i2c.cr1.volatileStore(cr1);
        interruptClear.clearNackFlag();
    }

    auto cr2 = i2c.cr2.volatileLoad();
    if (interrupStatus.transferComplete()) {
        transfer.bufferA = transfer.bufferB;
        size_t toWrite = transfer.bufferA.length;
        if (toWrite > 255) {
            toWrite = 255;
            cr2.RELOAD.set();
        } else {
            cr2.RELOAD.clear();
        }
        transfer.bufferA.length -= toWrite;
        cr2.NBYTES = toWrite;
        cr2.AUTOEND.set();

        if (transfer.mode == Mode::TransmitReceive) {
            transfer.mode = Mode::Receive;
            cr2.RD_WRN.set();
            cr2.START.set();
        }
    }

    if (interrupStatus.transferCompleteReload()) {
        size_t toWrite = transfer.bufferA.length;
        if (toWrite > 255) {
            toWrite = 255;
            cr2.RELOAD.set();
        } else {
            cr2.RELOAD.clear();
        }
        transfer.bufferA.length -= toWrite;
        cr2.NBYTES = toWrite;
    }
    i2c.icr.volatileStore(interruptClear);
    i2c.cr2.volatileStore(cr2);

    if (interrupStatus.TXIS) {
        if (activeSlave) {
            if (transfer.bufferA.length > 0) {
                i2c.txdr.volatileStore(*transfer.bufferA.ptr);
                transfer.bufferA.ptr++;
                transfer.bufferA.length--;
            } else {
                i2c.txdr.volatileStore(0x00);
            }
        } else {
            i2c.txdr.volatileStore(*transfer.bufferA.ptr);
            transfer.bufferA.ptr++;
        }
    }

    if (interrupStatus.receiveRegisterNotEmpty()) {
        if (activeSlave) {
            if (transfer.bufferA.length > 0) {
                transfer.bufferA.length--;
                *transfer.bufferA.ptr++ = (uint32_t)i2c.rxdr.volatileLoad();
            }
        } else {
            *transfer.bufferA.ptr++ = (uint32_t)i2c.rxdr.volatileLoad();
        }
    }
}

void I2C_interrupt::ErrorIRQFunction() {
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.STOP.set();
    i2c.cr2.volatileStore(cr2);

    error = I2C::errorCheckAndClear(&i2c, i2c.isr.volatileLoad());
    auto shouldYeld = semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#else
    (void)shouldYeld;
#endif
}
//***********************************************************************************************//
//                                          IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_I2C1_INTERRUPT
void I2C1_EV_IRQHandler(void) {
    I2C_interrupt::i2c1.IRQFunction();
}
#endif
#ifdef MICROHAL_USE_I2C2_INTERRUPT
void I2C2_EV_IRQHandler(void) {
    I2C_interrupt::i2c2.IRQFunction();
}
#endif
#ifdef MICROHAL_USE_I2C3_INTERRUPT
void I2C3_EV_IRQHandler(void) {
    I2C_interrupt::i2c3.IRQFunction();
}
#endif
//***********************************************************************************************//
//                                         error IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_I2C1_INTERRUPT
void I2C1_ER_IRQHandler(void) {
    I2C_interrupt::i2c1.ErrorIRQFunction();
}
#endif
#ifdef MICROHAL_USE_I2C2_INTERRUPT
void I2C2_ER_IRQHandler(void) {
    I2C_interrupt::i2c2.ErrorIRQFunction();
}
#endif
#ifdef MICROHAL_USE_I2C3_INTERRUPT
void I2C3_ER_IRQHandler(void) {
    I2C_interrupt::i2c3.ErrorIRQFunction();
}
#endif

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif
#endif
