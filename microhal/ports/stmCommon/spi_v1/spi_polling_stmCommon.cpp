/*
 * SPI_STM32f4xx.cpp
 *
 *  Created on: 13 mar 2014
 *      Author: pawel
 */
/* ************************************************************************************************
 * INCLUDES
 */
#include "spi_polling_stmCommon.h"
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#if _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_SPI(POLLING)                    // Check if driver is enabled in microhal port config

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

SPI::Error SPI_polling::write(const void *data, const size_t len, bool last) {
    uint8_t discard;
    for (size_t i = 0; i < len; i++) {
        SPI::Error error = writeNoRead(((uint8_t *)(data))[i]);
        if (error != Error::None && error != Error::Overrun) {
            clearErrorFlags(error);
            return error;
        }
    }
    if (last) {
        busyWait();
    }
    return Error::None;
}

SPI::Error SPI_polling::read(void *data, size_t len, uint8_t write) {
    registers::SPI::SR sr;
    SPI::Error error;
    do {
        sr = spi.sr.volatileLoad();
        error = errorCheck(sr);
        if (error != Error::None && error != Error::Overrun) return error;
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
    } while (sr.BSY || sr.FTLVL);
#else
    } while (sr.BSY);
#endif
    clearErrorFlags(error);
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
    for (size_t i = spi.sr.volatileLoad().FRLVL; i > 0; i--) {
        spi.dr.volatileLoad_8bit();
    }
#else
    spi.dr.volatileLoad();
#endif

    for (size_t i = 0; i < len; i++) {
        error = writeRead(write, static_cast<uint8_t *>(data)[i]);
        if (error != Error::None) break;
    }
    return error;
}

SPI::Error SPI_polling::writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) {
    registers::SPI::SR sr;
    SPI::Error error;
    do {
        sr = spi.sr.volatileLoad();
        error = errorCheck(sr);
        if (error != Error::None && error != Error::Overrun) return error;
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
    } while (sr.BSY || sr.FTLVL);
#else
    } while (sr.BSY);
#endif

    clearErrorFlags(error);
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
    for (size_t i = spi.sr.volatileLoad().FRLVL; i > 0; i--) {
        spi.dr.volatileLoad_8bit();
    }
#else
    spi.dr.volatileLoad();
#endif

    for (size_t i = 0; i < readWriteLength; i++) {
        error = writeRead(static_cast<const uint8_t *>(dataWrite)[i], static_cast<uint8_t *>(dataRead)[i]);
        if (error != Error::None) break;
    }

    return error;
}

SPI::Error SPI_polling::writeNoRead(uint8_t data) {
    registers::SPI::SR sr;
    SPI::Error error = Error::None;

    do {
        sr = spi.sr.volatileLoad();
        error = errorCheck(sr);
        if (error != Error::None && error != Error::Overrun) return error;
    } while (!(sr.TXE));

    spi.dr.volatileStore_8bit(data);

    return error;
}
SPI::Error SPI_polling::writeRead(uint8_t data, uint8_t &receivedData) {
    registers::SPI::SR sr;
    SPI::Error error = Error::None;
    do {
        sr = spi.sr.volatileLoad();
        error = errorCheck(sr);
        if (error != Error::None) return error;
    } while (!(sr.TXE));

    spi.dr.volatileStore_8bit(data);

    do {
        sr = spi.sr.volatileLoad();
        error = errorCheck(sr);
        if (error != Error::None) return error;
    } while (!(sr.RXNE));

    receivedData = (uint32_t)spi.dr.volatileLoad_8bit();

    return error;
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // ENABLED_ANY_SPI(POLLING)
#endif  //_MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1
