/*
 * mco.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_MCO_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_MCO_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "ahbClock.h"
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

#if defined(RCC_CFGR_MCO2)
struct MCO2 {
    enum class ClockSource : uint32_t { Sysclk = 0, PllI2S = 0b01, HSE = 0b10, PLL = 0b11 };
    /**
     *
     * @param clk - Microcontroller clock output source
     *
     * @note According to STM RM0090 - "Clock source selection may generate glitches on MCO. It is highly recommended to configure these bits
     * only after reset before enabling the external oscillators and the PLLs."
     */
    static void clockSource(ClockSource clk) { RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_MCO2) | static_cast<uint32_t>(clk); }
    static ClockSource clockSource() { return static_cast<ClockSource>(RCC->CFGR & RCC_CFGR_MCO2); }
    static uint32_t prescaler() {
        uint32_t cfgr = RCC->CFGR;
        // if prescaler is disabled return prescaler value equal one
        if (cfgr & RCC_CFGR_MCO2PRE_2) return 1;
        return ((cfgr >> 29) & 0b11) + 2;
    }
    /**
     *
     * @param prescaler - Microcontroller clock output prescaler, allowed values: 1, 2, 3, 4, 5
     * @return true when value was set, false if prescaler parameter was unsupported value.
     */
    static bool prescaler(uint32_t prescaler) {
        if (prescaler == 0 || prescaler > 5) return false;
        uint32_t cfgr = RCC->CFGR & ~RCC_CFGR_MCO2PRE;  // clear old configuration

        if (prescaler == 1)
            cfgr |= RCC_CFGR_MCO2PRE_2;
        else
            cfgr |= ((prescaler - 2) << 29);

        RCC->CFGR = cfgr;
        return true;
    }
};
#endif
struct MCO1 {
    enum class ClockSource : uint32_t { HSI = 0, LSE = 0b01, HSE = 0b10, PLL = 0b11 };
    /**
     *
     * @param clk - Microcontroller clock output source
     *
     * @note According to STM RM0090 - "Clock source selection may generate glitches on MCO. It is highly recommended to configure these bits
     * only after reset before enabling the external oscillators and the PLLs."
     */
    static void clockSource(ClockSource clk) {
        auto cfgr = registers::rcc->cfgr.volatileLoad();
        cfgr.MCO1 = static_cast<uint32_t>(clk);
        registers::rcc->cfgr.volatileStore(cfgr);
    }
    static ClockSource clockSource() { return static_cast<ClockSource>(registers::rcc->cfgr.volatileLoad().MCO1); }
    static uint32_t prescaler() {
        uint32_t mcoPre = registers::rcc->cfgr.volatileLoad().MCO1PRE;
        // if prescaler is disabled return prescaler value equal one
        if (mcoPre & 0b100) return 1;
        return (mcoPre & 0b11) + 2;
    }
    /**
     *
     * @param prescaler - Microcontroller clock output prescaler, allowed values: 1, 2, 3, 4, 5
     * @return true when value was set, false if prescaler parameter was unsupported value.
     */
    static bool prescaller(uint32_t prescaler) {
        if (prescaler == 0 || prescaler > 5) return false;
        auto cfgr = registers::rcc->cfgr.volatileLoad();

        if (prescaler == 1)
            cfgr.MCO1PRE = 0b100;
        else
            cfgr.MCO1PRE = prescaler - 2;

        registers::rcc->cfgr.volatileStore(cfgr);
        return true;
    }
};

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_MCO_H_ */
