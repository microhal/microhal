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
#include <utility>
#include "../registers/rtcRegisters_v2.h"
#include "../stmCommonDefines.h"
#include "ports/stmCommon/clockManager/rtcClock.h"
#include "signalSlot/signalSlot.h"
#include "utils/result.h"

#ifdef MICROHAL_RTC_ENABLE_POSIX_EPOCH
#include <time.h>
#endif

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
    enum class Alarm {
        A,
        B,
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

    enum class TimestampOnInternalEvent { Enable = 0b1, Disable = 0b0 };
    enum class TimestampOnExternalEvent { OnRisingEdge = 0b00, OnFallingEdge = 0b01, Disable };
    enum class TimestampOnTamperDetection { Enable = 0b1, Disable = 0b0 };

    enum class CalibrationCyclePeriod { CalibrationPeriod_32s = 0b00, CalibrationPeriod_16s = 0b10, CalibrationPeriod_8s = 0b01 };

    enum class Error { None, NoData };

    using ResultTime = Result<Time, Error, Error::None>;
    using ResultDate = Result<Date, Error, Error::None>;
    using ResultSubsecond = Result<uint32_t, Error, Error::None>;

    static constexpr const uint_fast16_t syncPrescalerMaxValue = 32768;
    static constexpr const uint_fast16_t asyncPrescalerMaxValue = 128;
    static Signal<void> interrupt;

    //--------------------------------------------------------------------------
    //                            date & time read
    //--------------------------------------------------------------------------
    static ResultSubsecond subsecond();
    static ResultSubsecond subsecond_ms(uint16_t synchronousPrescaler) {
        auto subsec = subsecond();
        return {subsec.error(), ((synchronousPrescaler - 1 - subsec.value()) * 1000) / synchronousPrescaler};
    }
    static ResultTime time();
    static ResultDate date();

#ifdef MICROHAL_RTC_ENABLE_POSIX_EPOCH
    /**
     *
     * @return number of seconds since 00:00, Jan 1 1970 UTC, or negative number when error occurred
     */
    static time_t epoch();
    static int64_t epoch_ms(uint16_t synchronousPrescaler);
#endif
    //--------------------------------------------------------------------------
    //                             timestamp read
    //--------------------------------------------------------------------------
    static ResultSubsecond timestampSubsecond();
    static ResultTime timestampTime();
    static ResultDate timestampDate();
#ifdef MICROHAL_RTC_ENABLE_POSIX_EPOCH
    /**
     *
     * @return number of seconds since 00:00, Jan 1 1970 UTC, or negative number when error occurred
     */
    static time_t timestampEpoch();
#endif

    static bool configureTimestamp(TimestampOnInternalEvent, TimestampOnExternalEvent, TimestampOnTamperDetection);
    //--------------------------------------------------------------------------
    //                             set date & time
    // Note: You need to enter configuration mode before using these functions
    //--------------------------------------------------------------------------
    static bool setSubsecond(uint16_t subsecond);
    static bool setTime(const Time &time);
    static bool setDate(const Date &date);
#ifdef MICROHAL_RTC_ENABLE_POSIX_EPOCH
    /**
     * Set RTC date and time without subsecond
     * @param time - number of seconds since 00:00, Jan 1 1970 UTC (POSIX time)
     * @return
     */
    static bool setEpoch(const time_t &time);
    static bool setEpoch_ms(const int64_t &time, uint16_t synchronousPrescaler);
#endif
    static bool setPrescaler(uint16_t async_prescaler, uint16_t sync_prescaler);
    //--------------------------------------------------------------------------
    //                             time calibration
    //--------------------------------------------------------------------------
    static bool subSecondCalibrate(int16_t subsecond_ms);
    static bool isSubSecondCalibratePending();
    static bool smoothDigitalCalibration(CalibrationCyclePeriod calibrationPeriod, int16_t rtcclkClockCyclesToMask);
    static std::pair<RTC::CalibrationCyclePeriod, int16_t> smoothDigitalCalibration();
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
    //                                Alarm
    // Note: You need to enter configuration mode before using these functions
    //--------------------------------------------------------------------------
    /**
     * Note: This function has effect only when RTC is in initialization mode or ALRxWF field in ICSR register is set to 1. When this condition is
     *       not meat function will return false.
     * Note: This function support masking of specific date or time fields. If you don't want to use specific field from date or time just set it to
     *       value outside reasonable value ie.: if you don't want use minutes set field time.minute to 70 or any other value grater than 60.
     * Note: year field from date parameter is not used.
     *
     * @param alarm
     * @param date
     * @param time
     * @param subsecond
     * @param subsecondMask
     * @return
     */
    static bool configureAlarm(Alarm alarm, const Date &date, const Time &time, uint16_t subsecond, uint8_t subsecondMask);
    static bool enableAlarm(Alarm alarm);
    static bool disableAlarm(Alarm alarm);
    static bool isAlarmEnabled(Alarm alarm);
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

#ifdef MICROHAL_RTC_USE_DIAGNOSTIC
template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel,
                                                                  _MICROHAL_ACTIVE_PORT_NAMESPACE::RTC::Error error) {
    constexpr const std::array<std::string_view, 2> str{"None", "NoData"};
    return logChannel << str[static_cast<uint8_t>(error)];
}

template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> &operator<<(microhal::diagnostic::LogLevelChannel<level, B> &logChannel,
                                                                   _MICROHAL_ACTIVE_PORT_NAMESPACE::RTC::Time time) {
    return logChannel << time.hour << ":" << time.minute << ":" << time.second;
}

template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> &operator<<(microhal::diagnostic::LogLevelChannel<level, B> &logChannel,
                                                                   _MICROHAL_ACTIVE_PORT_NAMESPACE::RTC::Date date) {
    return logChannel << date.monthDay << ":" << date.month << ":" << date.year;
}
#endif

}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_RTC_V2_RTC_H_ */
