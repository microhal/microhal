/*
 * usbClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_USBCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_USBCLOCK_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "apbClock.h"
#include "clockTypes.h"
#include "hsi.h"
#include "pll.h"

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

enum UsbClockSource { HSI48 = 0, PLL = 1 };

enum UsbPrescaler { Div1 = 1, Div1_5 = 0 };

#if defined(_MICROHAL_USB_BASE_ADDRESS)
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
#else
inline void enableUSB() {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    apb1enr.USBEN.set();
    registers::rcc->apb1enr.volatileStore(apb1enr);
}

inline void disableUSB() {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    apb1enr.USBEN.clear();
    registers::rcc->apb1enr.volatileStore(apb1enr);
}

#if defined(RCC_CFGR3_USBSW)
static void USBClockSource(UsbClockSource clockSource) {
    auto cfgr3 = registers::rcc->cfgr3.volatileLoad();
    cfgr3.USBSW = clockSource;
    registers::rcc->cfgr3.volatileStore(cfgr3);
}
#endif

inline UsbClockSource USBClockSource() {
#if defined(RCC_CFGR3_USBSW)
    return static_cast<UsbClockSource>(registers::rcc->cfgr3.volatileLoad().USBSW.get());
#endif
    return UsbClockSource::PLL;
}

[[maybe_unused]] static UsbPrescaler USBPrescaler() {
    auto cfgr = registers::rcc->cfgr.volatileLoad();
    return cfgr.OTGFSPRE.get() ? Div1 : Div1_5;
}

[[maybe_unused]] static uint32_t USBFrequency() {
    switch (USBPrescaler()) {
        case Div1:
            return PLL::VCOOutputFrequency();
        case Div1_5:
            return (PLL::VCOOutputFrequency() * 2) / 3;
    }
    std::terminate();
}

#endif
#endif
}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_USBCLOCK_H_ */
