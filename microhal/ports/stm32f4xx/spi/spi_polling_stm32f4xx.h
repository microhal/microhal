/*
 * SPI_STM32f4xx.h
 *
 *  Created on: 13 mar 2014
 *      Author: pawel
 */

#ifndef SPI_POLLING_STM32F4XX_H_
#define SPI_POLLING_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "../spi_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 * CLASS
 */
class SPI_polling : public stm32f4xx::SPI {
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
#ifdef MICROHAL_USE_SPI4_POLLING
  static SPI_polling spi4;
#endif
#ifdef MICROHAL_USE_SPI5_POLLING
  static SPI_polling spi5;
#endif
#ifdef MICROHAL_USE_SPI6_POLLING
  static SPI_polling spi6;
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

    for (uint16_t i = 0; i < len; i++) {
      error = writeNoRead(((uint8_t *)(data))[i]);
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

    volatile uint16_t tmp __attribute__((unused)) = spi.DR;

    for (uint16_t i = 0; i < len; i++) {
      error = writeRead(write, static_cast<uint8_t *>(data)[i]);
      if (error != NoError) break;
    }
    return error;
  }

 private:
  //---------------------------------------- constructors
  //---------------------------------------
  SPI_polling(SPI_TypeDef &spiPort, stm32f4xx::GPIO::IOPin misoPin)
      : SPI(spiPort, misoPin) {
    switch (reinterpret_cast<uint32_t>(&spi)) {
      case reinterpret_cast<uint32_t>(SPI1):
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
        break;
      case reinterpret_cast<uint32_t>(SPI2):
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
        break;
      case reinterpret_cast<uint32_t>(SPI3):
        RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
        break;
      case reinterpret_cast<uint32_t>(SPI4):
        RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
        break;
      case reinterpret_cast<uint32_t>(SPI5):
        RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
        break;
      case reinterpret_cast<uint32_t>(SPI6):
        RCC->APB2ENR |= RCC_APB2ENR_SPI6EN;
        break;
    }
  }

  virtual ~SPI_polling() {}
  //---------------------------------------- functions
  //----------------------------------------//
  inline SPI::Error writeNoRead(const uint8_t data) {
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
  inline SPI::Error writeRead(const uint8_t data, uint8_t &receivedData) {
    uint32_t sr;
    SPI::Error error;
    do {
      sr = spi.SR;
      error = errorCheck(sr);
      if (error != NoError) return error;
    } while (!(sr & SPI_SR_TXE));

    spi.DR = data;

    do {
      sr = spi.SR;
      error = errorCheck(sr);
      if (error != NoError) return error;
    } while (!(spi.SR & SPI_SR_RXNE));

    receivedData = spi.DR;

    return error;
  }
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // SPI_POLLING_STM32F4XX_H_
