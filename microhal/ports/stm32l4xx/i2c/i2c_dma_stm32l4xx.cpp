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
#include "i2c_dma_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {
/* ************************************************************************************************
 *                                   STATIC VARIABLES
 * ***********************************************************************************************/
#ifdef MICROHAL_USE_I2C1_DMA
I2C_dma I2C_dma::i2c1(*I2C1, DMA::dma1->channel[7], DMA::dma1->channel[6]);
I2C &I2C::i2c1 = I2C_dma::i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_DMA
I2C_dma I2C_dma::i2c2(*I2C2, DMA::dma1->channel[5], DMA::dma1->channel[4]);
I2C &I2C::i2c2 = I2C_dma::i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_DMA
I2C_dma I2C_dma::i2c3(*I2C3, DMA::dma1->channel[3], DMA::dma1->channel[2]);
I2C &I2C::i2c3 = I2C_dma::i2c3;
#endif
/* ************************************************************************************************
 *                                   Functions prototypes
 * ***********************************************************************************************/
static DMA::Channel::Request getRequestNumber(I2C_TypeDef &i2c);

/* ************************************************************************************************
 *                                  	 Constructors
 * ***********************************************************************************************/

/* ************************************************************************************************
 *                                 		  Functions
 * ***********************************************************************************************/
/**
 *
 * @param deviceAddress
 * @param data
 * @return
 */
I2C::Error I2C_dma::write(uint8_t deviceAddress, uint8_t data) {
    transfer.mode = Transmit;
    transfer.deviceAddress = deviceAddress;
    transfer.registerAddress = data;
    transfer.length = 0;

    errorSemaphore = UnknownError;

    while (i2c.ISR & I2C_ISR_BUSY) {
    }
    i2c.CR1 &= ~(I2C_CR1_RXDMAEN | I2C_CR1_TXDMAEN);
    i2c.CR2 |= I2C_CR2_START;

    while (errorSemaphore == UnknownError) {
    }

    return errorSemaphore;
}
/**
 *
 * @param deviceAddress
 * @param registerAddress
 * @param data
 * @return
 */
I2C::Error I2C_dma::write(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data) {
    return write(deviceAddress, registerAddress, &data, 1);
}
/**
 *
 * @param deviceAddress
 * @param registerAddress
 * @param data
 * @param length
 * @return
 */
I2C::Error I2C_dma::write(uint8_t deviceAddress, uint8_t registerAddress, const void *data, size_t length) {
    transfer.mode = Transmit;
    transfer.deviceAddress = deviceAddress;
    transfer.registerAddress = registerAddress;
    transfer.length = length;

    txChannel.setNumberOfItemsToTransfer(length);
    txChannel.setMemory(data);
    txChannel.enable();

    errorSemaphore = UnknownError;

    while (i2c.ISR & I2C_ISR_BUSY) {
    }
    i2c.CR1 &= ~(I2C_CR1_RXDMAEN | I2C_CR1_TXDMAEN);
    i2c.CR2 |= I2C_CR2_START;

    while (errorSemaphore == UnknownError) {
    }

    return errorSemaphore;
}
/**
 *
 * @param deviceAddress
 * @param data
 * @return
 */
I2C::Error I2C_dma::read(uint8_t deviceAddress, uint8_t &data) {
    transfer.mode = Receive;
    transfer.deviceAddress = deviceAddress;

    rxChannel.setNumberOfItemsToTransfer(1);
    rxChannel.setMemory(&data);
    rxChannel.enable();

    errorSemaphore = UnknownError;

    while (i2c.ISR & I2C_ISR_BUSY) {
    }
    i2c.CR1 &= ~(I2C_CR1_RXDMAEN | I2C_CR1_TXDMAEN);
    i2c.CR2 |= I2C_CR2_START;


    while (errorSemaphore == UnknownError) {
    }

    return errorSemaphore;
}
/**
 *
 * @param deviceAddress
 * @param registerAddress
 * @param data
 * @return
 */
I2C::Error I2C_dma::read(uint8_t deviceAddress, uint8_t registerAddress, uint8_t &data) {
    return read(deviceAddress, registerAddress, &data, 1);
}
/**
 *
 * @param deviceAddress
 * @param registerAddress
 * @param data
 * @param length
 * @return
 */
I2C::Error I2C_dma::read(uint8_t deviceAddress, uint8_t registerAddress, void *data, size_t length) {
    transfer.mode = ReceiveFromRegister;
    transfer.deviceAddress = deviceAddress;
    transfer.registerAddress = registerAddress;
    transfer.length = length;

    rxChannel.setNumberOfItemsToTransfer(length);
    rxChannel.setMemory(data);
    rxChannel.enable();

    errorSemaphore = UnknownError;

    while (i2c.ISR & I2C_ISR_BUSY) {
    }
    i2c.CR1 &= ~(I2C_CR1_RXDMAEN | I2C_CR1_TXDMAEN);
    i2c.CR2 |= I2C_CR2_START;

    while (errorSemaphore == UnknownError) {
    }

    return errorSemaphore;
}
/**
 *
 */
void I2C_dma::init(void) {
    DMA::dma1->clockEnable();

    // rx
    rxChannel.deinit();
    rxChannel.init(DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                   DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::PerToMem);
    rxChannel.setPeripheralAddress(&i2c.RXDR);
    rxChannel.enableInterrupt(DMA::Channel::Interrupt::TransferComplete);
    DMA::dma1->init(rxChannel, getRequestNumber(i2c));

    // tx
    txChannel.deinit();
    txChannel.init(DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte, DMA::Channel::MemoryIncrementMode::PointerIncremented,
                   DMA::Channel::PeripheralIncrementMode::PointerFixed, DMA::Channel::TransmisionDirection::MemToPer);
    txChannel.setPeripheralAddress(&i2c.TXDR);
    txChannel.enableInterrupt(DMA::Channel::Interrupt::TransferComplete);
    DMA::dma1->init(txChannel, getRequestNumber(i2c));

    IRQn_Type rxIRQ = DMA1_Channel1_IRQn, txIRQ = DMA1_Channel2_IRQn;
    switch (reinterpret_cast<uint32_t>(&i2c)) {
        case reinterpret_cast<uint32_t>(I2C1):
        	rxIRQ = DMA1_Channel7_IRQn;
        	txIRQ = DMA1_Channel6_IRQn;
        	break;

        case reinterpret_cast<uint32_t>(I2C2):
            rxIRQ = DMA1_Channel5_IRQn;
            txIRQ = DMA1_Channel4_IRQn;
            break;

        case reinterpret_cast<uint32_t>(I2C3):
            rxIRQ = DMA1_Channel3_IRQn;
            txIRQ = DMA1_Channel2_IRQn;
            break;
    }

    DMA::dma1->clearInterruptFlag(rxChannel, DMA::Channel::Interrupt::TransferComplete);
    NVIC_EnableIRQ(rxIRQ);
    NVIC_SetPriority(rxIRQ, 0);

    DMA::dma1->clearInterruptFlag(txChannel, DMA::Channel::Interrupt::TransferComplete);
    NVIC_EnableIRQ(txIRQ);
    NVIC_SetPriority(txIRQ, 0);

    i2c.CR1 |= I2C_CR1_PE;
}
/**
 *
 * @param i2c
 * @return
 */
DMA::Channel::Request getRequestNumber(I2C_TypeDef &i2c) {
    switch (reinterpret_cast<uint32_t>(&i2c)) {
        case reinterpret_cast<uint32_t>(I2C1):
            return DMA::Channel::Request::Request3;
        case reinterpret_cast<uint32_t>(I2C2):
            return DMA::Channel::Request::Request3;
        case reinterpret_cast<uint32_t>(I2C3):
            return DMA::Channel::Request::Request3;
    }
    // error
    while (1) {
    }
    // return for avoid warning
    return DMA::Channel::Request::Request3;
}
//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void I2C_dma::IRQFunction(I2C_dma &obj, I2C_TypeDef *i2c) {
	uint16_t status1 = i2c->ISR;

    if (!(i2c->CR2 & I2C_CR2_START)) {  // sent start sequence
        i2c->TXDR = obj.transfer.deviceAddress | (obj.transfer.mode & 0x01);
    } else if (status1 & I2C_ISR_ADDR) {// adres(MASTER MODE) was sent

        switch (obj.transfer.mode) {
            case I2C_dma::Transmit:
                i2c->TXDR = obj.transfer.registerAddress;
                break;

            case I2C_dma::Receive:
                i2c->CR1 |= I2C_CR1_TXDMAEN | I2C_CR1_RXDMAEN;
                break;

            case I2C_dma::ReceiveFromRegister:
                obj.transfer.mode = I2C_dma::Receive;
                i2c->RXDR = obj.transfer.registerAddress;
                break;
            default:
                i2c->CR2 |= I2C_CR2_STOP;
                obj.errorSemaphore = I2C_dma::NoError;
        }
    } else if (status1 & (I2C_ISR_TXE | I2C_ISR_BUSY)) {
        switch (obj.transfer.mode) {
            case I2C_dma::Receive:
                if (obj.transfer.length == 1) {
                    i2c->CR2 = (i2c->CR2 & (~I2C_CR2_NACK)) | I2C_CR2_START;
                } else {
                    i2c->CR2 |= I2C_CR2_NACK | I2C_CR2_START;
                }
                break;
            case I2C_dma::Transmit:
                if (obj.transfer.length > 0) {
                    i2c->CR1 |= I2C_CR1_TXDMAEN | I2C_CR1_RXDMAEN;
                } else {
                    i2c->CR2 |= I2C_CR2_STOP;
                    obj.errorSemaphore = I2C_dma::NoError;
                }
                break;
            default:
                i2c->CR2 |= I2C_CR2_STOP;
                obj.errorSemaphore = I2C_dma::NoError;
                break;
        }
    }
}

void I2C_dma::IRQErrorFunction(I2C_dma &obj, I2C_TypeDef *i2c) {
    // Disable rx and tx DMA streams
    obj.rxChannel.disable();
    obj.txChannel.disable();
    // send stop signal to I2C data bus
    i2c->CR2 |= I2C_CR2_STOP;
    // read error and store it in variable
    obj.errorSemaphore = I2C::errorCheckAndClear(i2c, i2c->ISR);
}
//***********************************************************************************************//
//                                         DMA IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_I2C1_DMA
//***********************************************************************************************//

void DMA1_Channel7_IRQHandler(void) {
    // I2C rx
    I2C1->CR2 |= I2C_CR2_STOP;

    DMA1->IFCR = DMA_IFCR_CTCIF7;
    DMA1_Channel7->CCR &= ~DMA_CCR_EN;

    I2C_dma::i2c1.errorSemaphore = I2C_dma::NoError;
}

void DMA1_Channel6_IRQHandler(void) {
    // I2C tx
    DMA1->IFCR = DMA_IFCR_CTCIF6;
    DMA1_Channel6->CCR &= ~DMA_CCR_EN;

    I2C1->CR2 |= I2C_CR2_STOP;

    I2C_dma::i2c1.errorSemaphore = I2C_dma::NoError;
}
#endif

#ifdef MICROHAL_USE_I2C2_DMA
//***********************************************************************************************//

void DMA1_Channel5_IRQHandler(void) {
    // I2C rx
    I2C2->CR2 |= I2C_CR2_STOP;

    DMA1->IFCR = DMA_IFCR_CTCIF5;
    DMA1_Channel5->CCR &= ~DMA_CCR_EN;

    I2C_dma::i2c2.errorSemaphore = I2C_dma::NoError;
}

void DMA1_Channel4_IRQHandler(void) {
    // I2C tx
    DMA1->IFCR = DMA_IFCR_CTCIF4;
    DMA1_Channel4->CCR &= ~DMA_CCR_EN;

    I2C2->CR2 |= I2C_CR2_STOP;

    I2C_dma::i2c2.errorSemaphore = I2C_dma::NoError;
}
#endif

#ifdef MICROHAL_USE_I2C3_DMA
//***********************************************************************************************//

void DMA1_Channel3_IRQHandler(void) {
    // I2C rx
    I2C3->CR2 |= I2C_CR2_STOP;

    DMA1->IFCR = DMA_IFCR_CTCIF3;
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;

    I2C_dma::i2c3.errorSemaphore = I2C_dma::NoError;
}

void DMA1_Channel2_IRQHandler(void) {
    // I2C tx
    DMA1->IFCR = DMA_IFCR_CTCIF2;
    DMA1_Channel2->CCR &= ~DMA_CCR_EN;

    I2C3->CR2 |= I2C_CR2_STOP;

    I2C_dma::i2c3.errorSemaphore = I2C_dma::NoError;
}
#endif
//***********************************************************************************************//
//                                          IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_I2C1_DMA
void I2C1_EV_IRQHandler(void) {
    I2C_dma::IRQFunction(I2C_dma::i2c1, I2C1);
}
#endif
#ifdef MICROHAL_USE_I2C2_DMA
void I2C2_EV_IRQHandler(void) {
    I2C_dma::IRQFunction(I2C_dma::i2c2, I2C2);
}
#endif
#ifdef MICROHAL_USE_I2C3_DMA
void I2C3_EV_IRQHandler(void) {
    I2C_dma::IRQFunction(I2C_dma::i2c3, I2C3);
}
#endif
//***********************************************************************************************//
//                                       I2C error IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_I2C1_DMA
void I2C1_ER_IRQHandler(void) {
    I2C_dma::IRQErrorFunction(I2C_dma::i2c1, I2C1);
}
#endif
#ifdef MICROHAL_USE_I2C2_DMA
void I2C2_ER_IRQHandler(void) {
    I2C_dma::IRQErrorFunction(I2C_dma::i2c2, I2C2);
}
#endif
#ifdef MICROHAL_USE_I2C3_DMA
void I2C3_ER_IRQHandler(void) {
    I2C_dma::IRQErrorFunction(I2C_dma::i2c3, I2C3);
}
#endif
}  // namespace stm32l4xx
}  // namespace microhal


