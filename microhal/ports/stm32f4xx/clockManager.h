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

#include <microhalPortConfig_stm32f4xx.h>
#include <exception>
#include <type_traits>
#include "gpio_stm32f4xx.h"

#include "device/stm32f4xx.h"

#define _MICROHAL_CLOCKMANAGER_HAS_POWERMODE 1

namespace microhal {
namespace stm32f4xx {

class ClockManager {
 public:
    using Frequency = uint32_t;
    enum class PowerMode { Normal = 0b1, Sleep = 0b10 };

 private:
    static constexpr bool isEnabled(PowerMode mode, PowerMode flag) {
        using enumType = std::underlying_type<PowerMode>::type;
        if (static_cast<enumType>(mode) & static_cast<enumType>(flag)) return true;
        return false;
    }

 public:
    ClockManager() = delete;
// ------------------------- functions used for enable clock ------------------------------------------------------
#if defined(DMA2D)
    static void enable(const DMA2D_TypeDef &dma, PowerMode mode) {
        if (&dma == DMA2D) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_DMA1LPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const DMA2D_TypeDef &dma, PowerMode mode) {
        if (&dma == DMA2D) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA1EN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_DMA1LPEN;
        } else {
            std::terminate();
        }
    }
#endif
#if defined(RCC_AHB1ENR_CCMDATARAMEN)
    static void enableCCMDataRAM() { RCC->AHB1ENR |= RCC_AHB1ENR_CCMDATARAMEN; }
    static void disableCCMDataRAM() { RCC->AHB1ENR &= RCC_AHB1ENR_CCMDATARAMEN; }
#endif
#if defined(RCC_AHB1ENR_BKPSRAMEN)
    static void enableBKPSRAM(PowerMode mode) {
        if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_BKPSRAMEN;
        if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_BKPSRAMLPEN;
    }
    static void disableBKSPRAM(PowerMode mode) {
        if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_BKPSRAMEN;
        if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_BKPSRAMLPEN;
    }
#endif
    static void enable(const CRC_TypeDef &crc, PowerMode mode) {
        if (&crc == CRC) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_CRCLPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const CRC_TypeDef &crc, PowerMode mode) {
        if (&crc == CRC) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_CRCEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_CRCLPEN;
        } else {
            std::terminate();
        }
    }

    static void enableGPIO(const registers::GPIO &gpio, PowerMode mode) {
        if ((int)&gpio == IOPin::PortA) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIOALPEN;
        } else if ((int)&gpio == IOPin::PortB) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIOBLPEN;
        } else if ((int)&gpio == IOPin::PortC) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIOCLPEN;
#if defined(GPIOD)
        } else if ((int)&gpio == IOPin::PortD) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIODLPEN;
#endif
#if defined(GPIOE)
        } else if ((int)&gpio == IOPin::PortE) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIOELPEN;
#endif
#if defined(GPIOF_BASE)
        } else if ((int)&gpio == IOPin::PortF) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIOFLPEN;
#endif
#if defined(GPIOG_BASE)
        } else if ((int)&gpio == IOPin::PortG) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIOGLPEN;
#endif
#if defined(GPIOH_BASE)
        } else if ((int)&gpio == IOPin::PortH) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIOHLPEN;
#endif
#if defined(GPIOI_BASE)
        } else if ((int)&gpio == IOPin::PortI) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIOILPEN;
#endif
#if defined(GPIOJ_BASE)
        } else if ((int)&gpio == IOPin::PortJ) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOJEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIOJLPEN;
#endif
#if defined(GPIOK_BASE)
        } else if ((int)&gpio == IOPin::PortK) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOKEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_GPIOKLPEN;
#endif
        } else {
            std::terminate();
        }
    }

    static void disable(const GPIO_TypeDef &gpio, PowerMode mode) {
        if (&gpio == GPIOA) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOAEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_GPIOALPEN;
        } else if (&gpio == GPIOB) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOBEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_GPIOBLPEN;
        } else if (&gpio == GPIOC) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOCEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_GPIOCLPEN;
#if defined(GPIOD)
        } else if (&gpio == GPIOD) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIODEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_GPIODLPEN;
#endif
#if defined(GPIOE)
        } else if (&gpio == GPIOE) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOEEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_GPIOELPEN;
#endif
#if defined(GPIOF_BASE)
        } else if (&gpio == GPIOF) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOFEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_GPIOFLPEN;
#endif
#if defined(GPIOG_BASE)
        } else if (&gpio == GPIOG) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_GPIOGLPEN;
#endif
#if defined(GPIOH_BASE)
        } else if (&gpio == GPIOH) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOHEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_GPIOHLPEN;
#endif
#if defined(GPIOI_BASE)
        } else if (&gpio == GPIOI) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOIEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_GPIOILPEN;
#endif
#if defined(GPIOJ_BASE)
        } else if (&gpio == GPIOJ) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOJEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_GPIOJLPEN;
#endif
#if defined(GPIOK_BASE)
        } else if (&gpio == GPIOK) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOKEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_GPIOKLPEN;
#endif
        } else {
            std::terminate();
        }
    }
#if defined(RNG)
    static void enable(const RNG_TypeDef &rng, PowerMode mode) {
        if (&rng == RNG) {
#if defined(RCC_AHB1ENR_RNGEN)
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR |= RCC_AHB1ENR_RNGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR |= RCC_AHB1LPENR_RNGLPEN;
#else
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB2LPENR |= RCC_AHB2LPENR_RNGLPEN;
#endif
        } else {
            std::terminate();
        }
    }
    static void disable(const RNG_TypeDef &rng, PowerMode mode) {
        if (&rng == RNG) {
#if defined(RCC_AHB1ENR_RNGEN)
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB1ENR &= ~RCC_AHB1ENR_RNGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB1LPENR &= ~RCC_AHB1LPENR_RNGLPEN;
#else
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB2ENR &= ~RCC_AHB2ENR_RNGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB2LPENR &= ~RCC_AHB2LPENR_RNGLPEN;
#endif
        } else {
            std::terminate();
        }
    }
#endif
#if defined(HASH)
    static void enable(const HASH_TypeDef &hash, PowerMode mode) {
        if (&hash == HASH) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB2LPENR |= RCC_AHB2LPENR_RNGLPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const HASH_TypeDef &hash, PowerMode mode) {
        if (&hash == HASH) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB2ENR &= ~RCC_AHB2ENR_RNGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB2LPENR &= ~RCC_AHB2LPENR_RNGLPEN;
        } else {
            std::terminate();
        }
    }
#endif
#if defined(CRYP)
    static void enable(const CRYP_TypeDef &cryp, PowerMode mode) {
        if (&cryp == CRYP) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB2ENR |= RCC_AHB2ENR_CRYPEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB2LPENR |= RCC_AHB2LPENR_CRYPLPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const CRYP_TypeDef &cryp, PowerMode mode) {
        if (&cryp == CRYP) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB2ENR &= ~RCC_AHB2ENR_CRYPEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB2LPENR &= ~RCC_AHB2LPENR_CRYPLPEN;
        } else {
            std::terminate();
        }
    }
#endif
#if defined(DCMI)
    static void enable(const DCMI_TypeDef &dcmi, PowerMode mode) {
        if (&dcmi == DCMI) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB2ENR |= RCC_AHB2ENR_DCMIEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB2LPENR |= RCC_AHB2LPENR_DCMILPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const DCMI_TypeDef &dcmi, PowerMode mode) {
        if (&dcmi == DCMI) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->AHB2ENR &= ~RCC_AHB2ENR_DCMIEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB2LPENR &= ~RCC_AHB2LPENR_DCMILPEN;
        } else {
            std::terminate();
        }
    }
#endif
#if defined(RCC_AHB3ENR_FMCEN)
    static void enableFMC(PowerMode mode) {
        if (isEnabled(mode, PowerMode::Normal)) RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
        if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB3LPENR |= RCC_AHB3LPENR_FMCLPEN;
    }
    static void disableFMC(PowerMode mode) {
        if (isEnabled(mode, PowerMode::Normal)) RCC->AHB3ENR &= ~RCC_AHB3ENR_FMCEN;
        if (isEnabled(mode, PowerMode::Sleep)) RCC->AHB3LPENR &= ~RCC_AHB3LPENR_FMCLPEN;
    }
#endif

#if defined(DAC)
    static void enable(const DAC_TypeDef &dac, PowerMode mode) {
        if (&dac == DAC) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB1ENR |= RCC_APB1ENR_DACEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB1LPENR |= RCC_APB1LPENR_DACLPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const DAC_TypeDef &dac, PowerMode mode) {
        if (&dac == DAC) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB1ENR &= ~RCC_APB1ENR_DACEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB1LPENR &= ~RCC_APB1LPENR_DACLPEN;
        } else {
            std::terminate();
        }
    }
#endif
    static void enable(const PWR_TypeDef &pwr, PowerMode mode) {
        if (&pwr == PWR) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB1ENR |= RCC_APB1ENR_PWREN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB1LPENR |= RCC_APB1LPENR_PWRLPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const PWR_TypeDef &pwr, PowerMode mode) {
        if (&pwr == PWR) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB1LPENR &= ~RCC_APB1LPENR_PWRLPEN;
        } else {
            std::terminate();
        }
    }

    static void enable(const WWDG_TypeDef &wwdg, PowerMode mode) {
        if (&wwdg == WWDG) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB1LPENR |= RCC_APB1LPENR_WWDGLPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const WWDG_TypeDef &wwdg, PowerMode mode) {
        if (&wwdg == WWDG) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB1ENR &= ~RCC_APB1ENR_WWDGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB1LPENR &= ~RCC_APB1LPENR_WWDGLPEN;
        } else {
            std::terminate();
        }
    }
#if defined(LTDC)
    static void enable(const LTDC_TypeDef &ltdc, PowerMode mode) {
        if (&ltdc == LTDC) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR |= RCC_APB2ENR_LTDCEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR |= RCC_APB2LPENR_LTDCLPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const LTDC_TypeDef &ltdc, PowerMode mode) {
        if (&ltdc == LTDC) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR &= ~RCC_APB2ENR_LTDCEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR &= ~RCC_APB2LPENR_LTDCLPEN;
        } else {
            std::terminate();
        }
    }
#endif
#if defined(SAI1)
    static void enable(const SAI_TypeDef &sai, PowerMode mode) {
        if (&sai == SAI1) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR |= RCC_APB2ENR_SAI1EN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR |= RCC_APB2LPENR_SAI1LPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const SAI_TypeDef &dac, PowerMode mode) {
        if (&dac == SAI1) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR &= ~RCC_APB2ENR_SAI1EN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR &= ~RCC_APB2LPENR_SAI1LPEN;
        } else {
            std::terminate();
        }
    }
#endif
    static void enable(const SYSCFG_TypeDef &syscfg, PowerMode mode) {
        if (&syscfg == SYSCFG) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR |= RCC_APB2LPENR_SYSCFGLPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const SYSCFG_TypeDef &syscfg, PowerMode mode) {
        if (&syscfg == SYSCFG) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR &= ~RCC_APB2ENR_SYSCFGEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR &= ~RCC_APB2LPENR_SYSCFGLPEN;
        } else {
            std::terminate();
        }
    }
#if defined(SDIO)
    static void enable(const SDIO_TypeDef &sdio, PowerMode mode) {
        if (&sdio == SDIO) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR |= RCC_APB2ENR_SDIOEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR |= RCC_APB2LPENR_SDIOLPEN;
        } else {
            std::terminate();
        }
    }
    static void disable(const SDIO_TypeDef &sdio, PowerMode mode) {
        if (&sdio == SDIO) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR &= ~RCC_APB2ENR_SDIOEN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR &= ~RCC_APB2LPENR_SDIOLPEN;
        } else {
            std::terminate();
        }
    }
#endif
    static void enable(const ADC_TypeDef &adc, PowerMode mode) {
        if (&adc == ADC1) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR |= RCC_APB2LPENR_ADC1LPEN;
#if defined(ADC2)
        } else if (&adc == ADC2) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR |= RCC_APB2LPENR_ADC2LPEN;
#endif
#if defined(ADC3)
        } else if (&adc == ADC3) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR |= RCC_APB2LPENR_ADC3LPEN;
#endif
        } else {
            std::terminate();
        }
    }
    static void disable(const ADC_TypeDef &adc, PowerMode mode) {
        if (&adc == ADC1) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR &= ~RCC_APB2LPENR_ADC1LPEN;
#if defined(ADC2)
        } else if (&adc == ADC2) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR &= ~RCC_APB2ENR_ADC2EN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR &= ~RCC_APB2LPENR_ADC2LPEN;
#endif
#if defined(ADC3)
        } else if (&adc == ADC3) {
            if (isEnabled(mode, PowerMode::Normal)) RCC->APB2ENR &= ~RCC_APB2ENR_ADC3EN;
            if (isEnabled(mode, PowerMode::Sleep)) RCC->APB2LPENR &= ~RCC_APB2LPENR_ADC3LPEN;
#endif
        } else {
            std::terminate();
        }
    }
    //--------------------------------------------------------------------------------------------------------------

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct SYSCLK {
        enum class Source : decltype(RCC_CFGR_SWS_0) { HSI, HSE = RCC_CFGR_SWS_0, PLL = RCC_CFGR_SWS_1 };
        static Source source() { return static_cast<Source>(RCC->CFGR & RCC_CFGR_SWS); }
        static void source(Source source) {
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
                    freq = PLLCLKFrequency();
                    break;
            }
            return freq;
        }
    };

    struct APB1 {
        static Frequency frequency() {
            const uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> 10;
            if (ppre1 & 0b100) {
                const uint32_t div[] = {2, 4, 8, 16};
                return AHB::frequency() / div[ppre1 & 0b011];
            } else {
                return AHB::frequency();
            }
        }
        static bool prescaler(uint_fast8_t prescaler) {
            switch (prescaler) {
                case 1:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1);
                    break;
                case 2:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1) | RCC_CFGR_PPRE1_DIV2;
                    break;
                case 4:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1) | RCC_CFGR_PPRE1_DIV4;
                    break;
                case 8:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1) | RCC_CFGR_PPRE1_DIV8;
                    break;
                case 16:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1) | RCC_CFGR_PPRE1_DIV16;
                    break;
                default:
                    return false;
            }
            return true;
        }
    };

    struct APB2 {
        static Frequency frequency() {
            const uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> 13;
            if (ppre2 & 0b100) {
                const uint32_t div[] = {2, 4, 8, 16};
                return AHB::frequency() / div[ppre2 & 0b011];
            } else {
                return AHB::frequency();
            }
        }
        static bool prescaler(uint_fast8_t prescaler) {
            switch (prescaler) {
                case 1:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE2);
                    break;
                case 2:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE2) | RCC_CFGR_PPRE2_DIV2;
                    break;
                case 4:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE2) | RCC_CFGR_PPRE2_DIV4;
                    break;
                case 8:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE2) | RCC_CFGR_PPRE2_DIV8;
                    break;
                case 16:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE2) | RCC_CFGR_PPRE2_DIV16;
                    break;
                default:
                    return false;
            }
            return true;
        }
    };

    struct AHB {
        static uint32_t frequency() noexcept { return SYSCLK::frequency() / prescaler(); }

        static uint32_t prescaler() {
            const uint32_t hpre = (RCC->CFGR & RCC_CFGR_HPRE) >> 4;
            if (hpre & 0b1000) {
                const uint32_t div[] = {2, 4, 8, 16, 64, 128, 256, 512};
                return div[hpre & 0b0111];
            }
            return 1;
        }
        static bool prescaler(uint32_t prescaler) {
            switch (prescaler) {
                case 1:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE);
                    break;
                case 2:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV2;
                    break;
                case 4:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV4;
                    break;
                case 8:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV8;
                    break;
                case 16:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV16;
                    break;
                case 64:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV64;
                    break;
                case 128:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV128;
                    break;
                case 256:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV256;
                    break;
                case 512:
                    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_HPRE) | RCC_CFGR_HPRE_DIV512;
                    break;
                default:
                    return false;
            }
            return true;
        }
    };

    static Frequency PLLCLKFrequency() noexcept { return PLL::Main::PFrequency(); }
#if defined(RCC_CFGR_MCO2)
    struct MCO2 {
        enum class ClockSource : uint32_t { Sysclk = 0, PllI2S = RCC_CFGR_MCO2_0, HSE = RCC_CFGR_MCO2_1, PLL = RCC_CFGR_MCO2_0 | RCC_CFGR_MCO2_1 };
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
        enum class ClockSource : uint32_t { HSI = 0, LSE = RCC_CFGR_MCO1_0, HSE = RCC_CFGR_MCO1_1, PLL = RCC_CFGR_MCO1_0 | RCC_CFGR_MCO1_1 };
        /**
         *
         * @param clk - Microcontroller clock output source
         *
         * @note According to STM RM0090 - "Clock source selection may generate glitches on MCO. It is highly recommended to configure these bits
         * only after reset before enabling the external oscillators and the PLLs."
         */
        static void clockSource(ClockSource clk) { RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_MCO1) | static_cast<uint32_t>(clk); }
        static ClockSource clockSource() { return static_cast<ClockSource>(RCC->CFGR & RCC_CFGR_MCO1); }
        static uint32_t prescaler() {
            uint32_t cfgr = RCC->CFGR;
            // if prescaler is disabled return prescaler value equal one
            if (cfgr & RCC_CFGR_MCO1PRE_2) return 1;
            return ((cfgr >> 29) & 0b11) + 2;
        }
        /**
         *
         * @param prescaler - Microcontroller clock output prescaler, allowed values: 1, 2, 3, 4, 5
         * @return true when value was set, false if prescaler parameter was unsupported value.
         */
        static bool prescaller(uint32_t prescaler) {
            if (prescaler == 0 || prescaler > 5) return false;
            uint32_t cfgr = RCC->CFGR & ~RCC_CFGR_MCO1PRE;  // clear old configuration

            if (prescaler == 1)
                cfgr |= RCC_CFGR_MCO1PRE_2;
            else
                cfgr |= ((prescaler - 2) << 29);

            RCC->CFGR = cfgr;
            return true;
        }
    };

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
        static bool isEnabled() noexcept { return RCC->CR & RCC_CR_HSION; }
        static bool trim(uint8_t trim) {
            if (trim <= 0b11111) {
                RCC->CR = (RCC->CR & ~RCC_CR_HSITRIM) | (trim << 3);
                return true;
            }
            return false;
        }
        static uint8_t trim() noexcept { return (RCC->CR & ~RCC_CR_HSITRIM) >> 3; }
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
        static bool isEnabled() noexcept { return RCC->CR & RCC_CR_HSEON; }
        static void enableBypas() noexcept { RCC->CR |= RCC_CR_HSEBYP; }
        static void disableBypas() noexcept { RCC->CR &= ~RCC_CR_HSEBYP; }
    };

    struct LSE {
        static Frequency frequency() noexcept {
            if (externalLSEPresent == false) {
                std::terminate();
            }
            return externalLSEFrequency;
        }

        static void enable() noexcept { RCC->BDCR |= RCC_BDCR_LSEON; }
        static void disable() noexcept { RCC->BDCR &= ~RCC_BDCR_LSEON; }
        static bool isReady() noexcept { return RCC->BDCR & RCC_BDCR_LSERDY; }
        static bool isEnabled() noexcept { return RCC->BDCR & RCC_BDCR_LSEON; }
        static void enableBypas() noexcept { RCC->BDCR |= RCC_BDCR_LSEBYP; }
        static void disableBypas() noexcept { RCC->BDCR &= ~RCC_BDCR_LSEBYP; }
    };

    struct LSI {
        /**
         * @brief This function return LSI frequency.
         *
         * @return LSI frequency in [Hz].
         */
        static constexpr Frequency frequency() noexcept { return 32000; }

        static void enable() noexcept { RCC->CSR |= RCC_CSR_LSION; }
        static void disable() noexcept { RCC->CSR &= ~RCC_CSR_LSION; }
        static bool isReady() noexcept { return RCC->CSR & RCC_CSR_LSIRDY; }
        static bool isEnabled() noexcept { return RCC->CSR & RCC_CSR_LSION; }
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
            static bool isEnabled() noexcept { return RCC->CR & RCC_CR_PLLON; }

            static float PFrequency() { return VCOOutputFrequency() / P(); }
            static float QFrequency() { return VCOOutputFrequency() / Q(); }

            static bool N(uint32_t n) noexcept {
                if (n < 2 || n > 432) return false;
                uint32_t tmp = RCC->PLLCFGR;
                tmp &= ~0x1FFU << 6;
                tmp |= n << 6;
                RCC->PLLCFGR = tmp;
                return true;
            }

            static bool P(uint32_t p) noexcept {
                if ((p > 8) || (p & 0b1)) return false;
                uint32_t tmp = RCC->PLLCFGR;
                tmp &= ~0b11U << 16;
                tmp |= (p / 2 - 1) << 16;
                RCC->PLLCFGR = tmp;
                return true;
            }

            static bool Q(uint32_t q) noexcept {
                if (q < 3 || q > 15) return false;
                uint32_t tmp = RCC->PLLCFGR;
                tmp &= ~0xFU << 24;
                tmp |= q << 24;
                RCC->PLLCFGR = tmp;
                return true;
            }

         private:
            static float VCOOutputFrequency() noexcept { return inputFrequency() * N(); }
            static uint32_t N() noexcept { return (RCC->PLLCFGR >> 6) & 0x1FF; }
            static uint32_t P() noexcept {
                const uint32_t tab[] = {2, 4, 6, 8};
                return tab[(RCC->PLLCFGR >> 16) & 0b11];
            }
            static uint32_t Q() noexcept { return (RCC->PLLCFGR >> 24) & 0xF; }
        };

#if defined(RCC_CR_PLLI2SON)
        struct I2S {
            static void enable() noexcept { RCC->CR |= RCC_CR_PLLI2SON; }
            static void disable() noexcept { RCC->CR &= ~RCC_CR_PLLI2SON; }
            static bool isReady() noexcept { return RCC->CR & RCC_CR_PLLI2SRDY; }
            static bool isEnabled() noexcept { return RCC->CR & RCC_CR_PLLI2SON; }
        };
#endif
#if defined(RCC_CR_PLLSAION)
        struct SAI {
            static void enable() noexcept { RCC->CR |= RCC_CR_PLLSAION; }
            static void disable() noexcept { RCC->CR &= ~RCC_CR_PLLSAION; }
            static bool isReady() noexcept { return RCC->CR & RCC_CR_PLLSAIRDY; }
            static bool isEnabled() noexcept { return RCC->CR & RCC_CR_PLLSAION; }
        };
#endif
    };
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // MICROHAL_PORTS_STM32F4XX_CLOCKMANAGER_H_
