/*
 * SPIDevice.h
 *
 *  Created on: 11 mar 2014
 *      Author: pawel
 */

#ifndef _MICROHAL_SPIDEVICE_H_
#define _MICROHAL_SPIDEVICE_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>

#include "ports/manager/spi_port.h"
#include "byteswap.h"
#include "gpio.h"
#include "platform.h"
#include "spi.h"

namespace microhal {
/**
 * @addtogroup HAL Interfaces
 * @{
 * @class SPIDevice
 * @}
 * \brief   This class provides basic SPI communication. SPI specified devices
 * class should inherit from this class.
 */
/* ************************************************************************************************
 * CLASS
 */
class SPIDevice {
 public:
    //---------------------------------------- constructors ---------------------------------------
    SPIDevice(SPI &spi, microhal::GPIO::IOPin CEPin) : spi(spi), cePin(CEPin, GPIO::Direction::Output), lastError(SPI::NoError) { cePin.set(); }
    ~SPIDevice() { cePin.set(); }
    //------------------------------------------ functions ----------------------------------------
    /**
     * @brief This function return last error of this device.
     *
     * @return lastError
     */
    SPI::Error getLastError() { return lastError; }
    /**
     * @brief This function write 8-bit data to SPI device. Function is thread safe.
     * This function firstly lock SPI interface, next reset SPI device CE pin and
     * start writing data.
     * When data will be sent function set CE pin and unlock SPI interface.
     *
     * @param[in] data to write.
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool write(uint8_t data) {
        spi.lock();  // fixme maybe lock_guard

        cePin.reset();

        volatile uint32_t i = 300;
        while (i--) {
        }

        SPI::Error status = spi.write(data, true);

        i = 300;
        while (i--) {
        }

        cePin.set();
        spi.unlock();

        if (status == SPI::NoError) {
            return true;
        } else {
            lastError = status;
            return false;
        }
    }
    /**
     * @brief This function read 8-bit data from SPI device. Function is thread
     * safe.
     * This function firstly lock SPI interface, next reset SPI device CE pin and
     * start reading data.
     * When data will be read function set CE pin and unlock SPI interface.
     *
     * @param[out] data - read data
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool read(uint8_t &data) {
        spi.lock();  // fixme maybe unique_lock
        cePin.reset();

        SPI::Error status = spi.read(data);

        cePin.set();
        spi.unlock();

        if (status == SPI::NoError) {
            return true;
        } else {
            lastError = status;
            return false;
        }
    }

    // writing registers functions
    inline bool writeRegister(uint8_t registerAddress, uint8_t data);
    inline bool writeRegister(uint8_t registerAddress, uint16_t data, Endianness endianness);
    inline bool writeRegister(uint8_t registerAddress, uint32_t data, Endianness endianness);

    bool writeRegisters(uint8_t registerAddress, const uint8_t *buffer, size_t length);

    // reading registers functions
    inline bool readRegister(uint8_t registerAddress, uint8_t &data);
    inline bool readRegister(uint8_t registerAddress, uint16_t &data, Endianness endianness);
    inline bool readRegister(uint8_t registerAddress, uint32_t &data, Endianness endianness);

    bool readRegisters(uint8_t registerAddress, uint8_t *buffer, size_t length);

    // register bit modification functions
    inline bool setBitsInRegister(uint8_t regAddr, uint8_t bitMask);
    inline bool setBitsInRegister(uint8_t registerAddress, uint16_t bitMask, Endianness endianness);
    inline bool setBitsInRegister(uint8_t registerAddress, uint32_t bitMask, Endianness endianness);

    inline bool clearBitsInRegister(uint8_t regAddr, uint8_t bitMask);
    inline bool clearBitsInRegister(uint8_t registerAddress, uint16_t bitMask, Endianness endianness);
    inline bool clearBitsInRegister(uint8_t registerAddress, uint32_t bitMask, Endianness endianness);

    //-------------------------------------------- buffer reading functions ---------------------------
    bool writeBuffer(const uint8_t *buffer, size_t length, bool last = true);
    /**
     * @brief This function write 16 bit buffer of data to SPI device. Function
     * works same as writeBuffer(const uint8_t *buffer, uint16_t length, bool last) but for 16 bit data.
     *
     * @param[in] buffer - pointer to data buffer
     * @param[in] length - length of buffer
     * @param[in] last - if false device will be still active and wait to next communication.
     *                   When true device will be set to unactive state.
     *
     * @retval true if data buffer was sent.
     * @retval false if an error occurred
     */
    bool writeBuffer(const uint16_t *buffer, size_t length, bool last = true) { return writeBuffer((uint8_t *)buffer, length, last); }
    /**
     * @brief This function write 32 bit buffer of data to SPI device. Function
     * works same as writeBuffer(const uint16_t *buffer, uint16_t length, bool last) but for 32 bit register.
     *
     * @param[in] buffer - pointer to data buffer
     * @param[in] length - length of buffer
     * @param[in] last - if false device will be still active and wait to next communication.
     *                   When true device will be set to unactive state.
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool writeBuffer(const uint32_t *buffer, size_t length, bool last = true) { return writeBuffer((uint8_t *)buffer, length, last); }
    /**
     * @brief This function write 8 bit buffer of data to SPI device. Function
     * works same as writeBuffer(const uint8_t *buffer, uint16_t length, bool last) but for signed 8-bit data.
     *
     * @param[in] buffer - pointer to data buffer
     * @param[in] length - length of buffer
     * @param[in] last - if false device will be still active and wait to next communication.
     *                   When true device will be set to unactive state.
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool writeBuffer(const int8_t *buffer, size_t length, bool last = true) { return writeBuffer((uint8_t *)buffer, length, last); }
    /**
     * @brief This function write 16 bit buffer of data to SPI device. Function
     * works same as writeBuffer(const uint8_t *buffer, uint16_t length, bool last) but for signed 16-bit data.
     *
     * @param[in] buffer - pointer to data buffer
     * @param[in] length - length of buffer
     * @param[in] last - if false device will be still active and wait to next communication.
     *                   When true device will be set to unactive state.
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool writeBuffer(const int16_t *buffer, size_t length, bool last = true) { return writeBuffer((uint8_t *)buffer, length, last); }
    /**
     * @brief This function write 32 bit buffer of data to SPI device. Function
     * works same as writeBuffer(const int16_t *buffer, uint16_t length, bool last) but for 32-bit data.
     *
     * @param[in] buffer - pointer to data buffer
     * @param[in] length - length of buffer
     * @param[in] last - if false device will be still active and wait to next communication.
     *                   When true device will be set to unactive state.
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool writeBuffer(const int32_t *buffer, size_t length, bool last = true) { return writeBuffer((uint8_t *)buffer, length, last); }
    //-------------------------------------------- buffer reading functions ---------------------------
    bool readBuffer(uint8_t *buffer, size_t length, bool last = true);
    /**
     * @brief This function read 16 bit buffer of data to SPI device. Function
     * works same as readBuffer(uint8_t *buffer, uint16_t length, bool last) but for 16-bit data.
     *
     * @param[out] buffer - pointer to data buffer
     * @param[in] length - length of buffer
     * @param[in] last - if false device will be still active and wait to next communication.
     *                   When true device will be set to unactive state.
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool readBuffer(uint16_t *buffer, size_t length, bool last = true) { return readBuffer((uint8_t *)buffer, length, last); }
    /**
     * @brief This function read 32 bit buffer of data to SPI device. Function works same as
     * readBuffer(uint16_t *buffer, uint16_t length, bool last) but for 32-bit data.
     *
     * @param[out] buffer - pointer to data buffer
     * @param[in] length - length of buffer
     * @param[in] last - if false device will be still active and wait to next communication.
     *                   When true device will be set to unactive state.
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool readBuffer(uint32_t *buffer, size_t length, bool last = true) { return readBuffer((uint8_t *)buffer, length, last); }
    /**
     * @brief This function read signed 8-bit buffer of data to SPI device.
     * Function works same as readBuffer(uint8_t *buffer, uint16_t length, bool last) but for signed 8-bit data.
     *
     * @param[out] buffer - pointer to data buffer
     * @param[in] length - length of buffer
     * @param[in] last - if false device will be still active and wait to next communication.
     *                   When true device will be set to unactive state.
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool readBuffer(int8_t *buffer, size_t length, bool last = true) { return readBuffer((uint8_t *)buffer, length, last); }
    /**
     * @brief This function read signed 16-bit buffer of data to SPI device.
     * Function works same as readBuffer(uint8_t *buffer, uint16_t length, bool last) but for signed 16-bit data.
     *
     * @param[out] buffer - pointer to data buffer
     * @param[in] length - length of buffer
     * @param[in] last - if false device will be still active and wait to next communication.
     *                   When true device will be set to unactive state.
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool readBuffer(int16_t *buffer, size_t length, bool last = true) { return readBuffer(buffer, length, last); }
    /**
     * @brief This function read signed 32-bit buffer of data to SPI device. Function works same as
     * readBuffer(int16_t *buffer, uint16_t length, bool last) but for signed 32-bit data.
     *
     * @param[out] buffer - pointer to data buffer
     * @param[in] length - length of buffer
     * @param[in] last - if false device will be still active and wait to next communication.
     *                   When true device will be set to unactive state.
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool readBuffer(int32_t *buffer, size_t length, bool last = true) { return readBuffer(buffer, length, last); }

    /**@brief This function write bits from data parameter to 8bit register but only bits set to one in mask parameter are changed.
     *
     * @param registerAddress address of register where bit will be cleared
     * @param data to write
     * @param bitMask if set then corresponding bit in register will be written.
     *
     * @retval true if bits was set
     * @retval false if an error occurred
     */
    bool writeRegisterWithMask(uint8_t registerAddress, uint8_t data, uint8_t mask) {
        bool status = false;
        spi.lock();

        uint8_t readData;
        if (readRegister_noLock(registerAddress, readData)) {
            // clear bits
            readData &= ~mask;
            // set new value
            readData |= (data & mask);
            status = writeRegister_noLock(registerAddress, readData);
        }

        spi.unlock();
        return status;
    }

 protected:
    void SPIlock() { spi.lock(); }

    void SPIunlock() { spi.unlock(); }

    bool writeRegister_noLock(uint8_t registerAddress, uint8_t data);
    bool writeRegister_noLock(uint8_t registerAddress, uint16_t data, Endianness endianness);
    bool writeRegister_noLock(uint8_t registerAddress, uint32_t data, Endianness endianness);

    bool readRegister_noLock(uint8_t registerAddress, uint8_t &data);
    bool readRegister_noLock(uint8_t registerAddress, uint16_t &data, Endianness endianness);
    bool readRegister_noLock(uint8_t registerAddress, uint32_t &data, Endianness endianness);

 private:
    SPI &spi;
    GPIO cePin;

    SPI::Error lastError;
};
/* ************************************************************************************************
 * INLINE FUNCTIONS
 */
/**
 * @brief This function write data to 8 bit register. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and
 * start writing data.
 * When data will be sent function set CE pin and unlock SPI interface.
 *
 * @param[in] registerAddress - address of register where data will be written
 * @param[in] data - data to write
 *
 * @retval true if data was sent successfully.
 * @retval false if an error occurred.
 */
bool SPIDevice::writeRegister(uint8_t registerAddress, uint8_t data) {
    spi.lock();
    bool status = writeRegister_noLock(registerAddress, data);
    spi.unlock();

    return status;
}
/**
 * @brief This function write data to 16 bit register. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and
 * start writing data.
 * When data will be sent function set CE pin and unlock SPI interface.
 *
 * @param[in] registerAddress - address of register where data will be written
 * @param[in] data - data to write
 * @param[in] endianness - order of byte MSB or LSB first
 *
 * @retval true if data was sent successfully.
 * @retval false if an error occurred.
 */
bool SPIDevice::writeRegister(uint8_t registerAddress, uint16_t data, Endianness endianness) {
    spi.lock();
    bool status = writeRegister_noLock(registerAddress, data, endianness);
    spi.unlock();

    return status;
}
/**
 * @brief This function write data to 32 bit register. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and
 * start writing data.
 * When data will be sent function set CE pin and unlock SPI interface.
 *
 * @param[in] registerAddress - address of register where data will be written
 * @param[in] data - data to write
 * @param[in] endianness - order of byte MSB or LSB first
 *
 * @retval true if data was sent successfully.
 * @retval false if an error occurred.
 */
bool SPIDevice::writeRegister(uint8_t registerAddress, uint32_t data, Endianness endianness) {
    spi.lock();
    bool status = writeRegister_noLock(registerAddress, data, endianness);
    spi.unlock();

    return status;
}
/**
 * @brief This function read data from 8 bit register. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and
 * start writing address
 * next function read data. When data will be read, function set CE pin and
 * unlock SPI interface.
 *
 * @param[in] registerAddress - address of register from data will be read.
 * @param[out] data - read data
 *
 * @retval true if data was read successfully.
 * @retval false if an error occurred.
 */
bool SPIDevice::readRegister(uint8_t registerAddress, uint8_t &data) {
    spi.lock();
    bool status = readRegister_noLock(registerAddress, data);
    spi.unlock();

    return status;
}
/**
 * @brief This function read data from 16 bit register. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and
 * start writing address
 * next function read data. When data will be read, function set CE pin and
 * unlock SPI interface.
 *
 * @param[in] registerAddress - address of register from data will be read.
 * @param[out] data - read data
 * @param[in] endianness - order of byte MSB or LSB first
 *
 * @retval true if data was read successfully.
 * @retval false if an error occurred.
 */
bool SPIDevice::readRegister(uint8_t registerAddress, uint16_t &data, Endianness endianness) {
    spi.lock();
    bool status = readRegister_noLock(registerAddress, data, endianness);
    spi.unlock();

    return status;
}
/**
 * @brief This function read data from 32 bit register. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and
 * start writing address
 * next function read data. When data will be read, function set CE pin and
 * unlock SPI interface.
 *
 * @param[in] registerAddress - address of register from data will be read.
 * @param[out] data - read data
 * @param[in] endianness - order of byte MSB or LSB first
 *
 * @retval true if data was read successfully.
 * @retval false if an error occurred.
 */
bool SPIDevice::readRegister(uint8_t registerAddress, uint32_t &data, Endianness endianness) {
    spi.lock();
    bool status = readRegister_noLock(registerAddress, data, endianness);
    spi.unlock();

    return status;
}
/**
 * @brief This function modify 8 bit register. Function set bitMask
 * corresponding bits in register.
 * Function is thread safe.
 *
 * @param registerAddress - address of register where bits will be set
 * @param bitMask - bits to set
 *
 * @retval true if data was sent.
 * @retval false if an error occurred.
 */
bool SPIDevice::setBitsInRegister(uint8_t registerAddress, uint8_t bitMask) {
    uint8_t registerValue;
    bool status = false;

    spi.lock();
    if (readRegister_noLock(registerAddress, registerValue) == true) {
        registerValue |= bitMask;
        status = writeRegister_noLock(registerAddress, registerValue);
    }
    spi.unlock();
    return status;
}
/**
 * @brief This function works same as setBitsInRegister(uint8_t registerAddress,
 * uint8_t bitMask)
 * but for 16-bit register.
 *
 * @param registerAddress - address of register where bits will be set
 * @param bitMask - bits to set
 * @param endianness - order of byte in register (MSB or LSB first)
 *
 * @retval true if data was sent.
 * @retval false if an error occurred.
 */
bool SPIDevice::setBitsInRegister(uint8_t registerAddress, uint16_t bitMask, Endianness endianness) {
    uint16_t registerValue;
    bool status = false;

    spi.lock();
    if (readRegister_noLock(registerAddress, registerValue, endianness) == true) {
        registerValue |= bitMask;
        status = writeRegister_noLock(registerAddress, registerValue, endianness);
    }
    spi.unlock();
    return status;
}
/**
 * @brief This function works same as setBitsInRegister(uint8_t registerAddress,
 * uint16_t bitMask,
 * const Endianness endianness) but for 32-bit register.
 *
 * @param registerAddress - address of register where bits will be set
 * @param bitMask - bits to set
 * @param endianness - order of byte in register (MSB or LSB first)
 *
 * @retval true if data was sent.
 * @retval false if an error occurred.
 */
bool SPIDevice::setBitsInRegister(uint8_t registerAddress, uint32_t bitMask, Endianness endianness) {
    uint32_t registerValue;
    bool status = false;

    spi.lock();
    if (readRegister_noLock(registerAddress, registerValue, endianness) == true) {
        registerValue |= bitMask;
        status = writeRegister_noLock(registerAddress, registerValue, endianness);
    }
    spi.unlock();
    return status;
}
/**
 * @brief This function modify 8 bit register. Function clear bitMask
 * corresponding bits in register.
 * Function is thread safe.
 *
 * @param registerAddress - address of register where bits will be clear
 * @param bitMask - bits to clear
 *
 * @retval true if data was sent.
 * @retval false if an error occurred.
 */
bool SPIDevice::clearBitsInRegister(uint8_t registerAddress, uint8_t bitMask) {
    uint8_t registerValue;
    bool status = false;

    spi.lock();
    if (readRegister_noLock(registerAddress, registerValue) == true) {
        registerValue &= ~bitMask;
        status = writeRegister_noLock(registerAddress, registerValue);
    }
    spi.unlock();
    return status;
}
/**
 * @brief This function works same as clearBitsInRegister(uint8_t
 * registerAddress, uint8_t bitMask)
 * but for 16-bit register.
 *
 * @param registerAddress - address of register where bits will be clear
 * @param bitMask - bits to clear
 * @param endianness - order of byte in register (MSB or LSB first)
 *
 * @retval true if data was sent.
 * @retval false if an error occurred.
 */
bool SPIDevice::clearBitsInRegister(uint8_t registerAddress, uint16_t bitMask, Endianness endianness) {
    uint16_t registerValue;
    bool status = false;

    spi.lock();
    if (readRegister_noLock(registerAddress, registerValue, endianness) == true) {
        registerValue &= ~bitMask;
        status = writeRegister_noLock(registerAddress, registerValue, endianness);
    }
    spi.unlock();
    return status;
}
/**
 * @brief This function works same as clearBitsInRegister(uint8_t
 * registerAddress, uint16_t bitMask,
 * const Endianness endianness) but for 32-bit register.
 *
 * @param registerAddress - address of register where bits will be clear
 * @param bitMask - bits to clear
 * @param endianness - order of byte in register (MSB or LSB first)
 *
 * @retval true if data was sent.
 * @retval false if an error occurred.
 */
bool SPIDevice::clearBitsInRegister(uint8_t registerAddress, uint32_t bitMask, Endianness endianness) {
    uint32_t registerValue;
    bool status = false;

    spi.lock();
    if (readRegister_noLock(registerAddress, registerValue, endianness) == true) {
        registerValue &= ~bitMask;
        status = writeRegister_noLock(registerAddress, registerValue, endianness);
    }
    spi.unlock();
    return status;
}

}  // namespace microhal

#endif  // _MICROHAL_SPIDEVICE_H_
