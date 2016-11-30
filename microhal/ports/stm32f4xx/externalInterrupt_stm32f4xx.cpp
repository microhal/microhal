/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      diagnostic component
 *
 * @authors    Pawel Okas
 * created on: 20-03-2015
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015, microHAL
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
/* ************************************************************************************************
 * INCLUDES
 */
#include "externalInterrupt_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

Signal<void> ExternalInterrupt::signals[16];

void ExternalInterrupt::init() {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_SetPriority(EXTI0_IRQn, 0x0F);

    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_SetPriority(EXTI1_IRQn, 0x0F);

    NVIC_EnableIRQ(EXTI2_IRQn);
    NVIC_SetPriority(EXTI2_IRQn, 0x0F);

    NVIC_EnableIRQ(EXTI3_IRQn);
    NVIC_SetPriority(EXTI3_IRQn, 0x0F);

    NVIC_EnableIRQ(EXTI4_IRQn);
    NVIC_SetPriority(EXTI4_IRQn, 0x0F);

    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn, 0x0F);

    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 0x0F);
}

void EXTI0_IRQHandler(void) {
    // clear interrupt
    EXTI->PR |= EXTI_PR_PR0;

    ExternalInterrupt::signals[0].emit();
}

void EXTI1_IRQHandler(void) {
    // clear interrupt
    EXTI->PR |= EXTI_PR_PR1;

    ExternalInterrupt::signals[1].emit();
}

void EXTI2_IRQHandler(void) {
    // clear interrupt
    EXTI->PR |= EXTI_PR_PR2;

    ExternalInterrupt::signals[2].emit();
}

void EXTI3_IRQHandler(void) {
    // clear interrupt
    EXTI->PR |= EXTI_PR_PR3;

    ExternalInterrupt::signals[3].emit();
}

void EXTI4_IRQHandler(void) {
    // clear interrupt
    EXTI->PR |= EXTI_PR_PR4;

    ExternalInterrupt::signals[4].emit();
}

void EXTI9_5_IRQHandler(void) {
    uint32_t pr = EXTI->PR;

    if (pr & EXTI_PR_PR5) {
        // clear interrupt
        EXTI->PR |= EXTI_PR_PR5;

        ExternalInterrupt::signals[5].emit();
    }
    if (pr & EXTI_PR_PR6) {
        // clear interrupt
        EXTI->PR |= EXTI_PR_PR6;

        ExternalInterrupt::signals[6].emit();
    }
    if (pr & EXTI_PR_PR7) {
        // clear interrupt
        EXTI->PR |= EXTI_PR_PR7;

        ExternalInterrupt::signals[7].emit();
    }
    if (pr & EXTI_PR_PR8) {
        // clear interrupt
        EXTI->PR |= EXTI_PR_PR8;

        ExternalInterrupt::signals[8].emit();
    }
    if (pr & EXTI_PR_PR9) {
        // clear interrupt
        EXTI->PR |= EXTI_PR_PR9;

        ExternalInterrupt::signals[9].emit();
    }
}

void EXTI15_10_IRQHandler(void) {
    uint32_t pr = EXTI->PR;

    if (pr & EXTI_PR_PR10) {
        // clear interrupt
        EXTI->PR |= EXTI_PR_PR10;

        ExternalInterrupt::signals[10].emit();
    }
    if (pr & EXTI_PR_PR11) {
        // clear interrupt
        EXTI->PR |= EXTI_PR_PR11;

        ExternalInterrupt::signals[11].emit();
    }
    if (pr & EXTI_PR_PR12) {
        // clear interrupt
        EXTI->PR |= EXTI_PR_PR12;

        ExternalInterrupt::signals[12].emit();
    }
    if (pr & EXTI_PR_PR13) {
        // clear interrupt
        EXTI->PR |= EXTI_PR_PR13;

        ExternalInterrupt::signals[13].emit();
    }
    if (pr & EXTI_PR_PR14) {
        // clear interrupt
        EXTI->PR |= EXTI_PR_PR14;

        ExternalInterrupt::signals[14].emit();
    }
    if (pr & EXTI_PR_PR15) {
        // clear interrupt
        EXTI->PR |= EXTI_PR_PR15;

        ExternalInterrupt::signals[15].emit();
    }
}

}  // namespace stm32f4xx
}  // namespace microhal

