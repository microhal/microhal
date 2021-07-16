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
#include "rcc_register_select.h"

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
#ifdef MCU_TYPE_STM32F1XX
    enum class ClockSource : uint32_t {
        NoClock = 0,
        SYSCLK = 0b0100,
        HSI = 0b0101,
        HSE = 0b0110,
        PLL = 0b0111,
        PLL2 = 0b1000,
        PLL3 = 0b1001,
        XT1 = 0b1010,
        PLL3ForEth = 0b1011
    };
#elif defined(MCU_TYPE_STM32G0XX)
    enum class ClockSource : uint32_t {
        NoClock = 0,
        SYSCLK = 0b0001,
        HSI48 = 0b0010,
        HSI16 = 0b0011,
        HSE = 0b0100,
        PLLRCLK = 0b0101,
        LSI = 0b0110,
        LSE = 0b0111,
        PLLPCLK = 0b1000,
        PLLQCLK = 0b1001,
        RTCCLK = 0b1010,
        RTC_WAKEUP = 0b1011
    };
#else
    enum class ClockSource : uint32_t { HSI = 0, LSE = 0b01, HSE = 0b10, PLL = 0b11 };
#endif
    /**
     *
     * @param clk - Microcontroller clock output source
     *
     * @note According to STM RM0090 - "Clock source selection may generate glitches on MCO. It is highly recommended to configure these bits
     * only after reset before enabling the external oscillators and the PLLs."
     */
    static void clockSource(ClockSource clk) {
        auto cfgr = registers::rcc->cfgr.volatileLoad();
#ifdef _MICROHAL_REGISTERS_RCC_CFGR_HAS_MCO1SEL
        cfgr.MCO1SEL = static_cast<uint32_t>(clk);
#else
        cfgr.MCO1 = static_cast<uint32_t>(clk);
#endif
        registers::rcc->cfgr.volatileStore(cfgr);
    }

    static ClockSource clockSource() {
#ifdef _MICROHAL_REGISTERS_RCC_CFGR_HAS_MCO1SEL
        return static_cast<ClockSource>(registers::rcc->cfgr.volatileLoad().MCO1SEL.get());
#else
        return static_cast<ClockSource>(registers::rcc->cfgr.volatileLoad().MCO1.get());
#endif
    }

#ifdef _MICROHAL_REGISTERS_RCC_CFGR_HAS_MCO1PRE
    static uint32_t prescaler() {
        const uint32_t mcoPre = registers::rcc->cfgr.volatileLoad().MCO1PRE;
#if _MICROHAL_REGISTERS_RCC_CFGR_HAS_MCO1PRE == 1
        // if prescaler is disabled return prescaler value equal one
        if (mcoPre & 0b100) return 1;
        return (mcoPre & 0b11) + 2;
#elif _MICROHAL_REGISTERS_RCC_CFGR_HAS_MCO1PRE == 2  // second version used in STM32G0xx
        return 1 << mcoPre;
#endif
    }

#if _MICROHAL_REGISTERS_RCC_CFGR_HAS_MCO1PRE == 1
    /**
     *
     * @param prescaler - Microcontroller clock output prescaler, allowed values: 1, 2, 3, 4, 5
     * @return true when value was set, false if prescaler parameter was unsupported value.
     */
    static bool prescaler(uint32_t prescaler) {
        if (prescaler == 0 || prescaler > 5) return false;
        auto cfgr = registers::rcc->cfgr.volatileLoad();

        if (prescaler == 1)
            cfgr.MCO1PRE = 0b100;
        else
            cfgr.MCO1PRE = prescaler - 2;

        registers::rcc->cfgr.volatileStore(cfgr);
        return true;
    }
#elif _MICROHAL_REGISTERS_RCC_CFGR_HAS_MCO1PRE == 2
    /**
     * This function is used on: STM32G0xx
     * @param prescaler - Microcontroller clock output prescaler, allowed values: 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024
     * @return true when value was set, false if prescaler parameter was unsupported value.
     */
    static bool prescaler(uint32_t prescaler) {
        const int32_t prescalerRegisterValue = [](uint32_t presc) -> int32_t {
            for (uint32_t preValue = 0; preValue < 10; preValue++) {
                if (presc == 1U << preValue) return preValue;
            }
            return -1;  // incorrect prescaler
        }(prescaler);

        if (prescalerRegisterValue >= 0) {
            auto cfgr = registers::rcc->cfgr.volatileLoad();
            cfgr.MCO1PRE = prescalerRegisterValue;
            registers::rcc->cfgr.volatileStore(cfgr);
            return true;
        }
        return false;
    }
#endif  // _MICROHAL_REGISTERS_RCC_CFGR_HAS_MCO1PRE == 1

#endif
};

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_MCO_H_ */
