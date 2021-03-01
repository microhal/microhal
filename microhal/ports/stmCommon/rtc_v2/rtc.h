/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      RTC driver for STM microcontrolers version 2
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

#ifndef _MICROHAL_PORTS_STMCOMMON_RTC_V2_RTC_H_
#define _MICROHAL_PORTS_STMCOMMON_RTC_V2_RTC_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "../registers/rtcRegisters_v2.h"
#include "../stmCommonDefines.h"
#include "ports/stmCommon/clockManager/rtcClock.h"
#include "signalSlot/signalSlot.h"
#include "utils/result.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

namespace rtc_detail {
enum class Interrupt {
    InternalTimestamp = 0b10'0000,
    TimestampOverflow = 0b01'0000,
    Timestamp = 0b00'1000,
    WakeupTimer = 0b00'0100,
    AlarmB = 0b00'0010,
    AlarmA = 0b00'0001
};

constexpr uint32_t operator&(Interrupt a, Interrupt b) {
    return static_cast<uint32_t>(a) & static_cast<uint32_t>(b);
}

constexpr Interrupt operator|(Interrupt a, Interrupt b) {
    return static_cast<Interrupt>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

}  // namespace rtc_detail

using rtc_detail::operator|;
using rtc_detail::operator&;

class RTC {
 public:
    using Interrupt = rtc_detail::Interrupt;
    enum class AM_PM : uint8_t { AM, PM };
    enum class WakeupTimerClk {
        RTCclk_div_16,    //!< RTC/16 clock is selected
        RTCclk_div_8,     //!< RTC/8 clock is selected
        RTCclk_div_4,     //!< RTC/4 clock is selected
        RTCclk_div_2,     //!< RTC/2 clock is selected
        ck_spare,         //!< ck_spre (usually 1Hz) clock is selected
        ck_spare_WUT_add  //!< ck_spre (usually 1 Hz) clock is selected and 2^16 is added to the WUT counter value
    };
    struct Time {
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        AM_PM amPm;
    };
    static_assert(sizeof(Time) == 4);

    struct Date {
        uint16_t year;
        uint8_t month;
        uint8_t weekDay;
        uint8_t monthDay;
    };

    enum class Error { None, NoData };

    using ResultTime = Result<Time, Error, Error::None>;
    using ResultDate = Result<Date, Error, Error::None>;
    using ResultSubsecond = Result<uint32_t, Error, Error::None>;

    static Signal<void> interrupt;

    //--------------------------------------------------------------------------
    //                            date & time read
    //--------------------------------------------------------------------------
    static ResultSubsecond subsecond();
    static ResultTime time();
    static ResultDate date();
    //--------------------------------------------------------------------------
    //                             timestamp read
    //--------------------------------------------------------------------------
    static ResultSubsecond timestampSubsecond();
    static ResultTime timestampTime();
    static ResultDate timestampDate();
    //--------------------------------------------------------------------------
    //                             set date & time
    // Note: You need to enter configuration mode before using these functions
    //--------------------------------------------------------------------------
    static bool setSubsecond(uint16_t subsecond);
    static bool setTime(const Time &time);
    static bool setDate(const Date &date);
    static bool setPrescaler(uint16_t async_prescaler, uint16_t sync_prescaler);
    //--------------------------------------------------------------------------
    //                             Wakeup timer
    // Note: You need to enter configuration mode before using these functions
    //--------------------------------------------------------------------------
    static bool configureWakeupTimer(WakeupTimerClk clk, uint16_t wakeup);
    static bool enableWakeupTimer(bool enable) {
        if (enable) {
            return enableWakeupTimer();
        } else {
            return disableWakeupTimer();
        }
    }
    static bool enableWakeupTimer();
    static bool disableWakeupTimer();
    static bool isWakeupTimerEnabled();
    //--------------------------------------------------------------------------
    //                             Interrupts
    //--------------------------------------------------------------------------
    static bool enableInterrupt(Interrupt interrupt);
    static bool disableInterrupt(Interrupt interrupt);
    static bool clearInterruptFlag(Interrupt interrupt);
    static Interrupt getInterruptFlag();

    static bool enableNVICInterrupt(uint32_t priority);
    //--------------------------------------------------------------------------
    //                             Configuration mode
    //--------------------------------------------------------------------------
    static void disableWriteProtection();
    static void enableWriteProtection();

    static bool enterConfigurationMode();
    static void exitConfigurationMode();
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_RTC_V2_RTC_H_ */
