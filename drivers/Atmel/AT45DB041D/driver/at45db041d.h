/* =========================================================================================== */ /**
 @file        at45db041d.h
 @authors     Pawel Okas, Kubaszek Mateusz
 @version     $Id$
 @package
 @brief       AT45DB041D driver
 @hardware    STM32F4xx
 @copyright   $Copyright$
 @details     Implementation of the AT45DB041D

 */ /* ============================================================================================
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      */

#ifndef AT45DB041D_H
#define AT45DB041D_H

/******************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "SPIDevice/SPIDevice.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"
#include "microhalDefs.h"

/** @addtogroup Devices
 *  @{
 *  @class AT45DB041D
 *  @}
 *
 * @brief Atmel AT45DBx serial FLASH memory driver
 */

using namespace std::literals::chrono_literals;

class AT45DB041D : private microhal::SPIDevice {
 public:
    enum class Hardware : uint16_t { Manufacturer_ID = 0x9d88 };
    enum class PageSize : uint16_t { Size_256 = 256, Size_264 = 264 };

    static constexpr uint8_t getShift(PageSize pageSize) {
        if (pageSize == PageSize::Size_256) {
            return 0;
        } else {
            return 1;
        }
    }

    /** Possible AT45DB Buffers numbers. */
    enum class BufferNumber : uint8_t {
        Buffer1 = 1,  ///< Selected buffer -> 1
        Buffer2 = 2   ///< Selected buffer -> 2
    };

 private:
    microhal::GPIO resetPin;
    microhal::GPIO wpPin;

 public:
    const PageSize pageSize;
    const uint8_t Shift;
    static constexpr uint16_t PagesCount = 2048;

    AT45DB041D(microhal::SPI &spi, const microhal::GPIO::IOPin CEpin, const microhal::GPIO::IOPin RESETpin, const microhal::GPIO::IOPin WPpin,
               PageSize pageSize)
        : microhal::SPIDevice(spi, CEpin),
          resetPin(RESETpin, microhal::GPIO::Direction::Output),
          wpPin(WPpin, microhal::GPIO::Direction::Output),
          pageSize(pageSize),
          Shift(getShift(pageSize)) {
        resetPin.set();
        wpPin.set();
    }

    inline bool disableSectorProtection(void);

    bool init();
    bool busyWait() {
        uint8_t status;
        do {
            if (!getStatus(status)) {
                return false;
            }
        } while (!(status & 0x80));
        return true;
    }

    bool manufacturerID(uint16_t &id) {
        uint8_t addr = 0xd7;
        return SPIDevice::readRegister(addr, id, microhal::Endianness::BigEndian);  //, microhal::Endianness::BigEndian);
    }

    bool pageErase(uint16_t pageAddress);

    bool bufferWrite(BufferNumber bufferNumber, uint16_t address, const void *src, uint16_t len);
    bool bufferRead(BufferNumber bufferNumber, uint16_t address, void *dst, uint16_t len);

    bool bufferToMainMemoryWithErase(BufferNumber bufferNumber, uint16_t pageAddress);
    bool bufferToMainMemoryWithoutErase(BufferNumber bufferNumber, uint16_t pageAddress);

    bool mainMemoryPageToBuffer(BufferNumber bufferNumber, uint16_t pageAddress);

    inline bool getStatus(uint8_t &statusRegister);
    inline bool isMemoryProtected();
    inline bool isPageSize256();

    bool mainMemoryPageReadDirect(uint16_t pageAddress, uint16_t byteAddress, void *dst, uint16_t len);

    bool continuousArrayRead(uint16_t pageAddress, uint16_t byteAddress, void *dst, uint16_t len);

    bool blockErase(uint16_t blockAddress);
    bool sectorErase(uint8_t sectorAddress);
    bool chipErase(void);

 private:
    inline bool switchToPowerOf2PageSize(void);
};
/* ************************************************************************************************
 * INLINE FUNCTIONS
 */
/**
 *@brief This function disable sector protection.
 *
 *\retval   true if sector protection was disabled
 *\retval   false if an error occurred
 */
bool AT45DB041D::disableSectorProtection(void) {
    uint8_t tmpBuffer[4] = {0x3D, 0x2A, 0x7F, 0x9A};
    bool ret = true;
    ret &= writeBuffer(tmpBuffer, sizeof(tmpBuffer));
    tmpBuffer[3] = 0xcf;
    ret &= writeBuffer(tmpBuffer, sizeof(tmpBuffer));
    return ret;
}
/**
 * \brief This function read status register.
 *
 * \param statusRegister
 *
 * \retval  true if status register was successfully read.
 * \retval  false if an error occurred
 */
bool AT45DB041D::getStatus(uint8_t &statusRegister) {
    const uint8_t opcode = 0xD7;  // read status register operation code
    const uint8_t repetitions = 10;
    uint8_t repetitionsCounter = 0;
    bool ret = true;
    do {
        ++repetitionsCounter;
        if (repetitionsCounter > repetitions) {
            return false;
        }
        ret &= SPIDevice::readRegister(opcode, statusRegister);
    } while ((0x1c != (statusRegister & 0x1c)) || (0xff == statusRegister));
    return ret;
}

/**
 * \brief This function switch memory page size to 256 bit.
 *
 * @retval true if memory page was changed.
 * @retval false if an error occurred.
 */
bool AT45DB041D::switchToPowerOf2PageSize(void) {
    const uint8_t tmpBuffer[] = {0x3D,  // command set to change page size
                                 0x2A, 0x80, 0xA6};

    return writeBuffer(tmpBuffer, sizeof(tmpBuffer));  // write command
}

bool AT45DB041D::isMemoryProtected() {
    uint8_t status;
    if (!getStatus(status)) {
        return false;
    }
    if (status & 0x2) {
        return true;
    }
    return false;
}

bool AT45DB041D::isPageSize256() {
    uint8_t status;
    if (!getStatus(status)) {
        return false;
    }
    if (status & 0x01) {
        return true;
    }
    return false;
}
#endif /* AT45DB041D_H */
