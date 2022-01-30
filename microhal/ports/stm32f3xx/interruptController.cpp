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
#include <cassert>
#include "device/stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {

void enableUSARTInterrupt(uint_fast8_t usartNumber, uint32_t priority) {
    assert(usartNumber > 0);
    assert(usartNumber <= 6);

    if (usartNumber == 1) {
        NVIC_SetPriority(USART1_IRQn, priority);
        NVIC_EnableIRQ(USART1_IRQn);
    } else if (usartNumber == 2) {
        NVIC_SetPriority(USART2_IRQn, priority);
        NVIC_EnableIRQ(USART2_IRQn);
    } else {
        NVIC_SetPriority(USART3_IRQn, priority);
        NVIC_EnableIRQ(USART3_IRQn);
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
        NVIC_SetPriority(USART3_IRQn, priority);
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
        NVIC_DisableIRQ(USART3_IRQn);
    }
}
}  // namespace stm32f3xx
}  // namespace microhal
