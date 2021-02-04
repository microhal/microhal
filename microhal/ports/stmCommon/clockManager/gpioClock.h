/*
 * gpioClock.h
 *
 *  Created on: Nov 23, 2019
 *      Author: pokas
 */

#ifndef MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_GPIOCLOCK_H_
#define MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_GPIOCLOCK_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "../IOPin.h"
#include "apbClock.h"
#include "clockTypes.h"

#if defined(STM32F102) || defined(STM32F107) || defined(STM32F103) || defined(STM32F101) || defined(STM32F100)
#include "../registers/gpio_v2.h"
#else
#include "../registers/gpio_v1.h"
#endif

#ifdef MCU_TYPE_STM32F0XX
#include "ports/stm32f0xx/RCC_2.h"
#endif
#ifdef MCU_TYPE_STM32F1XX
#include "ports/stm32f1xx/rcc_stm32f103.h"
#define _MICROHAL_REGISTERS_GPIO_USE_APB2ENR
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
#ifdef _MICROHAL_CLOCKMANAGER_HAS_POWERMODE
[[maybe_unused]] static void enableGPIO(const registers::GPIO &gpio, PowerMode mode) {
    using IOPin = _MICROHAL_ACTIVE_PORT_NAMESPACE::IOPin;

    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    auto ahb1lpenr = registers::rcc->ahb1lpenr.volatileLoad();

#if defined(_MICROHAL_GPIOA_BASE_ADDRESS)
    if ((int)&gpio == IOPin::PortA) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOAEN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOALPEN.set();
    } else
#endif
#if defined(_MICROHAL_GPIOB_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortB) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOBEN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOBLPEN.set();
    } else
#endif
#if defined(_MICROHAL_GPIOC_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortC) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOCEN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOCLPEN.set();
    } else
#endif
#if defined(_MICROHAL_GPIOD_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortD) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIODEN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIODLPEN.set();
    } else
#endif
#if defined(_MICROHAL_GPIOE_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortE) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOEEN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOELPEN.set();
    } else
#endif
#if defined(_MICROHAL_GPIOF_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortF) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOFEN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOFLPEN.set();
    } else
#endif
#if defined(_MICROHAL_GPIOG_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortG) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOGEN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOGLPEN.set();
    } else
#endif
#if defined(_MICROHAL_GPIOH_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortH) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOHEN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOHLPEN.set();
    } else
#endif
#if defined(_MICROHAL_GPIOI_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortI) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOIEN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOILPEN.set();
    } else
#endif
#if defined(_MICROHAL_GPIOJ_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortJ) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOJEN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOJLPEN.set();
    } else
#endif
#if defined(_MICROHAL_GPIOK_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortK) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOKEN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOKLPEN.set();
    } else
#endif
    {
        std::terminate();
    }

    registers::rcc->ahb1enr.volatileStore(ahb1enr);
    registers::rcc->ahb1lpenr.volatileStore(ahb1lpenr);
}

[[maybe_unused]] static void disableGPIO(const registers::GPIO &gpio, PowerMode mode) {
    using IOPin = _MICROHAL_ACTIVE_PORT_NAMESPACE::IOPin;

    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    auto ahb1lpenr = registers::rcc->ahb1lpenr.volatileLoad();

#if defined(_MICROHAL_GPIOA_BASE_ADDRESS)
    if ((int)&gpio == IOPin::PortA) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOAEN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOALPEN.clear();
    } else
#endif
#if defined(_MICROHAL_GPIOB_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortB) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOBEN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOBLPEN.clear();
    } else
#endif
#if defined(_MICROHAL_GPIOC_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortC) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOCEN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOCLPEN.clear();
    } else
#endif
#if defined(_MICROHAL_GPIOD_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortD) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIODEN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIODLPEN.clear();
    } else
#endif
#if defined(_MICROHAL_GPIOE_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortE) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOEEN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOELPEN.clear();
    } else
#endif
#if defined(_MICROHAL_GPIOF_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortF) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOFEN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOFLPEN.clear();
    } else
#endif
#if defined(_MICROHAL_GPIOG_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortG) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOGEN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOGLPEN.clear();
    } else
#endif
#if defined(_MICROHAL_GPIOH_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortH) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOHEN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOHLPEN.clear();
    } else
#endif
#if defined(_MICROHAL_GPIOI_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortI) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOIEN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOILPEN.clear();
    } else
#endif
#if defined(_MICROHAL_GPIOJ_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortJ) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOJEN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOJLPEN.clear();
    } else
#endif
#if defined(_MICROHAL_GPIOK_BASE_ADDRESS)
        if ((int)&gpio == IOPin::PortK) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.GPIOKEN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.GPIOKLPEN.clear();
    } else
#endif
    {
        std::terminate();
    }

    registers::rcc->ahb1enr.volatileStore(ahb1enr);
    registers::rcc->ahb1lpenr.volatileStore(ahb1lpenr);
}
#else
#ifdef _MICROHAL_REGISTERS_GPIO_USE_APB2ENR
static void enableGPIO(const registers::GPIO *const gpio) {
    using IOPin = _MICROHAL_ACTIVE_PORT_NAMESPACE::IOPin;

    auto apb2enr = registers::rcc->apb2enr.volatileLoad();
    if ((int)gpio == IOPin::PortA) {
        apb2enr.IOPAEN.set();
    } else if ((int)gpio == IOPin::PortB) {
        apb2enr.IOPBEN.set();
    } else if ((int)gpio == IOPin::PortC) {
        apb2enr.IOPCEN.set();
    } else if ((int)gpio == IOPin::PortD) {
        apb2enr.IOPDEN.set();
    } else if ((int)gpio == IOPin::PortE) {
        apb2enr.IOPEEN.set();
    } else if ((int)gpio == IOPin::PortF) {
        apb2enr.IOPFEN.set();
    } else if ((int)gpio == IOPin::PortG) {
        apb2enr.IOPGEN.set();
    } else {
        std::terminate();
    }
    registers::rcc->apb2enr.volatileStore(apb2enr);
}

static void disableGPIO(const registers::GPIO *const gpio) {
    using IOPin = _MICROHAL_ACTIVE_PORT_NAMESPACE::IOPin;

    auto apb2enr = registers::rcc->apb2enr.volatileLoad();
    if ((int)gpio == IOPin::PortA) {
        apb2enr.IOPAEN.clear();
    } else if ((int)gpio == IOPin::PortB) {
        apb2enr.IOPBEN.clear();
    } else if ((int)gpio == IOPin::PortC) {
        apb2enr.IOPCEN.clear();
    } else if ((int)gpio == IOPin::PortD) {
        apb2enr.IOPDEN.clear();
    } else if ((int)gpio == IOPin::PortE) {
        apb2enr.IOPEEN.clear();
    } else if ((int)gpio == IOPin::PortF) {
        apb2enr.IOPFEN.clear();
    } else if ((int)gpio == IOPin::PortG) {
        apb2enr.IOPGEN.clear();
    } else {
        std::terminate();
    }
    registers::rcc->apb2enr.volatileStore(apb2enr);
}
#else
[[maybe_unused]] static void enableGPIO(const registers::GPIO &gpio) {
    using IOPin = _MICROHAL_ACTIVE_PORT_NAMESPACE::IOPin;
#ifdef _MICROHAL_REGISTERS_RCC_HAS_IOPENR
    auto ahb1enr = registers::rcc->iopenr.volatileLoad();
#else
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
#endif

#ifdef _MICROHAL_GPIOA_BASE_ADDRESS
    if ((int)&gpio == IOPin::PortA) {
        ahb1enr.IOPAEN.set();
    } else
#endif
#ifdef _MICROHAL_GPIOB_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortB) {
        ahb1enr.IOPBEN.set();
    } else
#endif
#ifdef _MICROHAL_GPIOC_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortC) {
        ahb1enr.IOPCEN.set();
    } else
#endif
#ifdef _MICROHAL_GPIOD_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortD) {
        ahb1enr.IOPDEN.set();
    } else
#endif
#ifdef _MICROHAL_GPIOE_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortE) {
        ahb1enr.IOPEEN.set();
    } else
#endif
#ifdef _MICROHAL_GPIOF_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortF) {
        ahb1enr.IOPFEN.set();
    } else
#endif
#ifdef _MICROHAL_GPIOG_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortG) {
        ahb1enr.IOPGEN.set();
    } else
#endif
    {
        std::terminate();
    }
#ifdef _MICROHAL_REGISTERS_RCC_HAS_IOPENR
    registers::rcc->iopenr.volatileStore(ahb1enr);
#else
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
#endif
}

[[maybe_unused]] static void disableGPIO(const registers::GPIO &gpio) {
    using IOPin = _MICROHAL_ACTIVE_PORT_NAMESPACE::IOPin;

#ifdef _MICROHAL_REGISTERS_RCC_HAS_IOPENR
    auto ahb1enr = registers::rcc->iopenr.volatileLoad();
#else
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
#endif

#ifdef _MICROHAL_GPIOA_BASE_ADDRESS
    if ((int)&gpio == IOPin::PortA) {
        ahb1enr.IOPAEN.clear();
    } else
#endif
#ifdef _MICROHAL_GPIOB_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortB) {
        ahb1enr.IOPBEN.clear();
    } else
#endif
#ifdef _MICROHAL_GPIOC_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortC) {
        ahb1enr.IOPCEN.clear();
    } else
#endif
#ifdef _MICROHAL_GPIOD_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortD) {
        ahb1enr.IOPDEN.clear();
    } else
#endif
#ifdef _MICROHAL_GPIOE_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortE) {
        ahb1enr.IOPEEN.clear();
    } else
#endif
#ifdef _MICROHAL_GPIOF_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortF) {
        ahb1enr.IOPFEN.clear();
    } else
#endif
#ifdef _MICROHAL_GPIOG_BASE_ADDRESS
        if ((int)&gpio == IOPin::PortG) {
        ahb1enr.IOPGEN.clear();
    } else
#endif
    {
        std::terminate();
    }
#ifdef _MICROHAL_REGISTERS_RCC_HAS_IOPENR
    registers::rcc->iopenr.volatileStore(ahb1enr);
#else
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
#endif
}

#endif  // _MICROHAL_REGISTERS_GPIO_USE_APB2ENR
#endif  // _MICROHAL_CLOCKMANAGER_HAS_POWERMODE

}  // namespace ClockManager
}  // namespace microhal

#endif /* MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_GPIOCLOCK_H_ */
