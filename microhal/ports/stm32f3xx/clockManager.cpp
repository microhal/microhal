/*
 * clockManager.cpp
 *
 *  Created on: 20.11.2016
 *      Author: pawel
 */

#include "clockManager.h"

namespace microhal {
namespace stm32f3xx {

void ClockManager::enable(const USART_TypeDef &usart) {
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

void ClockManager::enable(const DAC_TypeDef &dac) {
    if (&dac == DAC1) {
        RCC->APB1ENR |= RCC_APB1ENR_DAC1EN;
        return;
    }
#if defined(DAC2)
    if (&dac == DAC2) {
        RCC->APB1ENR |= RCC_APB1ENR_DAC2EN;
        return;
    }
#endif
    std::terminate();  // critical error, should never go here
}

void ClockManager::enableGPIO(const microhal::registers::GPIO &gpio) {
    if ((int)&gpio == IOPin::PortA)
        RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    else if ((int)&gpio == IOPin::PortB)
        RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    else if ((int)&gpio == IOPin::PortC)
        RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    else if ((int)&gpio == IOPin::PortD)
        RCC->AHBENR |= RCC_AHBENR_GPIODEN;
#if defined(GPIOE_BASE)
    else if ((int)&gpio == IOPin::PortE)
        RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
#endif
#if defined(GPIOF_BASE)
    else if ((int)&gpio == IOPin::PortF)
        RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
#endif
    else {
        while (1)
            ;
    }
}

ClockManager::UsartClockSource ClockManager::USARTClockSource(const USART_TypeDef &usart) {
    uint32_t pos;
    if (&usart == USART1)
        pos = RCC_CFGR3_USART1SW_Pos;
    else if (&usart == USART2)
        // pos = RCC_CFGR3_USART2SW_Pos;
        return UsartClockSource::PCLK;
    else if (&usart == USART3)
        // pos = RCC_CFGR3_USART3SW_Pos;
        return UsartClockSource::PCLK;
    else {
        while (1)
            ;
    }
    return static_cast<UsartClockSource>((RCC->CFGR3 >> pos) & 0b11);
}

void ClockManager::USARTClockSource(const USART_TypeDef &usart, UsartClockSource source) {
    uint32_t pos;
    if (&usart == USART1)
        pos = RCC_CFGR3_USART1SW_Pos;
    else if (&usart == USART2)
        // pos = RCC_CFGR3_USART2SW_Pos;
        while (1)
            ;
    else if (&usart == USART3)
        // pos = RCC_CFGR3_USART3SW_Pos;
        while (1)
            ;
    else {
        while (1)
            ;
    }
    RCC->CFGR3 = (RCC->CFGR3 & ~(0b11 << pos)) | (source << pos);
}

uint32_t ClockManager::USARTFrequency(const USART_TypeDef &usart) {
    UsartClockSource usartClockSource = USARTClockSource(usart);

    switch (usartClockSource) {
        case PCLK:
            return APB1Frequency();
        case SYSCLK:
            return SYSCLK::frequency();
        case LSE:
            while (1)
                ;
            return 0;  // LSE::frequency();
            break;
        case HSI:
            return HSI::frequency();
    }
    std::terminate();
}

ClockManager::Frequency ClockManager::APB1Frequency() {
    const uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos;
    if (ppre1 & 0b100) {
        const uint32_t div[] = {2, 4, 8, 16};
        return AHBFrequency() / div[ppre1 & 0b011];
    } else {
        return AHBFrequency();
    }
}

ClockManager::Frequency ClockManager::APB2Frequency() {
    const uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos;
    if (ppre2 & 0b100) {
        const uint32_t div[] = {2, 4, 8, 16};
        return AHBFrequency() / div[ppre2 & 0b011];
    } else {
        return AHBFrequency();
    }
}

uint32_t ClockManager::AHBFrequency() noexcept {
    const uint32_t hpre = (RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos;
    if (hpre & 0b1000) {
        const uint32_t div[] = {2, 4, 8, 16, 64, 128, 256, 512};
        return SYSCLK::frequency() / div[hpre & 0b0111];
    } else {
        return SYSCLK::frequency();
    }
}

void ClockManager::flashLatency(Frequency sysclkFrequency_Hz) {
    if (sysclkFrequency_Hz < 24000000) {
        FLASH->ACR &= ~FLASH_ACR_LATENCY;
    } else if (sysclkFrequency_Hz < 24000000) {
        FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_0;
    } else {
        FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_1;
    }
}

void ClockManager::SYSCLK::source(Source source) {
    switch (source) {
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

uint32_t ClockManager::SYSCLK::frequency() noexcept {
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

float ClockManager::PLL::inputFrequency() {
    if (clockSource() == ClockSource::HSIDiv2) {
        return HSI::frequency() / 2;
    } else {
        return HSE::frequency() / divider();
    }
}

uint32_t ClockManager::PLL::frequency(uint32_t frequency_Hz) {
    if (clockSource() == ClockSource::HSIDiv2) {
        // we only can change pll mul
        uint32_t mul = round(frequency_Hz / inputFrequency());
        if (mul > 16) mul = 16;
        if (mul < 2) mul = 2;
        PLLMUL(mul);
        while (HSI::isReady() == false)
            ;
    } else {
    }

    enable();
    while (isReady() == false)
        ;
    return VCOOutputFrequency();
}

float ClockManager::PLL::VCOOutputFrequency() noexcept {
    return inputFrequency() * PLLMUL();
}

uint32_t ClockManager::PLL::PLLMUL() noexcept {
    uint32_t mul = ((RCC->CFGR & RCC_CFGR_PLLMUL_Msk) >> RCC_CFGR_PLLMUL_Pos);
    if (mul == 0b1111) return 16;
    return mul + 2;
}

bool ClockManager::PLL::PLLMUL(uint32_t mul) noexcept {
    if (mul >= 2 && mul <= 16) {
        RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PLLMUL_Msk) | ((mul - 2) << RCC_CFGR_PLLMUL_Pos);
        return true;
    }
    return false;
}

}  // namespace stm32f3xx
}  // namespace microhal
