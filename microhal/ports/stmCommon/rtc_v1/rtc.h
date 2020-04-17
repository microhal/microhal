/*
 * rtc.h
 *
 *  Created on: Apr 16, 2020
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_STMCOMMON_RTC_V1_RTC_H_
#define _MICROHAL_PORTS_STMCOMMON_RTC_V1_RTC_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "../registers/rtcRegisters_v1.h"
#include "../stmCommonDefines.h"
#include "ports/stmCommon/clockManager/rtcClock.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

class RTC {
 public:
    RTC();
    virtual ~RTC();

    using ClockSource = ClockManager::RTCClock;

    static void init(ClockSource clock) {
        ClockManager::RTCClockSource(clock);
        ClockManager::enableRTC();
    }

    static uint32_t getValue() {
        const uint32_t cnth = registers::rtc->cnth.volatileLoad();
        const uint32_t cntl = registers::rtc->cntl.volatileLoad();
        return cnth << 16 | cntl;
    }

    static uint32_t getPrescaler() {
        const uint32_t prlh = registers::rtc->prlh.volatileLoad();
        const uint32_t prll = registers::rtc->prll.volatileLoad();
        return prlh << 16 | prll;
    }

    static bool setPrescaler(uint32_t prescaler) {
        if (prescaler <= 0xFFFFF) {
            enterConfigurationMode();
            registers::rtc->prll.volatileStore(prescaler & 0xFFFF);
            registers::rtc->prlh.volatileStore((prescaler >> 16) & 0xF);
            exitConfigurationMode();
            return true;
        }
        return false;
    }

    static void enterConfigurationMode() {
        while (registers::rtc->crl.volatileLoad().RTOFF == 0) {
        }
        auto crl = registers::rtc->crl.volatileLoad();
        crl.CNF.set();
        registers::rtc->crl.volatileStore(crl);
    }
    static void exitConfigurationMode() {
        auto crl = registers::rtc->crl.volatileLoad();
        crl.CNF.clear();
        registers::rtc->crl.volatileStore(crl);
        while (registers::rtc->crl.volatileLoad().RTOFF == 0) {
        }
    }
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
#endif /* _MICROHAL_PORTS_STMCOMMON_RTC_V1_RTC_H_ */
