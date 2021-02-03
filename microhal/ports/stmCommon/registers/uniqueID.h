/*
 * uniqueID.h
 *
 *  Created on: Nov 30, 2020
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_REGISTERS_UNIQUEID_H_
#define SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_REGISTERS_UNIQUEID_H_

#include <utils/bitfield.h>
#include <utils/volatileRegister.h>
#include "registersBaseAddressDefinitions.h"

// Supported MCU: STM32F103

namespace microhal {
namespace registers {

struct UID {
    VolatileRegister<uint16_t, AccessType::ReadOnly> u_id1;
    VolatileRegister<uint16_t, AccessType::ReadOnly> u_id2;
    VolatileRegister<uint32_t, AccessType::ReadOnly> u_id3;
    VolatileRegister<uint32_t, AccessType::ReadOnly> u_id4;
};

#if defined(_MICROHAL_UID_BASE_ADDRESS)
[[maybe_unused]] inline UID *const uid = reinterpret_cast<UID *>(_MICROHAL_UID_BASE_ADDRESS);
#endif

}  // namespace registers
}  // namespace microhal

#endif /* SRC_THIRD_PARTY_MICROHAL_PORTS_STMCOMMON_REGISTERS_UNIQUEID_H_ */
