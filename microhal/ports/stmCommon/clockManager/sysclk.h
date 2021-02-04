/*
 * sysclk.h
 *
 *  Created on: Nov 22, 2019
 *      Author: pokas
 */

#ifndef MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_SYSCLK_H_
#define MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_SYSCLK_H_

#include <cstdint>
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
#ifdef MCU_TYPE_STM32G0XX
#include "ports/stm32g0xx/RCC_4.h"
#endif

namespace microhal {
namespace ClockManager {
struct SYSCLK {
    enum class Source : uint32_t {
        HSI = 0b00,
        HSE = 0b01,
        PLL = 0b10,
#ifdef _MICROHAL_REGISTERS_RCC_HAS_HSI48
        HSI48 = 0b11
#endif
    };
    static Source source() { return static_cast<Source>(registers::rcc->cfgr.volatileLoad().SWS.get()); }
    static void source(Source source);
    static uint32_t frequency() noexcept;
};
}  // namespace ClockManager
}  // namespace microhal

#endif /* MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_SYSCLK_H_ */
