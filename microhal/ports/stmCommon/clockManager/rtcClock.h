#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RTCCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RTCCLOCK_H_

#include <cstdint>
#include <exception>
#include "apbClock.h"
#include "clockTypes.h"
#include "rcc_register_select.h"

namespace microhal {
namespace ClockManager {

enum class RTCClock {
    None = 0,
    LSE,
    LSI,
#ifdef _MICROHAL_REGISTERS_RCC_BDCR_RTCSEL_HAS_HSE_DIV_32
    HSE_div_32,
#elif defined(_MICROHAL_REGISTERS_RCC_BDCR_RTCSEL_HAS_HSE_DIV_128)
    HSE_div_128
#endif
};

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

inline void enableRTCRegistersClock() {
#ifdef _MICROHAL_REGISTERS_RCC_APB1ENR_HAS_RTCAPBEN
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    apb1enr.RTCAPBEN.set();
    registers::rcc->apb1enr.volatileStore(apb1enr);
#endif
}

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

#endif  // defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
#endif  // defined(_MICROHAL_RTC_BASE_ADDRESS)

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_RTCCLOCK_H_ */
