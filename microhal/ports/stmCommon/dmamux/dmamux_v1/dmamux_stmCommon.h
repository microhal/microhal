/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      DMA MUX implementation version 1 for STM32
 *
 * @authors    Pawel Okas
 * created on: 10-06-2021
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

#ifndef _MICROHAL_PORTS_STMCOMMON_DMAMUX_V1_H_
#define _MICROHAL_PORTS_STMCOMMON_DMAMUX_V1_H_

#include "../../registers/dmamux_registers_v1.h"
#include "../../stmCommonDefines.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
namespace DMA {

template <typename Requests>
requires std::is_enum_v<Requests> class DMAMUXBase {
 public:
    using Request = Requests;

    static void configureChannel(int channelNumber, Request req) {
        auto cxcr = registers::dmamux1->cxcr[channelNumber].volatileLoad();
        cxcr.DMAREQ_ID = static_cast<uint32_t>(req);
        cxcr.SE.clear();  // synchronixation disabled
        cxcr.EGE.set();   // event generation enable
        registers::dmamux1->cxcr[channelNumber].volatileStore(cxcr);
    }
};

}  // namespace DMA
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_DMAMUX_V1_H_ */
