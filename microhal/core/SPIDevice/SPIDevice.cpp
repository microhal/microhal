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
/**
 * @brief This function write data to 8 bit register.
 *
 * @param[in] registerAddress - address of register where data will be written
 * @param[in] data - data to write
 *
 * @retval true if data was sent successfully.
 * @retval false if an error occurred.
 */
bool SPIDevice::writeRegister_noLock(uint8_t registerAddress, uint8_t data) {
    cePin.reset();

    // fixme is delay necessary
    volatile uint32_t i = 300;
    while (i--) {
    }

    SPI::Error status = spi.write(registerAddress, false);
    if (status == SPI::NoError) {
        status = spi.write(data, true);

        // fixme is delay necessary
        volatile uint32_t i = 300;
        while (i--) {
        }
    }

    cePin.set();

    if (status == SPI::NoError) {
        return true;
    } else {
        lastError = status;
        return false;
    }
}  // SPIDevice::writeRegister_noLock ------------------------------------------------------------//
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
bool SPIDevice::writeRegister_noLock(uint8_t registerAddress, uint16_t data, Endianness endianness) {
    uint16_t dataTmp = data;

    // convert endianness if needed
    if (endianness != DeviceEndianness) {
        // do conversion
        dataTmp = byteswap(data);
    }
    // activate spi device
    cePin.reset();

    // write write register address
    SPI::Error status = spi.write(registerAddress, false);
    // if register address was written successfully then send data;
    if (status == SPI::NoError) {
        status = spi.writeBuffer(&dataTmp, 2, true);
    }

    // fixme is delay necessary
    //  volatile uint32_t i = 300;
    //  while (i--) {
    //  }
    // deactivate spi device
    cePin.set();

    if (status == SPI::NoError) {
        return true;
    } else {
        lastError = status;
        return false;
    }
}  // SPIDevice::writeRegister_noLock ------------------------------------------------------------//
   /**
    * @brief This function write data to 32 bit register.
    *
    * @param[in] registerAddress - address of register where data will be written
    * @param[in] data - data to write
    * @param[in] endianness - order of byte MSB or LSB first
    *
    * @retval true if data was sent successfully.
    * @retval false if an error occurred.
    */
bool SPIDevice::writeRegister_noLock(uint8_t registerAddress, uint32_t data, Endianness endianness) {
    uint32_t dataTmp = data;

    // convert endianness if needed
    if (endianness != DeviceEndianness) {
        // do conversion
        dataTmp = byteswap(data);
    }
    // activate spi device
    cePin.reset();

    // write write register address
    SPI::Error status = spi.write(registerAddress, false);
    // if register address was written successfully then send data;
    if (status == SPI::NoError) {
        status = spi.writeBuffer(&dataTmp, 4, true);
    }

    //    //fixme is delay necessary
    //    volatile uint32_t i = 300;
    //    while (i--) {
    //    }

    // deactivate spi device
    cePin.set();

    if (status == SPI::NoError) {
        return true;
    } else {
        lastError = status;
        return false;
    }
}  // SPIDevice::writeRegister_noLock ------------------------------------------------------------//
   /**
    * @brief This function read data from 8 bit register.
    *
    * @param[in] registerAddress - address of register from data will be read.
    * @param[out] data - read data
    *
    * @retval true if data was read successfully.
    * @retval false if an error occurred.
    */
bool SPIDevice::readRegister_noLock(uint8_t registerAddress, uint8_t &data) {
    // activate spi device
    cePin.reset();

    // fixme is delay necessary
    volatile uint32_t i = 300;
    while (i--) {
    }

    // write write register address
    SPI::Error status = spi.write(registerAddress, false);
    // if register address was written successfully then read data;
    if (status == SPI::NoError) {
        status = spi.read(data);

        // fixme is delay necessary
        volatile uint32_t i = 300;
        while (i--) {
        }
    }

    // deactivate spi device
    cePin.set();

    if (status == SPI::NoError) {
        return true;
    } else {
        lastError = status;
        return false;
    }
}  // SPIDevice::readRegister_noLock -------------------------------------------------------------//
   /**
    * @brief This function read data from 16 bit register.
    *
    * @param[in] registerAddress - address of register from data will be read.
    * @param[out] data - read data
    * @param[in] endianness - order of byte MSB or LSB first
    *
    * @retval true if data was read successfully.
    * @retval false if an error occurred.
    */
bool SPIDevice::readRegister_noLock(uint8_t registerAddress, uint16_t &data, Endianness endianness) {
    // activate spi device
    cePin.reset();

    SPI::Error status = spi.write(registerAddress, false);
    if (status == SPI::NoError) {
        status = spi.readBuffer(&data, 2);
    }

    // deactivate spi device
    cePin.set();

    if (status == SPI::NoError) {
        // convert endianness if needed
        if (endianness != DeviceEndianness) {
            // do conversion
            data = byteswap(data);
        }
        return true;
    } else {
        lastError = status;
        return false;
    }
}  // SPIDevice::readRegister_noLock -------------------------------------------------------------//
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
bool SPIDevice::readRegister_noLock(uint8_t registerAddress, uint32_t &data, Endianness endianness) {
    // activate spi device
    cePin.reset();

    SPI::Error status = spi.write(registerAddress, false);
    if (status == SPI::NoError) {
        status = spi.readBuffer(&data, 4);
    }

    // deactivate spi device
    cePin.set();

    if (status == SPI::NoError) {
        // convert endianness if needed
        if (endianness != DeviceEndianness) {
            // do conversion
            data = byteswap(data);
        }
        return true;
    } else {
        lastError = status;
        return false;
    }
}  // SPIDevice::readRegister_noLock -------------------------------------------------------------//
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
bool SPIDevice::writeBuffer(const uint8_t *buffer, size_t length, bool last) {
    // wait until spi is busy next lock spi device
    spi.lock();
    // activate spi device
    cePin.reset();

    SPI::Error status = spi.writeBuffer(buffer, length, last);

    if (last == true) {
        // fixme is delay necessary
        volatile uint32_t i = 300;
        while (i--) {
        }

        // deactivate spi device
        cePin.set();
        // unlock
        spi.unlock();
    } else {
        spi.unlock();
    }

    if (status == SPI::NoError) {
        return true;
    } else {
        lastError = status;
        return false;
    }
}  // SPIDevice::writeBuffer ---------------------------------------------------------------------//
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
bool SPIDevice::readBuffer(uint8_t *buffer, size_t length, bool last) {
    // wait until spi is busy next lock spi device
    spi.lock();
    // activate spi device
    cePin.reset();

    SPI::Error status = spi.readBuffer(buffer, length);

    if (last == true) {
        // fixme is delay necessary
        volatile uint32_t i = 300;
        while (i--) {
        }

        // deactivate spi device
        cePin.set();
        // unlock
        spi.unlock();
    } else {
        spi.unlock();
    }

    if (status == SPI::NoError) {
        return true;
    } else {
        lastError = status;
        return false;
    }
}  // SPIDevice::readBuffer ----------------------------------------------------------------------//
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
bool SPIDevice::writeRegisters(uint8_t registerAddress, const uint8_t *buffer, size_t length) {
    // wait until spi is busy next lock spi device
    spi.lock();
    // activate spi device
    cePin.reset();

    SPI::Error status = spi.write(registerAddress, false);
    if (status == SPI::NoError) {
        status = spi.writeBuffer(buffer, length, true);
    }
    //
    //    //fixme is delay necessary
    //    volatile uint32_t i = 3;
    //    while (i--) {
    //    }

    // deactivate spi device
    cePin.set();
    // unlock spi device
    spi.unlock();

    if (status == SPI::NoError) {
        return true;
    } else {
        lastError = status;
        return false;
    }
}  // SPIDevice::writeRegisters ------------------------------------------------------------------//
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
bool SPIDevice::readRegisters(uint8_t registerAddress, uint8_t *buffer, size_t length) {
    // wait until spi is busy next lock spi device
    spi.lock();
    // activate spi device
    cePin.reset();

    bool status = spi.write(registerAddress, false);
    if (status == true) {
        // status = readBuffer(buffer, length, true);
        status = readBuffer(buffer, length);
    }

    // deactivate spi device
    cePin.set();
    // unlock spi device
    spi.unlock();

    return status;
}  // SPIDevice::readRegisters -------------------------------------------------------------------//

}  // namespace microhal
