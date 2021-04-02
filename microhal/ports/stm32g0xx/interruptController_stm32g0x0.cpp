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

#include "interruptController.h"

#include <array>
#include <bitset>
#include <cassert>
#include "nvic_stm32g0x0.h"

namespace microhal {
namespace stm32g0xx {

extern "C" {
void default_handler(void);

void TIM1_BRK_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM1_UP_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM1_TRG_COM_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM4_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void I2C2_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void I2C3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void SPI2_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void SPI3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USART3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USART4_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USART5_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USART6_IRQHandler(void) __attribute__((weak, alias("default_handler")));
}

static std::bitset<5> TIM1_BRK_UP_TRG_COM_IRQHandlerFlags = 0;
static std::bitset<5> TIM3_4_IRQHandlerFlags = 0;
static std::bitset<5> I2C2_3_IRQHandlerFlags = 0;
static std::bitset<5> SPI2_3_IRQHandlerFlags = 0;
static std::bitset<8> USART3_4_5_6_IRQHandlerFlags = 0;

static constexpr std::array<IRQn_Type, 17> timerIrq = {TIM1_CC_IRQn,   HardFault_IRQn, TIM3_TIM4_IRQn, TIM3_TIM4_IRQn, HardFault_IRQn, TIM6_IRQn,
                                                       TIM7_IRQn,      HardFault_IRQn, HardFault_IRQn, HardFault_IRQn, HardFault_IRQn, HardFault_IRQn,
                                                       HardFault_IRQn, TIM14_IRQn,     TIM15_IRQn,     TIM16_IRQn,     TIM17_IRQn};

void enableTimerInterrupt(uint8_t timerNumber, uint32_t priority) {
    assert(timerNumber > 0);
    assert(timerNumber < timerIrq.size());

    if (timerNumber == 1) {
        TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[0] = 1;
        TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[1] = 1;
        TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[2] = 1;
    } else if (timerNumber == 3 || timerNumber == 4) {
        TIM3_4_IRQHandlerFlags[timerNumber] = 1;
    }
    IRQn_Type irq = timerIrq[timerNumber - 1];
    assert(irq != HardFault_IRQn);
    NVIC_SetPriority(irq, priority);
    NVIC_EnableIRQ(irq);

    if (timerNumber == 1) {
        NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, priority);
        NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
    }
}

void disableTimerInterrupt(uint8_t timerNumber) {
    assert(timerNumber > 0);
    assert(timerNumber < timerIrq.size());

    if (timerNumber == 1) {
        TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[0] = 0;
        TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[1] = 0;
        TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[2] = 0;
        if (TIM1_BRK_UP_TRG_COM_IRQHandlerFlags.none()) {
            NVIC_DisableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
            NVIC_DisableIRQ(timerIrq[timerNumber - 1]);
        }
    } else if (timerNumber == 3 || timerNumber == 4) {
        TIM3_4_IRQHandlerFlags[timerNumber] = 0;
        if (TIM3_4_IRQHandlerFlags.none()) NVIC_DisableIRQ(TIM3_TIM4_IRQn);
    } else {
        NVIC_DisableIRQ(timerIrq[timerNumber - 1]);
    }
}

void enableI2CInterrupt(uint8_t i2cNumber) {
    assert(i2cNumber > 0);
    if (i2cNumber == 1) {
        NVIC_EnableIRQ(I2C1_IRQn);
    } else {
        I2C2_3_IRQHandlerFlags[i2cNumber] = 1;
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

void enableSPIInterrupt(uint8_t spiNumber) {
    assert(spiNumber > 0);
    if (spiNumber == 1) {
        NVIC_EnableIRQ(SPI1_IRQn);
    } else {
        SPI2_3_IRQHandlerFlags[spiNumber] = 1;
        NVIC_EnableIRQ(SPI2_SPI3_IRQn);
    }
}

void disableSPIInterrupt(uint8_t spiNumber) {
    assert(spiNumber > 0);
    if (spiNumber == 1) {
        NVIC_DisableIRQ(SPI1_IRQn);
    } else {
        SPI2_3_IRQHandlerFlags[spiNumber] = 0;
        if (SPI2_3_IRQHandlerFlags.none()) NVIC_DisableIRQ(SPI2_SPI3_IRQn);
    }
}

void enableUSARTInterrupt(uint8_t usartNumber, uint32_t priority) {
    assert(usartNumber > 0);
    assert(usartNumber <= 6);

    if (usartNumber == 1) {
        NVIC_EnableIRQ(USART1_IRQn);
        NVIC_SetPriority(USART1_IRQn, priority);
    } else if (usartNumber == 2) {
        NVIC_EnableIRQ(USART2_IRQn);
        NVIC_SetPriority(USART2_IRQn, priority);
    } else {
        USART3_4_5_6_IRQHandlerFlags[usartNumber] = 1;
        NVIC_EnableIRQ(USART3_4_5_6_IRQn);
        NVIC_SetPriority(USART3_4_5_6_IRQn, priority);
    }
}

void setUSARTInterruptPriority(uint8_t usartNumber, uint32_t priority) {
    assert(usartNumber > 0);
    assert(usartNumber <= 6);

    if (usartNumber == 1) {
        NVIC_SetPriority(USART1_IRQn, priority);
    } else if (usartNumber == 2) {
        NVIC_SetPriority(USART2_IRQn, priority);
    } else {
        NVIC_SetPriority(USART3_4_5_6_IRQn, priority);
    }
}

void disableUSARTInterrupt(uint8_t usartNumber) {
    assert(usartNumber > 0);
    assert(usartNumber <= 6);

    if (usartNumber == 1) {
        NVIC_DisableIRQ(USART1_IRQn);
    } else if (usartNumber == 1) {
        NVIC_DisableIRQ(USART2_IRQn);
    } else {
        USART3_4_5_6_IRQHandlerFlags[usartNumber] = 0;
        if (USART3_4_5_6_IRQHandlerFlags.none()) NVIC_DisableIRQ(USART3_4_5_6_IRQn);
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
void RTC_TAMP_IRQHandler(void) {}

extern "C" void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
    if (TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[0]) TIM1_BRK_IRQHandler();
    if (TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[1]) TIM1_UP_IRQHandler();
    if (TIM1_BRK_UP_TRG_COM_IRQHandlerFlags[2]) TIM1_TRG_COM_IRQHandler();
}

extern "C" void TIM3_4_IRQHandler(void) {
    if (TIM3_4_IRQHandlerFlags[3]) TIM3_IRQHandler();
    if (TIM3_4_IRQHandlerFlags[4]) TIM4_IRQHandler();
}

extern "C" void I2C2_3_IRQHandler(void) {
    if (I2C2_3_IRQHandlerFlags[2]) I2C2_IRQHandler();
    if (I2C2_3_IRQHandlerFlags[3]) I2C3_IRQHandler();
}

extern "C" void SPI2_3_IRQHandler(void) {
    if (SPI2_3_IRQHandlerFlags[2]) SPI2_IRQHandler();
    if (SPI2_3_IRQHandlerFlags[3]) SPI3_IRQHandler();
}

extern "C" void USART3_4_5_6_IRQHandler(void) {
    if (USART3_4_5_6_IRQHandlerFlags[3]) USART3_IRQHandler();
    if (USART3_4_5_6_IRQHandlerFlags[4]) USART4_IRQHandler();
    if (USART3_4_5_6_IRQHandlerFlags[5]) USART5_IRQHandler();
    if (USART3_4_5_6_IRQHandlerFlags[6]) USART6_IRQHandler();
}

}  // namespace stm32g0xx
}  // namespace microhal
