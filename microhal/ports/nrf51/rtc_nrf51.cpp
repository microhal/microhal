/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 11-12-2017
 * last modification: 11-12-2017
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "rtc_nrf51.h"

namespace microhal {
namespace nrf51 {

// struct system_clock {
//    typedef chrono::nanoseconds duration;
//    typedef duration::rep rep;
//    typedef duration::period period;
//    typedef chrono::time_point<system_clock, duration> time_point;
//
//    static_assert(system_clock::duration::min() < system_clock::duration::zero(), "a clock's minimum duration cannot be less than its epoch");
//
//    static constexpr bool is_steady = false;
//
//    static time_point now() noexcept;
//
//    // Map to C API
//    static std::time_t to_time_t(const time_point& __t) noexcept {
//        return std::time_t(duration_cast<chrono::seconds>(__t.time_since_epoch()).count());
//    }
//
//    static time_point from_time_t(std::time_t __t) noexcept {
//        typedef chrono::time_point<system_clock, seconds> __from;
//        return time_point_cast<system_clock::duration>(__from(chrono::seconds(__t)));
//    }
//};

void RTC::help() {
    if (!ClockManager::LFCLK::isEnabled()) {
        // enable LFCLK
        return;
    }
}

}  // namespace nrf51
}  // namespace microhal

void RTC0_Handler() {
    if (NRF_RTC0->EVENTS_OVRFLW && NRF_RTC0->INTENSET & RTC_INTENSET_OVRFLW_Msk) {
        NRF_RTC0->EVENTS_OVRFLW = 0;
    }
}
