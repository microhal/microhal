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
#include "rcc_register_select.h"

namespace microhal {
namespace ClockManager {

#ifdef MCU_TYPE_STM32F1XX
enum UsbClockSource { PLL = 1 };
enum class UsbPrescaler { Div3 = 0, Div2 = 1 };
#else
enum UsbClockSource { HSI48 = 0, PLL = 1 };
enum class UsbPrescaler { Div1 = 1, Div1_5 = 0 };
#endif

#if defined(_MICROHAL_USB_BASE_ADDRESS)
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
#else
inline void enableUSB() {
#ifdef _MICROHAL_REGISTERS_RCC_AHBENR_HAS_OTGFSEN
    auto ahbenr = registers::rcc->ahb1enr.volatileLoad();
    ahbenr.OTGFSEN.set();
    registers::rcc->ahb1enr.volatileStore(ahbenr);
#else
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    apb1enr.USBEN.set();
    registers::rcc->apb1enr.volatileStore(apb1enr);
#endif
}

inline void disableUSB() {
#ifdef _MICROHAL_REGISTERS_RCC_AHBENR_HAS_OTGFSEN
    auto ahbenr = registers::rcc->ahb1enr.volatileLoad();
    ahbenr.OTGFSEN.clear();
    registers::rcc->ahb1enr.volatileStore(ahbenr);
#else
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    apb1enr.USBEN.clear();
    registers::rcc->apb1enr.volatileStore(apb1enr);
#endif
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

[[maybe_unused]] static void USBPrescaler(UsbPrescaler prescaler) {
    auto cfgr = registers::rcc->cfgr.volatileLoad();
    cfgr.OTGFSPRE = static_cast<uint32_t>(prescaler);
    registers::rcc->cfgr.volatileStore(cfgr);
}

[[maybe_unused]] static UsbPrescaler USBPrescaler() {
    const auto cfgr = registers::rcc->cfgr.volatileLoad();
#ifdef MCU_TYPE_STM32F1XX
    return cfgr.OTGFSPRE.get() ? UsbPrescaler::Div2 : UsbPrescaler::Div3;
#else
    return cfgr.OTGFSPRE.get() ? Div1 : Div1_5;
#endif
}

[[maybe_unused]] static uint32_t USBFrequency() {
#ifdef MCU_TYPE_STM32F1XX
    switch (USBPrescaler()) {
        case UsbPrescaler::Div3:
            return (PLL::VCOOutputFrequency() * 2) / 3;
        case UsbPrescaler::Div2:
            return (PLL::VCOOutputFrequency() * 2) / 2;
    }
#else
    switch (USBPrescaler()) {
        case UsbPrescaler::Div1:
            return PLL::VCOOutputFrequency();
        case UsbPrescaler::Div1_5:
            return (PLL::VCOOutputFrequency() * 2) / 3;
    }
#endif
    std::terminate();
}

#endif
#endif
}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_USBCLOCK_H_ */
