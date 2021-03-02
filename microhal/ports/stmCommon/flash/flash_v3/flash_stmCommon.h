/*
 * flash.h
 *
 *  Created on: Feb 27, 2021
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_FLASH_FLASH_V1_FLASH_H_
#define SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_FLASH_FLASH_V1_FLASH_H_

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include <type_traits>
#include "../../registers/flash_v3.h"
#include "../../stmCommonDefines.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
namespace Flash {

inline constexpr const uint32_t pageSize = 2048;
inline constexpr const uint32_t flashBegin = 0x0800'0000;

void unlock();
void lock();
void erasePage(uint16_t pageNumber);
void writePage(uint16_t pageNumber, uint32_t *data, uint32_t size);

}  // namespace Flash
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_FLASH_FLASH_V1_FLASH_H_ */
