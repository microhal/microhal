/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 19-02-2017
 * last modification: 19-02-2017
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

#ifndef MICROHAL_PORTS_STM32F3XX_CLOCKMANAGER_H_
#define MICROHAL_PORTS_STM32F3XX_CLOCKMANAGER_H_

#include "microhalPortConfig_stm32f3xx.h"
#include <cmath>
#include <type_traits>

#include "device/stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {

class ClockManager {
 public:
    using Frequency = uint32_t;

    enum UsartClockSource {
        PCLK = 0,  // PCLK means APB clock
        SYSCLK = 1,
        LSE = 2,
        HSI = 3
    };

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
#if defined(USART6)
        else if (&usart == USART6)
            rccEnableFlag = RCC_APB2ENR_USART6EN;
#endif
        else {
            while (1)
                ;  // Error should newer go there
        }
#if defined(USART6)
        if (&usart == USART1 || &usart == USART6) {
#else
        if (&usart == USART1) {
#endif
            RCC->APB2ENR |= rccEnableFlag;
        } else {
            RCC->APB1ENR |= rccEnableFlag;
        }
    }
    static void enable(const I2C_TypeDef &i2c) {
        if (&i2c == I2C1)
            RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
#if defined(I2C2)
        else if (&i2c == I2C2)
            RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
#endif
#if defined(I2C3)
        else if (&i2c == I2C3)
            RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
#endif
        else {
            while (1)
                ;  // Error should newer go there
        }
    }
    static void enable(const SPI_TypeDef &spi) {
        if (&spi == SPI1)
            RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
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
        	while (1); // Error should newer go there
        }
    }
    static void enable(const TIM_TypeDef &timer) {
    	if (&timer == TIM1) {
    		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    	} else if (&timer == TIM2) {
			RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
		} else if (&timer == TIM3) {
			RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
#if defined(TIM4)
		} else if (&timer == TIM4) {
			RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
#endif
#if defined(TIM5)
		} else if (&timer == TIM5) {
			RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
#endif
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
#if defined(TIM9)
		} else if (&timer == TIM9) {
			RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
#endif
#if defined(TIM10)
		} else if (&timer == TIM10) {
			RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
#endif
#if defined(TIM11)
		} else if (&timer == TIM11) {
			RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
#endif
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
			while (1)
				;  // Error should newer go there
		}
    }
	static void enable(const GPIO_TypeDef &gpio) {
	if (&gpio == GPIOA)
			RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	else if (&gpio == GPIOB)
			RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	else if (&gpio == GPIOC)
			RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	else if (&gpio == GPIOD)
			RCC->AHBENR |= RCC_AHBENR_GPIODEN;
#if defined(GPIOE_BASE)
	else if (&gpio == GPIOE)
			RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
#endif
#if defined(GPIOF_BASE)
	else if (&gpio == GPIOF)
			RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
#endif
	else {
		while (1);
	}
}
    //--------------------------------------------------------------------------------------------------------------
	static UsartClockSource USARTClockSource(const USART_TypeDef &usart) {
		uint32_t pos;
		if (&usart == USART1)
			pos = RCC_CFGR3_USART1SW_Pos;
		else if (&usart == USART2)
			//pos = RCC_CFGR3_USART2SW_Pos;
			return UsartClockSource::PCLK;
		else if (&usart == USART3)
			//pos = RCC_CFGR3_USART3SW_Pos;
			return UsartClockSource::PCLK;
		else {
			while(1);
		}
		return static_cast<UsartClockSource>((RCC->CFGR3  >> pos) & 0b11);
	}

	static void USARTClockSource(const USART_TypeDef &usart, UsartClockSource source) {
		uint32_t pos;
		if (&usart == USART1)
			pos = RCC_CFGR3_USART1SW_Pos;
		else if (&usart == USART2)
			//pos = RCC_CFGR3_USART2SW_Pos;
			while(1);
		else if (&usart == USART3)
			//pos = RCC_CFGR3_USART3SW_Pos;
		while(1);
		else {
			while(1);
		}
		RCC->CFGR3 = (RCC->CFGR3 & ~(0b11 << pos)) | (source << pos);
	}
	/**
     * @brief This function return usart clock
     *
     * @param usart device pointer
     * @return
     */
    static uint32_t USARTFrequency(const USART_TypeDef &usart) {
        UsartClockSource usartClockSource = USARTClockSource(usart);

        switch(usartClockSource) {
        case PCLK:
        	return APB1Frequency();
        case SYSCLK:
        	return SYSCLK::frequency();
        case LSE:
        	while(1);
        	return 0;//LSE::frequency();
        	break;
        case HSI:
        	return HSI::frequency();
        }
    }
    /**
     * @brief This function return SPI clock
     *
     * @param spi device pointer
     * @return
     */
    static uint32_t SPIFrequency(const SPI_TypeDef &spi) {
        if (&spi == SPI1)
            return APB2Frequency();
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
        else {
            while (1)
                ;  // Error should newer go there
        }
    }
    /**
     * @brief This function return I2C clock
     *
     * @param i2c device pointer
     * @return
     */
    static uint32_t I2CFrequency(const I2C_TypeDef &i2c) {
        if (&i2c == I2C1)
        	if (RCC->CFGR3 & RCC_CFGR3_I2C1SW) {
        		return SYSCLK::frequency();
        	} else {
        		return HSI::frequency();
        	}
        else {
            while (1)
                ;  // Error should newer go there
        }
    }
    static uint32_t TimerFrequency(const TIM_TypeDef &tim) {
        if (&tim == TIM1)
            return APB2Frequency();
        else if (&tim == TIM2)
            return APB1Frequency();
        else if (&tim == TIM3)
            return APB1Frequency();
#if defined(TIM4)
        else if (&tim == TIM4)
            return APB1Frequency();
#endif
#if defined(TIM5)
        else if (&tim == TIM5)
            return APB1Frequency();
#endif
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
#if defined(TIM9)
        else if (&tim == TIM9)
            return APB2Frequency();
#endif
#if defined(TIM10)
        else if (&tim == TIM10)
            return APB2Frequency();
#endif
#if defined(TIM11)
        else if (&tim == TIM11)
            return APB2Frequency();
#endif
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
        else {
            while (1)
                ;  // Error should newer go there
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void flashLatency(Frequency sysclkFrequency_Hz) {
    	if (sysclkFrequency_Hz < 24000000) {
    		FLASH->ACR &= ~FLASH_ACR_LATENCY;
    	} else if (sysclkFrequency_Hz < 24000000) {
    		FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_0;
    	} else {
    		FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_1;
    	}
    }
    struct SYSCLK {
		enum class Source : decltype(RCC_CFGR_SWS_0) {
			HSI,
			HSE = RCC_CFGR_SWS_0,
			PLL = RCC_CFGR_SWS_1
		};
    	static Source source() {
    		return static_cast<Source>(RCC->CFGR & RCC_CFGR_SWS);
		}
    	static void source(Source source) {
    		switch(source) {
    		case Source::HSI:
    			flashLatency(HSI::frequency());
    			break;
    		case Source::HSE:
    			flashLatency(HSE::frequency());
    			break;
    		case Source::PLL:
    			flashLatency(PLL::VCOOutputFrequency());
    			break;
    		}
    		RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | (static_cast<typename std::underlying_type<Source>::type>(source) >> 2);
    	}
		static uint32_t frequency() noexcept {
			volatile uint32_t freq = 0;
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
    };

    static Frequency APB1Frequency() {
        const uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos;
        if (ppre1 & 0b100) {
            const uint32_t div[] = {2, 4, 8, 16};
            return AHBFrequency() / div[ppre1 & 0b011];
        } else {
            return AHBFrequency();
        }
    }

    static Frequency APB2Frequency() {
        const uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos;
        if (ppre2 & 0b100) {
            const uint32_t div[] = {2, 4, 8, 16};
            return AHBFrequency() / div[ppre2 & 0b011];
        } else {
            return AHBFrequency();
        }
    }

    static uint32_t AHBFrequency() noexcept {
        const uint32_t hpre = (RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos;
        if (hpre & 0b1000) {
            const uint32_t div[] = {2, 4, 8, 16, 64, 128, 256, 512};
            return SYSCLK::frequency() / div[hpre & 0b0111];
        } else {
            return SYSCLK::frequency();
        }
    }

    static constexpr Frequency LSEFrequency() noexcept {
        if (externalLSEPresent == false) {
            while (1)
                ;
        }
        return externalLSEFrequency;
    }

    static Frequency PLLCLKFrequency() noexcept { return PLL::VCOOutputFrequency(); }

    struct HSI {
        /**
         * @brief This function return HSI frequency.
         *
         * @return HSI frequency in [Hz].
         */
        static constexpr Frequency frequency() noexcept { return 8000000; }

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
        static constexpr Frequency frequency() noexcept {
            static_assert(externalClockFrequency >= 4000000 && externalClockFrequency <= 26000000,
                          "External HSE frequency out of allowed range. HSE have to be grater than 4MHz and lower than 26MHz.");
            if (externalClockPresent == false) {
                while (1)
                    ;
            }
            return externalClockFrequency;
        }

        static void enable() noexcept { RCC->CR |= RCC_CR_HSEON; }

        static void disable() noexcept { RCC->CR &= ~RCC_CR_HSEON; }

        static bool isReady() noexcept { return RCC->CR & RCC_CR_HSERDY; }
    };

    struct PLL {
        enum class ClockSource : decltype(RCC_CFGR_PLLSRC) { HSIDiv2 = 0, HSE = RCC_CFGR_PLLSRC };

        static ClockSource clockSource(ClockSource source) noexcept {
            RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PLLSRC) | static_cast<typename std::underlying_type<ClockSource>::type>(
                                                                      source);  // fixme (pokas) maybe bit banding will be faster solution
            return source;
        }

        static ClockSource clockSource() noexcept { return static_cast<ClockSource>(RCC->CFGR & RCC_CFGR_PLLSRC); }

        static void divider(uint16_t m) {
            if (m < 1 || m > 17)
                while (1)
                    ;
            RCC->CFGR2 = (RCC->CFGR2 & 0xFFFFFFF0) | m;
        }

        static uint32_t divider() { return (RCC->CFGR2 & 0b1111) + 1; }

        static float inputFrequency() {
            if (clockSource() == ClockSource::HSIDiv2) {
            	return HSI::frequency() / 2;
            } else {
            	return HSE::frequency() / divider();
            }
        }

        static uint32_t frequency(uint32_t frequency_Hz) {
        	if (clockSource() == ClockSource::HSIDiv2) {
        		// we only can change pll mul
        		uint32_t mul = round(frequency_Hz / inputFrequency());
        		if (mul > 16) mul = 16;
        		if (mul < 2) mul = 2;
        		PLLMUL(mul);
        		while (HSI::isReady() == false);
        	} else {

        	}

        	enable();
        	while (isReady() == false);
        	return VCOOutputFrequency();
        }

        static void enable() noexcept { RCC->CR |= RCC_CR_PLLON; }

        static void disable() noexcept { RCC->CR &= ~RCC_CR_PLLON; }

        static bool isReady() noexcept { return RCC->CR & RCC_CR_PLLRDY; }

        static float VCOOutputFrequency() noexcept { return inputFrequency() * PLLMUL(); }

    private:
        static uint32_t PLLMUL() noexcept {
        	uint32_t mul = ((RCC->CFGR & RCC_CFGR_PLLMUL_Msk) >> RCC_CFGR_PLLMUL_Pos);
        	if (mul == 0b1111) return 16;
        	return mul + 2;
        }

        static bool PLLMUL(uint32_t mul) noexcept {
        	if (mul >= 2 && mul <=16) {
        		RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PLLMUL_Msk) | ((mul - 2) << RCC_CFGR_PLLMUL_Pos);
        		return true;
        	}
        	return false;
        }
    };
};

}  // namespace stm32f3xx
}  // namespace microhal

#endif  // MICROHAL_PORTS_STM32F3XX_CLOCKMANAGER_H_
