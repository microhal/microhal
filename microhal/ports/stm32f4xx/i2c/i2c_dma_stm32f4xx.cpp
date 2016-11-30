/* =========================================================================================== */ /**
 @file        i2cSTM32F4xx.cpp
 @authors     Pawel Okas
 @version     $Id$
 @package
 @brief       I2C implementation for STM32F4xx microcontroler
 @hardware    STM32F4xx
 @copyright   $Copyright$
 @details
 */ /* ============================================================================================
                                                                                                          */
/* ************************************************************************************************
 * INCLUDES
 */
#include "i2c_dma_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 *                                   STATIC VARIABLES
 * ***********************************************************************************************/
#ifdef MICROHAL_USE_I2C1_DMA
#if MICROHAL_I2C1_DMA_RX_STREAM != 0 && MICROHAL_I2C1_DMA_RX_STREAM != 5
#error I2C RX DMA channel can be confugured as 0 or 5 only
#endif
#if MICROHAL_I2C1_DMA_TX_STREAM != 6 && MICROHAL_I2C1_DMA_TX_STREAM != 7
#error I2C TX DMA channel can be confugured as 6 or 7 only
#endif
I2C_dma I2C_dma::i2c1(*I2C1, DMA::dma1->stream[MICROHAL_I2C1_DMA_RX_STREAM], DMA::dma1->stream[MICROHAL_I2C1_DMA_TX_STREAM]);
I2C &I2C::i2c1 = I2C_dma::i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_DMA
#if MICROHAL_I2C2_DMA_RX_STREAM != 2 && MICROHAL_I2C2_DMA_RX_STREAM != 3
#error I2C RX DMA channel can be confugured as 2 or 3 only
#endif
I2C_dma I2C_dma::i2c2(*I2C2, DMA::dma1->stream[MICROHAL_I2C2_DMA_RX_STREAM], DMA::dma1->stream[7]);
I2C &I2C::i2c2 = I2C_dma::i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_DMA
I2C_dma I2C_dma::i2c3(*I2C3, DMA::dma1->stream[2], DMA::dma1->stream[4]);
I2C &I2C::i2c3 = I2C_dma::i2c3;
#endif
/* ************************************************************************************************
 *                                   Functions prototypes
 * ***********************************************************************************************/
static DMA::Stream::Channel getChannalNumber(I2C_TypeDef &i2c);

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

    while (i2c.SR1 & I2C_SR1_BTF) {
    }
    i2c.CR2 &= ~I2C_CR2_DMAEN & ~I2C_CR2_LAST;
    i2c.CR1 |= I2C_CR1_START;

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

    txStream.setNumberOfItemsToTransfer(length);
    txStream.setMemoryBank0(data);
    txStream.enable();

    errorSemaphore = UnknownError;

    while (i2c.SR1 & I2C_SR1_BTF) {
    }
    i2c.CR2 &= ~I2C_CR2_DMAEN & ~I2C_CR2_LAST;
    i2c.CR1 |= I2C_CR1_START;

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

    rxStream.setNumberOfItemsToTransfer(1);
    rxStream.setMemoryBank0(&data);
    rxStream.enable();

    errorSemaphore = UnknownError;

    while (i2c.SR1 & I2C_SR1_BTF) {
    }
    i2c.CR2 &= ~I2C_CR2_DMAEN & ~I2C_CR2_LAST;
    i2c.CR1 |= I2C_CR1_START;

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

    rxStream.setNumberOfItemsToTransfer(length);
    rxStream.setMemoryBank0(data);
    rxStream.enable();

    errorSemaphore = UnknownError;

    while (i2c.SR1 & I2C_SR1_BTF) {
    }
    i2c.CR2 &= ~I2C_CR2_DMAEN & ~I2C_CR2_LAST;
    i2c.CR1 |= I2C_CR1_START;  // | I2C_CR1_PE;

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
    rxStream.deinit();
    rxStream.init(getChannalNumber(i2c), DMA::Stream::MemoryBurst::SingleTransfer, DMA::Stream::PeripheralBurst::SingleTransfer,
                  DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte, DMA::Stream::MemoryIncrementMode::PointerIncremented,
                  DMA::Stream::PeripheralIncrementMode::PointerFixed, DMA::Stream::TransmisionDirection::PerToMem);
    rxStream.setPeripheralAddress(&i2c.DR);
    rxStream.enableInterrupt(DMA::Stream::Interrupt::TransferComplete);

    // tx
    txStream.deinit();
    txStream.init(getChannalNumber(i2c), DMA::Stream::MemoryBurst::SingleTransfer, DMA::Stream::PeripheralBurst::SingleTransfer,
                  DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte, DMA::Stream::MemoryIncrementMode::PointerIncremented,
                  DMA::Stream::PeripheralIncrementMode::PointerFixed, DMA::Stream::TransmisionDirection::MemToPer);
    txStream.setPeripheralAddress(&i2c.DR);
    txStream.enableInterrupt(DMA::Stream::Interrupt::TransferComplete);

    IRQn_Type rxIRQ = DMA2_Stream0_IRQn, txIRQ = DMA2_Stream0_IRQn;
    switch (reinterpret_cast<uint32_t>(&i2c)) {
        case reinterpret_cast<uint32_t>(I2C1):
#if MICROHAL_I2C1_DMA_RX_STREAM == 0
            rxIRQ = DMA2_Stream0_IRQn;
#elif MICROHAL_I2C1_DMA_RX_STREAM == 5
            rxIRQ = DMA2_Stream5_IRQn;
#endif
#if MICROHAL_I2C1_DMA_TX_STREAM == 6
            txIRQ = DMA2_Stream6_IRQn;
#elif MICROHAL_I2C1_DMA_TX_STREAM == 7
            txIRQ = DMA2_Stream7_IRQn;
#endif
            break;
        case reinterpret_cast<uint32_t>(I2C2):
#if MICROHAL_I2C2_DMA_RX_STREAM == 2
            rxIRQ = DMA1_Stream2_IRQn;
#elif MICROHAL_I2C2_DMA_RX_STREAM == 3
            rxIRQ = DMA1_Stream3_IRQn;
#endif
            txIRQ = DMA1_Stream7_IRQn;
            break;
        case reinterpret_cast<uint32_t>(I2C3):
            rxIRQ = DMA1_Stream2_IRQn;
            txIRQ = DMA1_Stream4_IRQn;
            break;
    }

    DMA::dma1->clearInterruptFlag(rxStream, DMA::Stream::Interrupt::TransferComplete);
    NVIC_EnableIRQ(rxIRQ);
    NVIC_SetPriority(rxIRQ, 0);

    DMA::dma1->clearInterruptFlag(txStream, DMA::Stream::Interrupt::TransferComplete);
    NVIC_EnableIRQ(txIRQ);
    NVIC_SetPriority(txIRQ, 0);

    i2c.CR1 |= I2C_CR1_PE;
}
/**
 *
 * @param i2c
 * @return
 */
DMA::Stream::Channel getChannalNumber(I2C_TypeDef &i2c) {
    switch (reinterpret_cast<uint32_t>(&i2c)) {
        case reinterpret_cast<uint32_t>(I2C1):
            // in stream 0 and 5 the same channel
            return DMA::Stream::Channel::Channel1;
        case reinterpret_cast<uint32_t>(I2C2):
            // in stream 2 and 3 the same channel
            return DMA::Stream::Channel::Channel7;
        case reinterpret_cast<uint32_t>(I2C3):
            return DMA::Stream::Channel::Channel3;
    }
    // error
    while (1) {
    }
    // return for avoid warning
    return DMA::Stream::Channel::Channel0;
}
//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void I2C_dma::IRQFunction(I2C_dma &obj, I2C_TypeDef *i2c) {
    uint16_t sr1 = i2c->SR1;

    if (sr1 == I2C_SR1_SB) {  // sent start sequence
        i2c->DR = obj.transfer.deviceAddress | (obj.transfer.mode & 0x01);
    } else if (sr1 & I2C_SR1_ADDR) {                      // adres(MASTER MODE) was sent
        __attribute__((unused)) uint16_t tmp = i2c->SR2;  // to clear interrupt flag register SR2 read is necessarily

        switch (obj.transfer.mode) {
            case I2C_dma::Transmit:
                i2c->DR = obj.transfer.registerAddress;
                break;

            case I2C_dma::Receive:
                i2c->CR2 |= I2C_CR2_DMAEN | I2C_CR2_LAST;
                break;

            case I2C_dma::ReceiveFromRegister:
                obj.transfer.mode = I2C_dma::Receive;
                i2c->DR = obj.transfer.registerAddress;
                break;
            default:
                i2c->CR1 |= I2C_CR1_STOP;
                obj.errorSemaphore = I2C_dma::NoError;
        }
        ////} else if (sr1 & (I2C_SR1_RXNE | I2C_SR1_BTF)) {
        ////        I2C2->CR1 |= I2C_CR1_STOP;
        ////        if (obj.transfer.length == 2) {
        ////            *obj.transfer.buffer_ptr++ = i2c->DR;
        ////        }
        ////        *obj.transfer.buffer_ptr = i2c->DR;
        ////
        ////        obj.ErrorSemaphore = I2C_interrupt::NoError;
    } else if (sr1 & (I2C_SR1_TXE | I2C_SR1_BTF)) {
        switch (obj.transfer.mode) {
            case I2C_dma::Receive:
                if (obj.transfer.length == 1) {
                    i2c->CR1 = (i2c->CR1 & (~I2C_CR1_ACK & ~I2C_CR1_POS)) | I2C_CR1_START;
                } else {
                    i2c->CR1 |= I2C_CR1_ACK | I2C_CR1_POS | I2C_CR1_START;
                }
                break;
            case I2C_dma::Transmit:
                if (obj.transfer.length > 0) {
                    i2c->CR2 |= I2C_CR2_DMAEN;
                } else {
                    i2c->CR1 |= I2C_CR1_STOP;
                    obj.errorSemaphore = I2C_dma::NoError;
                }
                break;
            default:
                i2c->CR1 |= I2C_CR1_STOP;
                obj.errorSemaphore = I2C_dma::NoError;
                break;
        }
    }
}

void I2C_dma::IRQErrorFunction(I2C_dma &obj, I2C_TypeDef *i2c) {
    // Disable rx and tx DMA streams
    obj.rxStream.disable();
    obj.txStream.disable();
    // send stop signal to I2C data bus
    i2c->CR1 |= I2C_CR1_STOP;
    // read error and store it in variable
    obj.errorSemaphore = I2C::errorCheckAndClear(i2c, i2c->SR1);
}
//***********************************************************************************************//
//                                         DMA IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_I2C1_DMA
//***********************************************************************************************//
#if MICROHAL_I2C1_DMA_RX_STREAM == 0
void DMA1_Stream0_IRQHandler(void) {
    // I2C rx
    I2C2->CR1 |= I2C_CR1_STOP;

    DMA1_Stream0->CR &= ~DMA_SxCR_EN;
    DMA1->LIFCR = DMA_LIFCR_CTCIF0;

    I2C_dma::i2c1.errorSemaphore = I2C_dma::NoError;
}
#endif
//***********************************************************************************************//
#if MICROHAL_I2C1_DMA_RX_STREAM == 5
void DMA1_Stream5_IRQHandler(void) {
    // I2C tx
    I2C2->CR1 |= I2C_CR1_STOP;

    DMA1_Stream5->CR &= ~DMA_SxCR_EN;
    DMA1->HIFCR = DMA_LIFCR_HTCIF5;

    I2C_dma::i2c1.errorSemaphore = I2C_dma::NoError;
}
#endif
//***********************************************************************************************//
#if MICROHAL_I2C1_DMA_TX_STREAM == 6
void DMA1_Stream6_IRQHandler(void) {
    // I2C rx
    DMA1_Stream6->CR &= ~DMA_SxCR_EN;
    DMA1->HIFCR = DMA_HIFCR_CTCIF6;

    I2C2->CR1 |= I2C_CR1_STOP;

    I2C_dma::i2c1.errorSemaphore = I2C_dma::NoError;
}
#endif
//***********************************************************************************************//
#if MICROHAL_I2C1_DMA_TX_STREAM == 6
void DMA1_Stream7_IRQHandler(void) {
    // I2C tx
    DMA1_Stream7->CR &= ~DMA_SxCR_EN;
    DMA1->HIFCR = DMA_LIFCR_HTCIF7;

    I2C2->CR1 |= I2C_CR1_STOP;

    I2C_dma::i2c1.errorSemaphore = I2C_dma::NoError;
}
#endif
#endif
#ifdef MICROHAL_USE_I2C2_DMA
//***********************************************************************************************//
#if MICROHAL_I2C2_DMA_RX_STREAM == 2
void DMA1_Stream2_IRQHandler(void) {
    I2C2->CR1 |= I2C_CR1_STOP;

    DMA1_Stream3->CR &= ~DMA_SxCR_EN;
    DMA1->LIFCR = DMA_LIFCR_CTCIF3;

    I2C_dma::i2c2.errorSemaphore = I2C_dma::NoError;
}
#endif
//***********************************************************************************************//
#if MICROHAL_I2C2_DMA_RX_STREAM == 3
void DMA1_Stream3_IRQHandler(void) {
    I2C2->CR1 |= I2C_CR1_STOP;
    DMA1_Stream3->CR &= ~DMA_SxCR_EN;

    DMA1->LIFCR = DMA_LIFCR_CTCIF3 | DMA_LIFCR_CTEIF3;

    I2C_dma::i2c2.errorSemaphore = I2C_dma::NoError;
}
#endif
//***********************************************************************************************//
void DMA1_Stream7_IRQHandler(void) {
    I2C_dma::i2c2.transfer.length = 0;
    DMA1_Stream7->CR &= ~DMA_SxCR_EN;
    DMA1->HIFCR = DMA_HIFCR_CTCIF7 | DMA_HIFCR_CTEIF7;
}
#endif
#ifdef MICROHAL_USE_I2C3_DMA
//***********************************************************************************************//
void DMA1_Stream2_IRQHandler(void) {
    // rx
    I2C2->CR1 |= I2C_CR1_STOP;

    DMA1_Stream2->CR &= ~DMA_SxCR_EN;
    DMA1->LIFCR = DMA_LIFCR_CTCIF2;

    I2C_dma::i2c3.errorSemaphore = I2C_dma::NoError;
}
//***********************************************************************************************//
void DMA1_Stream4_IRQHandler(void) {
    // tx
    DMA1_Stream4->CR &= ~DMA_SxCR_EN;
    DMA1->LIFCR = DMA_LIFCR_CTCIF4;

    I2C3->CR1 |= I2C_CR1_STOP;

    I2C_dma::i2c3.errorSemaphore = I2C_dma::NoError;
}
#endif
//***********************************************************************************************//
//                                          IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_I2C1_DMA
void I2C2_EV_IRQHandler(void) {
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
}  // namespace stm32f4xx
}  // namespace microhal
