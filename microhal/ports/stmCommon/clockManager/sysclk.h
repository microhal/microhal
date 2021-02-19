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
#include "rcc_register_select.h"

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
