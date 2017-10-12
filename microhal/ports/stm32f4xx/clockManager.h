/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 20-11-2016
 * last modification: 17-12-2016
 *
 * @copyright Copyright (c) 2016, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MICROHAL_PORTS_STM32F4XX_CLOCKMANAGER_H_
#define MICROHAL_PORTS_STM32F4XX_CLOCKMANAGER_H_

#include <exception>
#include <type_traits>

#include "device/stm32f4xx.h"
#include "microhalPortConfig_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

class ClockManager {
 public:
    using Frequency = uint32_t;

    ClockManager() = delete;
    // ------------------------- functions used for enable clock ------------------------------------------------------
    static void enable(const USART_TypeDef &usart) {
        uint32_t rccEnableFlag;

        if (&usart == USART1)
            rccEnableFlag = RCC_APB2ENR_USART1EN;
        else if (&usart == USART2)
            rccEnableFlag = RCC_APB1ENR_USART2EN;
#if defined(USART3)
        else if (&usart == USART3)
            rccEnableFlag = RCC_APB1ENR_USART3EN;
#endif
#if defined(UART4)
        else if (&usart == UART4)
            rccEnableFlag = RCC_APB1ENR_UART4EN;
#endif
#if defined(UART5)
        else if (&usart == UART5)
            rccEnableFlag = RCC_APB1ENR_UART5EN;
#endif
#if defined(USART6) && defined(RCC_APB2ENR_USART6EN)
        else if (&usart == USART6)
            rccEnableFlag = RCC_APB2ENR_USART6EN;
#endif
        else {
            std::terminate();  // Error should newer go there
        }

        if (&usart == USART1 || &usart == USART6) {
            RCC->APB2ENR |= rccEnableFlag;
        } else {
            RCC->APB1ENR |= rccEnableFlag;
        }
    }
    static void enable(const I2C_TypeDef &i2c) {
        if (&i2c == I2C1)
            RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
        else if (&i2c == I2C2)
            RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
#if defined(I2C3)
        else if (&i2c == I2C3)
            RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
#endif
        else {
            std::terminate();  // Error should newer go there
        }
    }
    static void enable(const SPI_TypeDef &spi) {
        if (&spi == SPI1) RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
#if defined(SPI2)
        else if (&spi == SPI2)
            RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
#endif
#if defined(SPI3)
        else if (&spi == SPI3)
            RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
#endif
#if defined(SPI4)
        else if (&spi == SPI4)
            RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
#endif
#if defined(SPI5)
        else if (&spi == SPI5)
            RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
#endif
#if defined(SPI6)
        else if (&spi == SPI6)
            RCC->APB2ENR |= RCC_APB2ENR_SPI6EN;
#endif
        else {
            std::terminate();  // Error should newer go there
        }
    }
    static void enable(const TIM_TypeDef &timer) {
        if (&timer == TIM1) {
            RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
#if defined(TIM2)
        } else if (&timer == TIM2) {
            RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
#endif
#if defined(TIM3)
        } else if (&timer == TIM3) {
            RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
#endif
#if defined(TIM4)
        } else if (&timer == TIM4) {
            RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
#endif
        } else if (&timer == TIM5) {
            RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
#if defined(TIM6)
        } else if (&timer == TIM6) {
            RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
#endif
#if defined(TIM7)
        } else if (&timer == TIM7) {
            RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
#endif
#if defined(TIM8)
        } else if (&timer == TIM8) {
            RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
#endif
        } else if (&timer == TIM9) {
            RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
#if defined(TIM10)
        } else if (&timer == TIM10) {
            RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
#endif
        } else if (&timer == TIM11) {
            RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
#if defined(TIM12)
        } else if (&timer == TIM12) {
            RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;
#endif
#if defined(TIM13)
        } else if (&timer == TIM13) {
            RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;
#endif
#if defined(TIM14)
        } else if (&timer == TIM14) {
            RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
#endif
        } else {
            std::terminate();  // Error should newer go there
        }
    }
    static void enable(const GPIO_TypeDef &gpio) {
        if (&gpio == GPIOA)
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        else if (&gpio == GPIOB)
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
        else if (&gpio == GPIOC)
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
#if defined(GPIOD)
        else if (&gpio == GPIOD)
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
#endif
#if defined(GPIOE)
        else if (&gpio == GPIOE)
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
#endif
#if defined(GPIOF_BASE)
        else if (&gpio == GPIOF)
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
#endif
        else {
            std::terminate();
        }
    }
    //--------------------------------------------------------------------------------------------------------------
    /**
     * @brief This function return usart clock
     *
     * @param usart device pointer
     * @return
     */
    static uint32_t USARTFrequency(const USART_TypeDef &usart) {
        if (&usart == USART1)
            return APB2Frequency();
        else if (&usart == USART2)
            return APB1Frequency();
#if defined(USART3)
        else if (&usart == USART3)
            return APB1Frequency();
#endif
#if defined(UART4)
        else if (&usart == UART4)
            return APB1Frequency();
#endif
#if defined(UART5)
        else if (&usart == UART5)
            return APB1Frequency();
#endif
#if defined(USART6)
        else if (&usart == USART6)
            return APB2Frequency();
#endif
#if defined(UART7)
        else if (&usart == UART7)
            return APB1Frequency();
#endif
#if defined(UART8)
        else if (&usart == UART8)
            return APB1Frequency();
#endif
        std::terminate();  // Error should newer go there
    }
    /**
     * @brief This function return SPI clock
     *
     * @param spi device pointer
     * @return
     */
    static uint32_t SPIFrequency(const SPI_TypeDef &spi) {
        if (&spi == SPI1) return APB2Frequency();
#if defined(SPI2)
        else if (&spi == SPI2)
            return APB1Frequency();
#endif
#if defined(SPI3)
        else if (&spi == SPI3)
            return APB1Frequency();
#endif
#if defined(SPI4)
        else if (&spi == SPI4)
            return APB2Frequency();
#endif
#if defined(SPI5)
        else if (&spi == SPI5)
            return APB2Frequency();
#endif
#if defined(SPI6)
        else if (&spi == SPI6)
            return APB2Frequency();
#endif
        std::terminate();  // Error should newer go there
    }
    /**
     * @brief This function return I2C clock
     *
     * @param i2c device pointer
     * @return
     */
    static uint32_t I2CFrequency(const I2C_TypeDef &i2c) {
        if (&i2c == I2C1)
            return APB1Frequency();
        else if (&i2c == I2C2)
            return APB1Frequency();
#if defined(I2C3)
        else if (&i2c == I2C3)
            return APB1Frequency();
#endif
        std::terminate();  // Error should newer go there
    }
    static uint32_t TimerFrequency(const TIM_TypeDef &tim) {
        if (&tim == TIM1) return APB2Frequency();
#if defined(TIM2)
        else if (&tim == TIM2)
            return APB1Frequency();
#endif
#if defined(TIM3)
        else if (&tim == TIM3)
            return APB1Frequency();
#endif
#if defined(TIM4)
        else if (&tim == TIM4)
            return APB1Frequency();
#endif
        else if (&tim == TIM5)
            return APB1Frequency();
#if defined(TIM6)
        else if (&tim == TIM6)
            return APB1Frequency();
#endif
#if defined(TIM7)
        else if (&tim == TIM7)
            return APB1Frequency();
#endif

#if defined(TIM8)
        else if (&tim == TIM8)
            return APB2Frequency();
#endif
        else if (&tim == TIM9)
            return APB2Frequency();
#if defined(TIM2)
        else if (&tim == TIM10)
            return APB2Frequency();
#endif

        else if (&tim == TIM11)
            return APB2Frequency();
#if defined(TIM12)
        else if (&tim == TIM12)
            return APB1Frequency();
#endif
#if defined(TIM13)
        else if (&tim == TIM13)
            return APB1Frequency();
#endif
#if defined(TIM14)
        else if (&tim == TIM14)
            return APB1Frequency();
#endif
        std::terminate();
    }

    static uint32_t SYSCLKFrequency() noexcept {
        uint32_t freq = 0;
        switch (RCC->CFGR & RCC_CFGR_SWS) {
            case 0:
                freq = HSI::frequency();
                break;
            case RCC_CFGR_SWS_0:
                freq = HSE::frequency();
                break;
            case RCC_CFGR_SWS_1:
                freq = PLLCLKFrequency();
                break;
        }
        return freq;
    }

    static Frequency APB1Frequency() {
        const uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> 10;
        if (ppre1 & 0b100) {
            const uint32_t div[] = {2, 4, 8, 16};
            return AHBFrequency() / div[ppre1 & 0b011];
        } else {
            return AHBFrequency();
        }
    }

    static Frequency APB2Frequency() {
        const uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> 13;
        if (ppre2 & 0b100) {
            const uint32_t div[] = {2, 4, 8, 16};
            return AHBFrequency() / div[ppre2 & 0b011];
        } else {
            return AHBFrequency();
        }
    }

    static uint32_t AHBFrequency() noexcept {
        const uint32_t hpre = (RCC->CFGR & RCC_CFGR_HPRE) >> 4;
        if (hpre & 0b1000) {
            const uint32_t div[] = {2, 4, 8, 16, 64, 128, 256, 512};
            return SYSCLKFrequency() / div[hpre & 0b0111];
        } else {
            return SYSCLKFrequency();
        }
    }

    static Frequency LSEFrequency() noexcept {
        if (externalLSEPresent == false) {
            std::terminate();
        }
        return externalLSEFrequency;
    }

    static Frequency PLLCLKFrequency() noexcept { return PLL::Main::PFrequency(); }

    struct HSI {
        /**
         * @brief This function return HSI frequency.
         *
         * @return HSI frequency in [Hz].
         */
        static constexpr Frequency frequency() noexcept { return 16000000; }

        static void enable() noexcept { RCC->CR |= RCC_CR_HSION; }

        static void disable() noexcept { RCC->CR &= ~RCC_CR_HSION; }

        static bool isReady() noexcept { return RCC->CR & RCC_CR_HSIRDY; }
    };

    struct HSE {
        /**
         * @brief This function return HSE frequency. This value is defined under HSE_FREQUENCY
         *
         * @return HSE frequency in [Hz]
         */
        static Frequency frequency() noexcept {
            static_assert(externalClockFrequency >= 4000000 && externalClockFrequency <= 26000000,
                          "External HSE frequency out of allowed range. HSE have to be grater than 4MHz and lower than 26MHz.");
            if (externalClockPresent == false) {
                std::terminate();
            }
            return externalClockFrequency;
        }

        static void enable() noexcept { RCC->CR |= RCC_CR_HSEON; }

        static void disable() noexcept { RCC->CR &= ~RCC_CR_HSEON; }

        static bool isReady() noexcept { return RCC->CR & RCC_CR_HSERDY; }
    };

    struct PLL {
        enum class ClockSource : decltype(RCC_PLLCFGR_PLLSRC) { HSI = 0, HSE = RCC_PLLCFGR_PLLSRC };

        static ClockSource clockSource(ClockSource source) noexcept {
            RCC->PLLCFGR = (RCC->PLLCFGR & ~RCC_PLLCFGR_PLLSRC) | static_cast<typename std::underlying_type<ClockSource>::type>(
                                                                      source);  // fixme (pokas) maybe bit banding will be faster solution
            return source;
        }

        static ClockSource clockSource() noexcept { return static_cast<ClockSource>(RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC); }

        static void MDivider(uint32_t m) {
            if (m < 2 || m > 63)
                while (1)
                    ;
            RCC->PLLCFGR = (RCC->PLLCFGR & 0xFFFFFFC0) | m;
        }

        static uint32_t MDivider() { return (RCC->PLLCFGR & 0b11111); }

        static float inputFrequency() {
            const float inputFrequency = clockSource() == ClockSource::HSI ? HSI::frequency() : HSE::frequency();
            return inputFrequency / MDivider();
        }

        struct Main {
            static void enable() noexcept { RCC->CR |= RCC_CR_PLLON; }

            static void disable() noexcept { RCC->CR &= ~RCC_CR_PLLON; }

            static bool isReady() noexcept { return RCC->CR & RCC_CR_PLLRDY; }

            static float PFrequency() { return VCOOutputFrequency() / P(); }

            static float QFrequency() { return VCOOutputFrequency() / Q(); }

         private:
            static float VCOOutputFrequency() noexcept { return inputFrequency() * N(); }
            static uint32_t N() noexcept { return (RCC->PLLCFGR >> 6) & 0x1FF; }
            static uint32_t P() noexcept {
                const uint32_t tab[] = {2, 4, 6, 8};
                return tab[(RCC->PLLCFGR >> 16) & 0b11];
            }
            static uint32_t Q() noexcept { return (RCC->PLLCFGR >> 24) & 0xF; }
        };

        struct I2S {};

        struct SAI {};
    };
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // MICROHAL_PORTS_STM32F4XX_CLOCKMANAGER_H_
