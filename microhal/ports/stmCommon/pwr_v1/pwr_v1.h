/*
 * pwr_v1.h
 *
 *  Created on: Apr 16, 2020
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_PWR_V1_PWR_V1_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_PWR_V1_PWR_V1_H_

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "../registers/pwrRegisters_v1.h"
#include "../stmCommonDefines.h"
#include "ports/stmCommon/clockManager/pwrClock.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

class PWR {
 public:
    static void init() { ClockManager::enablePWR(); }

    static void disableBackupDomainWriteProtection() {
        auto cr = registers::pwr->cr.volatileLoad();
        cr.DBP.set();
        registers::pwr->cr.volatileStore(cr);
    }
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_PWR_V1_PWR_V1_H_ */
