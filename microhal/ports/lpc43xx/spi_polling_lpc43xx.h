#ifndef SPI_POLLING_LPC43XX_H_
#define SPI_POLLING_LPC43XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "spi_lpc43xx.h"

namespace microhal {
namespace lpc43xx {
/* ************************************************************************************************
 * CLASS
 */
class SPI_polling : public lpc43xx::SPI {
 public:
    enum class Number { SSP0 = 0x40083000, SSP1 = 0x400C5000 };
    //---------------------------------------- constructors ---------------------------------------
    SPI_polling() = delete;
    SPI_polling(SPI_polling &) = delete;
    SPI_polling(Number num, IOPin misoPin) : SPI(*reinterpret_cast<SSP *>(num), misoPin) {}

    //---------------------------------------- functions ----------------------------------------//
    Error write(const void *data, size_t len, bool last) final {
        SPI::Error error = Error::None;

        for (size_t i = 0; i < len; i++) {
            error = writeNoRead(((uint8_t *)(data))[i]);
            if (error != Error::None) return error;
        }
        if (last) {
            busyWait();
        }
        return error;
    }

    Error read(void *data, size_t len, uint8_t write = 0x00) final {
        SPI::Error error;
        do {
            volatile uint16_t null[[gnu::unused]] = ssp.readData();
        } while (!ssp.isTxFifoEmpty());

        volatile uint16_t tmp[[gnu::unused]] = ssp.readData();

        for (uint16_t i = 0; i < len; i++) {
            error = writeRead(write, static_cast<uint8_t *>(data)[i]);
            if (error != Error::None) break;
        }
        return error;
    }

    Error writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) final {
        Error error;
        do {
            volatile uint16_t null[[gnu::unused]] = ssp.readData();
        } while (!ssp.isTxFifoEmpty());

        volatile uint16_t tmp[[gnu::unused]] = ssp.readData();

        for (uint16_t i = 0; i < readWriteLength; i++) {
            error = writeRead(static_cast<const uint8_t *>(dataWrite)[i], static_cast<uint8_t *>(dataRead)[i]);
            if (error != Error::None) break;
        }
        return error;
    }

 private:
    //---------------------------------------- functions ----------------------------------------//
    Error writeNoRead(uint8_t data) {
        Error error = Error::None;

        while (!ssp.isTxFifoNotFull()) {
        }
        ssp.writeData(data);

        return error;
    }
    Error writeRead(uint8_t data, uint8_t &receivedData) {
        Error error = Error::None;
        while (!ssp.isTxFifoNotFull()) {
        }
        ssp.writeData(data);

        while (!ssp.isRxFifoNotEmpty()) {
        }
        receivedData = ssp.readData();

        return error;
    }
};

}  // namespace lpc43xx
}  // namespace microhal

#endif  // SPI_POLLING_STM32F4XX_H_
