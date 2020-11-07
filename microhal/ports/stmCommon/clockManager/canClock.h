/*
 * canClock.h
 *
 *  Created on: Nov 18, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CANCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CANCLOCK_H_

#include <cstdint>
#include <exception>
#include <type_traits>
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
namespace ClockManagerDetail {
CREATE_SET_CLEAR_FUNCTION(CAN1EN)
CREATE_SET_CLEAR_FUNCTION(CANEN)
CREATE_SET_CLEAR_FUNCTION(CAN2EN)
CREATE_SET_CLEAR_FUNCTION(CAN1LPEN)
CREATE_SET_CLEAR_FUNCTION(CAN2LPEN)
}  // namespace ClockManagerDetail

#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
[[maybe_unused]] static void enableCan(uint8_t canNumber, PowerMode mode) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
    if (canNumber == 1) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_CAN1EN_ifExist(apb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_CAN1LPEN_ifExist(apb1enr);
    } else if (canNumber == 2) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_CAN2EN_ifExist(apb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_CAN2LPEN_ifExist(apb1enr);
    } else {
        std::terminate();
    }
    registers::rcc->ahb1lpenr.volatileStore(apb1lpenr);
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
[[maybe_unused]] static void disableCan(uint8_t canNumber, PowerMode mode) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
    if (canNumber == 1) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_CAN1EN_ifExist(apb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_CAN1LPEN_ifExist(apb1enr);
    } else if (canNumber == 2) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_CAN2EN_ifExist(apb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_CAN2LPEN_ifExist(apb1enr);
    } else {
        std::terminate();
    }
    registers::rcc->ahb1lpenr.volatileStore(apb1lpenr);
    registers::rcc->apb1enr.volatileStore(apb1enr);
}
#else
[[maybe_unused]] static void enableCan(uint8_t canNumber) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    if (canNumber == 1) {
        ClockManagerDetail::set_CAN1EN_ifExist(apb1enr);
        ClockManagerDetail::set_CANEN_ifExist(apb1enr);
    } else {
        ClockManagerDetail::set_CAN2EN_ifExist(apb1enr);
    }
    registers::rcc->apb1enr.volatileStore(apb1enr);
}

[[maybe_unused]] static void disableCan(uint8_t canNumber) {
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    if (canNumber == 1) {
        ClockManagerDetail::clear_CAN1EN_ifExist(apb1enr);
    } else {
        ClockManagerDetail::clear_CAN2EN_ifExist(apb1enr);
    }
    registers::rcc->apb1enr.volatileStore(apb1enr);
}

#endif

/**
 * @brief This function return CAN clock
 *
 * @param CAN device pointer
 * @return
 */
[[maybe_unused]] static uint32_t CANFrequency([[maybe_unused]] uint8_t canNumber) {
    return APB1::frequency();
}

}  // namespace ClockManager
}  // namespace microhal
#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_CANCLOCK_H_ */
