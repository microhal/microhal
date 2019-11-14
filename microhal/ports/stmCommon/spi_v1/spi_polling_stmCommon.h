/*
 * SPI_STM32f4xx.h
 *
 *  Created on: 13 mar 2014
 *      Author: pawel
 */

#ifndef _MICROHAL_SPI_POLLING_STMCOMMON_H_
#define _MICROHAL_SPI_POLLING_STMCOMMON_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "spi_stmCommon.h"
#include _MICROHAL_INCLUDE_PORT_clockManager
#include _MICROHAL_INCLUDE_PORT_CONFIG
#include _MICROHAL_INCLUDE_PORT_IOMANAGER

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
/* ************************************************************************************************
 * CLASS
 */
class SPI_polling : public _MICROHAL_ACTIVE_PORT_NAMESPACE::SPI {
 public:
    template <int number, IOPin::Port misoPort, IOPin::Pin misoPin, IOPin::Port mosiPort, IOPin::Pin mosiPin, IOPin::Port sckPort, IOPin::Pin sckPin>
    static SPI_polling &create(GPIO::OutputType mosiType = GPIO::PushPull, GPIO::OutputType sckType = GPIO::PushPull) {
        constexpr IOPin miso(misoPort, misoPin);
        constexpr IOPin mosi(mosiPort, mosiPin);
        constexpr IOPin sck(sckPort, sckPin);

        static_assert(IOManager::spiPinAssert(number, miso, mosi, sck), "Incorrect Pin configuration");
        IOManager::routeSpi<number>(miso, mosi, sck, mosiType, sckType);

        if constexpr (number == 1) {
            static SPI_polling spi1(*microhal::registers::spi1, miso);
            return spi1;
        }
        if constexpr (number == 2) {
            static SPI_polling spi2(*microhal::registers::spi2, miso);
            return spi2;
        }
        if constexpr (number == 3) {
            static SPI_polling spi3(*microhal::registers::spi3, miso);
            return spi3;
        }
    }
    //---------------------------------------- functions ----------------------------------------//
    SPI::Error write(const void *data, const size_t len, bool last) final;
    SPI::Error read(void *data, size_t len, uint8_t write = 0x00) final;
    SPI::Error writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) final;

 private:
    //---------------------------------------- constructors ---------------------------------------
    SPI_polling(registers::SPI &spi, _MICROHAL_ACTIVE_PORT_NAMESPACE::IOPin misoPin) : SPI(spi, misoPin) {
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
        ClockManager::enableSPI(getNumber(), ClockManager::PowerMode::Normal);
#else
        ClockManager::enableSPI(getNumber());
#endif
    }
    //---------------------------------------- functions ----------------------------------------//
    SPI::Error writeNoRead(uint8_t data);
    SPI::Error writeRead(uint8_t data, uint8_t &receivedData);
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // _MICROHAL_SPI_POLLING_STMCOMMON_H_
