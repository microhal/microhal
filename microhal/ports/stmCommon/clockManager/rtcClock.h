#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RTCCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RTCCLOCK_H_

#include <cstdint>
#include <exception>
#include "apbClock.h"
#include "clockTypes.h"

#ifdef MCU_TYPE_STM32F0XX
#include "ports/stm32f0xx/RCC_2.h"
#endif
#ifdef MCU_TYPE_STM32F1XX
#include "ports/stm32f1xx/rcc_stm32f103.h"
#endif
#ifdef MCU_TYPE_STM32F3XX
#include "ports/stm32f3xx/rcc_3x4.h"
#endif
#ifdef MCU_TYPE_STM32F4XX
#ifdef STM32F411xE
#include "ports/stm32f4xx/rcc_411.h"
#else
#include "ports/stm32f4xx/rcc_407.h"
#endif
#endif

namespace microhal {
namespace ClockManager {

enum class RTCClock { None = 0, LSE, LSI, HSE_div_128 };

#if defined(_MICROHAL_RTC_BASE_ADDRESS)
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
inline void enableRTC() {
    auto bdcr = registers::rcc->bdcr.volatileLoad();
    bdcr.RTCEN.set();
    registers::rcc->bdcr.volatileStore(bdcr);
}
inline void disableRTC() {
    auto bdcr = registers::rcc->bdcr.volatileLoad();
    bdcr.RTCEN.clear();
    registers::rcc->bdcr.volatileStore(bdcr);
}

inline RTCClock RTCClockSource() {
    auto bdcr = registers::rcc->bdcr.volatileLoad();
    return static_cast<RTCClock>(bdcr.RTCSEL.get());
}

inline void RTCClockSource(RTCClock clock) {
    auto bdcr = registers::rcc->bdcr.volatileLoad();
    bdcr.RTCSEL = static_cast<uint32_t>(clock);
    registers::rcc->bdcr.volatileStore(bdcr);
}
#else
static void enableRTC() {
    auto bdcr = registers::rcc->bdcr.volatileLoad();
    bdcr.RTCEN.set();
    registers::rcc->bdcr.volatileStore(bdcr);
}
inline void disableRTC() {
    auto bdcr = registers::rcc->bdcr.volatileLoad();
    bdcr.RTCEN.clear();
    registers::rcc->bdcr.volatileStore(bdcr);
}

inline RTCClock RTCClockSource() {
    auto bdcr = registers::rcc->bdcr.volatileLoad();
    return static_cast<RTCClock>(bdcr.RTCSEL.get());
}

static void RTCClockSource(RTCClock clock) {
    auto bdcr = registers::rcc->bdcr.volatileLoad();
    bdcr.RTCSEL = static_cast<uint32_t>(clock);
    registers::rcc->bdcr.volatileStore(bdcr);
}

#endif  // defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
#endif  // defined(_MICROHAL_RTC_BASE_ADDRESS)

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RTCCLOCK_H_ */
