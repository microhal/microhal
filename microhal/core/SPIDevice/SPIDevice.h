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
#include <array>
#include <cstdint>
#include "../utils/deviceRegister.h"
#include "gpio.h"
#include "gsl/span"
#include "ports/manager/hardware.h"
#include "ports/manager/spi_port.h"
#include "spi.h"
#include "utils/byteswap.h"
#include "utils/result.h"

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
    SPIDevice(SPI &spi, microhal::GPIO &CEPin) : spi(spi), cePin(CEPin) {
        cePin.configureAsOutput(GPIO::OutputType::PushPull, GPIO::PullType::NoPull);
        cePin.set();
    }
    ~SPIDevice() { cePin.set(); }
    //------------------------------------------ functions ----------------------------------------
    void chipEnablePinDelay(uint32_t delay_ns) { chipEnableDelay_ns = delay_ns; }
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
    SPI::Error write(uint8_t data);
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
    SPI::Error read_to(uint8_t &data);

    /**
     * @brief This function write data into register of the SPI device.
     * This function automatically handle endianness conversion.
     * @remark This function is thread-safe.
     * @remark This function is no-throw guarantee.
     *
     * @param reg register to write.
     * @param value to write
     *
     * @retval This function will return SPI::Error::None if data was written correctly. If an error occurred this function will return
     * corresponding SPI::Error code.
     */
    template <typename Register>
    SPI::Error writeRegister(Register reg, typename Register::Type value) {
        static_assert(reg.access() != Access::ReadOnly, "You can't write data to read only register.");
        if constexpr (reg.requireEndiannessConversion()) {
            value = microhal::convertEndianness(value);
        }
        return writeRegister_lock(reg.getWriteAddress(), value);
    }

    /**
     * @brief This function write data into FIFO register of the SPI device.
     * This function automatically handle endianness conversion.
     * @remark This function is thread-safe.
     * @remark This function is no-throw guarantee.
     *
     * @param reg register to write.
     * @param value to write
     *
     * @retval This function will return SPI::Error::None if data was written correctly. If an error occurred this function will return
     * corresponding SPI::Error code.
     */
    template <typename Register>
    SPI::Error writeFIFORegister(Register reg, gsl::span<typename Register::Type> value) {
        static_assert(reg.access() != Access::ReadOnly, "You can't write data to read only register.");
        if constexpr (reg.requireEndiannessConversion()) {
            value = microhal::convertEndianness(value);
        }
        return writeRegisters_noEndiannessConversion(reg.getWriteAddress(), value.data(), value.size());
    }

    /**
     * @brief This function read data from register of the SPI device.
     * This function automatically handle endianness conversion.
     * @remark This function is thread-safe.
     * @remark This function is no-throw guarantee.
     *
     * @param reg register to read.
     *
     * @retval This function will return @ref Result object that contain error code and read value. Result error code is equal SPI::Error::None if
     * data was read correctly. If an error occurred this function will return corresponding SPI::Error code. Result object can be converted to bool
     * witch true value when Result object don't hold error.
     */
    template <typename Register>
    microhal::Result<typename Register::Type, SPI::Error, SPI::Error::None> readRegister(Register reg) {
        static_assert(reg.access() != Access::WriteOnly, "You can't read data from write only register.");

        typename Register::Type value;
        if constexpr (reg.requireEndiannessConversion()) {
            const auto error = readRegister_convertEndiannes(reg.getReadAddress(), value);
            return {error, value};
        } else {
            const auto error = readRegister_noEndiannesConversion(reg.getReadAddress(), value);
            return {error, value};
        }
    }

    /**
     * @brief This function read data from register of the SPI device.
     * This function automatically handle endianness conversion.
     * @remark This function is thread-safe.
     * @remark This function is no-throw guarantee.
     *
     * @param[in] reg register to read.
     * @param[out] value reference to place where register value will be stored.
     *
     * @return This function will return SPI::Error::None if data was written correctly. If an error occurred this function will return
     * corresponding SPI::Error code.
     */
    template <typename Register>
    microhal::Result<typename Register::Type, SPI::Error, SPI::Error::None> readRegister_to(Register reg, typename Register::Type &value) {
        static_assert(reg.access() != Access::WriteOnly, "You can't read data from write only register.");

        if constexpr (reg.requireEndiannessConversion()) {
            return readRegister_convertEndiannes(reg.getReadAddress(), value);
        } else {
            return readRegister_noEndiannesConversion(reg.getReadAddress(), value);
        }
    }

    /**
     * @brief This function read data from FIFO register of the SPI device.
     * This function automatically handle endianness conversion.
     * @remark This function is thread-safe.
     * @remark This function is no-throw guarantee.
     *
     * @param[in] reg register to read.
     * @param[out] data pointer to place where register value will be stored.
     *
     * @return This function will return SPI::Error::None if data was written correctly. If an error occurred this function will return
     * corresponding SPI::Error code.
     */
    template <typename Register>
    SPI::Error readFIFORegister_to(Register reg, typename Register::Type *data, size_t length) {
        static_assert(reg.access() != Access::WriteOnly, "You can't read data from write only register.");

        if constexpr (reg.requireEndiannessConversion()) {
            return readRegisters_convertEndianness(reg.getReadAddress(), data, length);
        } else {
            return readRegisters_noEndiannessConversion(reg.getReadAddress(), data, length);
        }
    }

    /**
     * @brief This function sets bits in register of the SPI device.
     * This function automatically handle endianness conversion.
     * @remark This function is thread-safe.
     * @remark This function is no-throw guarantee.
     *
     * @param reg register where bits will be set.
     * @param value if bit in value is set then corresponding bit in register will be set.
     *
     * @retval This function will return SPI::Error::None if data was written correctly. If an error occurred this function will return
     * corresponding SPI::Error code.
     */
    template <typename Register>
    SPI::Error setBitsInRegister(Register reg, typename Register::Type value) {
        static_assert(reg.access() == Access::ReadWrite, "Bits can be modify only in WriteRead registers.");

        if constexpr (reg.requireEndiannessConversion()) {
            value = microhal::convertEndianness(value);
        }
        return modifyBitsInRegister_noEndiannesConversion(reg.getReadAddress(), reg.getWriteAddress(), value, value);
    }

    /**
     * @brief This function clear bits in register of the SPI device.
     * This function automatically handle endianness conversion.
     * @remark This function is thread-safe.
     * @remark This function is no-throw guarantee.
     *
     * @param reg register where bits will be cleared.
     * @param value if bits is set then corresponding bit in register will be cleared.
     *
     * @retval This function will return SPI::Error::None if data was written correctly. If an error occurred this function will return
     * corresponding SPI::Error code.
     */
    template <typename Register>
    SPI::Error clearBitsInRegister(Register reg, typename Register::Type value) {
        static_assert(reg.access() == Access::ReadWrite, "Bits can be modify only in WriteRead registers.");

        if constexpr (reg.requireEndiannessConversion()) {
            value = microhal::convertEndianness(value);
        }
        return modifyBitsInRegister_noEndiannesConversion(reg.getReadAddress(), reg.getWriteAddress(), ~value, value);
    }

    /**
     * @brief This function modify content of the register of the I2C device.
     * This function automatically handle endianness conversion.
     * @example Suppose that register content is equal to 0b1111; value field is set to 0b0101 and mask is set to 0b0011. As a result new register
     * value will be equal to 0b1101.
     * @remark This function is thread-safe.
     * @remark This function is no-throw guarantee.
     *
     * @param reg register to modify.
     * @param value new value to set. Bits from this field will be written only if corresponding bits in mask parameter are set.
     * @param mask if bit is set then corresponding bit in register will be set.
     *
     * @retval This function will return SPI::Error::None if data was written correctly. If an error occurred this function will return
     * corresponding SPI::Error code.
     */
    template <typename Register>
    SPI::Error modifyBitsInRegister(Register reg, typename Register::Type value, typename Register::Type mask) {
        static_assert(reg.access() == Access::ReadWrite, "Bits can be modify only in WriteRead registers.");

        if constexpr (reg.requireEndiannessConversion()) {
            mask = microhal::convertEndianness(mask);
            value = microhal::convertEndianness(value);
        }
        return modifyBitsInRegister_noEndiannesConversion(reg.getReadAddress(), reg.getWriteAddress(), value, mask);
    }

    /**
     * @brief This function allow you to write few register in one SPI transaction.
     * This function automatically handle endianness conversion.
     *
     * @remark This function is thread-safe.
     * @remark This function is no-throw guarantee.
     *
     * @param[in] array with data that will be written into registers.
     * @param reg registers to write. This registers have to be sorted in rising order and continuous.
     *
     * @retval This function will return SPI::Error::None if data was written correctly. If an error occurred this function will return
     * corresponding SPI::Error code.
     */
    template <typename ArrayType, size_t N, typename... Registers>
    SPI::Error writeMultipleRegisters(const std::array<ArrayType, N> &array, Registers... reg) {
        static_assert(sizeof...(Registers) > 1, "You are trying to write only one register, please use write function.");
        static_assert(sizeof...(Registers) == N, "Size of array have to be equal to number of registers.");
        static_assert(std::conjunction_v<std::is_same<ArrayType, typename Registers::Type>...>,
                      "Incompatible array and register types. Using this function you can write data into registers with the same type and your "
                      "array have to have the same type as registers. If you want to write multiple registers with different types please use "
                      "std::tuple instead of std::aray.");
        if constexpr (std::conjunction_v<std::is_same<ArrayType, typename Registers::Type>...>) {
            static_assert(sizeOfRegistersData(reg...) == N * sizeof(ArrayType), "microhal internal error.");
        }
        isContinous(reg...);
        accessCheck<Access::ReadOnly>(reg...);

        const auto firstReg = microhal::first(reg...);
        const auto registerWriteAddress = firstReg.getWriteAddress();

        if constexpr (sizeof(ArrayType) > 1) {
            std::array<ArrayType, N> tmpArray = array;
            convertEndianness(tmpArray, reg...);
            return writeRegisters_noEndiannessConversion(registerWriteAddress, (uint8_t *)tmpArray.data(), N * sizeof(ArrayType));
        } else {
            return writeRegisters_noEndiannessConversion(registerWriteAddress, (uint8_t *)array.data(), N * sizeof(ArrayType));
        }
    }

    /**
     * @brief This function allow you to read multiple register in one I2C transaction.
     * This function automatically handle endianness conversion.
     *
     * @remark This function is thread-safe.
     * @remark This function is no-throw guarantee.
     *
     * @param[in] array where register values will be stored.
     * @param reg registers to read. This registers have to be sorted in rising order and be continuous.
     *
     * @retval This function will return I2C::Error::None if data was written correctly. If an error occurred this function will return
     * corresponding I2C::Error code.
     */
    template <typename ArrayType, size_t N, typename... Registers>
    SPI::Error readMultipleRegisters_to(std::array<ArrayType, N> &array, Registers... reg) {
        static_assert(sizeof...(reg) > 1, "You are trying to read only one register, please use write function.");
        static_assert(sizeof...(reg) == N, "Size of array have to be equal to number of registers.");
        static_assert(std::conjunction_v<std::is_same<ArrayType, typename Registers::Type>...>, "Incompatible array type with register types.");
        if constexpr (std::conjunction_v<std::is_same<ArrayType, typename Registers::Type>...>) {
            static_assert(sizeOfRegistersData(reg...) == N * sizeof(ArrayType), "microhal internal error.");
        }
        microhal::isContinous(reg...);
        microhal::accessCheck<Access::WriteOnly>(reg...);

        const auto firstReg = microhal::first(reg...);
        const auto registerReadAddress = firstReg.getReadAddress();

        if constexpr (sizeof(ArrayType) > 1) {
            auto status = readRegisters_noEndiannessConversion(registerReadAddress, (uint8_t *)array.data(), N * sizeof(ArrayType));
            convertEndianness(array, reg...);
            return status;
        } else {
            return readRegisters_noEndiannessConversion(registerReadAddress, (uint8_t *)array.data(), N * sizeof(ArrayType));
        }
    }

    //-------------------------------------------- buffer reading functions ---------------------------
    SPI::Error writeBuffer(const uint8_t *buffer, size_t length, bool last = true);

    //-------------------------------------------- buffer reading functions ---------------------------
    bool readBuffer(uint8_t *buffer, size_t length, bool last = true);

 protected:
    void SPIlock() { spi.lock(); }
    void SPIunlock() { spi.unlock(); }

    // bool writeRegisters(uint8_t registerAddress, const uint8_t *buffer, size_t length);

    SPI::Error writeRegister_lock(uint8_t registerAddress, uint8_t data);
    SPI::Error writeRegister_lock(uint8_t registerAddress, uint16_t data);
    SPI::Error writeRegister_lock(uint8_t registerAddress, microhal::uint24_t data);
    SPI::Error writeRegister_noLock(uint8_t registerAddress, uint8_t data);

    SPI::Error readRegister_noLock(uint8_t registerAddress, uint8_t &data);
    SPI::Error readRegister_noLock_noEndiannesConversion(uint8_t registerAddress, uint8_t &data);
    SPI::Error readRegister_noLock_noEndiannessConversion(uint8_t registerAddress, uint16_t &data);
    SPI::Error readRegister_noLock_noEndiannessConversion(uint8_t registerAddress, microhal::uint24_t &data);
    SPI::Error readRegister_noLock_noEndiannessConversion(uint8_t registerAddress, uint32_t &data);
    SPI::Error readRegister_noEndiannesConversion(uint8_t registerAddress, uint8_t &data);
    SPI::Error readRegister_noEndiannesConversion(uint8_t registerAddress, uint16_t &data);
    SPI::Error readRegister_noEndiannesConversion(uint8_t registerAddress, uint32_t &data);
    SPI::Error readRegister_convertEndiannes(uint8_t registerAddress, uint16_t &data);
    SPI::Error readRegister_convertEndiannes(uint8_t registerAddress, microhal::uint24_t &data);
    SPI::Error readRegister_convertEndiannes(uint8_t registerAddress, uint32_t &data);

    SPI::Error writeRegister_noLock_noEndiannessConversion(uint8_t registerAddress, uint8_t data);
    SPI::Error writeRegister_noLock_noEndiannessConversion(uint8_t registerAddress, uint16_t data);
    SPI::Error writeRegister_noLock_noEndiannessConversion(uint8_t registerAddress, microhal::uint24_t data);
    SPI::Error writeRegister_noLock_noEndiannessConversion(uint8_t registerAddress, uint32_t data);

    SPI::Error writeRegisters_noEndiannessConversion(uint8_t registerAddress, const uint8_t *buffer, size_t length);
    SPI::Error readRegisters_noEndiannessConversion(uint8_t registerAddress, uint8_t *buffer, size_t length);

    SPI::Error modifyBitsInRegister_noEndiannesConversion(uint8_t readRegisterAddress, uint8_t writeRegisterAddress, uint8_t data, uint8_t mask);
    SPI::Error modifyBitsInRegister_noEndiannesConversion(uint8_t readRegisterAddress, uint8_t writeRegisterAddress, uint16_t data, uint16_t mask);
    SPI::Error modifyBitsInRegister_noEndiannesConversion(uint8_t readRegisterAddress, uint8_t writeRegisterAddress, uint32_t data, uint32_t mask);

    SPI::Error readRegister_noLock_impl(uint8_t registerAddress, uint8_t *data, size_t len);
    SPI::Error write_noLock(const uint8_t *data, size_t len);

 private:
    uint32_t chipEnableDelay_ns = 300;
    SPI &spi;
    GPIO &cePin;

    template <typename Type, size_t N, typename Register, typename... Registers>
    void convertEndianness(std::array<Type, N> &array, Register reg, Registers... regs) {
        auto index = array.size() - sizeof...(Registers) - 1;
        if constexpr (reg.requireEndiannessConversion()) {
            array[index] = microhal::convertEndianness(array[index]);
        }
        if constexpr (sizeof...(Registers)) convertEndianness(array, regs...);
    }
};

}  // namespace microhal

#endif  // _MICROHAL_SPIDEVICE_H_
