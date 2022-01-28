/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 26-03-2021
 *
 * @copyright Copyright (c) 2021, Pawel Okas
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

#include "ports/stmCommon/interruptController/interruptController.h"

#include <array>
#include <bitset>
#include <cassert>
#include "../dma_stm32g0xx.h"
#include "../nvic_stm32g0x1.h"

namespace microhal {
namespace stm32g0xx {

extern "C" {
void DMA1_Channel2_3_IRQHandler(void);
void DMA1_Channel4_5_6_7_DMAMUX_DMA2_Channel1_2_3_4_5_IRQHandler(void);
void RTC_TAMP_IRQHandler(void);
void RCC_CRS_IRQHandler(void);
void UCPD1_UCPD2_USB_Handler(void);
void ADC_COMP_IRQHandler(void);
void TIM1_BRK_UP_TRG_COM_IRQHandler(void);
void TIM3_4_IRQHandler(void);
void TIM6_DAC_LPTIM1_IRQHandler(void);
void TIM7_LPTIM2_IRQHandler(void);
void TIM16_FDCAN_IT0_IRQHandler(void);
void TIM17_FDCAN_IT1_IRQHandler(void);
void I2C2_3_IRQHandler(void);
void SPI2_3_IRQHandler(void);
void USART2_LPUART2_IRQHandler(void);
void USART3_4_5_6_LPUART1_IRQHandler(void);
void AES_RNG_IRQHandler(void);
}

extern "C" {
void default_handler(void);

void DMA1_Channel2_IRQHandler(void) __attribute__((interrupt, weak, alias("default_handler")));
void DMA1_Channel3_IRQHandler(void) __attribute__((interrupt, weak, alias("default_handler")));
void DMA1_Channel4_IRQHandler(void) __attribute__((interrupt, weak, alias("default_handler")));
void DMA1_Channel5_IRQHandler(void) __attribute__((interrupt, weak, alias("default_handler")));
void DMA1_Channel6_IRQHandler(void) __attribute__((interrupt, weak, alias("default_handler")));
void DMA1_Channel7_IRQHandler(void) __attribute__((interrupt, weak, alias("default_handler")));

void RTC_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TAMP_IRQHandler(void) __attribute__((weak, alias("default_handler")));

void DAC_IRQHandler(void) __attribute__((weak, alias("default_handler")));

void TIM1_BRK_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM1_UP_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM1_TRG_COM_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM4_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM5_IRQHandler(void);
void TIM6_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM7_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM16_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM17_IRQHandler(void) __attribute__((weak, alias("default_handler")));

void LPTIM1_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void LPTIM2_IRQHandler(void) __attribute__((weak, alias("default_handler")));

void I2C2_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void I2C3_IRQHandler(void) __attribute__((weak, alias("default_handler")));

void SPI2_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void SPI3_IRQHandler(void) __attribute__((weak, alias("default_handler")));

void USART2_IRQHandler(void);
void USART3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USART4_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USART5_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USART6_IRQHandler(void) __attribute__((weak, alias("default_handler")));

void LPUART1_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void LPUART2_IRQHandler(void) __attribute__((weak, alias("default_handler")));

void FDCAN_IT0_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void FDCAN_IT1_IRQHandler(void) __attribute__((weak, alias("default_handler")));
}

static std::bitset<5> DMA1_IRQHandlerFlags = 0;
static std::bitset<5> TIM1_BRK_UP_TRG_COM_IRQHandlerFlags = 0;
static std::bitset<5> TIM3_4_IRQHandlerFlags = 0;
static std::bitset<5> I2C2_3_IRQHandlerFlags = 0;
static std::bitset<5> SPI2_3_IRQHandlerFlags = 0;
static std::bitset<8> USART3_4_5_6_LPUART1_IRQHandlerFlags = 0;
static std::bitset<8> TIM6_DAC_LPTIM1_IRQHandlerFlags = 0;  // flags: 6 timer, 2 DAC, 1 LPTIM1
static std::bitset<8> TIM7_LPTIM2_IRQHandlerFlags = 0;      // flags: 7 timer, 2 LPTIM2
static std::bitset<8> USART2_LPUART2_IRQHandlerFlags = 0;   // flags: 2 USART2, 3 LPUART2
static std::bitset<8> TIM16_FDCAN_IT0_IRQHandlerFlags = 0;
static std::bitset<8> TIM17_FDCAN_IT1_IRQHandlerFlags = 0;
static std::bitset<8> RTC_TAMP_IRQHandlerFlags = 0;

static constexpr std::array<IRQn_Type, 17> timerIrq = {TIM1_CC_IRQn,         TIM2_IRQn,           TIM3_TIM4_IRQn, TIM3_TIM4_IRQn, HardFault_IRQn,
                                                       TIM6_DAC_LPTIM1_IRQn, TIM7_LPTIM2_IRQn,    HardFault_IRQn, HardFault_IRQn, HardFault_IRQn,
                                                       HardFault_IRQn,       HardFault_IRQn,      HardFault_IRQn, TIM14_IRQn,     TIM15_IRQn,
                                                       TIM16_FDCAN_IT0_IRQn, TIM17_FDCAN_IT1_IRQn};

static constexpr std::array<IRQn_Type, 7> dmaIrq = {DMA1_Channel1_IRQn,
                                                    DMA1_Channel2_3_IRQn,
                                                    DMA1_Channel2_3_IRQn,
                                                    DMA1_Ch4_7_DMAMUX1_DMA2_Ch1_5_IRQn,
                                                    DMA1_Ch4_7_DMAMUX1_DMA2_Ch1_5_IRQn,
                                                    DMA1_Ch4_7_DMAMUX1_DMA2_Ch1_5_IRQn,
                                                    DMA1_Ch4_7_DMAMUX1_DMA2_Ch1_5_IRQn};

void enableDMA1Interrupt(uint32_t channelNumber, uint32_t priority) {
    DMA1_IRQHandlerFlags[channelNumber] = 1;
    NVIC_SetPriority(dmaIrq[channelNumber], priority);
    NVIC_EnableIRQ(dmaIrq[channelNumber]);
}

void disableDMA1Interrupt(uint32_t channelNumber) {
    DMA1_IRQHandlerFlags[channelNumber] = 0;
    if (channelNumber == 0) {
        NVIC_DisableIRQ(dmaIrq[channelNumber]);
    } else if (channelNumber < 3) {
        if (DMA1_IRQHandlerFlags[1] == 0 && DMA1_IRQHandlerFlags[2] == 0) NVIC_DisableIRQ(dmaIrq[channelNumber]);
    } else if (DMA1_IRQHandlerFlags[3] == 0 && DMA1_IRQHandlerFlags[4] == 0 && DMA1_IRQHandlerFlags[5] == 0 && DMA1_IRQHandlerFlags[6] == 0) {
        NVIC_DisableIRQ(dmaIrq[channelNumber]);
    }
}

void enableRTCInterrupt(uint32_t priority) {
    RTC_TAMP_IRQHandlerFlags[0] = 1;
    NVIC_SetPriority(RTC_TAMP_IRQn, priority);
    NVIC_EnableIRQ(RTC_TAMP_IRQn);
}

void disableRTCInterrupt() {
    RTC_TAMP_IRQHandlerFlags[0] = 0;
    if (RTC_TAMP_IRQHandlerFlags.none()) NVIC_DisableIRQ(RTC_TAMP_IRQn);
}

void enableTimerInterrupt(uint8_t timerNumber, uint32_t priority) {
    assert(timerNumber > 0);

    if (timerNumber == 1) {
        TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[0] = 1;
    } else if (timerNumber == 3 || timerNumber == 4) {
        TIM3_4_IRQHandlerFlags[timerNumber] = 1;
    } else if (timerNumber == 6) {
        TIM6_DAC_LPTIM1_IRQHandlerFlags[timerNumber] = 1;
    } else if (timerNumber == 7) {
        TIM7_LPTIM2_IRQHandlerFlags[timerNumber] = 1;
    } else if (timerNumber == 16) {
        TIM16_FDCAN_IT0_IRQHandlerFlags[timerNumber] = 1;
    } else if (timerNumber == 17) {
        TIM17_FDCAN_IT1_IRQHandlerFlags[timerNumber] = 1;
    }
    IRQn_Type irq = timerIrq[timerNumber - 1];
    assert(irq != HardFault_IRQn);
    NVIC_SetPriority(irq, priority);
    NVIC_EnableIRQ(irq);
}

void disableTimerInterrupt(uint8_t timerNumber) {
    assert(timerNumber > 0);

    if (timerNumber == 1) {
        TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[0] = 0;
        if (TIM1_BRK_UP_TRG_COM_IRQHandlerFlags.none()) NVIC_DisableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
    } else if (timerNumber == 3 || timerNumber == 4) {
        TIM3_4_IRQHandlerFlags[timerNumber] = 0;
        if (TIM3_4_IRQHandlerFlags.none()) NVIC_DisableIRQ(TIM3_TIM4_IRQn);
    } else if (timerNumber == 6) {
        TIM6_DAC_LPTIM1_IRQHandlerFlags[timerNumber] = 0;
        if (TIM6_DAC_LPTIM1_IRQHandlerFlags.none()) NVIC_DisableIRQ(TIM6_DAC_LPTIM1_IRQn);
    } else if (timerNumber == 7) {
        TIM7_LPTIM2_IRQHandlerFlags[timerNumber] = 0;
        if (TIM7_LPTIM2_IRQHandlerFlags.none()) NVIC_DisableIRQ(TIM7_LPTIM2_IRQn);
    } else {
        NVIC_DisableIRQ(timerIrq[timerNumber]);
    }
}

void enableI2CInterrupt(uint8_t i2cNumber, uint32_t priority) {
    assert(i2cNumber > 0);

    if (i2cNumber == 1) {
        NVIC_SetPriority(I2C1_IRQn, priority);
        NVIC_EnableIRQ(I2C1_IRQn);
    } else {
        I2C2_3_IRQHandlerFlags[i2cNumber] = 1;
        NVIC_SetPriority(I2C2_I2C3_IRQn, priority);
        NVIC_EnableIRQ(I2C2_I2C3_IRQn);
    }
}

void disableI2CInterrupt(uint8_t i2cNumber) {
    assert(i2cNumber > 0);

    if (i2cNumber == 1) {
        NVIC_DisableIRQ(I2C1_IRQn);
    } else {
        I2C2_3_IRQHandlerFlags[i2cNumber] = 0;
        if (I2C2_3_IRQHandlerFlags.none()) NVIC_DisableIRQ(I2C2_I2C3_IRQn);
    }
}

void enableSPIInterrupt(uint_fast8_t spiNumber, uint32_t priority) {
    assert(spiNumber < 3);
    if (spiNumber == 0) {
        NVIC_SetPriority(SPI1_IRQn, priority);
        NVIC_EnableIRQ(SPI1_IRQn);
    } else {
        SPI2_3_IRQHandlerFlags[spiNumber] = 1;
        NVIC_SetPriority(SPI2_SPI3_IRQn, priority);
        NVIC_EnableIRQ(SPI2_SPI3_IRQn);
    }
}

void disableSPIInterrupt(uint_fast8_t spiNumber) {
    assert(spiNumber < 3);
    if (spiNumber == 0) {
        NVIC_DisableIRQ(SPI1_IRQn);
    } else {
        SPI2_3_IRQHandlerFlags[spiNumber] = 0;
        if (SPI2_3_IRQHandlerFlags.none()) NVIC_DisableIRQ(SPI2_SPI3_IRQn);
    }
}

void enableUSARTInterrupt(uint_fast8_t usartNumber, uint32_t priority) {
    assert(usartNumber > 0);
    assert(usartNumber <= 6);

    if (usartNumber == 1) {
        NVIC_SetPriority(USART1_IRQn, priority);
        NVIC_EnableIRQ(USART1_IRQn);
    } else if (usartNumber == 2) {
        USART2_LPUART2_IRQHandlerFlags[usartNumber] = 1;
        NVIC_SetPriority(USART2_LPUART2_IRQn, priority);
        NVIC_EnableIRQ(USART2_LPUART2_IRQn);
    } else {
        USART3_4_5_6_LPUART1_IRQHandlerFlags[usartNumber] = 1;
        NVIC_SetPriority(USART3_4_5_6_LPUART1_IRQn, priority);
        NVIC_EnableIRQ(USART3_4_5_6_LPUART1_IRQn);
    }
}

void setUSARTInterruptPriority(uint_fast8_t usartNumber, uint32_t priority) {
    assert(usartNumber > 0);
    assert(usartNumber <= 6);

    if (usartNumber == 1) {
        NVIC_SetPriority(USART1_IRQn, priority);
    } else if (usartNumber == 2) {
        NVIC_SetPriority(USART2_LPUART2_IRQn, priority);
    } else {
        NVIC_SetPriority(USART3_4_5_6_LPUART1_IRQn, priority);
    }
}

void disableUSARTInterrupt(uint_fast8_t usartNumber) {
    assert(usartNumber > 0);
    assert(usartNumber <= 6);

    if (usartNumber == 1) {
        NVIC_DisableIRQ(USART1_IRQn);
    } else if (usartNumber == 1) {
        USART2_LPUART2_IRQHandlerFlags[usartNumber] = 0;
        if (USART2_LPUART2_IRQHandlerFlags.none()) NVIC_DisableIRQ(USART2_LPUART2_IRQn);
    } else {
        USART3_4_5_6_LPUART1_IRQHandlerFlags[usartNumber] = 0;
        if (USART3_4_5_6_LPUART1_IRQHandlerFlags.none()) NVIC_DisableIRQ(USART3_4_5_6_LPUART1_IRQn);
    }
}

void default_handler(void) {
    __asm volatile("BKPT #01");
    while (1)
        ;
}

//****************************************************************************//
//                           interrupt handlers                               //
//****************************************************************************//
void DMA1_Channel2_3_IRQHandler(void) {
    const uint32_t isr1 = DMA::dma1->getIsr();
    if ((isr1 & (0b1111 << (4 * 1))) && (DMA1_IRQHandlerFlags[1] == 1)) DMA1_Channel2_IRQHandler();
    if ((isr1 & (0b1111 << (4 * 2))) && (DMA1_IRQHandlerFlags[2] == 1)) DMA1_Channel3_IRQHandler();
}

void DMA1_Channel4_5_6_7_DMAMUX_DMA2_Channel1_2_3_4_5_IRQHandler(void) {
    const uint32_t isr1 = DMA::dma1->getIsr();
    if ((isr1 & (0b1111 << (4 * 3))) && (DMA1_IRQHandlerFlags[3] == 1)) DMA1_Channel4_IRQHandler();
    if ((isr1 & (0b1111 << (4 * 4))) && (DMA1_IRQHandlerFlags[4] == 1)) DMA1_Channel5_IRQHandler();
    if ((isr1 & (0b1111 << (4 * 5))) && (DMA1_IRQHandlerFlags[5] == 1)) DMA1_Channel6_IRQHandler();
    if ((isr1 & (0b1111 << (4 * 6))) && (DMA1_IRQHandlerFlags[6] == 1)) DMA1_Channel7_IRQHandler();

#ifdef _MICROHAL_DMA2_BASE_ADDRESS
    const uint32_t isr2 = DMA::dma2->getIsr();
    if ((isr2 & (0b1111 << 0)) && (DMA2_IRQHandlerFlags[0] == 1)) DMA2_Channel1_IRQHandler();
    if ((isr2 & (0b1111 << 1)) && (DMA2_IRQHandlerFlags[1] == 1)) DMA2_Channel2_IRQHandler();
    if ((isr2 & (0b1111 << 2)) && (DMA2_IRQHandlerFlags[2] == 1)) DMA2_Channel3_IRQHandler();
    if ((isr2 & (0b1111 << 3)) && (DMA2_IRQHandlerFlags[3] == 1)) DMA2_Channel4_IRQHandler();
    if ((isr2 & (0b1111 << 4)) && (DMA2_IRQHandlerFlags[4] == 1)) DMA2_Channel5_IRQHandler();
#endif
}

void RTC_TAMP_IRQHandler(void) {
    if (RTC_TAMP_IRQHandlerFlags[0]) RTC_IRQHandler();
    if (RTC_TAMP_IRQHandlerFlags[1]) TAMP_IRQHandler();
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
    if (TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[0]) TIM1_BRK_IRQHandler();
    if (TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[1]) TIM1_UP_IRQHandler();
    if (TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[2]) TIM1_TRG_COM_IRQHandler();
}

void TIM3_4_IRQHandler(void) {
    if (TIM3_4_IRQHandlerFlags[3]) TIM3_IRQHandler();
    if (TIM3_4_IRQHandlerFlags[4]) TIM4_IRQHandler();
}

void TIM6_DAC_LPTIM1_IRQHandler(void) {
    if (TIM6_DAC_LPTIM1_IRQHandlerFlags[6]) TIM6_IRQHandler();
    if (TIM6_DAC_LPTIM1_IRQHandlerFlags[2]) DAC_IRQHandler();
    if (TIM6_DAC_LPTIM1_IRQHandlerFlags[1]) LPTIM1_IRQHandler();
}

void TIM7_LPTIM2_IRQHandler(void) {
    if (TIM7_LPTIM2_IRQHandlerFlags[7]) TIM7_IRQHandler();
    if (TIM7_LPTIM2_IRQHandlerFlags[2]) LPTIM2_IRQHandler();
}

void TIM16_FDCAN_IT0_IRQHandler(void) {
    if (TIM16_FDCAN_IT0_IRQHandlerFlags[1]) TIM16_IRQHandler();
    if (TIM16_FDCAN_IT0_IRQHandlerFlags[0]) FDCAN_IT0_IRQHandler();
}

void TIM17_FDCAN_IT1_IRQHandler(void) {
    if (TIM17_FDCAN_IT1_IRQHandlerFlags[2]) TIM17_IRQHandler();
    if (TIM17_FDCAN_IT1_IRQHandlerFlags[1]) FDCAN_IT1_IRQHandler();
}

void I2C2_3_IRQHandler(void) {
    if (I2C2_3_IRQHandlerFlags[2]) I2C2_IRQHandler();
    if (I2C2_3_IRQHandlerFlags[3]) I2C3_IRQHandler();
}

void SPI2_3_IRQHandler(void) {
    if (SPI2_3_IRQHandlerFlags[1]) SPI2_IRQHandler();
    if (SPI2_3_IRQHandlerFlags[2]) SPI3_IRQHandler();
}

void USART2_LPUART2_IRQHandler(void) {
    if (USART2_LPUART2_IRQHandlerFlags[2]) USART2_IRQHandler();
    if (USART2_LPUART2_IRQHandlerFlags[3]) LPUART2_IRQHandler();
}

void USART3_4_5_6_LPUART1_IRQHandler(void) {
    if (USART3_4_5_6_LPUART1_IRQHandlerFlags[3]) USART3_IRQHandler();
    if (USART3_4_5_6_LPUART1_IRQHandlerFlags[4]) USART4_IRQHandler();
    if (USART3_4_5_6_LPUART1_IRQHandlerFlags[5]) USART5_IRQHandler();
    if (USART3_4_5_6_LPUART1_IRQHandlerFlags[6]) USART6_IRQHandler();
}

}  // namespace stm32g0xx
}  // namespace microhal
