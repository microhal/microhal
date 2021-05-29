/*
 * SPI_STM32f4xx.h
 *
 *  Created on: 13 mar 2014
 *      Author: pawel
 */

#ifndef _MICROHAL_SPI_POLLING_STMCOMMON_H_
#define _MICROHAL_SPI_POLLING_STMCOMMON_H_
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#include "spi_stmCommon.h"
#if _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_SPI(POLLING)                    // Check if driver is enabled in microhal port config
/* ************************************************************************************************
 * INCLUDES
 */
#include _MICROHAL_INCLUDE_PORT_IOMANAGER

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
/* ************************************************************************************************
 * CLASS
 */
class SPI_polling : public _MICROHAL_ACTIVE_PORT_NAMESPACE::SPI {
 public:
    template <int number, IOPin miso, IOPin mosi, IOPin sck>
    static SPI_polling &create(GPIO::OutputType mosiType = GPIO::OutputType::PushPull, GPIO::OutputType sckType = GPIO::OutputType::PushPull) {
        static_assert(IOManager::spiPinAssert(number, miso, mosi, sck), "Incorrect Pin configuration");
        IOManager::routeSPI<number, MISO, miso>();
        IOManager::routeSPI<number, MOSI, mosi>(GPIO::PullType::NoPull, mosiType);
        IOManager::routeSPI<number, SCK, sck>(GPIO::PullType::NoPull, sckType);

        if constexpr (number == 1) {
            static SPI_polling spi1(*microhal::registers::spi1, miso);
            return spi1;
        }
#if defined(_MICROHAL_SPI2_BASE_ADDRESS)
        if constexpr (number == 2) {
            static SPI_polling spi2(*microhal::registers::spi2, miso);
            return spi2;
        }
#endif
#if defined(_MICROHAL_SPI3_BASE_ADDRESS)
        if constexpr (number == 3) {
            static SPI_polling spi3(*microhal::registers::spi3, miso);
            return spi3;
        }
#endif
        std::terminate();
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

#endif  // ENABLED_ANY_SPI(POLLING)
#endif  // _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1
#endif  // _MICROHAL_SPI_POLLING_STMCOMMON_H_
