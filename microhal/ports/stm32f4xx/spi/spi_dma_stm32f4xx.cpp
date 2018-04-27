/*
 * SPI_dma_stm32f4xx.cpp
 *
 *  Created on: 17 kwi 2014
 *      Author: pawel
 */
/* ************************************************************************************************
 * INCLUDES
 */
#include "spi_dma_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
//***********************************************************************************************//
//                                         SPI objects //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SPI1_DMA
#if MICROHAL_SPI1_DMA_RX_STREAM != 0 && MICROHAL_SPI1_DMA_RX_STREAM != 2
#error SPI RX DMA channel can be confugured as 0 or 2 only
#endif
#if MICROHAL_SPI1_DMA_TX_STREAM != 3 && MICROHAL_SPI1_DMA_TX_STREAM != 5
#error SPI TX DMA channel can be confugured as 3 or 5 only
#endif
GPIO::IOPin spi1MisoPin(GPIO::PortB, 4);
SPI_dma SPI_dma::spi1(*SPI1, *DMA::dma2, DMA::dma2->stream[MICROHAL_SPI1_DMA_RX_STREAM], DMA::dma2->stream[MICROHAL_SPI1_DMA_TX_STREAM], spi1MisoPin);
SPI &SPI::spi1 = SPI_dma::spi1;
#endif
#ifdef MICROHAL_USE_SPI2_DMA
GPIO::IOPin spi2MisoPin(GPIO::PortB, 4);
SPI_dma SPI_dma::spi2(*SPI2, *DMA::dma1, DMA::dma1->stream[3], DMA::dma1->stream[4], spi2MisoPin);
SPI &SPI::spi2 = SPI_dma::spi2;
#endif
#ifdef MICROHAL_USE_SPI3_DMA
#if MICROHAL_SPI3_DMA_RX_STREAM != 0 && MICROHAL_SPI3_DMA_RX_STREAM != 2
#error SPI RX DMA channel can be confugured as 0 or 2 only
#endif
#if MICROHAL_SPI3_DMA_TX_STREAM != 5 && MICROHAL_SPI3_DMA_TX_STREAM != 7
#error SPI TX DMA channel can be confugured as 5 or 7 only
#endif
GPIO::IOPin spi3MisoPin(GPIO::PortB, 4);
SPI_dma SPI_dma::spi3(*SPI3, *DMA::dma1, DMA::dma1->stream[MICROHAL_SPI3_DMA_RX_STREAM], DMA::dma1->stream[MICROHAL_SPI3_DMA_TX_STREAM], spi3MisoPin);
SPI &SPI::spi3 = SPI_dma::spi3;
#endif
#ifdef MICROHAL_USE_SPI4_DMA
#if MICROHAL_SPI4_DMA_RX_STREAM != 0 && MICROHAL_SPI4_DMA_RX_STREAM != 3
#error SPI RX DMA channel can be confugured as 0 or 3 only
#endif
#if MICROHAL_SPI4_DMA_TX_STREAM != 1 && MICROHAL_SPI4_DMA_TX_STREAM != 4
#error SPI TX DMA channel can be confugured as 1 or 4 only
#endif
GPIO::IOPin spi4MisoPin(GPIO::PortB, 4);
SPI_dma SPI_dma::spi4(*SPI4, *DMA::dma2, DMA::dma2->stream[MICROHAL_SPI4_DMA_RX_STREAM], DMA::dma2->stream[MICROHAL_SPI4_DMA_TX_STREAM], spi4MisoPin);
SPI &SPI::spi4 = SPI_dma::spi4;
#endif
#ifdef MICROHAL_USE_SPI5_DMA
#if MICROHAL_SPI5_DMA_RX_STREAM != 3 && MICROHAL_SPI5_DMA_RX_STREAM != 5
#error SPI RX DMA channel can be confugured as 3 or 5 only
#endif
#if MICROHAL_SPI5_DMA_TX_STREAM != 4 && MICROHAL_SPI5_DMA_TX_STREAM != 6
#error SPI TX DMA channel can be confugured as 4 or 6 only
#endif
GPIO::IOPin spi5MisoPin(GPIO::PortB, 4);
SPI_dma SPI_dma::spi5(*SPI5, *DMA::dma2, DMA::dma2->stream[MICROHAL_SPI5_DMA_RX_STREAM], DMA::dma2->stream[MICROHAL_SPI5_DMA_TX_STREAM], spi5MisoPin);
SPI &SPI::spi5 = SPI_dma::spi5;
#endif
#ifdef MICROHAL_USE_SPI6_DMA
SPI_dma SPI_dma::spi6(*SPI6, *DMA::dma2, DMA::dma2->stream[5], DMA::dma2->stream[6], spi1MisoPin);
SPI &SPI::spi6 = SPI_dma::spi6;
#endif
//***********************************************************************************************//
//                                         Functions
//***********************************************************************************************//
inline SPI::Error SPI_dma::write(const void *data, size_t len, bool /*last*/) {
    txStream.setMemoryIncrement(DMA::Stream::MemoryIncrementMode::PointerIncremented);
    txStream.setNumberOfItemsToTransfer(len);
    txStream.setMemoryBank0(data);
    txStream.enable();

    spi.CR2 |= SPI_CR2_TXDMAEN;

    if (semaphore.wait(std::chrono::milliseconds::max())) {
        //  if (last) {
        busyWait();
        // workaround, I don't know why but BSY flag is cleared in the middle of last bit. This may cause some error when other function will
        // deassert CS pin
        volatile uint32_t i = 150;
        while (i--) {
        }
        // }
        return Error::None;
    }

    return Error::Timeout;
}

SPI::Error SPI_dma::writeRead(const void *writePtr, void *readPtr, size_t writeLen, size_t readLen) {
    txStream.disableInterrupt(DMA::Stream::Interrupt::TransferComplete);
    if (writeLen < readLen) {
        writeLen = readLen;
        txStream.setMemoryIncrement(DMA::Stream::MemoryIncrementMode::PointerFixed);
    } else {
        txStream.setMemoryIncrement(DMA::Stream::MemoryIncrementMode::PointerIncremented);
    }
    // configure tx
    txStream.setNumberOfItemsToTransfer(writeLen);
    txStream.setMemoryBank0(writePtr);
    // configure rx
    rxStream.setNumberOfItemsToTransfer(readLen);
    rxStream.setMemoryBank0(readPtr);

    //    busyWait();
    volatile uint8_t tmp __attribute__((unused)) = spi.DR;

    rxStream.enable();
    txStream.enable();

    spi.CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;

    // spi.CR2 |= SPI_CR2_TXDMAEN;
    txStream.enableInterrupt(DMA::Stream::Interrupt::TransferComplete);
    // rxStream.enableInterrupt(DMA::Stream::Interrupt::TransferComplete);
    if (semaphore.wait(std::chrono::milliseconds::max())) {
        busyWait();
        rxStream.disable();
        spi.CR2 &= ~SPI_CR2_RXDMAEN;
        // workaround, I don't know why but (tested on stm32f407) BSY flag is cleared in the middle of last bit. This may cause some error when other
        // function will
        // deassert CS pin
        volatile uint32_t i = 150;
        while (i--) {
        }
        return Error::None;
    }
    return Error::Timeout;
}

void SPI_dma::init() {
    dma.clockEnable();
    // rx
    rxStream.deinit();
    rxStream.init(dma.channel(rxStream, &spi), DMA::Stream::MemoryBurst::SingleTransfer, DMA::Stream::PeripheralBurst::SingleTransfer,
                  DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte, DMA::Stream::MemoryIncrementMode::PointerIncremented,
                  DMA::Stream::PeripheralIncrementMode::PointerFixed, DMA::Stream::TransmisionDirection::PerToMem);
    rxStream.setPeripheralAddress(&spi.DR);
    rxStream.enableInterrupt(DMA::Stream::Interrupt::TransferComplete);
    // tx
    txStream.deinit();
    txStream.init(dma.channel(txStream, &spi), DMA::Stream::MemoryBurst::SingleTransfer, DMA::Stream::PeripheralBurst::SingleTransfer,
                  DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte, DMA::Stream::MemoryIncrementMode::PointerIncremented,
                  DMA::Stream::PeripheralIncrementMode::PointerFixed, DMA::Stream::TransmisionDirection::MemToPer);
    txStream.setPeripheralAddress(&spi.DR);
    txStream.enableInterrupt(DMA::Stream::Interrupt::TransferComplete);

    dma.clearInterruptFlag(rxStream, DMA::Stream::Interrupt::TransferComplete);
    dma.enableInterrupt(rxStream, 6);

    dma.clearInterruptFlag(txStream, DMA::Stream::Interrupt::TransferComplete);
    dma.enableInterrupt(txStream, 6);
}

//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void SPI_dma::IRQfunction(SPI_dma &object, SPI_TypeDef *spi) {
    const SPI::Error error = SPI::errorCheck(spi->SR);

    if (error != SPI::Error::None) {
        bool shouldYeld = object.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
        portYIELD_FROM_ISR(shouldYeld);
#else
        (void)shouldYeld;
#endif
    }
}

//***********************************************************************************************//
//                                         DMA IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SPI1_DMA
#if MICROHAL_SPI1_DMA_RX_STREAM == 1
void DMA2_Stream1_IRQHandler(void) {
    // spi1 rx
    DMA2->LIFCR = DMA_LIFCR_CTCIF1;
    SPI1->CR2 &= ~SPI_CR2_RXDMAEN & ~SPI_CR2_TXDMAEN;
    DMA2_Stream1->CR &= ~DMA_SxCR_EN;
#if MICROHAL_SPI1_DMA_TX_STREAM == 3
    DMA2_Stream3->CR &= ~DMA_SxCR_EN;
#endif
#if MICROHAL_SPI1_DMA_TX_STREAM == 5
    DMA2_Stream5->CR &= ~DMA_SxCR_EN;
#endif
    SPI_dma::spi1.semaphore = false;
}
#endif
#if MICROHAL_SPI1_DMA_RX_STREAM == 2
void DMA2_Stream2_IRQHandler(void) {
    // spi1 rx
    DMA2->LIFCR = DMA_LIFCR_CTCIF2;
    SPI1->CR2 &= ~SPI_CR2_RXDMAEN & ~SPI_CR2_TXDMAEN;
    DMA2_Stream2->CR &= ~DMA_SxCR_EN;
#if MICROHAL_SPI1_DMA_TX_STREAM == 3
    DMA2_Stream3->CR &= ~DMA_SxCR_EN;
#endif
#if MICROHAL_SPI1_DMA_TX_STREAM == 5
    DMA2_Stream5->CR &= ~DMA_SxCR_EN;
#endif
    bool shouldYeld = SPI_dma::spi1.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#else
    (void)shouldYeld;
#endif
}
#endif
#if MICROHAL_SPI1_DMA_TX_STREAM == 3
void DMA2_Stream3_IRQHandler(void) {
    // spi1 tx
    DMA2->LIFCR = DMA_LIFCR_CTCIF3;
    SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
    DMA2_Stream3->CR &= ~DMA_SxCR_EN;

    SPI_dma::spi1.semaphore.giveFromISR();
}
#endif
#if MICROHAL_SPI1_DMA_TX_STREAM == 5
void DMA2_Stream5_IRQHandler(void) {
    // spi1 tx
    DMA2->HIFCR = DMA_HIFCR_CTCIF5;
    SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
    DMA2_Stream5->CR &= ~DMA_SxCR_EN;

    bool shouldYeld = SPI_dma::spi1.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#else
    (void)shouldYeld;
#endif
}
#endif
#endif
#ifdef MICROHAL_USE_SPI2_DMA
void DMA1_Stream3_IRQHandler(void) {
    // spi2 rx
    // clear DMA Stream3 interrupt flag
    DMA1->LIFCR = DMA_LIFCR_CTCIF3;
    // disable DMA Stream3
    DMA1_Stream3->CR &= ~DMA_SxCR_EN;
    DMA1_Stream4->CR &= ~DMA_SxCR_EN;
    // disable SPI DMA request
    SPI2->CR2 &= ~SPI_CR2_RXDMAEN & ~SPI_CR2_TXDMAEN;

    bool shouldYeld = SPI_dma::spi2.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#endif
}
void DMA1_Stream4_IRQHandler(void) {
    // spi2 tx
    // clear DMA Stream4 interrupt flag
    DMA1->HIFCR = DMA_HIFCR_CTCIF4;
    // disable DMA Stream4
    DMA1_Stream4->CR &= ~DMA_SxCR_EN;
    // disable SPI DMA request
    SPI2->CR2 &= ~SPI_CR2_TXDMAEN;

    bool shouldYeld = SPI_dma::spi2.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#endif
}
#endif
#ifdef MICROHAL_USE_SPI3_DMA
#if MICROHAL_SPI3_DMA_RX_STREAM == 0
void DMA1_Stream0_IRQHandler(void) {
    // spi3 rx
    // clear DMA Stream3 interrupt flag
    DMA1->LIFCR = DMA_LIFCR_CTCIF0;
    // disable DMA Stream3
    DMA1_Stream0->CR &= ~DMA_SxCR_EN;
    // DMA1_Stream4->CR &= ~DMA_SxCR_EN;
    // disable SPI DMA request
    SPI3->CR2 &= ~SPI_CR2_RXDMAEN & ~SPI_CR2_TXDMAEN;

    bool shouldYeld = SPI_dma::spi3.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#endif
}
#endif
#if MICROHAL_SPI3_DMA_RX_STREAM == 2
void DMA1_Stream2_IRQHandler(void) {
    // spi3 rx
    // clear DMA Stream3 interrupt flag
    DMA1->LIFCR = DMA_LIFCR_CTCIF2;
    // disable DMA Stream3
    DMA1_Stream2->CR &= ~DMA_SxCR_EN;
    // DMA1_Stream4->CR &= ~DMA_SxCR_EN;
    // disable SPI DMA request
    SPI3->CR2 &= ~SPI_CR2_RXDMAEN & ~SPI_CR2_TXDMAEN;

    SPI_dma::spi3.semaphore.giveFromISR();
}
#endif
#if MICROHAL_SPI3_DMA_TX_STREAM == 5
void DMA1_Stream5_IRQHandler(void) {
    // spi3 tx
    // clear DMA Stream4 interrupt flag
    DMA1->HIFCR = DMA_HIFCR_CTCIF5;
    // disable DMA Stream4
    DMA1_Stream5->CR &= ~DMA_SxCR_EN;
    // disable SPI DMA request
    SPI3->CR2 &= ~SPI_CR2_TXDMAEN;

    SPI_dma::spi3.semaphore.giveFromISR();
}
#endif
#if MICROHAL_SPI3_DMA_TX_STREAM == 7
void DMA1_Stream7_IRQHandler(void) {
    // spi3 tx
    // clear DMA Stream4 interrupt flag
    DMA1->HIFCR = DMA_HIFCR_CTCIF7;
    // disable DMA Stream4
    DMA1_Stream7->CR &= ~DMA_SxCR_EN;
    // disable SPI DMA request
    SPI3->CR2 &= ~SPI_CR2_TXDMAEN;

    bool shouldYeld = SPI_dma::spi3.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#endif
}
#endif
#endif
#ifdef MICROHAL_USE_SPI4_DMA
#endif
#ifdef MICROHAL_USE_SPI5_DMA
#endif
#ifdef MICROHAL_USE_SPI6_DMA
#endif
//***********************************************************************************************//
//                                         SPI IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SPI1_DMA
extern "C" void SPI1_IRQHandler(void) {
    SPI_dma::IRQfunction(SPI_dma::spi1, SPI1);
}
#endif
#ifdef MICROHAL_USE_SPI2_DMA
void SPI2_IRQHandler(void) {
    SPI_dma::IRQfunction(SPI_dma::spi2, SPI2);
}
#endif
#ifdef MICROHAL_USE_SPI3_DMA
void SPI3_IRQHandler(void) {
    SPI_dma::IRQfunction(SPI_dma::spi3, SPI3);
}
#endif
#ifdef MICROHAL_USE_SPI4_DMA
void SPI4_IRQHandler(void) {
    SPI_dma::IRQfunction(SPI_dma::spi4, SPI4);
}
#endif
#ifdef MICROHAL_USE_SPI5_DMA
void SPI5_IRQHandler(void) {
    SPI_dma::IRQfunction(SPI_dma::spi5, SPI5);
}
#endif
#ifdef MICROHAL_USE_SPI6_DMA
void SPI6_IRQHandler(void) {
    SPI_dma::IRQfunction(SPI_dma::spi6, SPI6);
}
#endif
}  // namespace stm32f4xx
}  // namespace microhal
