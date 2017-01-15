/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    pawel
 * created on: 29-12-2016
 * last modification: 29-12-2016
 *
 * @copyright Copyright (c) 2016, microHAL
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

#ifndef _MICROHAL_DEVICEREGISTER_H_
#define _MICROHAL_DEVICEREGISTER_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include "microhalDefs.h"
#include "ports/manager/hardware.h"

/* **************************************************************************************************************************************************
 * CLASS
 */
namespace microhal {

enum class Access {
  ReadOnly,
  WriteOnly,
  ReadWrite
};

template <typename T, T Value, Endianness ...endianness>
class Address {
	static_assert(std::is_unsigned<T>::value, "");
    static_assert((sizeof(T) == 1 && sizeof...(endianness) == 0) || sizeof(T) > 1, "");
    static_assert((sizeof(T) > 1 && sizeof...(endianness) == 1) || sizeof(T) == 1, "");
 public:
  using Type = T;
 	enum : T {
    	value = Value
    };
    constexpr bool isEndiannessConveted() {
    	return true;
    }
};

template <typename T, Access _Access, typename AddressType, Endianness ...endianness_>
class Register {
 public:
	using Address = AddressType;
	using Type = T;
	//const Address address;

	constexpr typename Address::Type getAddress() const {
		return Address::value;
	}

	constexpr Endianness endianness() const noexcept { return get(endianness_...); }

	constexpr bool requireEndiannessConversion() const noexcept {
		return endianness() != microhal::hardware::Device::endianness;
	}

	constexpr Access access() const {
		return _Access;
	}

 private:
	constexpr Endianness get(Endianness endian) const {
		return endian;
	}
};

template <typename T, Access _Access, typename AddressType>
class Register<T, _Access, AddressType> {
 public:
	using Address = AddressType;
	using Type = T;
	//const Address address;

	constexpr typename Address::Type getAddress() const {
		return Address::value;
	}

	constexpr bool requireEndiannessConversion() const noexcept {
		return false;
	}

	constexpr Access access() const {
		return _Access;
	}
};


//////////////
template<Access RequiredAccess, typename Register>
constexpr void accessCheck(Register reg){
	static_assert(reg.access() != RequiredAccess,"");
}
template<Access RequiredAccess, typename Register, typename... Rest>
constexpr void accessCheck(Register reg, Rest... rest){
	static_assert(reg.access() != RequiredAccess,"");
    accessCheck<RequiredAccess>(rest...);
}
/////////////////
template<typename Register, typename Register2>
constexpr void isContinous(Register reg, Register2 reg2) {
	(void)reg;
	(void)reg2;
	static_assert(Register::Address::value + sizeof(typename Register::Type) == Register2::Address::value, "");
}
template<typename Register, typename Register2, typename... Rest>
constexpr void isContinous(Register reg, Register2 reg2, Rest... regs) {
	(void)reg;
	(void)reg2;
	static_assert(Register::Address::value + sizeof(typename Register::Type) == Register2::Address::value, "");
	isContinous(reg2, regs...);
}
////////////////////////
template<typename Register>
constexpr size_t sizeOfRegistersData(Register reg) {
	(void) reg;
	return sizeof(typename Register::Type);
}
template<typename Register, typename... Rest>
constexpr size_t sizeOfRegistersData(Register reg, Rest... regs) {
	(void) reg;
	return sizeof(typename Register::Type) + sizeOfRegistersData(regs...);
}
////////////////////////
template<typename Type, typename Register>
void dataTypeCheck(Register reg) {
	(void)reg;
	static_assert(std::is_same<Type, typename Register::Type>::value, "Unexpected register type.");
}
template<typename Type, typename Register, typename... Registers>
void dataTypeCheck(Register reg, Registers... regs) {
	(void)reg;
	static_assert(std::is_same<Type, typename Register::Type>::value, "Unexpected register type.");
	dataTypeCheck<Type>(regs...);
}


//template<typename Register, typename Register2>
//constexpr auto first_impl(Register min, Register2 current) {
//	if (min.getAddress() > current.getAddress()){
//		return current;
//    } else {
//    	return min;
//    }
//}
//template<typename Register, typename Register2, typename... Registers>
//constexpr auto first_impl(Register min, Register2 current, Registers... regs) {
//	if (min.getAddress() > current.getAddress()){
//		return first_impl(current, regs...);
//    } else {
//    	return first_impl(min, regs...);
//    }
//}
//template<typename Register, typename Register2, typename... Registers>
//constexpr auto first(Register min, Register2 current, Registers ...regs) {
//     return first_impl(min, regs...);
//}

template<typename Register, typename... Registers>
constexpr auto first(Register reg, Registers ...regs) {
    return reg;
}

//////////////
template<typename Type, Access access, typename AddressType>
constexpr auto makeRegister(AddressType address) {
	(void)address;
	static_assert(sizeof(Type) == 1, "");
	return Register<Type, access, AddressType>{};
}

template<typename Type, Access access, Endianness endianness, typename AddressType>
constexpr auto makeRegister(AddressType address) {
	(void)address;
	static_assert(sizeof(Type) > 1, "");
	return Register<Type, access, AddressType, endianness>{};
}

}  // namespace microhal

#endif  // _MICROHAL_DEVICEREGISTER_H_
