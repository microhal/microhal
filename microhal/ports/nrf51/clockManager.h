/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 19-11-2017
 * last modification: 19-11-2017
 *
 * @copyright Copyright (c) 2017, microHAL
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

#ifndef _MICROHAL_CLOCKMANAGER_H_
#define _MICROHAL_CLOCKMANAGER_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include <exception>
#include "device/nrf.h"

namespace microhal {
namespace nrf51 {
/* **************************************************************************************************************************************************
 * CLASS
 */
class ClockManager {
 public:
    ClockManager();
    virtual ~ClockManager();

    struct HCLK {
        enum class Source : uint32_t { HFINT = 0, HFXO = 1 };

        Source source() { return static_cast<Source>(NRF_CLOCK->HFCLKSTAT & 1); }
    };

    struct LFCLK {
        enum class Source : uint32_t { RC = 0, Xtal = 1, Synth = 2 };

        void source(Source source) { NRF_CLOCK->LFCLKSRC = static_cast<uint32_t>(source); }
        Source source() { return static_cast<Source>(NRF_CLOCK->LFCLKSTAT & 0b11); }
        void enable() { NRF_CLOCK->TASKS_LFCLKSTART = 1; }
        void disable() { NRF_CLOCK->TASKS_LFCLKSTOP = 1; }
        bool isEnabled() { return NRF_CLOCK->LFCLKRUN & 1; }
    };

    struct HFXO {
        void enable() { NRF_CLOCK->TASKS_HFCLKSTART = 1; }
        void disable() { NRF_CLOCK->TASKS_HFCLKSTOP = 1; }
        bool isEnabled() { return NRF_CLOCK->HFCLKRUN & 1; }
    };
};

}  // namespace nrf51
}  // namespace microhal

#endif  // _MICROHAL_CLOCKMANAGER_H_
