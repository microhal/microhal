/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 03-08-2018
 * last modification: 03-08-2018
 *
 * @copyright Copyright (c) 2018, Pawel Okas
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

#ifndef _MICROHAL_CLOCKMANAGER_H_
#define _MICROHAL_CLOCKMANAGER_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include <exception>
#include <type_traits>
#include "device/stm32f0xx.h"
#include "gpio_stm32f0xx.h"
#include "microhalPortConfig_stm32f0xx.h"

namespace microhal {
namespace stm32f0xx {

class ClockManager {
 public:
    using Frequency = uint32_t;

    enum UsartClockSource {
        PCLK = 0,  // PCLK means APB clock
        SYSCLK = 1,
        LSE = 2,
        HSI = 3
    };

    enum UsbClockSource { HSI48 = 0, PLL = 1 };

 public:
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
            std::terminate();
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

    static void enableGPIO(const registers::GPIO &gpio) {
        if ((int)&gpio == IOPin::PortA) {
            RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
        } else if ((int)&gpio == IOPin::PortB) {
            RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
        } else if ((int)&gpio == IOPin::PortC) {
            RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
#if defined(GPIOD_BASE)
        } else if ((int)&gpio == IOPin::PortD) {
            RCC->AHBENR |= RCC_AHBENR_GPIODEN;
#endif
#if defined(GPIOE_BASE)
        } else if ((int)&gpio == IOPin::PortE) {
            RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
#endif
#if defined(GPIOF_BASE)
        } else if ((int)&gpio == IOPin::PortF) {
            RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
#endif
#if defined(GPIOG_BASE)
        } else if ((int)&gpio == IOPin::PortG) {
            RCC->AHBENR |= RCC_AHBENR_GPIOGEN;
#endif
#if defined(GPIOH_BASE)
        } else if ((int)&gpio == IOPin::PortH) {
            RCC->AHBENR |= RCC_AHBENR_GPIOHEN;
#endif
#if defined(GPIOI_BASE)
        } else if ((int)&gpio == IOPin::PortI) {
            RCC->AHBENR |= RCC_AHBENR_GPIOIEN;
#endif
#if defined(GPIOJ_BASE)
        } else if ((int)&gpio == IOPin::PortJ) {
            RCC->AHBENR |= RCC_AHBENR_GPIOJEN;
#endif
#if defined(GPIOK_BASE)
        } else if ((int)&gpio == IOPin::PortK) {
            RCC->AHBENR |= RCC_AHBENR_GPIOKEN;
#endif
        } else {
            std::terminate();
        }
    }

    static void enable(const I2C_TypeDef &i2c) {
        if (&i2c == I2C1) RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
#if defined(I2C2)
        else if (&i2c == I2C2)
            RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
#endif
#if defined(I2C3)
        else if (&i2c == I2C3)
            RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
#endif
        else {
            std::terminate();
        }
    }

    static void disable(const USART_TypeDef &usart) {
        uint32_t rccEnableFlag;

        if (&usart == USART1)
            rccEnableFlag = ~RCC_APB2ENR_USART1EN;
        else if (&usart == USART2)
            rccEnableFlag = ~RCC_APB1ENR_USART2EN;
#if defined(USART3)
        else if (&usart == USART3)
            rccEnableFlag = ~RCC_APB1ENR_USART3EN;
#endif
#if defined(UART4)
        else if (&usart == UART4)
            rccEnableFlag = ~RCC_APB1ENR_UART4EN;
#endif
#if defined(UART5)
        else if (&usart == UART5)
            rccEnableFlag = ~RCC_APB1ENR_UART5EN;
#endif
#if defined(USART6)
        else if (&usart == USART6)
            rccEnableFlag = ~RCC_APB2ENR_USART6EN;
#endif
        else {
            std::terminate();
        }
#if defined(USART6)
        if (&usart == USART1 || &usart == USART6) {
#else
        if (&usart == USART1) {
#endif
            RCC->APB2ENR &= rccEnableFlag;
        } else {
            RCC->APB1ENR &= rccEnableFlag;
        }
    }

    static void disable(const GPIO_TypeDef &gpio) {
        if (&gpio == GPIOA) {
            RCC->AHBENR &= ~RCC_AHBENR_GPIOAEN;
        } else if (&gpio == GPIOB) {
            RCC->AHBENR &= ~RCC_AHBENR_GPIOBEN;
        } else if (&gpio == GPIOC) {
            RCC->AHBENR &= ~RCC_AHBENR_GPIOCEN;
#if defined(GPIOD)
        } else if (&gpio == GPIOD) {
            RCC->AHBENR &= ~RCC_AHBENR_GPIODEN;
#endif
#if defined(GPIOE)
        } else if (&gpio == GPIOE) {
            RCC->AHBENR &= ~RCC_AHBENR_GPIOEEN;
#endif
#if defined(GPIOF_BASE)
        } else if (&gpio == GPIOF) {
            RCC->AHBENR &= ~RCC_AHBENR_GPIOFEN;
#endif
#if defined(GPIOG_BASE)
        } else if (&gpio == GPIOG) {
            RCC->AHBENR &= ~RCC_AHBENR_GPIOGEN;
#endif
#if defined(GPIOH_BASE)
        } else if (&gpio == GPIOH) {
            RCC->AHBENR &= ~RCC_AHBENR_GPIOHEN;
#endif
#if defined(GPIOI_BASE)
        } else if (&gpio == GPIOI) {
            RCC->AHBENR &= ~RCC_AHBENR_GPIOIEN;
#endif
#if defined(GPIOJ_BASE)
        } else if (&gpio == GPIOJ) {
            RCC->AHBENR &= ~RCC_AHBENR_GPIOJEN;
#endif
#if defined(GPIOK_BASE)
        } else if (&gpio == GPIOK) {
            RCC->AHBENR &= ~RCC_AHBENR_GPIOKEN;
#endif
        } else {
            std::terminate();
        }
    }

#if defined(USB)
    static void enable(const USB_TypeDef &usb) {
        if (&usb == USB) {
            RCC->APB1ENR |= RCC_APB1ENR_USBEN;
        } else {
            std::terminate();
        }
    }

    static void disable(const USB_TypeDef &usb) {
        if (&usb == USB) {
            RCC->APB1ENR &= ~RCC_APB1ENR_USBEN;
        } else {
            std::terminate();
        }
    }
#endif

#if defined(CRS)
    static void enable(const CRS_TypeDef &crs) {
        if (&crs == CRS) {
            RCC->APB1ENR |= RCC_APB1ENR_CRSEN;
        } else {
            std::terminate();
        }
    }

    static void disable(const CRS_TypeDef &crs) {
        if (&crs == CRS) {
            RCC->APB1ENR &= ~RCC_APB1ENR_CRSEN;
        } else {
            std::terminate();
        }
    }
#endif

    static uint32_t USARTFrequency(const USART_TypeDef &usart);

    static UsartClockSource USARTClockSource(const USART_TypeDef &usart);

    static uint32_t I2CFrequency(const I2C_TypeDef &i2c) {
        if (&i2c == I2C1) {
            if (RCC->CFGR3 & RCC_CFGR3_I2C1SW) {
                return SYSCLK::frequency();
            } else {
                return HSI::frequency();
            }
        }
        std::terminate();
    }

#if defined(RCC_CFGR3_USBSW)
    static void USBClockSource(UsbClockSource clockSource) { RCC->CFGR3 = (RCC->CFGR3 & ~RCC_CFGR3_USBSW) | clockSource; }
    static UsbClockSource USBClockSource() { return static_cast<UsbClockSource>((RCC->CFGR3 >> RCC_CFGR3_USBSW_Pos) & 0x01); }
#endif
    static void flashLatency(Frequency sysclkFrequency_Hz) {
        if (sysclkFrequency_Hz < 24000000) {
            FLASH->ACR &= ~FLASH_ACR_LATENCY;
        } else {
            FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | (0b001 << FLASH_ACR_LATENCY_Pos);
        }
    }

    static Frequency APB1Frequency();

    static uint32_t AHBFrequency() noexcept;

    struct SYSCLK {
        enum class Source : decltype(RCC_CFGR_SWS_0) { HSI, HSE = RCC_CFGR_SWS_0, PLL = RCC_CFGR_SWS_1, HSI48 = RCC_CFGR_SWS_1 | RCC_CFGR_SWS_0 };
        static Source source() { return static_cast<Source>(RCC->CFGR & RCC_CFGR_SWS); }
        static void source(Source source) {
            switch (source) {
                case Source::HSI:
                    flashLatency(HSI::frequency());
                    break;
                case Source::HSE:
                    flashLatency(HSE::frequency());
                    break;
                case Source::PLL:
                    // flashLatency(PLL::VCOOutputFrequency());
                    std::terminate();
                    break;
#if defined(RCC_CR2_HSI48ON)
                case Source::HSI48:
                    flashLatency(HSI48::frequency());
                    break;
#endif
            }
            RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | (static_cast<typename std::underlying_type<Source>::type>(source) >> 2);
        }
        static uint32_t frequency() noexcept {
            uint32_t freq = 0;
            switch (RCC->CFGR & RCC_CFGR_SWS) {
                case 0:
                    freq = HSI::frequency();
                    break;
                case RCC_CFGR_SWS_0:
                    freq = HSE::frequency();
                    break;
                case RCC_CFGR_SWS_1:
                    // freq = PLLCLKFrequency();
                    std::terminate();
                    break;
#if defined(RCC_CR2_HSI48ON)
                case RCC_CFGR_SWS_1 | RCC_CFGR_SWS_0:
                    freq = HSI48::frequency();
                    break;
#endif
            }
            return freq;
        }
    };

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
#if defined(RCC_CR2_HSI48ON)
    struct HSI48 {
        /**
         * @brief This function return HSI48 frequency.
         *
         * @return HSI frequency in [Hz].
         */
        static constexpr Frequency frequency() noexcept { return 48000000; }

        static void enable() noexcept { RCC->CR2 |= RCC_CR2_HSI48ON; }
        static void disable() noexcept { RCC->CR2 &= ~RCC_CR2_HSI48ON; }
        static bool isReady() noexcept { return RCC->CR2 & RCC_CR2_HSI48RDY; }
    };
#endif
    struct HSE {
        /**
         * @brief This function return HSE frequency. This value is defined under HSE_FREQUENCY
         *
         * @return HSE frequency in [Hz]
         */
        static Frequency frequency() noexcept {
            static_assert(externalClockFrequency >= 4000000 && externalClockFrequency <= 32000000,
                          "External HSE frequency out of allowed range. HSE have to be grater than 4MHz and lower than 32MHz.");
            if (externalClockPresent == false) {
                std::terminate();
            }
            return externalClockFrequency;
        }

        static void enable() noexcept { RCC->CR |= RCC_CR_HSEON; }
        static void disable() noexcept { RCC->CR &= ~RCC_CR_HSEON; }
        static bool isReady() noexcept { return RCC->CR & RCC_CR_HSERDY; }
    };
};

}  // namespace stm32f0xx
}  // namespace microhal

#endif  // _MICROHAL_CLOCKMANAGER_H_
