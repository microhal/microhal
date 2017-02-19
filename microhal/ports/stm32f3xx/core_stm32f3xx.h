/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    buleks
 * created on: 20-07-2016
 * last modification: 20-07-2016
 *
 * @copyright Copyright (c) 2016, microHAL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_CORE_STM32F3XX_H_
#define _MICROHAL_CORE_STM32F3XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "microhalPortConfig_stm32f3xx.h"
#include "device/stm32f3xx.h"

/* **************************************************************************************************************************************************
 * CLASS
 */
namespace microhal {
namespace stm32f3xx {
class Core {  // this class is not tested completely
    Core(void) = delete;

 public:
    enum UsartClockSource {
        PCLK = 0,  // PCLK means APB clock
        SYSCLK = 1,
        LSE = 2,
        HSI = 3
    };

    static void fpu_enable(void) {
        SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // set CP10 and CP11 Full Access
    }

//    static uint32_t getHSIfrequency() { return 8000000; }
//    static uint32_t getHSEfrequency() {/* return hse_value;*/ }
//    static uint32_t getPLLinputClock() {
//        switch (RCC->CFGR & RCC_CFGR_PLLSRC_Msk) {
//            case RCC_CFGR_PLLSRC_HSI_DIV2:
//                return (getHSIfrequency() / 2) / (((RCC->CFGR & RCC_CFGR_PLLXTPRE_Msk) >> RCC_CFGR_PLLXTPRE_Pos) + 1);
//            case RCC_CFGR_PLLSRC_HSE_PREDIV:
//                return (getHSEfrequency() / (RCC->CFGR2 + 1)) / (((RCC->CFGR & RCC_CFGR_PLLXTPRE_Msk) >> RCC_CFGR_PLLXTPRE_Pos) + 1);
//        }
//    }
//    static uint32_t getPLLMultiplier() { return ((RCC->CFGR & RCC_CFGR_PLLMUL_Msk) >> RCC_CFGR_PLLMUL_Pos) + 2; }
//    static uint32_t getPLLfrequency() {
//        uint32_t multiplier;
//        uint32_t divider;
//        uint32_t result = getPLLinputClock();
//        result *= getPLLMultiplier();
//        return result;
//    }
//    static uint32_t getSYSCLKfrequency() {
//        switch (RCC->CFGR & RCC_CFGR_SWS) {
//            case RCC_CFGR_SWS_HSI:
//                return getHSIfrequency();
//                break;
//            case RCC_CFGR_SWS_HSE:
//                return getHSEfrequency();
//                break;
//            case RCC_CFGR_SWS_PLL:
//                return getPLLfrequency();
//                break;
//        }
//    }
//    static uint32_t getAPB1frequency() {
//        uint32_t result = getAHBfrequency();
//        uint32_t divider = 1;
//        volatile uint32_t cfgr = RCC->CFGR;
//        if ((RCC->CFGR & RCC_CFGR_PPRE1_Msk) >> (RCC_CFGR_PPRE1_Pos + 2) != 0) {
//            divider = 2 << ((((RCC->CFGR & RCC_CFGR_PPRE1_Msk) >> RCC_CFGR_PPRE1_Pos) - 3) - 1);
//        }
//        return result / divider;
//    }
//
//    static uint32_t getAPB2frequency() {
//        uint32_t result = getAHBfrequency();
//        uint32_t divider = 1;
//        if ((RCC->CFGR & RCC_CFGR_PPRE2_Msk) >> (RCC_CFGR_PPRE2_Pos + 2) != 0) {
//            divider = 2 << ((((RCC->CFGR & RCC_CFGR_PPRE2_Msk) >> RCC_CFGR_PPRE2_Pos) - 3) - 1);
//        }
//        return result / divider;
//    }
//
//    static uint32_t getAHBfrequency() {
//        uint32_t result = getSYSCLKfrequency();
//        uint32_t divider = 1;
//        if ((RCC->CFGR & RCC_CFGR_HPRE_Msk) >> (RCC_CFGR_HPRE_Pos + 3) != 0) {
//            if (((RCC->CFGR & RCC_CFGR_HPRE_Msk) >> RCC_CFGR_HPRE_Pos) <= 11) {
//                divider = 2 << ((((RCC->CFGR & RCC_CFGR_HPRE_Msk) >> RCC_CFGR_HPRE_Pos) - 7) - 1);
//            } else {
//                divider = 2 << ((((RCC->CFGR & RCC_CFGR_HPRE_Msk) >> RCC_CFGR_HPRE_Pos) - 6) - 1);
//            }
//        }
//        return result / divider;
//    }
//
//    static void FreqTest() {
//        volatile uint32_t freq;
//        freq = Core::getHSEfrequency();
//        freq = Core::getPLLinputClock();
//        freq = Core::getSYSCLKfrequency();
//        freq = Core::getAHBfrequency();
//        freq = Core::getAPB1frequency();
//        freq = Core::getAPB2frequency();
//    }
//
//    static void SetUSART1ClockSource(UsartClockSource source) { RCC->CFGR3 = (source << RCC_CFGR3_USART1SW_Pos) & RCC_CFGR3_USART1SW_Msk; }
//
//    static uint32_t GetUSARTclk(USART_TypeDef *usart) {
//        uint32_t cfgr3 = RCC->CFGR3;
//        switch (reinterpret_cast<uint32_t>(usart)) {
//            case reinterpret_cast<uint32_t>(USART1):
//                switch (((cfgr3 & RCC_CFGR3_USART1SW_Msk) >> RCC_CFGR3_USART1SW_Pos)) {
//                    case PCLK:
//                        return getAPB2frequency();
//                        break;
//
//                    default:
//                        return 0;
//                }
//
//            default:
//                return 0;
//        }
//    }
};
}  // namespace stm32f3xx
}  // namespace microhal

#endif  // _MICROHAL_CORE_STM32F3XX_H_
