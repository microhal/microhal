#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "microhalDefs.h"

namespace microhal {

typedef enum {
    DRIVERS_STM32F4XX, DRIVERS_STM32F0XX, DRIVERS_RX63N, DRIVERS_ATMEGA8, DRIVERS_LINUX
} Platform;

//static constexpr hal::Platform activePlatform = hal::DRIVERS_STM32F4XX;
static constexpr Platform activePlatform = microhal::DRIVERS_STM32F0XX;
//static constexpr hal::Platform activePlatform = hal::DRIVERS_RX63N;
//static constexpr hal::Platform activePlatform = hal::DRIVERS_ATMEGA8;
//static constexpr hal::Platform activePlatform = hal::DRIVERS_LINUX;
static constexpr microhal::ByteOrder platformOrder = microhal::LSB_FIRST;
static constexpr microhal::Endianness DeviceEndianness = LittleEndian;

}/*namespace microhal */

#endif /* PLATFORM_H_ */
