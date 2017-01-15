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
#include "../utils/deviceRegister.h"
#include "byteswap.h"
#include "ports/manager/hardware.h"

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
	using DeviceAddress = I2C::DeviceAddress;
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
    explicit constexpr I2CDevice(I2C &i2c, DeviceAddress meAddress) : i2c(i2c), deviceAddress(meAddress) {}
    //------------------------------------------ functions ----------------------------------------
    /**
     * @brief This function return address of the device.
     *
     * @return I2C device address. Last bit of returned address indicate write or read operation and is always set to 0.
     */
    constexpr DeviceAddress address() const noexcept { return deviceAddress; }
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
    I2C::Error write(uint8_t data) {
    	std::lock_guard<I2C> guard(i2c);
        return i2c.write(deviceAddress, data);
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
    I2C::Error read(uint8_t *data) {
    	std::lock_guard<I2C> guard(i2c);
        return i2c.read(deviceAddress, data, 1);
    }


    template<typename Register>
    I2C::Error write(Register reg, typename Register::Type value) {
    	static_assert(reg.access() != Access::ReadOnly, "You can't write data to read only register.");
    	typename Register::Address::Type tmp = Register::Address::value;
        std::lock_guard<I2C> guard(i2c);
        return i2c.write(deviceAddress, reinterpret_cast<const uint8_t*>(&tmp), sizeof(tmp), reinterpret_cast<const uint8_t*>(&value), sizeof(value));
    }

    template<typename Register>
    I2C::Error read(Register reg, typename Register::Type &value) {
    	static_assert(reg.access() != Access::WriteOnly, "You can't read data from write only register.");
    	const auto tmp = reg.getAddress();
    	std::lock_guard<I2C> guard(i2c);
        return i2c.writeRead(deviceAddress, static_cast<const uint8_t*>(&tmp), sizeof(tmp), reinterpret_cast<uint8_t*>(&value), sizeof(value));
    }

    template<typename Register>
    I2C::Error bitsSet(Register reg, typename Register::Type value) {
    	static_assert(reg.access() == Access::ReadWrite, "Bits can be modify only in WriteRead registers.");
        typename Register::Type tmp;
        typename Register::Address::Type address = Register::Address::value;
        std::lock_guard<I2C> guard(i2c);
        const auto status = i2c.writeRead(deviceAddress, &address, sizeof(address), reinterpret_cast<uint8_t*>(&tmp), sizeof(tmp));
        if (status == I2C::Error::NoError) {
        	tmp |= value;
        	return i2c.write(deviceAddress, &address, sizeof(address), reinterpret_cast<const uint8_t*>(&tmp), sizeof(tmp));
        }
        return status;
    }

    template<typename Register>
    I2C::Error bitsClear(Register reg, typename Register::Type value) {
    	static_assert(reg.access() == Access::ReadWrite, "Bits can be modify only in WriteRead registers.");
        typename Register::Type tmp;
        typename Register::Address::Type address = Register::Address::value;
        std::lock_guard<I2C> guard(i2c);
        const auto status = i2c.writeRead(deviceAddress, static_cast<const uint8_t*>(&address), sizeof(address), reinterpret_cast<uint8_t*>(&tmp), sizeof(tmp));
        if (status == I2C::Error::NoError) {
        	tmp &= ~value;
        	return i2c.write(deviceAddress, static_cast<const uint8_t*>(&address), sizeof(address), reinterpret_cast<const uint8_t*>(&tmp), sizeof(tmp));
        }
        return status;
    }

    template<typename Register>
    I2C::Error bitsModify(Register reg, typename Register::Type value, typename Register::Type mask) {
    	static_assert(reg.access() == Access::ReadWrite, "Bits can be modify only in WriteRead registers.");
    	typename Register::Type tmp;
    	typename Register::Address::Type address = Register::Address::value;
    	std::lock_guard<I2C> guard(i2c);
    	const auto status = i2c.writeRead(deviceAddress, static_cast<const uint8_t*>(&address), sizeof(address), reinterpret_cast<uint8_t*>(&tmp), sizeof(tmp));
    	if (status == I2C::Error::NoError) {
    		tmp &= ~mask;
    		tmp |= value & mask;
    		return i2c.write(deviceAddress, static_cast<const uint8_t*>(&address), sizeof(address), reinterpret_cast<const uint8_t*>(&tmp), sizeof(tmp));
    	}
    	return status;
    }

    template <typename ArrayType, size_t N, typename ...Registers>
    I2C::Error readRegisters(std::array<ArrayType, N> &array, Registers... reg) {
    	static_assert(sizeof...(reg) > 1, "");
    	static_assert(sizeof...(reg) == array.size(), "Size of array have to be equal to number of registers.");
    	// because we are reading data to array we have to check if all registers have the same data type and type is equal
    	// with std::array type
    	microhal::dataTypeCheck<ArrayType>(reg...);
    	microhal::isContinous(reg...);
    	microhal::accessCheck<Access::WriteOnly>(reg...);

    	const auto firstReg = microhal::first(reg...);

    	const auto registerAddress = firstReg.getAddress();
    	auto status = i2c.writeRead(deviceAddress, static_cast<const uint8_t*>(&registerAddress), sizeof(registerAddress), reinterpret_cast<uint8_t*>(array.data()), array.size() * sizeof(ArrayType));
    	convertEndianness(array, reg...);
    	return status;
    }

    template <typename ...Registers>
    I2C::Error readRegisters(std::tuple<typename Registers::Type...> &data, Registers... reg) {
    	static_assert(sizeof...(reg) > 1, "");
    	microhal::accessCheck<Access::WriteOnly>(reg...);
    	microhal::isContinous(reg...);
    	const auto firstReg = microhal::first(reg...);
    	uint8_t tmp[sizeOfRegistersData(reg...)];

    	const auto registerAddress = firstReg.getAddress();
    	auto status = i2c.writeRead(deviceAddress, static_cast<const uint8_t*>(&registerAddress), sizeof(registerAddress), tmp, sizeof(tmp));
    	setTuple<0>(data, tmp, reg...);
    	return status;
    }

    template<typename ArrayType, size_t N, typename ...Registers>
    I2C::Error writeRegisters(const std::array<ArrayType, N> &array, Registers... reg) {
    	static_assert(sizeof...(reg) > 1, "");
    	static_assert(sizeof...(reg) == array.size(), "Size of array have to be equal to number of registers.");
    	static_assert(sizeOfRegistersData(reg...) == array.size() * sizeof(ArrayType), "microhal internal error.");
    	// because we are reading data to array we have to check if all registers have the same data type and type is equal
    	// with std::array type
    	microhal::dataTypeCheck<ArrayType>(reg...);
    	isContinous(reg...);
    	accessCheck<Access::ReadOnly>(reg...);

    	const auto firstReg = microhal::first(reg...);
    	const auto registerAddress = firstReg.getAddress();

    	std::array<ArrayType, N> tmpArray = array;
    	convertEndianness(tmpArray, reg...);
    	return i2c.write(deviceAddress, static_cast<const uint8_t*>(&registerAddress), sizeof(registerAddress), reinterpret_cast<const uint8_t*>(tmpArray.data()), tmpArray.size() * sizeof(ArrayType));
    }

    template<typename ArrayType, size_t N, typename ...Registers>
    I2C::Error writeRegisters(const std::tuple<typename Registers::Type...> &data, Registers... reg) {
    	static_assert(sizeof...(reg) > 1, "");
    	// because we are reading data to array we have to check if all registers have the same data type and type is equal
    	// with std::array type
    	microhal::dataTypeCheck<ArrayType>(reg...);
    	isContinous(reg...);
    	accessCheck<Access::ReadOnly>(reg...);

    	const auto firstReg = microhal::first(reg...);
    	const auto registerAddress = firstReg.getAddress();
    	//if (sizeof(data) == sizeOfRegistersData(reg...)) {
    		//return i2c.write(deviceAddress, static_cast<const uint8_t*>(&registerAddress), sizeof(registerAddress), reinterpret_cast<const uint8_t*>(array.data()), array.size() * sizeof(ArrayType));
    	//} else {
    		uint8_t tmp[sizeOfRegistersData(reg...)];
    		setArrayFromTuple<0>(tmp, data, reg...);
    		return i2c.write(deviceAddress, static_cast<const uint8_t*>(&registerAddress), sizeof(registerAddress), tmp, sizeof(tmp));
    	//}
    }

 private:
    I2C &i2c;
    const DeviceAddress deviceAddress;  ///< address of I2C slave device

/////////////////////////////
	template<size_t i, typename Tuple, typename Register>
	constexpr void setTuple(Tuple &tuple, const uint8_t *data, Register reg) {
		(void)reg;
		using DataType = typename Register::Type;
		DataType value = *reinterpret_cast<const DataType*>(data);
		if (reg.requireEndiannessConversion()) {
			value = microhal::convertEndianness(value, Endianness::Big, Endianness::Little);
		}
		std::get<i>(tuple) = value;
	}
	template<size_t i, typename Tuple, typename Register, typename... Registers>
	constexpr void setTuple(Tuple &tuple, const uint8_t *data, Register reg, Registers... regs) {
		(void)reg;
		using DataType = typename Register::Type;
		DataType value = *reinterpret_cast<const DataType*>(data);
		if (reg.requireEndiannessConversion()) {
			value = microhal::convertEndianness(value, Endianness::Big, Endianness::Little);
		}
		std::get<i>(tuple) = value;//convertEndiannessIfRequired(*value, reg.getEndianness());
		setTuple<i+1>(tuple, data + sizeof(DataType), regs...);
	}
//////////////////////////////
  	template<size_t i, typename Tuple, typename Register>
 	void setArrayFromTuple(uint8_t *array, Tuple &tuple, Register reg) {
 		using DataType = typename std::tuple_element<i, Tuple>::type;
 		DataType value = std::get<i>(tuple);
 		if (reg.requireEndiannessConversion()) {
 			*reinterpret_cast<DataType*>(array) = microhal::convertEndianness(value, Endianness::Big, Endianness::Little);
 		} else {
 			*reinterpret_cast<DataType*>(array) = value;
 		}
 	}
 	template<size_t i, typename Tuple, typename Register, typename... Registers>
 	void setArrayFromTuple(uint8_t *array, Tuple &tuple, Register reg, Registers... regs) {
 		using DataType = typename std::tuple_element<i, Tuple>::type;
 		DataType value = std::get<i>(tuple);
 		if (reg.requireEndiannessConversion()) {
 			*reinterpret_cast<DataType*>(array) = microhal::convertEndianness(value, Endianness::Big, Endianness::Little);
 		} else {
 			*reinterpret_cast<DataType*>(array) = value;
 		}
 		setArrayFromTuple<i+1>(array + sizeof(DataType), tuple);
 	}
//////////////////////////////
  	template <typename Type, size_t N, typename Register>
    void convertEndianness(std::array<Type, N> &array, Register reg) {
 		if (reg.requireEndiannessConversion()) {
 			array[array.size() - 1] = microhal::convertEndianness(array[array.size() - 1], Endianness::Big, Endianness::Little);
 		}
    }
 	template <typename Type, size_t N, typename Register, typename... Registers>
    void convertEndianness(std::array<Type, N> &array, Register reg, Registers... regs) {
 		if (reg.requireEndiannessConversion()) {
 			array[array.size() - sizeof...(Registers) - 1] = microhal::convertEndianness(array[array.size() - sizeof...(Registers) - 1], Endianness::Big, Endianness::Little);
 		}
 		convertEndianness(array, regs...);
    }
};
/**
 * @}
 */
}  // namespace microhal
#endif  // _MICROHAL_I2CDEVICE_H_
