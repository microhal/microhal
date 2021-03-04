/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      PWR version 2, driver for STM microcontrolers
 *
 * @authors    Pawel Okas
 * created on: 09-02-2021
 *
 * @copyright Copyright (c) 2021, Pawel Okas
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

#ifndef _MICROHAL_PORTS_STMCOMMON_PWR_H_
#define _MICROHAL_PORTS_STMCOMMON_PWR_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <ports/stmCommon/clockManager/pwrClock.h>
#include "../registers/pwrRegisters_v2.h"
#include "../stmCommonDefines.h"
#include _MICROHAL_INCLUDE_PORT_DEVICE

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

class PWR {
 public:
    enum class PowerMode { Stop0 = 0b000, Stop1 = 0b001, Standby = 0b011, Shutdown = 0b100 };
    enum class WakeupPin { WKUP1 = 0, WKUP2, WKUP3, WKUP4, WKUP5, WKUP6 };
    enum class Edge { Rising = 0, Falling };

    static void init() { ClockManager::enablePWR(); }

    static void configureWakeup(WakeupPin pinNumber, Edge edge);
    static void enableWakeup(WakeupPin pinNumber);
    static void disableWakeup(WakeupPin pinNumber);

    static void disableBackupDomainWriteProtection() {
        auto cr1 = registers::pwr->cr1.volatileLoad();
        cr1.DBP.set();
        registers::pwr->cr1.volatileStore(cr1);
    }

    static void enterLowPowerMode(PowerMode powerMode) {
        if (powerMode == PowerMode::Shutdown) SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

        auto cr1 = registers::pwr->cr1.volatileLoad();
        cr1.LPMS = static_cast<uint32_t>(powerMode);
        registers::pwr->cr1.volatileStore(cr1);
    }

    static void enableInternalWakeupLine() {
        auto cr3 = registers::pwr->cr3.volatileLoad();
        cr3.EIWUL.set();
        registers::pwr->cr3.volatileStore(cr3);
    }
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_PWR_H_ */
