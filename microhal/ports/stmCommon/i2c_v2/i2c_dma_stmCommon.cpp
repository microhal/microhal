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
/* ************************************************************************************************
 * INCLUDES
 */
#include "i2c_dma_stmCommon.h"

/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#if _MICROHAL_PORT_STM_I2C_DRIVER_VERSION == 2  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_I2C(DMA)                        // Check if driver is enabled in microhal port config

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
/* ************************************************************************************************
 *                                   STATIC VARIABLES
 * ***********************************************************************************************/
#if MICROHAL_USE_I2C1_DMA == 1
#if MICROHAL_I2C1_DMA_RX_STREAM != 6 && MICROHAL_I2C1_DMA_RX_STREAM != 7
#error I2C RX DMA channel can be confugured as 0 or 5 only
#endif
#if MICROHAL_I2C1_DMA_TX_STREAM != 6 && MICROHAL_I2C1_DMA_TX_STREAM != 7
#error I2C TX DMA channel can be confugured as 6 or 7 only
#endif
I2C_dma I2C_dma::i2c1(*registers::i2c1, DMA::dma1->stream[MICROHAL_I2C1_DMA_RX_STREAM - 1], DMA::dma1->stream[MICROHAL_I2C1_DMA_TX_STREAM - 1]);
I2C &I2C::i2c1 = I2C_dma::i2c1;
#endif
#if MICROHAL_USE_I2C2_DMA == 1
#if MICROHAL_I2C2_DMA_RX_STREAM != 2 && MICROHAL_I2C2_DMA_RX_STREAM != 3
#error I2C RX DMA channel can be confugured as 2 or 3 only
#endif
I2C_dma I2C_dma::i2c2(*I2C2, DMA::dma1->stream[MICROHAL_I2C2_DMA_RX_STREAM], DMA::dma1->stream[7]);
I2C &I2C::i2c2 = I2C_dma::i2c2;
#endif
#if MICROHAL_USE_I2C3_DMA == 1
I2C_dma I2C_dma::i2c3(*I2C3, DMA::dma1->stream[2], DMA::dma1->stream[4]);
I2C &I2C::i2c3 = I2C_dma::i2c3;
#endif
/* ************************************************************************************************
 *                                   Functions prototypes
 * ***********************************************************************************************/

/* ************************************************************************************************
 *                                  	 Constructors
 * ***********************************************************************************************/

/* ************************************************************************************************
 *                                 		  Functions
 * ***********************************************************************************************/
I2C::Error I2C_dma::write(DeviceAddress deviceAddress, const uint8_t *data, size_t size) noexcept {
    // configure DMA
    txStream.setNumberOfItemsToTransfer(size);
    txStream.setMemoryAddress(const_cast<uint8_t *>(data));
    txStream.enable();

    // configure I2C
    auto cr2 = i2c.cr2.volatileLoad();
    // clear device address and number of bytes and read flag
    // cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk & ~I2C_CR2_RD_WRN;
    cr2.RD_WRN.clear();
    // set new device address, set number of bytes to transfer, set transfer direction to transmit
    cr2.SADD = deviceAddress;
    if (size > 255) {
        cr2.NBYTES = 255;
        cr2.RELOAD.set();
        // cr2 |= deviceAddress | (255 << I2C_CR2_NBYTES_Pos) | I2C_CR2_RELOAD;
        size -= 255;
    } else {
        cr2.NBYTES = size;
        cr2.RELOAD.clear();
        // cr2 |= deviceAddress | (size << I2C_CR2_NBYTES_Pos);
        size = 0;
    }

    transfer.mode = Mode::Transmit;
    transfer.txLength = size;

    auto cr1 = i2c.cr1.volatileLoad();
    cr1.TCIE.set();
    cr1.TXDMAEN.set();
    i2c.cr1.volatileStore(cr1);
    // i2c.CR1 |= I2C_CR1_TCIE | I2C_CR1_TXDMAEN;
    cr2.START.set();
    i2c.cr2.volatileStore(cr2);
    // i2c.CR2 = cr2 | I2C_CR2_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}

I2C::Error I2C_dma::write(DeviceAddress deviceAddress, const uint8_t *dataA, size_t dataASize, const uint8_t *dataB, size_t dataBSize) noexcept {
    txStream.setNumberOfItemsToTransfer(dataASize);
    txStream.setMemoryAddress(const_cast<uint8_t *>(dataA));
    txStream.enable();

    auto cr2 = i2c.cr2.volatileLoad();
    // clear device address and number of bytes and read flag
    // cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk & ~I2C_CR2_RD_WRN;
    cr2.RD_WRN.clear();
    // set new device address, set number of bytes to transfer, set transfer direction to transmit
    cr2.SADD = deviceAddress;
    if (dataASize > 255) {
        cr2.NBYTES = 255;
        cr2.RELOAD.set();
        // cr2 |= deviceAddress | (255 << I2C_CR2_NBYTES_Pos);
        dataASize -= 255;
    } else {
        cr2.NBYTES = deviceAddress;
        cr2.RELOAD.clear();
        // cr2 |= deviceAddress | (dataASize << I2C_CR2_NBYTES_Pos);
        dataASize = 0;
    }

    transfer.mode = Mode::TransmitDoubleBuffer;
    transfer.txLength = dataASize;
    transfer.bufferB.ptr = const_cast<uint8_t *>(dataB);
    transfer.bufferB.length = dataBSize;

    auto cr1 = i2c.cr1.volatileLoad();
    cr1.TCIE.set();
    cr1.TXDMAEN.set();
    i2c.cr1.volatileStore(cr1);
    // i2c.CR1 |= I2C_CR1_TCIE | I2C_CR1_TXDMAEN;

    cr2.START.set();
    // i2c.CR2 = cr2 | I2C_CR2_RELOAD | I2C_CR2_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}

I2C::Error I2C_dma::read(DeviceAddress deviceAddress, uint8_t *data, size_t size) noexcept {
    rxStream.setNumberOfItemsToTransfer(size);
    rxStream.setMemoryAddress(data);
    rxStream.enable();

    auto cr2 = i2c.cr2.volatileLoad();
    // clear device address and number of bytes
    // cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk;
    // set new device address, set number of bytes to transfer, set transfer direction to receive
    cr2.RD_WRN.set();
    if (size > 255) {
        cr2.SADD = deviceAddress;
        cr2.NBYTES = 255;
        cr2.RELOAD.set();
        // cr2 |= deviceAddress | (255 << I2C_CR2_NBYTES_Pos) | I2C_CR2_RD_WRN | I2C_CR2_RELOAD;
        size -= 255;
    } else {
        cr2.SADD = deviceAddress;
        cr2.NBYTES = size;
        // cr2 |= deviceAddress | (size << I2C_CR2_NBYTES_Pos) | I2C_CR2_RD_WRN;
        size = 0;
    }

    transfer.mode = Mode::Receive;
    transfer.rxLength = size;

    auto cr1 = i2c.cr1.volatileLoad();
    cr1.TCIE.set();
    cr1.RXDMAEN.set();
    i2c.cr1.volatileStore(cr1);
    // i2c.CR1 |= I2C_CR1_TCIE | I2C_CR1_RXDMAEN;
    cr2.START.set();
    i2c.cr2.volatileStore(cr2);
    // i2c.CR2 = cr2 | I2C_CR2_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}

I2C::Error I2C_dma::read(uint8_t deviceAddress, uint8_t *dataA, size_t dataASize, uint8_t *dataB, size_t dataBSize) noexcept {
    rxStream.setNumberOfItemsToTransfer(dataASize);
    rxStream.setMemoryAddress(dataA);
    rxStream.enable();

    auto cr2 = i2c.cr2.volatileLoad();
    // clear device address and number of bytes and read flag
    // cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk;
    // set new device address, set number of bytes to transfer, set transfer direction to receive
    if (dataASize > 255) {
        cr2.SADD = deviceAddress;
        cr2.NBYTES = 255;
        cr2.RD_WRN.set();
        // cr2 |= deviceAddress | (255 << I2C_CR2_NBYTES_Pos) | I2C_CR2_RD_WRN;
        dataASize -= 255;
    } else {
        cr2.SADD = deviceAddress;
        cr2.NBYTES = dataASize;
        cr2.RD_WRN.set();
        // cr2 |= deviceAddress | (dataASize << I2C_CR2_NBYTES_Pos) | I2C_CR2_RD_WRN;
        dataASize = 0;
    }

    transfer.mode = Mode::ReceiveDoubleBuffer;
    transfer.rxLength = dataASize;
    transfer.bufferB.length = dataBSize;
    transfer.bufferB.ptr = dataB;

    auto cr1 = i2c.cr1.volatileLoad();
    cr1.TCIE.set();
    cr1.RXDMAEN.set();
    i2c.cr1.volatileStore(cr1);
    // i2c.CR1 |= I2C_CR1_TCIE | I2C_CR1_RXDMAEN;
    cr2.RELOAD.set();
    cr2.START.set();
    i2c.cr2.volatileStore(cr2);
    // i2c.CR2 = cr2 | I2C_CR2_RELOAD | I2C_CR2_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}

I2C::Error I2C_dma::writeRead(DeviceAddress deviceAddress, const uint8_t *writeData, size_t writeSize, uint8_t *readData, size_t readSize) noexcept {
    txStream.setNumberOfItemsToTransfer(writeSize);
    txStream.setMemoryAddress(const_cast<uint8_t *>(writeData));
    txStream.enable();

    rxStream.setNumberOfItemsToTransfer(readSize);
    rxStream.setMemoryAddress(readData);
    rxStream.enable();

    auto cr2 = i2c.cr2.volatileLoad();
    // clear device address and number of bytes and read flag
    // cr2 &= ~I2C_CR2_SADD_Msk & ~I2C_CR2_NBYTES_Msk & ~I2C_CR2_RD_WRN;
    cr2.RD_WRN.clear();
    // set new device address, set number of bytes to transfer, set transfer direction to transmit
    if (writeSize > 255) {
        cr2.SADD = deviceAddress;
        cr2.NBYTES = 255;
        cr2.RELOAD.set();
        // cr2 |= deviceAddress | (255 << I2C_CR2_NBYTES_Pos) | I2C_CR2_RELOAD;
        writeSize -= 255;
    } else {
        cr2.SADD = deviceAddress;
        cr2.NBYTES = 255;
        // cr2 |= deviceAddress | (writeSize << I2C_CR2_NBYTES_Pos);
        writeSize = 0;
    }

    transfer.mode = Mode::TransmitReceive;
    transfer.txLength = writeSize;
    transfer.rxLength = readSize;

    // send start
    auto cr1 = i2c.cr1.volatileLoad();
    cr1.TCIE.set();
    cr1.RXDMAEN.set();
    cr1.TXDMAEN.set();
    i2c.cr1.volatileStore(cr1);
    // i2c.CR1 |= I2C_CR1_TCIE | I2C_CR1_RXDMAEN | I2C_CR1_TXDMAEN;
    cr2.START.set();
    i2c.cr2.volatileStore(cr2);
    // i2c.CR2 = cr2 | I2C_CR2_START;

    error = Error::None;
    semaphore.wait(std::chrono::milliseconds::max());

    return error;
}
/**
 *
 */
void I2C_dma::init(void) {
    DMA::DMA &dma = *DMA::dma1;
    dma.clockEnable();

    // rx
    rxStream.deinit();
    rxStream.init(DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                  DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::PerToMem);
    rxStream.setPeripheralAddress(&i2c.rxdr);

    // tx
    txStream.deinit();
    txStream.init(DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                  DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::MemToPer);
    txStream.setPeripheralAddress(&i2c.txdr);
}
//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void I2C_dma::IRQFunction(I2C_dma &obj, registers::I2C *i2c) {
    using Mode = I2C_dma::Mode;

    auto isr = i2c->isr.volatileLoad();

    if (isr.TC) {
        if (obj.transfer.mode == Mode::TransmitReceive) {
            obj.transfer.mode = Mode::Receive;

            const size_t toWrite = obj.transfer.rxLength > 255 ? 255 : obj.transfer.rxLength;
            obj.transfer.rxLength -= toWrite;

            auto cr2 = i2c->cr2.volatileLoad();
            // clear number of bytes
            // cr2 &= ~I2C_CR2_NBYTES_Msk;
            // set number of bytes to transfer, set transfer direction to receive
            cr2.NBYTES = toWrite;
            cr2.RD_WRN.set();
            i2c->cr2.volatileStore(cr2);
            cr2.START.set();
            i2c->cr2.volatileStore(cr2);
            // cr2 |= I2C_CR2_RD_WRN | (toWrite << I2C_CR2_NBYTES_Pos);
            // i2c->CR2 = cr2;
            // i2c->CR2 |= I2C_CR2_START;
        } else {
            auto cr2 = i2c->cr2.volatileLoad();
            cr2.STOP.set();
            i2c->cr2.volatileStore(cr2);
            // i2c->CR2 |= I2C_CR2_STOP;
            obj.rxStream.disable();
            obj.txStream.disable();
            // maybe it is better to enable stop interrupt and give semaphore in when stop interrupt occurs
            auto shouldYeld = obj.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
            portYIELD_FROM_ISR(shouldYeld);
#else
            (void)shouldYeld;
#endif
        }
    }

    if (isr.TCR) {
        auto cr2 = i2c->cr2.volatileLoad();
        bool receive = cr2.RD_WRN;
        auto &transferLength = receive ? obj.transfer.rxLength : obj.transfer.txLength;

        // clear number of bytes
        // cr2 &= ~I2C_CR2_NBYTES_Msk & ~I2C_CR2_RELOAD;
        cr2.RELOAD.clear();

        size_t toWrite = transferLength;
        if (toWrite != 0) {
            if (toWrite > 255) {
                toWrite = 255;
                cr2.RELOAD.set();  // |= I2C_CR2_RELOAD;
            }
            transferLength -= toWrite;
        } else {
            DMA::Channel &stream = receive ? obj.rxStream : obj.txStream;
            // we are in double buffer mode
            // switch buffers
            transferLength = obj.transfer.bufferB.length;
            toWrite = transferLength;
            if (toWrite > 255) {
                toWrite = 255;
                cr2.RELOAD.set();
                // cr2 |= I2C_CR2_RELOAD;
            }
            transferLength -= toWrite;

            stream.disable();
            stream.setMemoryAddress(obj.transfer.bufferB.ptr);
            stream.setNumberOfItemsToTransfer(obj.transfer.bufferB.length);
            stream.enable();
        }

        // set number of bytes to transfer, set transfer direction to transmit
        cr2.NBYTES = toWrite;
        // cr2 |= (toWrite << I2C_CR2_NBYTES_Pos);
        i2c->cr2.volatileStore(cr2);
        // i2c->CR2 = cr2;
    }
}

void I2C_dma::IRQErrorFunction() {
    // Disable rx and tx DMA streams
    rxStream.disable();
    txStream.disable();
    // send stop signal to I2C data bus
    auto cr2 = i2c.cr2.volatileLoad();
    cr2.STOP.set();
    i2c.cr2.volatileStore(cr2);
    // read error and store it in variable
    error = I2C::errorCheckAndClear(&i2c, i2c.isr.volatileLoad());
    auto shouldYeld = semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#else
    (void)shouldYeld;
#endif
}
//***********************************************************************************************//
//                                          IRQHandlers
//***********************************************************************************************//
#if MICROHAL_USE_I2C1_DMA == 1
void I2C1_EV_IRQHandler(void) {
    I2C_dma::IRQFunction(I2C_dma::i2c1, registers::i2c1);
}
#endif
#if MICROHAL_USE_I2C2_DMA == 1
void I2C2_EV_IRQHandler(void) {
    I2C_dma::IRQFunction(I2C_dma::i2c2, registers::i2c2);
}
#endif
#if MICROHAL_USE_I2C3_DMA == 1
void I2C3_EV_IRQHandler(void) {
    I2C_dma::IRQFunction(I2C_dma::i2c3, registers::i2c3);
}
#endif
//***********************************************************************************************//
//                                       I2C error IRQHandlers
//***********************************************************************************************//
#if MICROHAL_USE_I2C1_DMA == 1
void I2C1_ER_IRQHandler(void) {
    I2C_dma::i2c1.IRQErrorFunction();
}
#endif
#if MICROHAL_USE_I2C2_DMA == 1
void I2C2_ER_IRQHandler(void) {
    I2C_dma::i2c2.IRQErrorFunction();
}
#endif
#if MICROHAL_USE_I2C3_DMA == 1
void I2C3_ER_IRQHandler(void) {
    I2C_dma::i2c3.IRQErrorFunction();
}
#endif
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
#endif
#endif
