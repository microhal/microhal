/*
 * microhalDefs.h
 *
 *  Created on: 24 mar 2015
 *      Author: Pawel
 */

#ifndef MICROHALDEFS_H_
#define MICROHALDEFS_H_

namespace microhal{

/**
 * This enum define possible byte order.
 */
typedef enum {
    LSB_FIRST, ///< in first address is LSB data
    MSB_FIRST ///< in first address is MSB data
} ByteOrder;

typedef enum {
	LittleEndian = LSB_FIRST,
	BigEndian = MSB_FIRST
} Endianness;

}// namespace microhal

#endif /* MICROHALDEFS_H_ */
