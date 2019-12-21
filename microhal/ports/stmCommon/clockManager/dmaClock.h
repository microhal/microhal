/*
 * dmaClock.h
 *
 *  Created on: Nov 17, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_DMACLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_DMACLOCK_H_

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
CREATE_SET_CLEAR_FUNCTION(DMA1EN)
CREATE_SET_CLEAR_FUNCTION(DMA2EN)
CREATE_SET_CLEAR_FUNCTION(DMA1LPEN)
CREATE_SET_CLEAR_FUNCTION(DMA2LPEN)
}  // namespace ClockManagerDetail

#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
[[maybe_unused]] static void enableDMA(uint8_t dmaNumber, PowerMode mode) {
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    auto ahb1lpenr = registers::rcc->ahb1lpenr.volatileLoad();
    if (dmaNumber == 1) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_DMA1EN_ifExist(ahb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_DMA1LPEN_ifExist(ahb1lpenr);
    } else if (dmaNumber == 2) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_DMA2EN_ifExist(ahb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_DMA2LPEN_ifExist(ahb1lpenr);
    } else {
        std::terminate();
    }
    registers::rcc->ahb1lpenr.volatileStore(ahb1lpenr);
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
}
[[maybe_unused]] static void disableDMA(uint8_t dmaNumber, PowerMode mode) {
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    auto ahb1lpenr = registers::rcc->ahb1lpenr.volatileLoad();
    if (dmaNumber == 1) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_DMA1EN_ifExist(ahb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_DMA1LPEN_ifExist(ahb1lpenr);
    } else if (dmaNumber == 2) {
        if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_DMA2EN_ifExist(ahb1enr);
        if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_DMA2LPEN_ifExist(ahb1lpenr);
    } else {
        std::terminate();
    }
    registers::rcc->ahb1lpenr.volatileStore(ahb1lpenr);
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
}
#else
[[maybe_unused]] static void enableDMA(uint8_t dmaNumber) {
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    if (dmaNumber == 1) {
        ClockManagerDetail::set_DMA1EN_ifExist(ahb1enr);
    } else if (dmaNumber == 2) {
        ClockManagerDetail::set_DMA2EN_ifExist(ahb1enr);
    } else {
        std::terminate();
    }
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
}

[[maybe_unused]] static void disableDMA(uint8_t dmaNumber) {
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    if (dmaNumber == 1) {
        ClockManagerDetail::clear_DMA1EN_ifExist(ahb1enr);
    } else if (dmaNumber == 2) {
        ClockManagerDetail::clear_DMA2EN_ifExist(ahb1enr);
    } else {
        std::terminate();
    }
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
}
#endif
}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_DMACLOCK_H_ */
