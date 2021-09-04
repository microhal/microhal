/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 09-04-2021
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
#include <cassert>
#include "nvic.h"

namespace microhal {
namespace stm32f1xx {

static constexpr std::array<IRQn_Type, 3> spiIrq = {SPI1_IRQn, SPI2_IRQn, SPI3_IRQn};

static constexpr std::array<IRQn_Type, 8> timer1Irq = {TIM1_BRK_IRQn, TIM1_UP_IRQn, TIM1_TRG_COM_IRQn};
static constexpr std::array<IRQn_Type, 8> timer8Irq = {TIM8_BRK_IRQn, TIM8_UP_IRQn, TIM8_TRG_COM_IRQn};
static constexpr std::array<IRQn_Type, 8> timerIrq = {TIM1_CC_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn, TIM5_IRQn, TIM6_IRQn, TIM7_IRQn, TIM8_CC_IRQn};

static constexpr std::array<IRQn_Type, 5> usartIrq = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn};

void enableCanTxInterrupt(uint8_t canNumber, uint32_t priority) {
    assert(canNumber > 0);
    assert(canNumber <= 2);

    NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, priority);
    NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
}
void enableCanRx0Interrupt(uint8_t canNumber, uint32_t priority) {
    assert(canNumber > 0);
    assert(canNumber <= 2);

    NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, priority);
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}

void enableCanRx1Interrupt(uint8_t canNumber, uint32_t priority) {
    assert(canNumber > 0);
    assert(canNumber <= 2);

    NVIC_SetPriority(CAN1_RX1_IRQn, priority);
    NVIC_EnableIRQ(CAN1_RX1_IRQn);
}

void enableCanSceInterrupt(uint8_t canNumber, uint32_t priority) {
    assert(canNumber > 0);
    assert(canNumber <= 2);

    NVIC_SetPriority(CAN1_SCE_IRQn, priority);
    NVIC_EnableIRQ(CAN1_SCE_IRQn);
}

void disableCanTxInterrupt(uint8_t canNumber) {
    NVIC_DisableIRQ(USB_HP_CAN1_TX_IRQn);
}

void disableCanRx0Interrupt(uint8_t canNumber) {
    NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
}

void disableCanRx1Interrupt(uint8_t canNumber) {
    NVIC_DisableIRQ(CAN1_RX1_IRQn);
}

void disableCanSceInterrupt(uint8_t canNumber) {
    NVIC_DisableIRQ(CAN1_SCE_IRQn);
}

// void enableDMA1Interrupt(uint32_t channelNumber, uint32_t priority) {
//    DMA1_IRQHandlerFlags[channelNumber] = 1;
//    NVIC_SetPriority(dmaIrq[channelNumber], priority);
//    NVIC_EnableIRQ(dmaIrq[channelNumber]);
//}
//
// void disableDMA1Interrupt(uint32_t channelNumber) {
//    DMA1_IRQHandlerFlags[channelNumber] = 0;
//    if (channelNumber == 0) {
//        NVIC_DisableIRQ(dmaIrq[channelNumber]);
//    } else if (channelNumber < 3) {
//        if (DMA1_IRQHandlerFlags[1] == 0 && DMA1_IRQHandlerFlags[2] == 0) NVIC_DisableIRQ(dmaIrq[channelNumber]);
//    } else if (DMA1_IRQHandlerFlags[3] == 0 && DMA1_IRQHandlerFlags[4] == 0 && DMA1_IRQHandlerFlags[5] == 0 && DMA1_IRQHandlerFlags[6] == 0) {
//        NVIC_DisableIRQ(dmaIrq[channelNumber]);
//    }
//}

void enableSPIInterrupt(uint8_t spiNumber, uint32_t priority) {
    assert(spiNumber < spiIrq.size());

    NVIC_SetPriority(spiIrq[spiNumber], priority);
    NVIC_EnableIRQ(spiIrq[spiNumber]);
}
void disableSPIInterrupt(uint8_t spiNumber) {
    assert(spiNumber < spiIrq.size());

    NVIC_DisableIRQ(spiIrq[spiNumber]);
}

void enableTimerInterrupt(uint8_t timerNumber, uint32_t priority) {
    assert(timerNumber > 0);
    assert(timerNumber < timerIrq.size());

    IRQn_Type irq = timerIrq[timerNumber - 1];
    assert(irq != HardFault_IRQn);
    NVIC_SetPriority(irq, priority);
    NVIC_EnableIRQ(irq);

    if (timerNumber == 1) {
        for (auto irq : timer1Irq) {
            NVIC_SetPriority(irq, priority);
            NVIC_EnableIRQ(irq);
        }
    }

    if (timerNumber == 8) {
        for (auto irq : timer8Irq) {
            NVIC_SetPriority(irq, priority);
            NVIC_EnableIRQ(irq);
        }
    }
}

void disableTimerInterrupt(uint8_t timerNumber) {
    assert(timerNumber > 0);
    assert(timerNumber < timerIrq.size());

    IRQn_Type irq = timerIrq[timerNumber - 1];
    assert(irq != HardFault_IRQn);
    NVIC_DisableIRQ(irq);

    if (timerNumber == 1) {
        for (auto irq : timer1Irq) {
            NVIC_DisableIRQ(irq);
        }
    }
    if (timerNumber == 8) {
        for (auto irq : timer8Irq) {
            NVIC_DisableIRQ(irq);
        }
    }
}

void enableUSARTInterrupt(uint8_t usartNumber, uint32_t priority) {
    assert(usartNumber > 0);
    assert(usartNumber <= 6);

    NVIC_SetPriority(usartIrq[usartNumber - 1], priority);
    NVIC_EnableIRQ(usartIrq[usartNumber - 1]);
}

}  // namespace stm32f1xx
}  // namespace microhal
