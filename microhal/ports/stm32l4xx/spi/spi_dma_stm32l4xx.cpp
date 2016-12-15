/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 08-10-2016
 * last modification: 08-10-2016
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
#include "spi_dma_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {
//***********************************************************************************************//
//                                         SPI objects //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SPI1_DMA
#if MICROHAL_SPI1_DMA_RX_CHANNEL != 2 && MICROHAL_SPI1_DMA_RX_CHANNEL != 3
#error SPI RX DMA channel can be confugured as 0 or 2 only
#endif
#if MICROHAL_SPI1_DMA_TX_CHANNEL != 3 && MICROHAL_SPI1_DMA_TX_CHANNEL != 4
#error SPI TX DMA channel can be confugured as 3 or 5 only
#endif
GPIO::IOPin spi1MisoPin(GPIO::PortA, 6);

#if MICROHAL_SPI1_DMA_RX_CHANNEL == 2
SPI_dma SPI_dma::spi1(*SPI1, *DMA::dma1,
                      DMA::dma1->channel[MICROHAL_SPI1_DMA_RX_CHANNEL],
                      DMA::dma1->channel[MICROHAL_SPI1_DMA_TX_CHANNEL],
                      spi1MisoPin);
#else
SPI_dma SPI_dma::spi1(*SPI1, *DMA::dma2,
                      DMA::dma2->channel[MICROHAL_SPI1_DMA_RX_CHANNEL],
                      DMA::dma2->channel[MICROHAL_SPI1_DMA_TX_CHANNEL],
                      spi1MisoPin);
#endif
SPI &SPI::spi1 = SPI_dma::spi1;
#endif
#ifdef MICROHAL_USE_SPI2_DMA
GPIO::IOPin spi2MisoPin(GPIO::PortB, 4);
SPI_dma SPI_dma::spi2(*SPI2, *DMA::dma1, DMA::dma1->channel[4],
                      DMA::dma1->channel[5], spi2MisoPin);
SPI &SPI::spi2 = SPI_dma::spi2;
#endif
#ifdef MICROHAL_USE_SPI3_DMA
GPIO::IOPin spi3MisoPin(GPIO::PortB, 4);
SPI_dma SPI_dma::spi3(*SPI3, *DMA::dma1, DMA::dma2->stream[1],
                      DMA::dma2->stream[2], spi3MisoPin);
SPI &SPI::spi3 = SPI_dma::spi3;
#endif
//***********************************************************************************************//
//                                         Functions //
//***********************************************************************************************//
static DMA::Channel::Request getRxRequestNumber(SPI_TypeDef &spi) {
	switch (reinterpret_cast<uint32_t>(&spi)) {
    case reinterpret_cast<uint32_t>(SPI1):
#if MICROHAL_SPI1_DMA_RX_CHANNEL == 2
    	return DMA::Channel::Request::Request1;
#else
    	return DMA::Channel::Request::Request4;
#endif
    case reinterpret_cast<uint32_t>(SPI2):
		return DMA::Channel::Request::Request1;
    case reinterpret_cast<uint32_t>(SPI3):
    	return DMA::Channel::Request::Request3;
  }
  while (1)
    ;
  return DMA::Channel::Request::Request0;
}

static DMA::Channel::Request getTxRequestNumber(SPI_TypeDef &spi) {
	switch (reinterpret_cast<uint32_t>(&spi)) {
    case reinterpret_cast<uint32_t>(SPI1):
#if MICROHAL_SPI1_DMA_TX_CHANNEL == 3
    	return DMA::Channel::Request::Request1;
#else
    	return DMA::Channel::Request::Request4;
#endif
    case reinterpret_cast<uint32_t>(SPI2):
		return DMA::Channel::Request::Request1;
    case reinterpret_cast<uint32_t>(SPI3):
    	return DMA::Channel::Request::Request3;
  }
  while (1)
    ;
  return DMA::Channel::Request::Request0;
}

SPI::Error SPI_dma::write(uint8_t data, bool last) {
  return writeBuffer(&data, 1, last);
}
SPI::Error SPI_dma::read(uint8_t &data, uint8_t write) {
  return writeRead(&write, &data, 1, 1);
}
inline SPI::Error SPI_dma::writeBuffer(const void *data, size_t len, bool last) {
  //semaphore = true;

  txChannel.setNumberOfItemsToTransfer(len);
  txChannel.setMemory(data);
  txChannel.enable();

  spi.CR2 |= SPI_CR2_TXDMAEN;
  //while (semaphore) {
  //}
  semaphore.wait(std::chrono::milliseconds::max());

  if (last) {
    while (spi.SR & SPI_SR_BSY) {
    }
  }

  return NoError;
}
SPI::Error SPI_dma::readBuffer(void *data, size_t len, uint8_t write) {
  return writeRead(&write, data, 1, len);
}

SPI::Error SPI_dma::writeRead(void *writePtr, void *readPtr, size_t writeLen, size_t readLen) {
  //semaphore = true;

  txChannel.disableInterrupt(DMA::Channel::Interrupt::TransferComplete);

  if (writeLen < readLen) {
	  writeLen = readLen;
	  txChannel.setMemoryIncrement(DMA::Channel::MemoryIncrementMode::PointerFixed);
  }
  // configure tx
  txChannel.setNumberOfItemsToTransfer(writeLen);
  txChannel.setMemory(writePtr);
  txChannel.enable();
  // configure rx
  rxChannel.setNumberOfItemsToTransfer(readLen);
  rxChannel.setMemory(readPtr);
  rxChannel.enable();

  bool busy = false;
  if (spi.SR & SPI_SR_BSY) busy = true;

  spi.CR2 |= SPI_CR2_TXDMAEN;

  if (busy) {
    while (!(spi.SR & SPI_SR_RXNE)) {
    }
  }

  uint8_t tmp __attribute__((unused)) = spi.DR;

  spi.CR2 |= SPI_CR2_RXDMAEN;
  txChannel.enableInterrupt(DMA::Channel::Interrupt::TransferComplete);
  //rxChannel.enableInterrupt(DMA::Channel::Interrupt::TransferComplete);
  //while (semaphore) {
  //}
  semaphore.wait(std::chrono::milliseconds::max());
  txChannel.setMemoryIncrement(DMA::Channel::MemoryIncrementMode::PointerIncremented);
  // threadID = xTaskGetCurrentTaskHandle();
  // vTaskSuspend(threadID);

  return NoError;
}

void SPI_dma::init() {
  dma.clockEnable();
  // rx
  rxChannel.deinit();
  rxChannel.init(
      DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte,
      DMA::Channel::MemoryIncrementMode::PointerIncremented,
      DMA::Channel::PeripheralIncrementMode::PointerFixed,
      DMA::Channel::TransmisionDirection::PerToMem);
  rxChannel.setPeripheralAddress(&spi.DR);
  rxChannel.enableInterrupt(DMA::Channel::Interrupt::TransferComplete);
  dma.init(rxChannel, getRxRequestNumber(spi));
  // tx
  txChannel.deinit();
  txChannel.init(
      DMA::Channel::MemoryDataSize::Byte, DMA::Channel::PeripheralDataSize::Byte,
      DMA::Channel::MemoryIncrementMode::PointerIncremented,
      DMA::Channel::PeripheralIncrementMode::PointerFixed,
      DMA::Channel::TransmisionDirection::MemToPer);
  txChannel.setPeripheralAddress(&spi.DR);
  txChannel.enableInterrupt(DMA::Channel::Interrupt::TransferComplete);
  dma.init(txChannel, getTxRequestNumber(spi));

  IRQn_Type rxIRQ = DMA1_Channel4_IRQn, txIRQ = DMA1_Channel5_IRQn;

  switch (reinterpret_cast<uint32_t>(&spi)) {
    case reinterpret_cast<uint32_t>(SPI1):
#if MICROHAL_SPI1_DMA_RX_CHANNEL == 2
      rxIRQ = DMA1_Channel2_IRQn;
#elif (MICROHAL_SPI1_DMA_RX_CHANNEL == 3)
      rxIRQ = DMA2_Channel3_IRQn;
#endif
#if MICROHAL_SPI1_DMA_TX_CHANNEL == 3
      txIRQ = DMA1_Channel3_IRQn;
#elif MICROHAL_SPI1_DMA_TX_CHANNEL == 4
      txIRQ = DMA2_Channel4_IRQn;
#endif
      break;
    case reinterpret_cast<uint32_t>(SPI2):
      rxIRQ = DMA1_Channel4_IRQn;
      txIRQ = DMA1_Channel5_IRQn;
      break;
    case reinterpret_cast<uint32_t>(SPI3):
      rxIRQ = DMA2_Channel1_IRQn;
      txIRQ = DMA2_Channel2_IRQn;
      break;
  }

  dma.clearInterruptFlag(rxChannel, DMA::Channel::Interrupt::TransferComplete);
  NVIC_EnableIRQ(rxIRQ);
  NVIC_SetPriority(rxIRQ, 6);

  dma.clearInterruptFlag(txChannel, DMA::Channel::Interrupt::TransferComplete);
  NVIC_EnableIRQ(txIRQ);
  NVIC_SetPriority(txIRQ, 6);
}

//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void SPI_dma::IRQfunction(SPI_dma &object, SPI_TypeDef *spi) {
  const SPI::Error error = SPI::errorCheck(spi->SR);

  if (error != SPI::NoError) {
    //object.semaphore = false;
      bool shouldYeld = object.semaphore.giveFromISR();
#if defined (HAL_RTOS_FreeRTOS)
      portYIELD_FROM_ISR( shouldYeld );
#endif
  }
}

//***********************************************************************************************//
//                                         DMA IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SPI1_DMA
#if MICROHAL_SPI1_DMA_RX_CHANNEL == 2
void DMA1_Channel1_IRQHandler(void) {
  // spi1 rx
  DMA1->IFCR = DMA_IFCR_CTCIF1;
  SPI1->CR2 &= ~SPI_CR2_RXDMAEN & ~SPI_CR2_TXDMAEN;
  DMA1_Channel1->CCR &= ~DMA_CCR_EN;
  SPI_dma::spi1.semaphore = false;
}
#endif
#if MICROHAL_SPI1_DMA_RX_CHANNEL == 3
void DMA2_Channel3_IRQHandler(void) {
  // spi1 rx
  DMA2->IFCR = DMA_IFCR_CTCIF3;
  SPI1->CR2 &= ~SPI_CR2_RXDMAEN & ~SPI_CR2_TXDMAEN;
  DMA2_Channel3->CCR &= ~DMA_CCR_EN;
  SPI_dma::spi1.semaphore = false;
}
#endif
#if MICROHAL_SPI1_DMA_TX_CHANNEL == 3
void DMA1_Channel3_IRQHandler(void) {
  // spi1 tx
  DMA1->IFCR = DMA_IFCR_CTCIF3;
  SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
  DMA1_Channel3->CCR &= ~DMA_CCR_EN;
  SPI_dma::spi1.semaphore = false;
}
#endif
#if MICROHAL_SPI1_DMA_TX_CHANNEL == 4
void DMA2_Channel4_IRQHandler(void) {
  // spi1 tx
  DMA2->IFCR = DMA_IFCR_CTCIF4;
  SPI1->CR2 &= ~SPI_CR2_TXDMAEN;
  DMA2_Channel4->CCR &= ~DMA_CCR_EN;
  SPI_dma::spi1.semaphore = false;
}
#endif
#endif
#ifdef MICROHAL_USE_SPI2_DMA
void DMA1_Channel4_IRQHandler(void) {
  // spi1 rx
  DMA1->IFCR = DMA_IFCR_CTCIF4;
  SPI2->CR2 &= ~SPI_CR2_RXDMAEN & ~SPI_CR2_TXDMAEN;
  DMA1_Channel4->CCR &= ~DMA_CCR_EN;
  SPI_dma::spi2.semaphore = false;
}
void DMA1_Channel5_IRQHandler(void) {
  // spi1 tx
  DMA1->IFCR = DMA_IFCR_CTCIF5;
  SPI2->CR2 &= ~SPI_CR2_TXDMAEN;
  DMA1_Channel5->CCR &= ~DMA_CCR_EN;
  SPI_dma::spi2.semaphore = false;
}
#endif
#ifdef MICROHAL_USE_SPI3_DMA
void DMA2_Channel1_IRQHandler(void) {
  // spi1 rx
  DMA2->IFCR = DMA_IFCR_CTCIF1;
  SPI3->CR2 &= ~SPI_CR2_RXDMAEN & ~SPI_CR2_TXDMAEN;
  DMA2_Channel1->CCR &= ~DMA_CCR_EN;
  SPI_dma::spi3.semaphore = false;
}
void DMA2_Channel2_IRQHandler(void) {
  // spi1 tx
  DMA2->IFCR = DMA_IFCR_CTCIF2;
  SPI3->CR2 &= ~SPI_CR2_TXDMAEN;
  DMA2_Channel2->CCR &= ~DMA_CCR_EN;
  SPI_dma::spi3.semaphore = false;
}
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

}  // namespace stm32l4xx
}  // namespace microhal

