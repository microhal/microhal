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
#include "spi_interrupt_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {

#ifdef MICROHAL_USE_SPI1_INTERRUPT
GPIO::IOPin spi1MisoPin(GPIO::PortB, 4);
SPI_interrupt SPI_interrupt::spi1(*SPI1, spi1MisoPin);
SPI &SPI::spi1 = SPI_interrupt::spi1;
#endif
#ifdef MICROHAL_USE_SPI2_INTERRUPT
GPIO::IOPin spi2MisoPin(GPIO::PortB, 14);
SPI_interrupt SPI_interrupt::spi2(*SPI2, spi2MisoPin);
SPI &SPI::spi2 = SPI_interrupt::spi2;
#endif
#ifdef MICROHAL_USE_SPI3_INTERRUPT
GPIO::IOPin spi3MisoPin(GPIO::PortC, 11);
SPI_interrupt SPI_interrupt::spi3(*SPI3, spi3MisoPin);
SPI &SPI::spi3 = SPI_interrupt::spi3;
#endif

SPI::Error SPI_interrupt::write(uint8_t data, bool last) {
  bufferPtr = (uint8_t *)&data;
  bufferEnd = (uint8_t *)&data + 1;
  mode = TRANSMIT;

  //semaphore = true;
  enableTransmitterEmptyInterrupt();
  semaphore.wait(std::chrono::milliseconds::max());
  //semaphore.wait(std::chrono::milliseconds::max());
  //while (semaphore) {
  //}

  if (last) {
    while (spi.SR & SPI_SR_BSY) {
    }
  }

  return SPI::NoError;
}
SPI::Error SPI_interrupt::read(uint8_t &data, uint8_t write) {
  bufferPtr = &data;
  bufferEnd = &data + 1;
  writeData = write;
  mode = RECEIVE;

  while (spi.SR & SPI_SR_BSY) {
  }
  volatile uint16_t tmp __attribute__((unused)) = spi.DR;

  enableReceiverNotEmptyInterrupt();
  //semaphore = true;
  spi.DR = write;
  //while (semaphore) {
  //}
  semaphore.wait(std::chrono::milliseconds::max());
  //semaphore.wait(std::chrono::milliseconds::max());
  return SPI::NoError;
}
SPI::Error SPI_interrupt::writeBuffer(const void *data, size_t len, bool last) {
  bufferPtr = (uint8_t *)data;
  bufferEnd = (uint8_t *)data + len;
  mode = TRANSMIT;

  //semaphore = true;
  enableTransmitterEmptyInterrupt();
  semaphore.wait(std::chrono::milliseconds::max());
  //semaphore.wait(std::chrono::milliseconds::max());
  // while (semaphore) {
  //}

  if (last) {
    while (spi.SR & SPI_SR_BSY) {
    }
  }

  return SPI::NoError;
}
SPI::Error SPI_interrupt::readBuffer(void *data, size_t len, uint8_t write) {
  bufferPtr = (uint8_t *)data;
  bufferEnd = (uint8_t *)data + len;
  writeData = write;
  mode = RECEIVE;

  while (spi.SR & SPI_SR_BSY) {
  }
  volatile uint16_t tmp __attribute__((unused)) = spi.DR;

  enableReceiverNotEmptyInterrupt();
  //semaphore = true;
  spi.DR = write;
  //while (semaphore) {
  //}
  semaphore.wait(std::chrono::milliseconds::max());
  //semaphore.wait(std::chrono::milliseconds::max());
  return SPI::NoError;
}
//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
inline void IRQfunction(SPI_interrupt &object, SPI_TypeDef *spi) {
  uint32_t sr = spi->SR;

  if ((sr & SPI_SR_RXNE) && (object.mode == SPI_interrupt::RECEIVE)) {
    *object.bufferPtr++ = spi->DR;
    if (object.bufferPtr == object.bufferEnd) {
      object.mode = SPI_interrupt::WAITING;
      spi->CR2 &= ~(SPI_CR2_RXNEIE);  // fixme maybe bitband
      //object.semaphore = false;
      bool shouldYeld = object.semaphore.giveFromISR();
#if defined (HAL_RTOS_FreeRTOS)
      portYIELD_FROM_ISR( shouldYeld );
#endif
    } else {
      spi->DR = object.writeData;
    }
  }
  if ((sr & SPI_SR_TXE) && (object.mode == SPI_interrupt::TRANSMIT)) {
    spi->DR = *object.bufferPtr++;
    if (object.bufferPtr == object.bufferEnd) {
      object.mode = SPI_interrupt::WAITING;
      spi->CR2 &= ~(SPI_CR2_TXEIE);  // fixme maybe bitband
      //object.semaphore = false;
      bool shouldYeld = object.semaphore.giveFromISR();
#if defined (HAL_RTOS_FreeRTOS)
      portYIELD_FROM_ISR( shouldYeld );
#endif
    }
  }

  sr = ~sr;
  spi->SR = sr;
}
//***********************************************************************************************//
//                                          IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SPI1_INTERRUPT
void SPI1_IRQHandler(void) { IRQfunction(SPI_interrupt::spi1, SPI1); }
#endif
#ifdef MICROHAL_USE_SPI2_INTERRUPT
void SPI2_IRQHandler(void) { IRQfunction(SPI_interrupt::spi2, SPI2); }
#endif
#ifdef MICROHAL_USE_SPI3_INTERRUPT
void SPI3_IRQHandler(void) { IRQfunction(SPI_interrupt::spi3, SPI3); }
#endif
}  // namespace stm32l4xx
}  // namespace microhal


