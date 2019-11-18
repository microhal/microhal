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

#ifdef MCU_TYPE_STM32F1XX
#include "ports/stm32f1xx/rcc_stm32f103.h"
#endif
#ifdef MCU_TYPE_STM32F3XX
#include "ports/stm32f3xx/rcc_3x4.h"
#endif
#ifdef MCU_TYPE_STM32F4XX
#include "ports/stm32f4xx/rcc_411.h"
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
    registers::rcc->ahb1lpenr.volatileStore(apb1lpenr);
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
    registers::rcc->ahb1lpenr.volatileStore(apb1lpenr);
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
#else
[[maybe_unused]] static void enableI2C(uint16_t number) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    if (number == 1) {
        ClockManagerDetail::set_I2C1EN_ifExist(apb1enr);
    } else if (number == 2) {
        ClockManagerDetail::set_I2C2EN_ifExist(apb1enr);
    } else if (number == 3) {
        ClockManagerDetail::set_I2C3EN_ifExist(apb1enr);
    } else {
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
    return apb1Frequency();
}

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_I2CCLOCK_H_ */
