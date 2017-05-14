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
#include "../clockManager.h"
#include "../spi_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 * CLASS
 */
class SPI_polling : public stm32f4xx::SPI {
 public:
//---------------------------------------- variables ----------------------------------------//
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
    //---------------------------------------- functions ----------------------------------------//
    SPI::Error write(const void *data, const size_t len, bool last) final {
        SPI::Error error = Error::None;

        for (uint16_t i = 0; i < len; i++) {
            error = writeNoRead(((uint8_t *)(data))[i]);
            if (error != Error::None) return error;
        }
        if (last) {
            while (!(spi.SR & SPI_SR_TXE))
                ;
            while (spi.SR & SPI_SR_BSY)
                ;
        }
        return error;
    }
    SPI::Error read(void *data, size_t len, uint8_t write = 0x00) final {
        uint32_t sr;
        SPI::Error error;
        do {
            sr = spi.SR;
            error = errorCheck(sr);
            if (error != Error::None) return error;
        } while (sr & SPI_SR_BSY);

        volatile uint16_t tmp __attribute__((unused)) = spi.DR;

        for (uint16_t i = 0; i < len; i++) {
            error = writeRead(write, static_cast<uint8_t *>(data)[i]);
            if (error != Error::None) break;
        }
        return error;
    }

    SPI::Error writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) final {
        uint32_t sr;
        SPI::Error error;
        do {
            sr = spi.SR;
            error = errorCheck(sr);
            if (error != Error::None) return error;
        } while (sr & SPI_SR_BSY);

        volatile uint16_t tmp __attribute__((unused)) = spi.DR;

        for (uint16_t i = 0; i < readWriteLength; i++) {
            error = writeRead(static_cast<const uint8_t *>(dataWrite)[i], static_cast<uint8_t *>(dataRead)[i]);
            if (error != Error::None) break;
        }
        return error;
    }

 private:
    //---------------------------------------- constructors ---------------------------------------
    SPI_polling(SPI_TypeDef &spi, stm32f4xx::GPIO::IOPin misoPin) : SPI(spi, misoPin) { ClockManager::enable(spi); }
    //---------------------------------------- functions ----------------------------------------//
    SPI::Error writeNoRead(uint8_t data) {
        uint32_t sr;
        SPI::Error error = Error::None;

        do {
            sr = spi.SR;
            // error = errorCheck(sr);
            // if(error != Error::None) return error;
        } while (!(sr & SPI_SR_TXE));

        spi.DR = data;

        return error;
    }
    SPI::Error writeRead(uint8_t data, uint8_t &receivedData) {
        uint32_t sr;
        SPI::Error error;
        do {
            sr = spi.SR;
            error = errorCheck(sr);
            if (error != Error::None) return error;
        } while (!(sr & SPI_SR_TXE));

        spi.DR = data;

        do {
            sr = spi.SR;
            error = errorCheck(sr);
            if (error != Error::None) return error;
        } while (!(spi.SR & SPI_SR_RXNE));

        receivedData = spi.DR;

        return error;
    }
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // SPI_POLLING_STM32F4XX_H_
