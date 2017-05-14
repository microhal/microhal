/* ************************************************************************************************
 * INCLUDES
 */
#include "spi_dma_stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {
//***********************************************************************************************//
//                                          SPI objects
//***********************************************************************************************//
#ifdef MICROHAL_USE_SPI1_DMA
#if MICROHAL_SPI1_DMA_RX_STREAM != 0 && MICROHAL_SPI1_DMA_RX_STREAM != 2
#error SPI RX DMA channel can be confugured as 0 or 2 only
#endif
#if MICROHAL_SPI1_DMA_TX_STREAM != 3 && MICROHAL_SPI1_DMA_TX_STREAM != 5
#error SPI TX DMA channel can be confugured as 3 or 5 only
#endif
GPIO::IOPin spi1MisoPin(GPIO::PortB, 4);
SPI_dma SPI_dma::spi1(*SPI1, *DMA::dma1,
                      DMA::dma1->stream[MICROHAL_SPI1_DMA_RX_STREAM-1],
                      DMA::dma1->stream[MICROHAL_SPI1_DMA_TX_STREAM-1],
                      spi1MisoPin);
SPI &SPI::spi1 = SPI_dma::spi1;
#endif
//#ifdef MICROHAL_USE_SPI2_DMA
//GPIO::IOPin spi2MisoPin(GPIO::PortB, 4);
//SPI_dma SPI_dma::spi2(*SPI2, *DMA::dma1, DMA::dma1->stream[3],
//                                            DMA::dma1->stream[4], spi2MisoPin);
//SPI &SPI::spi2 = SPI_dma::spi2;
//#endif
//#ifdef MICROHAL_USE_SPI3_DMA
//#if MICROHAL_SPI3_DMA_RX_STREAM != 0 && MICROHAL_SPI3_DMA_RX_STREAM != 2
//#error SPI RX DMA channel can be confugured as 0 or 2 only
//#endif
//#if MICROHAL_SPI3_DMA_TX_STREAM != 5 && MICROHAL_SPI3_DMA_TX_STREAM != 7
//#error SPI TX DMA channel can be confugured as 5 or 7 only
//#endif
//GPIO::IOPin spi3MisoPin(GPIO::PortB, 4);
//SPI_dma SPI_dma::spi3(*SPI3, *DMA::dma1,
//                                            DMA::dma1->stream[MICROHAL_SPI3_DMA_RX_STREAM],
//                                            DMA::dma1->stream[MICROHAL_SPI3_DMA_TX_STREAM],
//                                            spi3MisoPin);
//SPI &SPI::spi3 = SPI_dma::spi3;
//#endif
//***********************************************************************************************//
//                                           Functions
//***********************************************************************************************//
inline SPI::Error SPI_dma::write(const void *data, size_t len, bool last) {
    txStream.numberOfItemsToTransfer(len);
    txStream.memoryAddress(const_cast<void*>(data));
    txStream.enable();

    spi.CR2 |= SPI_CR2_TXDMAEN;

    if (semaphore.wait(std::chrono::milliseconds::max())) {
    	if (last) {
    		while (spi.SR & SPI_SR_FTLVL_Msk) {}
    		while (spi.SR & SPI_SR_BSY) {}
    	}
    	return NoError;
    }
    return Error::UnknownError;
}

SPI::Error SPI_dma::writeRead(const void *writePtr, void *readPtr, size_t writeLen, size_t readLen) {
    if (writeLen < readLen) {
    	writeLen = readLen;
    	txStream.memoryIncrement(DMA::Channel::MemoryIncrementMode::PointerFixed);
    } else {
    	txStream.memoryIncrement(DMA::Channel::MemoryIncrementMode::PointerIncremented);
    }
    // configure tx
    txStream.numberOfItemsToTransfer(writeLen);
    txStream.memoryAddress(const_cast<void*>(writePtr));

    // configure rx
    rxStream.numberOfItemsToTransfer(readLen);
    rxStream.memoryAddress(readPtr);

    while (spi.SR & SPI_SR_FTLVL_Msk) {}
    while ((spi.SR & SPI_SR_BSY)) {}
    while (spi.SR & SPI_SR_FRLVL_Msk) {
    	volatile uint8_t tmp __attribute__((unused)) = spi.DR;
    }

    spi.CR2 |= SPI_CR2_RXDMAEN;
    txStream.enable();
    rxStream.enable();

    txStream.enableInterrupt(DMA::Channel::Interrupt::TransferComplete);
    spi.CR2 |= SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN;

    if (semaphore.wait(std::chrono::milliseconds::max())) {
		while (spi.SR & SPI_SR_FTLVL_Msk) {}
		while (spi.SR & SPI_SR_BSY) {}
		rxStream.disable();
		spi.CR2 &= ~SPI_CR2_TXDMAEN & ~SPI_CR2_RXDMAEN;
		return NoError;
    }
    return Error::UnknownError;
}

void SPI_dma::init() {
    dma.clockEnable();
    // rx
    rxStream.deinit();
    rxStream.init(
            DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte,
            DMA::Channel::MemoryIncrementMode::PointerIncremented,
            DMA::Channel::PeripheralIncrementMode::PointerFixed,
            DMA::Channel::TransmisionDirection::PerToMem);
    rxStream.peripheralAddress(&spi.DR);
    rxStream.enableInterrupt(DMA::Channel::Interrupt::TransferComplete);
    // tx
    txStream.deinit();
    txStream.init(
            DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte,
            DMA::Channel::MemoryIncrementMode::PointerIncremented,
            DMA::Channel::PeripheralIncrementMode::PointerFixed,
            DMA::Channel::TransmisionDirection::MemToPer);
    txStream.peripheralAddress(&spi.DR);
    txStream.enableInterrupt(DMA::Channel::Interrupt::TransferComplete);

    IRQn_Type rxIRQ = DMA1_Channel2_IRQn, txIRQ = DMA1_Channel3_IRQn;

    dma.clearInterruptFlag(rxStream, DMA::Channel::Interrupt::TransferComplete);
    NVIC_EnableIRQ(rxIRQ);
    NVIC_SetPriority(rxIRQ, 6);

    dma.clearInterruptFlag(txStream, DMA::Channel::Interrupt::TransferComplete);
    NVIC_EnableIRQ(txIRQ);
    NVIC_SetPriority(txIRQ, 6);
}

//***********************************************************************************************//
//                                    interrupt functions
//***********************************************************************************************//
void SPI_dma::IRQfunction(SPI_dma &object, SPI_TypeDef *spi) {
    const SPI::Error error = SPI::errorCheck(spi->SR);

    if (error != SPI::NoError) {
        bool shouldYeld = object.semaphore.giveFromISR();
#if defined (HAL_RTOS_FreeRTOS)
        portYIELD_FROM_ISR( shouldYeld );
#else
        (void)shouldYeld;
#endif
    }
}

//***********************************************************************************************//
//                                       DMA IRQHandlers
//***********************************************************************************************//
#ifdef MICROHAL_USE_SPI1_DMA
#if MICROHAL_SPI1_DMA_RX_STREAM == 2
void DMA1_Channel2_IRQHandler(void) {
    // spi1 rx
    DMA1->IFCR = DMA_IFCR_CTCIF2;
    SPI1->CR2 &= ~SPI_CR2_RXDMAEN & ~SPI_CR2_TXDMAEN;
    DMA1_Channel2->CCR &= ~DMA_CCR_EN;
#if MICROHAL_SPI1_DMA_TX_STREAM == 3
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;
#endif
#if MICROHAL_SPI1_DMA_TX_STREAM == 5
    DMA1_Channel5->CCR &= ~DMA_CCR_EN;
#endif
    bool shouldYeld = SPI_dma::spi1.semaphore.giveFromISR();
#if defined (HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR( shouldYeld );
#else
    (void)shouldYeld;
#endif
}
#endif
#if MICROHAL_SPI1_DMA_TX_STREAM == 3
void DMA1_Channel3_IRQHandler(void) {
    // spi1 tx
    DMA1->IFCR = DMA_IFCR_CTCIF3;
    SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;

    bool shouldYeld = SPI_dma::spi1.semaphore.giveFromISR();
#if defined (HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR( shouldYeld );
#else
    (void)shouldYeld;
#endif
}
#endif
#if MICROHAL_SPI1_DMA_TX_STREAM == 5
void DMA1_Channel5_IRQHandler(void) {
    // spi1 tx
    DMA1->IFCR = DMA_IFCR_CTCIF5;
    SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
    DMA1_Channel5->CCR &= ~DMA_CCR_EN;

    bool shouldYeld = SPI_dma::spi1.semaphore.giveFromISR();
#if defined (HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR( shouldYeld );
#else
    (void)shouldYeld;
#endif
}
#endif
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
void SPI2_IRQHandler(void) { SPI_dma::IRQfunction(SPI_dma::spi2, SPI2); }
#endif
#ifdef MICROHAL_USE_SPI3_DMA
void SPI3_IRQHandler(void) { SPI_dma::IRQfunction(SPI_dma::spi3, SPI3); }
#endif

} // namespace stm32f3xx
} // namespace microhal
