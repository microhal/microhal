/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 11-12-2017
 * last modification: 11-12-2017
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

#ifndef _MICROHAL_RTC_NRF51_H_
#define _MICROHAL_RTC_NRF51_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <exception>
#include <ratio>
#include "device/nrf.h"

#include "clockManager.h"
#include "signalSlot/signalSlot.h"

namespace microhal {
namespace nrf51 {

extern "C" {
void RTC0_IRQHandler();
void RTC1_IRQHandler();
}
/* **************************************************************************************************************************************************
 * CLASS
 */
class RTC {
 public:
    using ratio = std::ratio<1, 32768>;
    using duration = std::chrono::duration<int64_t, ratio>;
    using rep = duration::rep;
    using period = duration::period;
    using time_point = std::chrono::time_point<RTC, duration>;

    static_assert(RTC::duration::min() < RTC::duration::zero(), "a clock's minimum duration cannot be less than its epoch");

    static constexpr bool is_steady = false;

    time_point now() noexcept { return time_point(duration(count() * (prescaler() + 1))); };

    void setTime(const time_point &time) { setCount(time.time_since_epoch().count() / (prescaler() + 1)); }

    // Map to C API
    static std::time_t to_time_t(const time_point &t) noexcept {
        return std::time_t(std::chrono::duration_cast<std::chrono::seconds>(t.time_since_epoch()).count());
    }

    enum class Event : uint32_t {
        Tick = RTC_EVTENCLR_TICK_Enabled << RTC_EVTENCLR_TICK_Pos,
        Overflow = RTC_EVTENCLR_OVRFLW_Enabled << RTC_EVTENCLR_OVRFLW_Pos,
        Compare0 = RTC_EVTENCLR_COMPARE0_Enabled << RTC_EVTENCLR_COMPARE0_Pos,
        Compare1 = RTC_EVTENCLR_COMPARE1_Enabled << RTC_EVTENCLR_COMPARE1_Pos,
        Compare2 = RTC_EVTENCLR_COMPARE2_Enabled << RTC_EVTENCLR_COMPARE2_Pos,
        Compare3 = RTC_EVTENCLR_COMPARE3_Enabled << RTC_EVTENCLR_COMPARE3_Pos,
    };

    enum class Interrupt : uint32_t {
        Tick = RTC_INTENSET_TICK_Enabled << RTC_EVTENCLR_TICK_Pos,
        Overflow = RTC_INTENSET_OVRFLW_Enabled << RTC_INTENSET_OVRFLW_Pos,
        Compare0 = RTC_INTENSET_COMPARE0_Enabled << RTC_INTENSET_COMPARE0_Pos,
        Compare1 = RTC_INTENSET_COMPARE1_Enabled << RTC_INTENSET_COMPARE1_Pos,
        Compare2 = RTC_INTENSET_COMPARE2_Enabled << RTC_INTENSET_COMPARE2_Pos,
        Compare3 = RTC_INTENSET_COMPARE3_Enabled << RTC_INTENSET_COMPARE3_Pos,
    };

    constexpr RTC(NRF_RTC_Type &rtc) : rtc(rtc) {}

    void start() {
        rtc.TASKS_START = 1;
        started = true;
    }
    void stop() { rtc.TASKS_STOP = 1; }
    void clear() { rtc.TASKS_CLEAR = 1; }

    uint32_t frequency() { return ClockManager::LFCLK::frequency() / (prescaler() + 1); }

    uint32_t prescaler() { return rtc.PRESCALER; }
    uint32_t prescaler(uint32_t presc) {
        if (started) {
            // RTC already started, it is impossible to write new prescaler value while RTC is working, so return current value.
            return prescaler();
        } else {
            rtc.PRESCALER = presc;
            return presc;
        }
    }

    uint64_t count() const {
        criticalSectionBegin();
        uint32_t tmpCounter = rtc.COUNTER;
        uint32_t tmpOverflowCount = overflowCount;
        criticalSectionEnd();

        tmpCounter &= RTC_COUNTER_COUNTER_Msk;
        return (static_cast<uint64_t>(tmpOverflowCount) << 24) | tmpCounter;
    }

    void setCount(uint64_t count) {
        criticalSectionBegin();
        stop();
        overflowCount = count >> 24;
        rtc.COUNTER = RTC_COUNTER_COUNTER_Msk & count;
        start();
        criticalSectionEnd();
    }

    void enableEvent(Event event) { rtc.EVTENSET = static_cast<uint32_t>(event); }
    void disableEvent(Event event) { rtc.EVTENCLR = static_cast<uint32_t>(event); }

    void enableInterrupt(Interrupt interr) { rtc.INTENSET = static_cast<uint32_t>(interr); }
    void disbleInterrupt(Interrupt interr) { rtc.INTENCLR = static_cast<uint32_t>(interr); }

    void enableModuleInterrupts(uint32_t priority) {
        NVIC_SetPriority(getIRQNumber(), priority);
        NVIC_EnableIRQ(getIRQNumber());
    }
    void disableModuleInterrupts() { NVIC_DisableIRQ(getIRQNumber()); }

    void help();

 private:
    NRF_RTC_Type &rtc;
    bool started = false;
    uint32_t overflowCount = 0;

    void criticalSectionBegin() const { NVIC_DisableIRQ(getIRQNumber()); }
    void criticalSectionEnd() const { NVIC_EnableIRQ(getIRQNumber()); }

    IRQn_Type getIRQNumber() const {
        if (&rtc == NRF_RTC0) return RTC0_IRQn;
        if (&rtc == NRF_RTC1) return RTC1_IRQn;
        std::terminate();  // critical error, should newer go here because nrf51 have only two rtc
    }
};

}  // nrf51
}  // microhal

#endif  // _MICROHAL_RTC_NRF51_H_
