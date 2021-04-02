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

#include <ports/stmCommon/rtc_v2/rtc.h>
#include <cassert>
#include _MICROHAL_INCLUDE_PORT_INTERRUPT_CONTROLLER

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

Signal<void> RTC::interrupt;

//------------------------------------------------------------------------------
//                             time read
//------------------------------------------------------------------------------
RTC::ResultSubsecond RTC::subsecond() {
    ResultSubsecond result(Error::NoData);
    if (registers::rtc->icsr.volatileLoad().RSF) {
        auto ssr = registers::rtc->ssr.volatileLoad();
        result.value(ssr.SS.get());
    }
    return result;
}

RTC::ResultTime RTC::time() {
    ResultTime result(Error::NoData);
    if (registers::rtc->icsr.volatileLoad().RSF) {
        auto tr = registers::rtc->tr.volatileLoad();
        Time &time = result.value();
        time.hour = tr.HT * 10 + tr.HU;
        time.minute = tr.MNT * 10 + tr.MNU;
        time.second = tr.ST * 10 + tr.SU;
        time.amPm = static_cast<AM_PM>(tr.PM.get());
        result.error(Error::None);
        return result;
    }
    return result;
}

RTC::ResultDate RTC::date() {
    ResultDate result(Error::NoData);
    if (registers::rtc->icsr.volatileLoad().RSF) {
        auto dr = registers::rtc->dr.volatileLoad();
        Date &date = result.value();
        date.weekDay = dr.WDU;
        date.month = dr.MT * 10 + dr.MU;
        date.monthDay = dr.DT * 10 + dr.DU;
        date.year = (dr.YT * 10) + dr.YU + 2000;
        result.error(Error::None);
    }
    return result;
}

#ifdef MICROHAL_RTC_ENABLE_POSIX_EPOCH
time_t RTC::epoch() {
    auto time = stm32g0xx::RTC::time();
    auto date = stm32g0xx::RTC::date();
    if (time && date) {
        tm tm_time;
        tm_time.tm_isdst = 0;
        tm_time.tm_year = date.value().year - 1900;
        // tm_time.tm_yday;
        tm_time.tm_mon = date.value().month - 1;
        tm_time.tm_mday = date.value().monthDay;
        tm_time.tm_wday = date.value().weekDay - 1;
        tm_time.tm_hour = time.value().hour;
        tm_time.tm_min = time.value().minute;
        tm_time.tm_sec = time.value().second;
        return mktime(&tm_time);
    }
    return -1;
}

int64_t RTC::epoch_ms(uint16_t synchronousPrescaler) {
    auto subsecond = stm32g0xx::RTC::subsecond_ms(synchronousPrescaler);
    auto time = stm32g0xx::RTC::time();
    auto date = stm32g0xx::RTC::date();
    if (subsecond && time && date) {
        tm tm_time;
        tm_time.tm_isdst = 0;
        tm_time.tm_year = date.value().year - 1900;
        // tm_time.tm_yday;
        tm_time.tm_mon = date.value().month - 1;
        tm_time.tm_mday = date.value().monthDay;
        tm_time.tm_wday = date.value().weekDay - 1;
        tm_time.tm_hour = time.value().hour;
        tm_time.tm_min = time.value().minute;
        tm_time.tm_sec = time.value().second;
        int64_t time = mktime(&tm_time);
        if (time >= 0) {
            return time * 1000 + subsecond.value();
        }
    }
    return -1;
}
#endif
//------------------------------------------------------------------------------
//                             timestamp read
//------------------------------------------------------------------------------
RTC::ResultSubsecond RTC::timestampSubsecond() {
    ResultSubsecond result(Error::NoData);
    if (registers::rtc->sr.volatileLoad().TSF) {
        auto ssr = registers::rtc->tsssr.volatileLoad();
        result.value(ssr.SS.get());
    }
    return result;
}

RTC::ResultTime RTC::timestampTime() {
    ResultTime result(Error::NoData);
    auto sr = registers::rtc->sr.volatileLoad();
    if (sr.TSF) {
        auto tstr = registers::rtc->tstr.volatileLoad();
        Time &time = result.value();
        time.hour = tstr.HT * 10 + tstr.HU;
        time.minute = tstr.MNT * 10 + tstr.MNU;
        time.second = tstr.ST * 10 + tstr.SU;
        time.amPm = static_cast<AM_PM>(tstr.PM.get());
        result.error(Error::None);
    }
    return result;
}

RTC::ResultDate RTC::timestampDate() {
    ResultDate result(Error::NoData);
    auto sr = registers::rtc->sr.volatileLoad();
    if (sr.TSF) {
        auto tsdr = registers::rtc->tsdr.volatileLoad();
        Date &date = result.value();
        date.weekDay = tsdr.WDU;
        date.month = tsdr.MT * 10 + tsdr.MU;
        date.monthDay = tsdr.DT * 10 + tsdr.DU;
        // date.year = tsdr.yt * 10 + tsdr.yu + 2000;
        date.year = 0;
        result.error(Error::None);
    }
    return result;
}

#ifdef MICROHAL_RTC_ENABLE_POSIX_EPOCH
time_t RTC::timestampEpoch() {
    auto dateTimestamp = timestampDate();
    auto time = timestampTime();
    auto currentDate = date();
    tm tm_time;
    tm_time.tm_isdst = 0;
    tm_time.tm_year = currentDate.value().year - 1900;
    // tm_time.tm_yday;
    tm_time.tm_mon = dateTimestamp.value().month - 1;
    tm_time.tm_mday = dateTimestamp.value().monthDay;
    tm_time.tm_wday = dateTimestamp.value().weekDay - 1;
    tm_time.tm_hour = time.value().hour;
    tm_time.tm_min = time.value().minute;
    tm_time.tm_sec = time.value().second;
    return mktime(&tm_time);
}
#endif

bool RTC::configureTimestamp(TimestampOnInternalEvent timestampInternalEvent, TimestampOnExternalEvent timestampExternalEvent,
                             TimestampOnTamperDetection timestampTamperDetection) {
    auto cr = registers::rtc->cr.volatileLoad();
    cr.ITSE = static_cast<uint32_t>(timestampInternalEvent);
    cr.TSE = timestampExternalEvent != TimestampOnExternalEvent::Disable ? 1 : 0;
    cr.TSEDGE = static_cast<uint32_t>(timestampExternalEvent) & 0b1;
    cr.TAMPTS = static_cast<uint32_t>(timestampTamperDetection);
    registers::rtc->cr.volatileStore(cr);
    return true;
}

//------------------------------------------------------------------------------
//                             set date & time
//------------------------------------------------------------------------------
bool RTC::setSubsecond(uint16_t subsecond) {
    if (registers::rtc->icsr.volatileLoad().INITF) {
        registers::RTC::SSR ssr{};
        ssr.SS = subsecond;
        registers::rtc->ssr.volatileStore(ssr);
        return true;
    }
    return false;
}

bool RTC::setTime(const Time &time) {
    if (registers::rtc->icsr.volatileLoad().INITF) {
        registers::RTC::TR tr{};
        tr.HT = time.hour / 10;
        tr.HU = time.hour % 10;
        tr.MNT = time.minute / 10;
        tr.MNU = time.minute % 10;
        tr.ST = time.second / 10;
        tr.SU = time.second % 10;
        tr.PM = static_cast<uint8_t>(time.amPm);
        registers::rtc->tr.volatileStore(tr);
        return true;
    }
    return false;
}

bool RTC::setDate(const Date &date) {
    assert(date.month > 0);
    assert(date.month <= 12);
    assert(date.weekDay > 0);
    assert(date.weekDay < 8);
    assert(date.year >= 2000);
    assert(date.year < 2100);

    if (registers::rtc->icsr.volatileLoad().INITF) {
        registers::RTC::DR dr{};
        dr.YT = (date.year % 100) / 10;
        dr.YU = date.year % 10;
        dr.MT = date.month / 10;
        dr.MU = date.month % 10;
        dr.WDU = date.weekDay;
        dr.DT = date.monthDay / 10;
        dr.DU = date.monthDay % 10;
        registers::rtc->dr.volatileStore(dr);
        return true;
    }
    return false;
}

#ifdef MICROHAL_RTC_ENABLE_POSIX_EPOCH
bool RTC::setEpoch(const time_t &time) {
    struct tm *tm_time = gmtime(&time);
    if (tm_time) {
        RTC::Date date = {};
        date.month = tm_time->tm_mon + 1;
        date.monthDay = tm_time->tm_mday;
        date.weekDay = tm_time->tm_wday + 1;
        date.year = tm_time->tm_year + 1900;
        RTC::Time tm = {};
        tm.hour = tm_time->tm_hour;
        tm.minute = tm_time->tm_min;
        tm.second = tm_time->tm_sec;
        tm.amPm = RTC::AM_PM::AM;

        if (RTC::setTime(tm)) {
            return RTC::setDate(date);
        }
    }
    return false;
}

bool RTC::setEpoch_ms(const int64_t &time, uint16_t synchronousPrescaler) {
    assert(time >= 0);

    time_t epoch = time / 1000;
    if (setEpoch(epoch)) {
        uint32_t subsecond = time % 1000;
        return RTC::setSubsecond(((subsecond * synchronousPrescaler) / 1000) - (synchronousPrescaler - 1));
    }
    return false;
}
#endif

bool RTC::setPrescaler(uint16_t async_prescaler, uint16_t sync_prescaler) {
    assert(async_prescaler > 0);
    assert(async_prescaler <= asyncPrescalerMaxValue);
    assert(sync_prescaler > 0);
    assert(sync_prescaler <= syncPrescalerMaxValue);

    registers::RTC::PRER prer{};
    prer.PREDIV_A = async_prescaler - 1;
    prer.PREDIV_S = sync_prescaler - 1;
    registers::rtc->prer.volatileStore(prer);
    return true;
}
//--------------------------------------------------------------------------
//                             time calibration
//--------------------------------------------------------------------------
bool RTC::subSecondCalibrate(int16_t subsecond_ms) {
    assert(subsecond_ms < 1000 && subsecond_ms > -1000);

    uint32_t timeout = 1000;
    while (1) {
        if (auto subsec = subsecond(); subsec && timeout) {
            if (!(subsec.value() & 1 << 15)) break;
        } else {
            return false;
        }
        --timeout;
    }

    registers::RTC::SHIFTR shiftr;
    shiftr = 0;
    uint32_t sync_prescaler = registers::rtc->prer.volatileLoad().PREDIV_S.get() + 1;
    if (subsecond >= 0) {
        shiftr.ADD1S.set();
        shiftr.SUBFS = sync_prescaler - (int32_t(subsecond_ms) * sync_prescaler) / 1000;
    } else {
        shiftr.SUBFS = (int32_t(-subsecond_ms) * sync_prescaler) / 1000;
    }
    registers::rtc->shiftr.volatileStore(shiftr);

    return true;
}
//------------------------------------------------------------------------------
//                             Wakeup timer
//------------------------------------------------------------------------------
bool RTC::configureWakeupTimer(WakeupTimerClk clk, uint16_t wakeup) {
    auto cr = registers::rtc->cr.volatileLoad();
    if (!cr.WUTE) {
        auto icsr = registers::rtc->icsr.volatileLoad();
        if (!icsr.INIT) {
            // Poll WUTWF until it is set
            while (!icsr.WUTWF) {
                icsr = registers::rtc->icsr.volatileLoad();
            }
        }

        cr.WUCKSEL = static_cast<uint32_t>(clk);
        registers::rtc->cr.volatileStore(cr);

        registers::rtc->wutr.volatileStore(wakeup);
        return true;
    }
    return false;
}

bool RTC::enableWakeupTimer() {
    auto cr = registers::rtc->cr.volatileLoad();
    cr.WUTE.set();
    registers::rtc->cr.volatileStore(cr);
    if (!registers::rtc->cr.volatileLoad().WUTE) return false;
    return true;
}

bool RTC::disableWakeupTimer() {
    auto cr = registers::rtc->cr.volatileLoad();
    if (cr.WUTE) {
        cr.WUTE.clear();
        registers::rtc->cr.volatileStore(cr);
        if (registers::rtc->cr.volatileLoad().WUTE) return false;
    }
    return true;
}

bool RTC::isWakeupTimerEnabled() {
    return registers::rtc->cr.volatileLoad().WUTE;
}
//------------------------------------------------------------------------------
//                                Alarm
//------------------------------------------------------------------------------
bool RTC::configureAlarm(Alarm alarm, const Date &date, const Time &time, uint16_t subsecond, uint8_t subsecondMask) {
    assert(subsecondMask < 16);
    assert(subsecond < 0x8000U);
    auto icsr = registers::rtc->icsr.volatileLoad();
    if (registers::rtc->icsr.volatileLoad().INITF || (alarm == Alarm::A && icsr.ALRAWF.get()) || (alarm == Alarm::B && icsr.ALRBWF.get())) {
        registers::RTC::ALRMxR alrm = {};

        // date or weak day
        if (date.weekDay > 1 && date.weekDay < 8) {
            alrm.MSK4.clear();
            alrm.WDSEL.set();
            alrm.DU = date.weekDay;
        } else if (date.monthDay > 0 && date.monthDay < 32) {
            alrm.MSK4.clear();
            alrm.WDSEL.clear();
            alrm.DT = date.monthDay / 10;
            alrm.DU = date.monthDay % 10;
        } else {
            alrm.MSK4.set();
        }
        // hours
        if (time.hour < 25) {
            alrm.MSK3.clear();
            alrm.HT = time.hour / 10;
            alrm.HU = time.hour % 10;
        } else {
            alrm.MSK3.set();
        }
        // minutes
        if (time.minute < 60) {
            alrm.MSK2.clear();
            alrm.MNT = time.minute / 10;
            alrm.MNU = time.minute % 10;
        } else {
            alrm.MSK2.set();
        }
        // seconds
        if (time.second < 60) {
            alrm.MSK1.clear();
            alrm.ST = time.second / 10;
            alrm.SU = time.second % 10;
        } else {
            alrm.MSK1.set();
        }
        // subsecond
        registers::RTC::ALRMxSSR alrmssr = {};
        alrmssr.SS = subsecond;
        alrmssr.MASKSS = subsecondMask;

        if (alarm == Alarm::A) {
            registers::rtc->alrmar.volatileStore(alrm);
            registers::rtc->alrmassr.volatileStore(alrmssr);
        } else {
            registers::rtc->alrmbr.volatileStore(alrm);
            registers::rtc->alrmbssr.volatileStore(alrmssr);
        }
        return true;
    }
    return false;
}
bool RTC::enableAlarm(Alarm alarm) {
    auto cr = registers::rtc->cr.volatileLoad();
    switch (alarm) {
        case Alarm::A:
            cr.ALRAE.set();
            break;
        case Alarm::B:
            cr.ALRBE.set();
            break;
    }
    registers::rtc->cr.volatileStore(cr);
    return true;
}

bool RTC::disableAlarm(Alarm alarm) {
    auto cr = registers::rtc->cr.volatileLoad();
    switch (alarm) {
        case Alarm::A:
            cr.ALRAE.clear();
            break;
        case Alarm::B:
            cr.ALRBE.clear();
            break;
    }
    registers::rtc->cr.volatileStore(cr);
    return true;
}

//------------------------------------------------------------------------------
//                             Interrupts
//------------------------------------------------------------------------------
bool RTC::enableInterrupt(Interrupt interrupt) {
    auto cr = registers::rtc->cr.volatileLoad();
    if (interrupt & Interrupt::AlarmA) cr.ALRAIE.set();
    if (interrupt & Interrupt::AlarmB) cr.ALRBIE.set();
    if (interrupt & (Interrupt::Timestamp | Interrupt::TimestampOverflow)) cr.TSIE.set();
    if (interrupt & Interrupt::WakeupTimer) cr.WUTIE.set();
    registers::rtc->cr.volatileStore(cr);
    return true;
}

bool RTC::disableInterrupt(Interrupt interrupt) {
    auto cr = registers::rtc->cr.volatileLoad();
    if (interrupt & Interrupt::AlarmA) cr.ALRAIE.clear();
    if (interrupt & Interrupt::AlarmB) cr.ALRBIE.clear();
    if (interrupt & (Interrupt::Timestamp | Interrupt::TimestampOverflow)) cr.TSIE.clear();
    if (interrupt & Interrupt::WakeupTimer) cr.WUTIE.clear();
    registers::rtc->cr.volatileStore(cr);
    return true;
}

bool RTC::clearInterruptFlag(Interrupt interrupt) {
    registers::rtc->scr.volatileStore(static_cast<uint32_t>(interrupt));
    return true;
}

RTC::Interrupt RTC::getInterruptFlag() {
    return static_cast<Interrupt>((uint32_t)registers::rtc->sr.volatileLoad());
}

bool RTC::enableNVICInterrupt(uint32_t priority) {
    enableRTCInterrupt(priority);
    return true;
}
//------------------------------------------------------------------------------
//                             Configuration mode
//------------------------------------------------------------------------------
bool RTC::enterConfigurationMode() {
    auto icsr = registers::rtc->icsr.volatileLoad();
    icsr.INIT.set();
    registers::rtc->icsr.volatileStore(icsr);
    if (registers::rtc->icsr.volatileLoad().INIT) {
        while (!registers::rtc->icsr.volatileLoad().INITF) {
        }
        return true;
    }
    return false;
}

void RTC::exitConfigurationMode() {
    auto icsr = registers::rtc->icsr.volatileLoad();
    icsr.INIT.clear();
    registers::rtc->icsr.volatileStore(icsr);
}

void RTC::disableWriteProtection() {
    uint32_t key = 0xCA;
    registers::rtc->wpr.volatileStore(key);
    key = 0x53;
    registers::rtc->wpr.volatileStore(key);
}

void RTC::enableWriteProtection() {
    uint32_t key = 0x00;
    registers::rtc->wpr.volatileStore(key);
}

extern "C" void RTC_IRQHandler(void) {
    RTC::interrupt.emit();
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
