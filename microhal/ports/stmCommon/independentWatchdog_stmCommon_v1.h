/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 02-10-2018
 * last modification: dd-mm-yyyy
 *
 * @copyright Copyright (c) 2018, Pawel Okas
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

#ifndef _MICROHAL_PORTS_INDEPENDENTWATCHDOG_STMCOMMON_H_
#define _MICROHAL_PORTS_INDEPENDENTWATCHDOG_STMCOMMON_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "registers/iwdgRegisters_v1.h"
#include "stmCommonDefines.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

/* **************************************************************************************************************************************************
 * CLASS
 */
class IndependentWatchdog {
 public:
    enum class Prescaler {
        Div4 = 0,
        Div8,
        Div16,
        Div32,
        Div64,
        Div128,
        Div256,
    };

    static void enable() { registers::iwdg->kr.volatileStore(0x0000'CCCC); }
    static void kick() { registers::iwdg->kr.volatileStore(0x0000'AAAA); }
    static bool setPrescaler(Prescaler prescaler) {
        if (registers::iwdg->sr.volatileLoad().PVU == 0) {
            registers::iwdg->kr.volatileStore(0x0000'5555);
            registers::iwdg->pr.volatileStore(static_cast<uint32_t>(prescaler));
            return true;
        }
        return false;
    }
    static bool setReloadValue(uint32_t reload) {
        if (reload <= 0xFFF && registers::iwdg->sr.volatileLoad().RVU == 0) {
            registers::iwdg->kr.volatileStore(0x0000'5555);
            registers::iwdg->rlr.volatileStore(reload);
            return true;
        }
        return false;
    }
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
#endif  // _MICROHAL_PORTS_INDEPENDENTWATCHDOG_STMCOMMON_H_
