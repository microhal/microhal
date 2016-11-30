/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      I2C device driver
 *
 * @authors    Pawel Okas
 * created on: 22-11-2013
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015-2016, microHAL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_I2CDEVICE_H_
#define _MICROHAL_I2CDEVICE_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "../i2c.h"
#include "byteswap.h"
#include "platform.h"

namespace microhal {
/**
 * @addtogroup I2C
 * @{
 * @class I2CDevice
 *
 * This class provides mechanism for I2C communication. The class was implemented to help you in writing I2C drivers.
 * We recommend that your driver should privately inherent from I2CDevice class. @n
 * If you previously wrote I2C drivers you probably saw that some part of your code where commonly used, for example when you wanted to clear some bit
 * in register (or memory at some address) of I2C device you had to read the register value, cleared the bit and finally wrote modified value
 * to the register. To simplify this operation I2CDevice class contains  @b clearBitsInRegister method. If you want to set bits in register you should
 * use @b setBitsInRegister method. Some devices that can be connected to I2C data bus have more than 8 bit registers so all functions in I2CDevice
 * class have been overloaded for different registers length. I2CDevice methods are overloaded for: <b>uint8_t, uint16_t, uint32_t, int8_t, int16_t,
 * int32_t</b> types. If the register of a device has length greater than 8 bits it may have different endianness so all functions taking parameter
 * greater than 8 bits also take @a endianness parameter. Thanks to @a endianness parameter your driver is platform independent because all necessary
 * conversions are implemented inside I2CDevice class. Briefly, API of I2CDevice class looks like this: @n <b>
 * bool writeRegister(address, value) @n
 * bool readRegister(address, value) @n
 * bool setBitsInRegister(address, value) @n
 * bool clearBitsInRegister(address, value) @n
 * bool writeRegisterWithMask(address, value, mask) @n</b>
 * All functions above take additional parameter @a endianness when value parameter is more than 8 bit.
 * Also when you just want to write and read data from a device you can use: @n <b>
 * bool write(uint8_t data) @n
 * bool read(uint8_t &data) @n</b>
 * All methods mentioned above are thread safe, so you can use them in multithread environment.
 * When some operations fail you can check error by checking a result of @ref getLastError method. @n
 * For more information read description of each methods.
 */
/* **************************************************************************************************************************************************
 * CLASS
 */
class I2CDevice {
 public:
    /**
     * Class copying was disabled by deleting copy constructor and assigning operator.
     */
    I2CDevice(I2CDevice &) = delete;                   // disable copying by delete copy constructor
    I2CDevice &operator=(const I2CDevice &) = delete;  // and assign operator
    I2CDevice() = delete;
    /**
     * @brief Default constructor for I2Cdevice class.
     *
     * @param i2c - reference to I2C controller object
     * @param meAddress - address of device connected to I2C data buss. This address is in 8bit form @a aaaaaaax where @a a are address bits and
     * 					  @a x bit changes when you want to write or read from device. When you passing device address @a x have to be 0.
     */
    explicit constexpr I2CDevice(I2C &i2c, const uint8_t meAddress) : i2c(i2c), deviceAddress(meAddress), lastError(I2C::NoError) {}
    ~I2CDevice() = default;
    //------------------------------------------ functions ----------------------------------------
    /**
     * @brief This function return address of the device.
     *
     * @return I2C device address. Last bit of returned address indicate write or read operation and is always set to 0.
     */
    uint8_t getMyI2CAddress() const noexcept { return deviceAddress; }
    /**
     * @brief This function return last error of this device. To see full error list please go to @ref I2C::Error
     *
     * @return last error
     */
    I2C::Error getLastError() const noexcept { return lastError; }
    /**
     * @brief This function write 8 bit data into the I2C device. If an error occurred this function return false and you can use @ref getLastError
     * function to check the error cause.
     * @remark This function is thread-safe.
     *
     * @param data to write.
     *
     * @retval true - if data was sent.
     * @retval false - if an error occurred.
     */
    bool write(uint8_t data) {
        i2c.lock();
        I2C::Error status = i2c.write(deviceAddress, data);
        i2c.unlock();
        if (status == I2C::NoError) {
            return true;
        } else {
            lastError = status;
            return false;
        }
    }
    /**
     * @brief This function read 8 bit data from the I2C device. If an error occurred you can use @ref getLastError function
     * to check the error cause.
     * @remark This function is thread-safe.
     *
     * @param[out] data - reference to memory where read data will be stored.
     *
     * @retval true if data was sent.
     * @retval false if an error occurred.
     */
    bool read(uint8_t &data) {
        i2c.lock();
        I2C::Error status = i2c.read(deviceAddress, data);
        i2c.unlock();
        if (status == I2C::NoError) {
            return true;
        } else {
            lastError = status;
            return false;
        }
    }

    //    /**
    //     * @brief This function read 8bit data from I2C device.
    //     *
    //     * @param data read data.
    //     *
    //     * @retval true if data was sent.
    //     * @retval false if an error occurred.
    //     */
    //    bool read(uint8_t *data, size_t size) {
    //        i2c.lock();
    //        I2C::Error status = i2c.read(deviceAddress, data, size);
    //        i2c.unlock();
    //        if (status == I2C::NoError) {
    //            return true;
    //        } else {
    //            lastError = status;
    //            return false;
    //        }
    //    }
    // ---------------------------------------------------------------------------- write unsigned types
    // ------------------------------------------------
    /**@brief This function write 8 bit data at specified address. This address can point to memory cell or some device register.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[in] data to write. Unsigned 8 bit variable.
     *
     * @retval true if writing was successful
     * @retval false if an error occurred
     */
    inline bool writeRegister(uint8_t address, uint8_t data) {
        i2c.lock();
        bool status = writeRegister_noLock(address, data);
        i2c.unlock();

        return status;
    }
    /**
     * @brief This function write 16 bit unsigned data to register/memory cell at specified address.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[in] data to write. Data length is 16 bit.
     * @param[in] endianness - register byte order in device.
     *
     * @retval true if data was write
     * @retval false if an error occurred.
     */
    inline bool writeRegister(uint8_t address, uint16_t data, Endianness endianness) { return writeRegister_impl(address, data, endianness); }
    /**
     * @brief This function write 32 bit unsigned data to register/memory cell at specified address.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[in] data - data to write
     * @param[in] endianness - register byte order in device.
     *
     * @retval true if data was write
     * @retval false if an error occurred.
     */
    inline bool writeRegister(uint8_t address, uint32_t data, Endianness endianness) { return writeRegister_impl(address, data, endianness); }
    inline bool writeRegister(uint8_t address, uint64_t data, Endianness endianness) { return writeRegister_impl(address, data, endianness); }
    // ---------------------------------------------------------------------------- write signed types
    // --------------------------------------------------
    /**
     * @brief This function work same as writeRegister(uint8_t address, uint8_t data) but takes signed parameter.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[in] data to write in to register
     *
     * @retval true if writing was successful
     * @retval false if an error occurred
     */
    inline bool writeRegister(uint8_t address, int8_t data) __attribute__((always_inline)) {
        return writeRegister(address, static_cast<uint8_t>(data));
    }
    /**@brief This function work same as writeRegister(uint8_t address, uint16_t data, Endianness endianness) but takes signed parameter.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[in] data to write in the register
     * @param[in] endianness - byte order in I2C device
     *
     * @retval true if writing was successful
     * @retval false if an error occurred
     */
    inline bool writeRegister(uint8_t address, int16_t data, Endianness endianness) __attribute__((always_inline)) {
        return writeRegister(address, static_cast<uint16_t>(data), endianness);
    }
    /**@brief TThis function work same as writeRegister(uint8_t address, uint32_t data, Endianness endianness) but takes signed parameter.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[in] data to write in the register
     * @param[in] endianness - byte order in I2C device
     *
     * @retval true if writing was successful
     * @retval false if an error occurred
     */
    inline bool writeRegister(uint8_t address, int32_t data, Endianness endianness) __attribute__((always_inline)) {
        return writeRegister(address, static_cast<uint32_t>(data), endianness);
    }
    // ---------------------------------------------------------------------------- read unsigned types -------------------------------------------------
    /**@brief This function read 8 bit unsigned data from register or memory cell at specified address of the I2C device.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[out] data - read data
     *
     * @retval true if data was read.
     * @retval false if an error occurred.
     */
    inline bool readRegister(uint8_t address, uint8_t &data) {
        i2c.lock();
        bool status = readRegister_noLock(address, data);
        i2c.unlock();

        return status;
    }
    /**
     * @brief This function read 16 bit unsigned data from register or memory cell at specified address of the I2C device.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[out] data - read data
     * @param[in] endianness - byte order in I2C device
     *
     * @retval true if data was read.
     * @retval false if an error occurred.
     */
    inline bool readRegister(uint8_t address, uint16_t &data, Endianness endianness) { return readRegister_impl(address, data, endianness); }
    /**
     * @brief This function read 32 bit unsigned data from register or memory cell at specified address of the I2C device.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[out] data - read data
     * @param[in] endianness - byte order in I2C device
     *
     * @retval true if data was read.
     * @retval false if an error occurred.
     */
    inline bool readRegister(uint8_t address, uint32_t &data, Endianness endianness) { return readRegister_impl(address, data, endianness); }
    inline bool readRegister(uint8_t address, uint64_t &data, Endianness endianness) { return readRegister_impl(address, data, endianness); }
    // ---------------------------------------------------------------------------- read signed types ---------------------------------------------------
    /**@brief This function work same as readRegister(uint8_t address, uint8_t &data) but takes signed parameter.
         * @remark This function is thread-safe.
         *
         * @param[in] address of register/memory cell from data will be read. This address is 8 bit long.
         * @param[out] data read from the register
         *
         * @retval true if writing was successful
         * @retval false if an error occurred
         */
    inline bool readRegister(uint8_t address, int8_t &data) __attribute__((always_inline)) { return readRegister(address, (uint8_t &)data); }
    /**@brief This function work same as readRegister(uint8_t address, uint16_t &data, Endianness endianness) but takes signed parameter.
         * @remark This function is thread-safe.
         *
         * @param[in] address of register/memory cell from data will be read. This address is 8 bit long.
         * @param[out] data read from the register
         * @param[in] endianness - byte order in I2C device
         *
         * @retval true if writing was successful
         * @retval false if an error occurred
         */
    inline bool readRegister(uint8_t address, int16_t &data, Endianness endianness) __attribute__((always_inline)) {
        return readRegister(address, (uint16_t &)data, endianness);
    }
    /**@brief This function work same as readRegister(uint8_t address, uint32_t &data, Endianness endianness) but takes signed parameter.
         * @remark This function is thread-safe.
         *
         * @param[in] address of register/memory cell from data will be read. This address is 8 bit long.
         * @param[out] data read from the register
         * @param[in] endianness - byte order in I2C device
         *
         * @retval true if writing was successful
         * @retval false if an error occurred
         */
    inline bool readRegister(uint8_t address, int32_t &data, Endianness endianness) __attribute__((always_inline)) {
        return readRegister(address, (uint32_t &)data, endianness);
    }
    // ---------------------------------------------------------------------------- write unsigned types ------------------------------------------------
    /**@brief This function write data to 8 bit registers.
     *
     * @param address address of first register where data will be written
     * @param buff pointer to buffer witch data to write
     * @param size number of bytes to be written
     *
     * @retval true if writing data was successful
     * @retval false if an error occurred
     */
    bool writeRegisters(uint8_t address, const uint8_t *buff, size_t size) {
        i2c.lock();

        I2C::Error status = i2c.write(deviceAddress, address, buff, size);

        i2c.unlock();

        if (status == I2C::NoError) {
            return true;
        } else {
            lastError = status;
            return false;
        }
    }
    /**@brief This function write data to 16 bit registers.
     *
     * @param address address of first register where data will be written
     * @param buff pointer to buffer witch data to write
     * @param size number of bytes to be written
     *
     * @retval true if writing data was successful
     * @retval false if an error occurred
     */
    bool writeRegisters(uint8_t address, const uint16_t *buff, size_t size) {
        return writeRegisters(address, reinterpret_cast<const uint8_t *>(buff), 2 * size);
    }
    /**@brief This function write data to 16 bit registers.
     *
     * @param address address of first register where data will be written
     * @param buff pointer to buffer witch data to write
     * @param size number of bytes to be written
     *
     * @retval true if writing data was successful
     * @retval false if an error occurred
     */
    bool writeRegisters(uint8_t address, const uint32_t *buff, size_t size) {
        return writeRegisters(address, reinterpret_cast<const uint8_t *>(buff), 4 * size);
    }
    // ---------------------------------------------------------------------------- read unsigned types -------------------------------------------------
    /**@brief This function read 8 bit registers.
     *
     * @param[in] address - address of first register
     * @param[out] buff - pointer to buffer where data will be stored.
     * @param[in] size - number of bytes to read.
     *
     * @retval true if data was read.
     * @retval false if an error occurred.
     */
    bool readRegisters(uint8_t address, uint8_t *buff, size_t size) {
        i2c.lock();

        I2C::Error status = i2c.read(deviceAddress, address, buff, size);

        i2c.unlock();

        if (status == I2C::NoError) {
            return true;
        } else {
            lastError = status;
            return false;
        }
    }

    bool readRegisters(uint8_t address, uint16_t *buffer, size_t size, Endianness endianness);
    bool readRegisters(uint8_t address, uint32_t *buffer, size_t size, Endianness endianness);
    // ---------------------------------------------------------------------------- set bits in register ------------------------------------------------
    /**@brief This function set bits in 8 bit register.
     * @remark This function is thread-safe.
     *
     * @param address of register/memory cell where bits will be set. This address is 8 bit long.
     * @param bits to set. When 1 then corresponding bits in register will be set.
     *
     * @retval true if bit was set.
     * @retval false if an error occurred.
     */
    inline bool setBitsInRegister(uint8_t address, uint8_t bits) {
        i2c.lock();

        uint8_t data;
        bool status = readRegister_noLock(address, data);
        if (status == true) {
            data |= bits;
            status = writeRegister_noLock(address, data);
        }

        i2c.unlock();
        return status;
    }
    /**
     * @brief This function set bits in 16bit unsigned register.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[in] bits - if 1 then corresponding bit will be set.
     * @param[in] endianness - byte order in I2C device.
     *
     * @retval true if bits was set.
     * @retval false if an error occurred.
     */
    bool setBitsInRegister(uint8_t address, uint16_t bits, Endianness endianness);
    /**
     * @brief This function set bits in 32bit unsigned register.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[in] bits - if 1 then corresponding bit will be set.
     * @param[in] endianness - byte order in I2C device.
     *
     * @retval true if bits was set.
     * @retval false if an error occurred.
     */
    bool setBitsInRegister(uint8_t address, uint32_t bits, Endianness endianness);
    // ---------------------------------------------------------------------------- clear bits in register ----------------------------------------------
    /**@brief This function clears bits in 8 bit register.
     * @remark This function is thread-safe.
     *
     * @param address of register/memory cell where data will be written. This address is 8 bit long.
     * @param bits to clear. When 1 then corresponding bit will be cleared.
     *
     * @retval true if bit was set
     * @retval false if an error occurred
     */
    bool clearBitsInRegister(uint8_t address, uint8_t bits) {
        i2c.lock();

        uint8_t data;
        bool status = readRegister_noLock(address, data);
        if (status == true) {
            data &= ~bits;
            status = writeRegister_noLock(address, data);
        }

        i2c.unlock();
        return status;
    }
    /**
     * @brief This function clears bits in 16 bit unsigned register.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[in] bits to clear. When 1 then corresponding bit will be cleared.
     * @param[in] endianness - byte order in the I2C device
     *
     * @retval true if bits was cleared successful.
     * @retval false if an error occurred.
     */
    bool clearBitsInRegister(const uint8_t address, uint16_t bits, Endianness endianness);
    /**
     * @brief This function clears bits in 32 bit unsigned register.
     * @remark This function is thread-safe.
     *
     * @param[in] address of register/memory cell where data will be written. This address is 8 bit long.
     * @param[in] bits to clear. When 1 then corresponding bit will be cleared.
     * @param[in] endianness - byte order in the I2C device
     *
     * @retval true if bits was cleared successful.
     * @retval false if an error occurred.
     */
    bool clearBitsInRegister(const uint8_t address, uint32_t bits, Endianness endianness);
    // ---------------------------------------------------------------------------- write bits in register with mask ------------------------------------
    /**@brief This function write bits from data parameter to 8bit register but only bits set to one in mask parameter are changed.
     * @remark This function is thread-safe.
     *
     * @param address of register/memory cell where data will be written. This address is 8 bit long.
     * @param data to write
     * @param mask if set then corresponding bit in register will be written.
     *
     * @retval true if bits was set
     * @retval false if an error occurred
     */
    inline bool writeRegisterWithMask(uint8_t address, uint8_t data, uint8_t mask) {
        bool status = false;
        i2c.lock();

        uint8_t readData;
        if (readRegister_noLock(address, readData)) {
            // clear bits
            readData &= ~mask;
            // set new value
            readData |= (data & mask);
            status = writeRegister_noLock(address, readData);
        }

        i2c.unlock();
        return status;
    }
    /**@brief This function write bits from data parameter to 8bit register but only bits set to one in mask parameter are changed.
     * @remark This function is thread-safe.
     *
     * @param address of register/memory cell where data will be written. This address is 8 bit long.
     * @param data to write
     * @param mask if set then corresponding bit in register will be written.
     * @param[in] endianness - byte order in the I2C device
     *
     * @retval true if bits was set
     * @retval false if an error occurred
     */
    bool writeRegisterWithMask(uint8_t address, uint16_t data, uint16_t mask, Endianness endianness);
    /**@brief This function write bits from data parameter to 8bit register but only bits set to one in mask parameter are changed.
     * @remark This function is thread-safe.
     *
     * @param address of register/memory cell where data will be written. This address is 8 bit long.
     * @param data to write
     * @param mask if set then corresponding bit in register will be written.
     * @param[in] endianness - byte order in the I2C device
     *
     * @retval true if bits was set
     * @retval false if an error occurred
     */
    bool writeRegisterWithMask(uint8_t address, uint32_t data, uint32_t mask, Endianness endianness);

 private:
    I2C &i2c;
    const uint8_t deviceAddress;  ///< address of I2C slave device
    I2C::Error lastError;

    template <typename T>
    inline bool writeRegister_impl(uint8_t address, T data, Endianness endianness) {
        // convert endianness if needed
        if (endianness != DeviceEndianness) {
            // do conversion
            data = byteswap(data);
        }
        // write registers
        return writeRegisters(address, reinterpret_cast<uint8_t *>(&data), sizeof(T));
    }

    template <typename T>
    inline bool readRegister_impl(uint8_t address, T &data, Endianness endianness) {
        if (readRegisters(address, reinterpret_cast<uint8_t *>(&data), sizeof(T))) {
            // convert endianness if needed
            if (endianness != DeviceEndianness) {
                // do conversion
                data = byteswap(data);
            }

            return true;
        } else {
            return false;
        }
    }
    /**
     * @brief This function read few registers from I2C device.
     *
     * @param[in] registerAddress - address of first register to read.
     * @param[out] buffer - pointer to buffer where read data will be stored.
     * @param[in] size - number of registers to read
     * @param[in] endianness - byte order in I2C device
     *
     * @retval true if data was read
     * @retval false if an error occurred.
     */
    template <typename T>
    bool readRegisters_impl(uint8_t registerAddress, T *buffer, size_t size, Endianness endianness) {
        if (readRegisters(registerAddress, (uint8_t *)buffer, sizeof(T) * size) == true) {
            // convert endianness if needed
            if (endianness != DeviceEndianness) {
                // do conversion
                for (size_t i = 0; i < size; i++) {
                    buffer[i] = byteswap(buffer[i]);
                }
            }
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    bool setBitsInRegister_impl(uint8_t address, T bitMask, Endianness endianness);

    template <typename T>
    bool clearBitsInRegister_impl(uint8_t address, T bitMask, Endianness endianness);

    template <typename T>
    bool writeRegisterWithMask_impl(uint8_t address, T data, T mask, Endianness endianness);

    inline bool readRegister_noLock(uint8_t address, uint8_t &data) {
        I2C::Error status = i2c.read(deviceAddress, address, data);

        if (status == I2C::NoError) {
            return true;
        } else {
            lastError = status;
            return false;
        }
    }
    inline bool writeRegister_noLock(uint8_t address, const uint8_t data) {
        I2C::Error status = i2c.write(deviceAddress, address, data);

        if (status == I2C::NoError) {
            return true;
        }
        lastError = status;
        return false;
    }
};
/**
 * @}
 */
}  // namespace microhal
#endif  // _MICROHAL_I2CDEVICE_H_
