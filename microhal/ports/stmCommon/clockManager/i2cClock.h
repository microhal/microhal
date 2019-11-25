/*
 * i2cClock.h
 *
 *  Created on: Nov 17, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_I2CCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_I2CCLOCK_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "apbClock.h"
#include "clockTypes.h"
#include "hsi.h"

#ifdef MCU_TYPE_STM32F0XX
#include "ports/stm32f0xx/RCC_2.h"
#endif
#ifdef MCU_TYPE_STM32F1XX
#include "ports/stm32f1xx/rcc_stm32f103.h"
#endif
#ifdef MCU_TYPE_STM32F3XX
#include "ports/stm32f3xx/rcc_3x4.h"
#define _MICROHAL_REGISTERS_I2C_HAS_CLOCKSOURCE
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
namespace ClockManagerDetail {
CREATE_SET_CLEAR_FUNCTION(I2C1EN)
CREATE_SET_CLEAR_FUNCTION(I2C2EN)
CREATE_SET_CLEAR_FUNCTION(I2C3EN)
CREATE_SET_CLEAR_FUNCTION(I2C1LPEN)
CREATE_SET_CLEAR_FUNCTION(I2C2LPEN)
CREATE_SET_CLEAR_FUNCTION(I2C3LPEN)
}  // namespace ClockManagerDetail

#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
[[maybe_unused]] static void enableI2C(uint32_t number, PowerMode mode) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
    if (number == 1) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_I2C1EN_ifExist(apb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_I2C1LPEN_ifExist(apb1lpenr);
    } else if (number == 2) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_I2C2EN_ifExist(apb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_I2C2LPEN_ifExist(apb1lpenr);
    } else if (number == 3) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_I2C3EN_ifExist(apb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_I2C3LPEN_ifExist(apb1lpenr);
    } else {
        std::terminate();  // Error should newer go there
    }
    registers::rcc->apb1lpenr.volatileStore(apb1lpenr);
    registers::rcc->apb1enr.volatileStore(apb1enr);
}

[[maybe_unused]] static void disableI2C(uint32_t number, PowerMode mode) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
    if (number == 1) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_I2C1EN_ifExist(apb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_I2C1LPEN_ifExist(apb1lpenr);
    } else if (number == 2) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_I2C2EN_ifExist(apb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_I2C2LPEN_ifExist(apb1lpenr);
    } else if (number == 3) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_I2C3EN_ifExist(apb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_I2C3LPEN_ifExist(apb1lpenr);
    } else {
        std::terminate();  // Error should newer go there
    }
    registers::rcc->apb1lpenr.volatileStore(apb1lpenr);
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
#else
#ifdef _MICROHAL_REGISTERS_I2C_HAS_CLOCKSOURCE
enum class I2CClockSource { HSI = 0, SYSCLK = 1 };

[[maybe_unused]] static I2CClockSource getI2CClockSource(uint16_t i2cNumber) {
    auto cfgr3 = registers::rcc->cfgr3.volatileLoad();
#if defined(_MICROHAL_I2C1_BASE_ADDRESS)
    if (i2cNumber == 1) {
        return static_cast<I2CClockSource>(cfgr3.I2C1SW.get());
    } else
#endif
#if defined(_MICROHAL_I2C2_BASE_ADDRESS)
        if (i2cNumber == 2) {
        return static_cast<I2CClockSource>(cfgr3.I2C2SW.get());
    } else
#endif
    {
        std::terminate();
    }
}
#endif

[[maybe_unused]] static void enableI2C(uint16_t number) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
#ifdef _MICROHAL_I2C1_BASE_ADDRESS
    if (number == 1) {
        ClockManagerDetail::set_I2C1EN_ifExist(apb1enr);
    } else
#endif
#ifdef _MICROHAL_I2C2_BASE_ADDRESS
        if (number == 2) {
        ClockManagerDetail::set_I2C2EN_ifExist(apb1enr);
    } else
#endif
#ifdef _MICROHAL_I2C3_BASE_ADDRESS
        if (number == 3) {
        ClockManagerDetail::set_I2C3EN_ifExist(apb1enr);
    } else
#endif
    {
        std::terminate();  // Error should newer go there
    }
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
#endif

/**
 * @brief This function return I2C clock
 *
 * @param i2c device pointer
 * @return
 */
[[maybe_unused]] static uint32_t I2CFrequency([[maybe_unused]] uint_fast8_t i2cNumber) {
#ifdef _MICROHAL_REGISTERS_I2C_HAS_CLOCKSOURCE
    I2CClockSource clkSource = getI2CClockSource(i2cNumber);
    switch (clkSource) {
        case I2CClockSource::HSI:
            return HSI::frequency();
        case I2CClockSource::SYSCLK:
            return SYSCLK::frequency();
    }
#else
    return APB1::frequency();
#endif
}

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_I2CCLOCK_H_ */
