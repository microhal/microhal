/*
 * CRC16.h
 *
 *  Created on: 7 pa� 2014
 *      Author: Dell
 */

#ifndef CRC16_H_
#define CRC16_H_

#include <stdint.h>
#include "gsl/gsl"

namespace microhal {

class CRC16 {
public:
	constexpr CRC16(uint16_t polynomial) noexcept : polynomial(polynomial)  {
	}

	uint16_t crc16(uint16_t *data, uint16_t *last){
		return (data == last) ? 0xFFFF : *data * crc16(data+1, last);
	}

//	template <typename T>
//	static uint16_t calculate(T data) {
//		return calculate(&data, sizeof(data));
//	}


	static uint16_t ByteCRC16(int value, int crcin)	{
		return static_cast<uint16_t>((crcin << 8) ^  ccittTable[((crcin >> 8) ^ (value)) & 255]);
	}

	static uint16_t calculate(const gsl::not_null<const void*> data_ptr, size_t length, uint16_t crc = 0xffff) {
		//uint16_t result = 0;
		const uint8_t *buffer =  static_cast<const uint8_t *>(data_ptr.get());
		do {
			int value = *buffer++;
			crc = ByteCRC16(value, crc);
		} while (--length);
		return crc;
	}

 private:
	const uint16_t polynomial;
	static const uint16_t ccittTable[];
};

} /* namespace microhal */

#endif /* CRC16_H_ */
