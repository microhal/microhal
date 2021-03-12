/*
 * SPIDevice.cpp
 *
 *  Created on: 11 mar 2014
 *      Author: pawel
 */
/* ************************************************************************************************
 * INCLUDES
 */
#include "SPIDevice.h"

namespace microhal {

static void delay(uint32_t);

SPI::Error SPIDevice::write(uint8_t data) {
    std::lock_guard<SPI> guard(spi);

    cePin.reset();
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);
    const SPI::Error status = spi.write(data, true);
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);
    cePin.set();

    return status;
}

SPI::Error SPIDevice::read_to(uint8_t &data) {
    std::lock_guard<SPI> guard(spi);
    cePin.reset();
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);
    SPI::Error status = spi.read(data);
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);
    cePin.set();

    return status;
}

SPI::Error SPIDevice::modifyBitsInRegister_noEndiannesConversion(uint8_t readRegisterAddress, uint8_t writeRegisterAddress, uint8_t data,
                                                                 uint8_t mask) {
    std::lock_guard<SPI> guard(spi);
    uint8_t regValue;
    SPI::Error error = readRegister_noLock(readRegisterAddress, regValue);
    if (error == SPI::Error::None) {
        regValue &= ~mask;
        regValue |= (data & mask);
        return writeRegister_noLock(writeRegisterAddress, regValue);
    }
    return error;
}

SPI::Error SPIDevice::modifyBitsInRegister_noEndiannesConversion(uint8_t readRegisterAddress, uint8_t writeRegisterAddress, uint16_t data,
                                                                 uint16_t mask) {
    std::lock_guard<SPI> guard(spi);
    uint16_t regValue;
    SPI::Error error = readRegister_noLock_noEndiannessConversion(readRegisterAddress, regValue);
    if (error == SPI::Error::None) {
        regValue &= ~mask;
        regValue |= (data & mask);
        return writeRegister_noLock_noEndiannessConversion(writeRegisterAddress, regValue);
    }
    return error;
}

SPI::Error SPIDevice::modifyBitsInRegister_noEndiannesConversion(uint8_t readRegisterAddress, uint8_t writeRegisterAddress, uint32_t data,
                                                                 uint32_t mask) {
    std::lock_guard<SPI> guard(spi);
    uint32_t regValue;
    SPI::Error error = readRegister_noLock_noEndiannessConversion(readRegisterAddress, regValue);
    if (error == SPI::Error::None) {
        regValue &= ~mask;
        regValue |= (data & mask);
        return writeRegister_noLock_noEndiannessConversion(writeRegisterAddress, regValue);
    }
    return error;
}

/**
 * @brief This function write data to 8 bit register.
 *
 * @param[in] registerAddress - address of register where data will be written
 * @param[in] data - data to write
 *
 * @retval true if data was sent successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::writeRegister_lock(uint8_t registerAddress, uint8_t data) {
    std::lock_guard<SPI> guard(spi);
    return writeRegister_noLock(registerAddress, data);
}
/**
 * @brief This function write data to 24 bit register.
 *
 * @param[in] registerAddress - address of register where data will be written
 * @param[in] data - data to write
 *
 * @retval true if data was sent successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::writeRegister_lock(uint8_t registerAddress, uint16_t data) {
    std::lock_guard<SPI> guard(spi);
    return writeRegister_noLock_noEndiannessConversion(registerAddress, data);
}
/**
 * @brief This function write data to 24 bit register.
 *
 * @param[in] registerAddress - address of register where data will be written
 * @param[in] data - data to write
 *
 * @retval true if data was sent successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::writeRegister_lock(uint8_t registerAddress, microhal::uint24_t data) {
    std::lock_guard<SPI> guard(spi);
    return writeRegister_noLock_noEndiannessConversion(registerAddress, data);
}

SPI::Error SPIDevice::readRegister_noEndiannesConversion(uint8_t registerAddress, uint8_t &data) {
    std::lock_guard<SPI> guard(spi);
    return readRegister_noLock(registerAddress, data);
}

SPI::Error SPIDevice::readRegister_noEndiannesConversion(uint8_t registerAddress, uint16_t &data) {
    std::lock_guard<SPI> guard(spi);
    return readRegister_noLock_noEndiannessConversion(registerAddress, data);
}

SPI::Error SPIDevice::readRegister_noEndiannesConversion(uint8_t registerAddress, uint32_t &data) {
    std::lock_guard<SPI> guard(spi);
    return readRegister_noLock_noEndiannessConversion(registerAddress, data);
}

SPI::Error SPIDevice::readRegister_convertEndiannes(uint8_t registerAddress, uint16_t &data) {
    std::lock_guard<SPI> guard(spi);
    SPI::Error error = readRegister_noLock_noEndiannessConversion(registerAddress, data);
    data = microhal::convertEndianness(data);
    return error;
}

SPI::Error SPIDevice::readRegister_convertEndiannes(uint8_t registerAddress, microhal::uint24_t &data) {
    std::lock_guard<SPI> guard(spi);
    SPI::Error error = readRegister_noLock_noEndiannessConversion(registerAddress, data);
    data = microhal::convertEndianness(data);
    return error;
}

SPI::Error SPIDevice::readRegister_convertEndiannes(uint8_t registerAddress, uint32_t &data) {
    std::lock_guard<SPI> guard(spi);
    SPI::Error error = readRegister_noLock_noEndiannessConversion(registerAddress, data);
    data = microhal::convertEndianness(data);
    return error;
}

/**
 * @brief This function write data to 8 bit register.
 *
 * @param[in] registerAddress - address of register where data will be written
 * @param[in] data - data to write
 *
 * @retval true if data was sent successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::writeRegister_noLock(uint8_t registerAddress, uint8_t data) {
    const uint8_t buffer[2] = {registerAddress, data};
    return write_noLock(buffer, sizeof(buffer));
}
/**
 * @brief This function write data to 16 bit register.
 *
 * @param[in] registerAddress - address of register where data will be written
 * @param[in] data - data to write
 * @param[in] endianness - order of byte MSB or LSB first
 *
 * @retval true if data was sent successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::writeRegister_noLock_noEndiannessConversion(uint8_t registerAddress, uint16_t data) {
    struct {
        uint8_t unused;
        uint8_t registerAddress;
        uint16_t data;
    } buffer;
    buffer.registerAddress = registerAddress;
    buffer.data = data;
    return write_noLock(&buffer.registerAddress, 3);
}
/**
 * @brief This function write data to 16 bit register.
 *
 * @param[in] registerAddress - address of register where data will be written
 * @param[in] data - data to write
 * @param[in] endianness - order of byte MSB or LSB first
 *
 * @retval true if data was sent successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::writeRegister_noLock_noEndiannessConversion(uint8_t registerAddress, microhal::uint24_t data) {
    struct {
        uint8_t unused;
        uint8_t unused2;
        uint8_t unused3;
        uint8_t registerAddress;
        uint32_t data;
    } buffer;
    buffer.registerAddress = registerAddress;
    buffer.data = data;
    return write_noLock(&buffer.registerAddress, 4);
}
/**
 * @brief This function write data to 16 bit register.
 *
 * @param[in] registerAddress - address of register where data will be written
 * @param[in] data - data to write
 * @param[in] endianness - order of byte MSB or LSB first
 *
 * @retval true if data was sent successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::writeRegister_noLock_noEndiannessConversion(uint8_t registerAddress, uint32_t data) {
    struct {
        uint8_t unused;
        uint8_t unused2;
        uint8_t unused3;
        uint8_t registerAddress;
        uint32_t data;
    } buffer;
    buffer.registerAddress = registerAddress;
    buffer.data = data;
    return write_noLock(&buffer.registerAddress, 5);
}

/**
 * @brief This function read data from 8 bit register.
 *
 * @param[in] registerAddress - address of register from data will be read.
 * @param[out] data - read data
 *
 * @retval true if data was read successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::readRegister_noLock(uint8_t registerAddress, uint8_t &data) {
    // activate spi device
    cePin.reset();
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);

    // write write register address
    SPI::Error status = spi.write(registerAddress, false);
    // if register address was written successfully then read data;
    if (status == SPI::Error::None) {
        status = spi.read(data);
    }

    // deactivate spi device
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);
    cePin.set();
    return status;
}
/**
 * @brief This function read data from 32 bit register. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and start writing address
 * next function read data. When data will be read, function set CE pin and unlock SPI interface.
 *
 * @param[in] registerAddress - address of register from data will be read.
 * @param[out] data - read data
 * @param[in] endianness - order of byte MSB or LSB first
 *
 * @retval true if data was read successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::readRegister_noLock_noEndiannessConversion(uint8_t registerAddress, uint16_t &data) {
    return readRegister_noLock_impl(registerAddress, (uint8_t *)&data, sizeof(data));
}
/**
 * @brief This function read data from 32 bit register. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and start writing address
 * next function read data. When data will be read, function set CE pin and unlock SPI interface.
 *
 * @param[in] registerAddress - address of register from data will be read.
 * @param[out] data - read data
 * @param[in] endianness - order of byte MSB or LSB first
 *
 * @retval true if data was read successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::readRegister_noLock_noEndiannessConversion(uint8_t registerAddress, microhal::uint24_t &data) {
    return readRegister_noLock_impl(registerAddress, (uint8_t *)&data, 3);
}
/**
 * @brief This function read data from 32 bit register. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and start writing address
 * next function read data. When data will be read, function set CE pin and unlock SPI interface.
 *
 * @param[in] registerAddress - address of register from data will be read.
 * @param[out] data - read data
 * @param[in] endianness - order of byte MSB or LSB first
 *
 * @retval true if data was read successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::readRegister_noLock_noEndiannessConversion(uint8_t registerAddress, uint32_t &data) {
    return readRegister_noLock_impl(registerAddress, (uint8_t *)&data, sizeof(data));
}
/**
 * @brief This function write 8 bit data. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and start writing data.
 * When data will be written and last is set to true, function set CE pin and unlock SPI interface,
 * or if last is false function return but spi device will be active and spi interface will be
 * locked.
 *
 * @param[in] buffer - pointer to buffer to send.
 * @param[in] length - number of bytes to write
 * @param[in] last - indication of last transfer. If true then this transfer is last.
 *
 * @retval true if data was read successfully.
 * @retval false if an error occurred.
 */
// bool SPIDevice::writeBuffer(const uint8_t *buffer, size_t length, bool last) {
//    // wait until spi is busy next lock spi device
//    spi.lock();
//    // activate spi device
//    cePin.reset();
//
//    SPI::Error status = spi.write(buffer, length, last);
//
//    if (last == true) {
//        // fixme is delay necessary
//        volatile uint32_t i = 300;
//        while (i--) {
//        }
//
//        // deactivate spi device
//        cePin.set();
//        // unlock
//        spi.unlock();
//    } else {
//        spi.unlock();
//    }
//
//    if (status == SPI::SPI::Error::None) {
//        return true;
//    } else {
//        lastError = status;
//        return false;
//    }
//}  // SPIDevice::write ---------------------------------------------------------------------//
/**
 * @brief This function read 8 bit data. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and start reading data.
 * When data will be read and last is set to true, function set CE pin and unlock SPI interface,
 * or if last is false function return but spi device will be active and spi interface will be
 * locked.
 *
 * @param[out] buffer - pointer to data where read data will be stored.
 * @param[in] length - number of byte to read.
 * @param[in] last - indication of last transfer. If true then this transfer is last.
 *
 * @retval true if data was read successfully.
 * @retval false if an error occurred.
 */
// bool SPIDevice::readBuffer(uint8_t *buffer, size_t length, bool last) {
//    // wait until spi is busy next lock spi device
//    spi.lock();
//    // activate spi device
//    cePin.reset();
//
//    SPI::Error status = spi.read(buffer, length);
//
//    if (last == true) {
//        // fixme is delay necessary
//        volatile uint32_t i = 300;
//        while (i--) {
//        }
//
//        // deactivate spi device
//        cePin.set();
//        // unlock
//        spi.unlock();
//    } else {
//        spi.unlock();
//    }
//
//    if (status == SPI::SPI::Error::None) {
//        return true;
//    } else {
//        lastError = status;
//        return false;
//    }
//}  // SPIDevice::read ----------------------------------------------------------------------//
/**
 * @brief This function write data to 8 bit registers. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and start writing data.
 * When data will be sent function set CE pin and unlock SPI interface.
 *
 * @param[in] registerAddress - address of first register
 * @param[in] buffer - pointer to data to write
 * @param[in] length - number of bytes to write
 *
 * @retval true if data was read successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::writeRegisters_noEndiannessConversion(uint8_t registerAddress, const uint8_t *buffer, size_t length) {
    std::lock_guard<SPI> guard(spi);

    // activate spi device
    cePin.reset();
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);

    SPI::Error status = spi.write(registerAddress, false);
    if (status == SPI::SPI::Error::None) {
        status = spi.write(buffer, length, true);
    }

    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);
    cePin.set();
    return status;
}
/**
 * @brief This function read data from 8 bit registers. Function is thread safe.
 * This function firstly lock SPI interface, next reset SPI device CE pin and start writing address
 * next function read data. When data will be read, function set CE pin and unlock SPI interface.
 *
 * @param[in] registerAddress - address of first register
 * @param[out] buffer - pointer to buffer where read data will be stored.
 * @param[in] length - number of bytes to read.
 *
 * @retval true if data was read successfully.
 * @retval false if an error occurred.
 */
SPI::Error SPIDevice::readRegisters_noEndiannessConversion(uint8_t registerAddress, uint8_t *buffer, size_t length) {
    std::lock_guard<SPI> guard(spi);

    // activate spi device
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);
    cePin.reset();

    SPI::Error status = spi.write(registerAddress, false);
    if (status == SPI::Error::None) {
        status = spi.read(buffer, length);
    }

    // deactivate spi device
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);
    cePin.set();

    return status;
}

SPI::Error SPIDevice::readRegister_noLock_impl(uint8_t registerAddress, uint8_t *data, size_t len) {
    // activate spi device
    cePin.reset();
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);

    SPI::Error status = spi.write(registerAddress, false);
    if (status == SPI::SPI::Error::None) {
        status = spi.read(data, len);
    }

    // deactivate spi device
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);
    cePin.set();

    return status;
}

SPI::Error SPIDevice::write_noLock(const uint8_t *data, size_t len) {
    // activate spi device
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);
    cePin.reset();

    SPI::Error status = spi.write(data, len, true);

    // deactivate spi device
    if (chipEnableDelay_ns > 0) delay(chipEnableDelay_ns);
    cePin.set();

    return status;
}

void delay(uint32_t delayTime) {
    volatile uint32_t i = delayTime;
    while (i--) {
    }
}
}  // namespace microhal
