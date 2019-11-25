/*
 * dma2dClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_DMA2DCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_DMA2DCLOCK_H_

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
#if defined(_MICROHAL_DMA2D_BASE_ADDRESS)
static void enable(uint8_t dma2dNumber, PowerMode mode) {
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    auto ahb1lpenr = registers::rcc->ahb1lpenr.volatileLoad();
    if (dma2dNumber == 1) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.DMA1EN.set();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.DMA1LPEN.set();
    } else {
        std::terminate();
    }
    registers::rcc->ahb1lpenr.volatileStore(ahb1lpenr);
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
}
static void disable(uint8_t dma2dNumber, PowerMode mode) {
    auto ahb1enr = registers::rcc->ahb1enr.volatileLoad();
    auto ahb1lpenr = registers::rcc->ahb1lpenr.volatileLoad();
    if (dma2dNumber == 1) {
        if (isEnabled(mode, PowerMode::Normal)) ahb1enr.DMA1EN.clear();
        if (isEnabled(mode, PowerMode::Sleep)) ahb1lpenr.DMA1LPEN.clear();
    } else {
        std::terminate();
    }
    registers::rcc->ahb1lpenr.volatileStore(ahb1lpenr);
    registers::rcc->ahb1enr.volatileStore(ahb1enr);
}
#endif

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_DMA2DCLOCK_H_ */
