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
#include "nvic_connectivityLine.h"

namespace microhal {
namespace stm32f1xx {

static constexpr const std::array<IRQn_Type, 5> usartIrq = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn};
static constexpr const std::array<IRQn_Type, 2> canTxIrq = {CAN1_TX_IRQn, CAN2_TX_IRQn};
static constexpr const std::array<IRQn_Type, 2> canRx0Irq = {CAN1_RX0_IRQn, CAN2_RX0_IRQn};
static constexpr const std::array<IRQn_Type, 2> canRx1Irq = {CAN1_RX1_IRQn, CAN2_RX1_IRQn};
static constexpr const std::array<IRQn_Type, 2> canSceIrq = {CAN1_SCE_IRQn, CAN2_SCE_IRQn};

void enableCanTxInterrupt(uint8_t canNumber, uint32_t priority) {
    assert(canNumber > 0);
    assert(canNumber <= 2);

    NVIC_SetPriority(canTxIrq[canNumber - 1], priority);
    NVIC_EnableIRQ(canTxIrq[canNumber - 1]);
}
void enableCanRx0Interrupt(uint8_t canNumber, uint32_t priority) {
    assert(canNumber > 0);
    assert(canNumber <= 2);

    NVIC_SetPriority(canRx0Irq[canNumber - 1], priority);
    NVIC_EnableIRQ(canRx0Irq[canNumber - 1]);
}

void enableCanRx1Interrupt(uint8_t canNumber, uint32_t priority) {
    assert(canNumber > 0);
    assert(canNumber <= 2);

    NVIC_SetPriority(canRx1Irq[canNumber - 1], priority);
    NVIC_EnableIRQ(canRx1Irq[canNumber - 1]);
}

void enableCanSceInterrupt(uint8_t canNumber, uint32_t priority) {
    assert(canNumber > 0);
    assert(canNumber <= 2);

    NVIC_SetPriority(canSceIrq[canNumber - 1], priority);
    NVIC_EnableIRQ(canSceIrq[canNumber - 1]);
}

void disableCanTxInterrupt(uint8_t canNumber) {
    NVIC_DisableIRQ(canTxIrq[canNumber - 1]);
}

void disableCanRx0Interrupt(uint8_t canNumber) {
    NVIC_DisableIRQ(canRx0Irq[canNumber - 1]);
}

void disableCanRx1Interrupt(uint8_t canNumber) {
    NVIC_DisableIRQ(canRx1Irq[canNumber - 1]);
}

void disableCanSceInterrupt(uint8_t canNumber) {
    NVIC_DisableIRQ(canSceIrq[canNumber - 1]);
}

void enableUSARTInterrupt(uint8_t usartNumber, uint32_t priority) {
    assert(usartNumber > 0);
    assert(usartNumber <= 6);

    NVIC_SetPriority(usartIrq[usartNumber - 1], priority);
    NVIC_EnableIRQ(usartIrq[usartNumber - 1]);
}

void enableUSBInterrupt(uint32_t priority) {
    NVIC_SetPriority(OTG_FS_IRQn, priority);
    NVIC_EnableIRQ(OTG_FS_IRQn);
}

void disableUSBInterrupt(uint32_t priority) {
    NVIC_DisableIRQ(OTG_FS_IRQn);
}

void enableUSBWakeupInterrupt(uint32_t priority) {
    NVIC_SetPriority(OTG_FS_WKUP_IRQn, priority);
    NVIC_EnableIRQ(OTG_FS_WKUP_IRQn);
}

void disableUSBWakeupInterrupt() {
    NVIC_DisableIRQ(OTG_FS_WKUP_IRQn);
}

}  // namespace stm32f1xx
}  // namespace microhal
