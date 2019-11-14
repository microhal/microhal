/*
 * SPI_dma_stm32f4xx.cpp
 *
 *  Created on: 17 kwi 2014
 *      Author: pawel
 */
/* ************************************************************************************************
 * INCLUDES
 */
#include "spi_dma_stmCommon.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
//***********************************************************************************************//
//                                         SPI objects //
//***********************************************************************************************//
#if defined(MICROHAL_USE_SPI1_DMA) && MICROHAL_USE_SPI1_DMA == 1
SPI_dma *SPI_dma::spi1;
#endif
#if defined(MICROHAL_USE_SPI2_DMA) && MICROHAL_USE_SPI2_DMA == 1
SPI_dma *SPI_dma::spi2;
#endif
#if defined(MICROHAL_USE_SPI3_DMA) && MICROHAL_USE_SPI3_DMA == 1
SPI_dma *SPI_dma::spi3;
#endif
#if defined(MICROHAL_USE_SPI4_DMA) && MICROHAL_USE_SPI4_DMA == 1
SPI_dma *SPI_dma::spi4;
#endif
#if defined(MICROHAL_USE_SPI5_DMA) && MICROHAL_USE_SPI5_DMA == 1
SPI_dma *SPI_dma::spi5;
#endif
#if defined(MICROHAL_USE_SPI6_DMA) && MICROHAL_USE_SPI6_DMA == 1
SPI_dma *SPI_dma::spi6;
#endif
//***********************************************************************************************//
//                                         Functions
//***********************************************************************************************//
inline SPI::Error SPI_dma::write(const void *data, size_t len, bool /*last*/) {
    txStream.setMemoryIncrement(DMA::Stream::MemoryIncrementMode::PointerIncremented);
    txStream.setNumberOfItemsToTransfer(len);
    txStream.setMemoryBank0(data);
    txStream.enable();

    auto cr2 = spi.cr2.volatileLoad();
    cr2.TXDMAEN.set();
    spi.cr2.volatileStore(cr2);

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
    spi.dr.volatileLoad();

    rxStream.enable();
    txStream.enable();

    auto cr2 = spi.cr2.volatileLoad();
    cr2.RXDMAEN.set();
    cr2.TXDMAEN.set();
    spi.cr2.volatileStore(cr2);

    // spi.CR2 |= SPI_CR2_TXDMAEN;
    txStream.enableInterrupt(DMA::Stream::Interrupt::TransferComplete);
    // rxStream.enableInterrupt(DMA::Stream::Interrupt::TransferComplete);
    if (semaphore.wait(std::chrono::milliseconds::max())) {
        busyWait();
        rxStream.disable();
        auto cr2 = spi.cr2.volatileLoad();
        cr2.RXDMAEN.clear();
        spi.cr2.volatileStore(cr2);
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
    rxStream.setPeripheralAddress(&spi.dr);
    rxStream.enableInterrupt(DMA::Stream::Interrupt::TransferComplete);
    // tx
    txStream.deinit();
    txStream.init(dma.channel(txStream, &spi), DMA::Stream::MemoryBurst::SingleTransfer, DMA::Stream::PeripheralBurst::SingleTransfer,
                  DMA::Stream::MemoryDataSize::Byte, DMA::Stream::PeripheralDataSize::Byte, DMA::Stream::MemoryIncrementMode::PointerIncremented,
                  DMA::Stream::PeripheralIncrementMode::PointerFixed, DMA::Stream::TransmisionDirection::MemToPer);
    txStream.setPeripheralAddress(&spi.dr);
    txStream.enableInterrupt(DMA::Stream::Interrupt::TransferComplete);

    dma.clearInterruptFlag(rxStream, DMA::Stream::Interrupt::TransferComplete);
    dma.enableInterrupt(rxStream, 6);

    dma.clearInterruptFlag(txStream, DMA::Stream::Interrupt::TransferComplete);
    dma.enableInterrupt(txStream, 6);
}

//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void SPI_dma::IRQfunction(SPI_dma &object, registers::SPI *spi) {
    const SPI::Error error = SPI::errorCheck(spi->sr.volatileLoad());

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
#if defined(MICROHAL_USE_SPI1_DMA) && MICROHAL_USE_SPI1_DMA == 1
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
    SPI_dma::spi1->semaphore = false;
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
    bool shouldYeld = SPI_dma::spi1->semaphore.giveFromISR();
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

    SPI_dma::spi1->semaphore.giveFromISR();
}
#endif
#if MICROHAL_SPI1_DMA_TX_STREAM == 5
void DMA2_Stream5_IRQHandler(void) {
    // spi1 tx
    DMA2->HIFCR = DMA_HIFCR_CTCIF5;
    SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
    DMA2_Stream5->CR &= ~DMA_SxCR_EN;

    bool shouldYeld = SPI_dma::spi1->semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#else
    (void)shouldYeld;
#endif
}
#endif
#endif
#if defined(MICROHAL_USE_SPI2_DMA) && MICROHAL_USE_SPI2_DMA == 1
void DMA1_Stream3_IRQHandler(void) {
    // spi2 rx
    // clear DMA Stream3 interrupt flag
    DMA1->LIFCR = DMA_LIFCR_CTCIF3;
    // disable DMA Stream3
    DMA1_Stream3->CR &= ~DMA_SxCR_EN;
    DMA1_Stream4->CR &= ~DMA_SxCR_EN;
    // disable SPI DMA request
    SPI2->CR2 &= ~SPI_CR2_RXDMAEN & ~SPI_CR2_TXDMAEN;

    bool shouldYeld = SPI_dma::spi2->semaphore.giveFromISR();
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

    bool shouldYeld = SPI_dma::spi2->semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#endif
}
#endif
#if defined(MICROHAL_USE_SPI3_DMA) && MICROHAL_USE_SPI3_DMA == 1
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

    bool shouldYeld = SPI_dma::spi3->semaphore.giveFromISR();
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

    SPI_dma::spi3->semaphore.giveFromISR();
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

    SPI_dma::spi3->semaphore.giveFromISR();
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

    bool shouldYeld = SPI_dma::spi3->semaphore.giveFromISR();
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
#if defined(MICROHAL_USE_SPI1_DMA) && MICROHAL_USE_SPI1_DMA == 1
extern "C" void SPI1_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi1, registers::spi1);
}
#endif
#if defined(MICROHAL_USE_SPI2_DMA) && MICROHAL_USE_SPI2_DMA == 1
void SPI2_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi2, registers::spi2);
}
#endif
#if defined(MICROHAL_USE_SPI3_DMA) && MICROHAL_USE_SPI3_DMA == 1
void SPI3_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi3, registers::spi3);
}
#endif
#if defined(MICROHAL_USE_SPI4_DMA) && MICROHAL_USE_SPI4_DMA == 1
void SPI4_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi4, registers::spi4);
}
#endif
#if defined(MICROHAL_USE_SPI5_DMA) && MICROHAL_USE_SPI5_DMA == 1
void SPI5_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi5, registers::spi5);
}
#endif
#if defined(MICROHAL_USE_SPI6_DMA) && MICROHAL_USE_SPI6_DMA == 1
void SPI6_IRQHandler(void) {
    SPI_dma::IRQfunction(*SPI_dma::spi6, registers::spi6);
}
#endif
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
