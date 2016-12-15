/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 18-09-2016
 * last modification: 18-09-2016
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

#ifndef _MICROHAL_SPI_POLLING_STM32L4XX_H_
#define _MICROHAL_SPI_POLLING_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "../spi_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {
/* **************************************************************************************************************************************************
 * CLASS
 */
class SPI_polling : public stm32l4xx::SPI {
 public:
//---------------------------------------- variables
//----------------------------------------//
#ifdef MICROHAL_USE_SPI1_POLLING
  static SPI_polling spi1;
#endif
#ifdef MICROHAL_USE_SPI2_POLLING
  static SPI_polling spi2;
#endif
#ifdef MICROHAL_USE_SPI3_POLLING
  static SPI_polling spi3;
#endif
  //---------------------------------------- functions
  //----------------------------------------//
  SPI::Error write(const uint8_t data, bool last) final {
    const SPI::Error error = writeNoRead(data);
    if (last) {
      while (spi.SR & SPI_SR_BSY) {
      }
    }
    return error;
  }
  SPI::Error read(uint8_t &data, const uint8_t write = 0x00) final {
    while (spi.SR & SPI_SR_BSY) {
    }
    volatile uint16_t tmp __attribute__((unused)) = spi.DR;

    return writeRead(write, data);
  }
  SPI::Error writeBuffer(const void *data, const size_t len, bool last) final {
    SPI::Error error;
    const uint16_t *data16 = (uint16_t*)data;

    for (uint16_t i = 0; i < len; i++) {
      error = writeNoRead(data16[i]);
      if (error != NoError) return error;
    }
    if (last) {
      while (spi.SR & SPI_SR_BSY) {
      }
    }
    return error;
  }
  SPI::Error readBuffer(void *data, const size_t len,
                        const uint8_t write = 0x00) final {
    uint32_t sr;
    SPI::Error error;
    do {
      sr = spi.SR;
      error = errorCheck(sr);
      if (error != NoError) return error;
    } while (sr & SPI_SR_BSY);

//    volatile uint16_t tmp __attribute__((unused)) = spi.DR; // CO to tutaj robi????

    for (uint16_t i = 0; i < len; i++) {
      error = writeRead(write, static_cast<uint8_t *>(data)[i]);
      if (error != NoError) break;
    }
    return error;
  }

 private:
  //---------------------------------------- constructors
  //---------------------------------------
  SPI_polling(SPI_TypeDef &spiPort, stm32l4xx::GPIO::IOPin misoPin)
      : SPI(spiPort, misoPin) {
    switch (reinterpret_cast<uint32_t>(&spi)) {
      case reinterpret_cast<uint32_t>(SPI1):
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
        break;
      case reinterpret_cast<uint32_t>(SPI2):
        RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
        break;
      case reinterpret_cast<uint32_t>(SPI3):
        RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;
        break;
    }
  }

  virtual ~SPI_polling() {}
  //---------------------------------------- functions
  //----------------------------------------//
  inline SPI::Error writeNoRead(const uint16_t data) {
    uint32_t sr;
    SPI::Error error = NoError;

    do {
      sr = spi.SR;
      // error = errorCheck(sr);
      // if(error != NoError) return error;
    } while (!(sr & SPI_SR_TXE));

    spi.DR = data;
    return error;
  }
  inline SPI::Error writeRead(const uint16_t data, uint8_t &receivedData) {
    uint32_t sr;
    SPI::Error error;
    do {
      sr = spi.SR;
      error = errorCheck(sr);
      if (error != NoError) return error;
    } while (!(sr & SPI_SR_TXE));

    spi.DR = data;

    uint32_t cr1 = spi.CR1;
    uint32_t cr2 = spi.CR2;

    do {
      sr = spi.SR;
      error = errorCheck(sr);
      if (error != NoError) return error;
    } while (!(sr & SPI_SR_RXNE));

    receivedData = spi.DR;

    return error;
  }
};

}  // namespace stm32l4xx
}  // namespace microhal

#endif  // _MICROHAL_SPI_POLLING_STM32L4XX_H_
