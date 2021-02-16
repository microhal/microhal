/*
 * tamper.h
 *
 *  Created on: Feb 10, 2021
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_STMCOMMON_TAMPERANDBACKUP_TAMPER_H_
#define _MICROHAL_PORTS_STMCOMMON_TAMPERANDBACKUP_TAMPER_H_

#include <cassert>
#include <cstdint>

#include "../registers/tamp_registers_v1.h"
#include "../stmCommonDefines.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

class Tamper {
 public:
    static constexpr const size_t backupRegisterCount = _MICROHAL_REGISTERS_TAMP_BKP_REGISTES_COUNT;

    static uint32_t getBackupRegister(uint_fast8_t i) {
        assert(i < backupRegisterCount);
        return registers::tamp->bkpxr[i].volatileLoad();
    }

    static void setBackupRegister(uint_fast8_t i, uint32_t value) {
        assert(i < backupRegisterCount);
        return registers::tamp->bkpxr[i].volatileStore(value);
    }
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_TAMPERANDBACKUP_TAMPER_H_ */
