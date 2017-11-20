/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 19-11-2017
 * last modification: 19-11-2017
 *
 * @copyright Copyright (c) 2017, microHAL
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "timer.h"

namespace microhal {
namespace nrf51 {
Signal<void> Timer::interruptTimer0[6];
Signal<void> Timer::interruptTimer1[6];
Signal<void> Timer::interruptTimer2[6];

void TIMER0_IRQHandler() {
    if (NRF_TIMER0->EVENTS_COMPARE[0] == 1) {
        NRF_TIMER0->EVENTS_COMPARE[0] = 0;
        Timer::interruptTimer0[0].emit();
    }
    if (NRF_TIMER0->EVENTS_COMPARE[1] != 0) {
        NRF_TIMER0->EVENTS_COMPARE[1] = 0;
        Timer::interruptTimer0[1].emit();
    }
    if (NRF_TIMER0->EVENTS_COMPARE[2] != 0) {
        NRF_TIMER0->EVENTS_COMPARE[2] = 0;
        Timer::interruptTimer0[2].emit();
    }
    if (NRF_TIMER0->EVENTS_COMPARE[3] != 0) {
        NRF_TIMER0->EVENTS_COMPARE[3] = 0;
        Timer::interruptTimer0[3].emit();
    }
    if (NRF_TIMER0->EVENTS_COMPARE[4] != 0) {
        NRF_TIMER0->EVENTS_COMPARE[4] = 0;
        Timer::interruptTimer0[4].emit();
    }
    if (NRF_TIMER0->EVENTS_COMPARE[5] != 0) {
        NRF_TIMER0->EVENTS_COMPARE[5] = 0;
        Timer::interruptTimer0[5].emit();
    }
}

void TIMER1_IRQHandler() {
    if (NRF_TIMER1->EVENTS_COMPARE[0] == 1) {
        NRF_TIMER1->EVENTS_COMPARE[0] = 0;
        Timer::interruptTimer1[0].emit();
    }
    if (NRF_TIMER1->EVENTS_COMPARE[1] != 0) {
        NRF_TIMER1->EVENTS_COMPARE[1] = 0;
        Timer::interruptTimer1[1].emit();
    }
    if (NRF_TIMER1->EVENTS_COMPARE[2] != 0) {
        NRF_TIMER1->EVENTS_COMPARE[2] = 0;
        Timer::interruptTimer1[2].emit();
    }
    if (NRF_TIMER1->EVENTS_COMPARE[3] != 0) {
        NRF_TIMER1->EVENTS_COMPARE[3] = 0;
        Timer::interruptTimer1[3].emit();
    }
    if (NRF_TIMER1->EVENTS_COMPARE[4] != 0) {
        NRF_TIMER1->EVENTS_COMPARE[4] = 0;
        Timer::interruptTimer1[4].emit();
    }
    if (NRF_TIMER1->EVENTS_COMPARE[5] != 0) {
        NRF_TIMER1->EVENTS_COMPARE[5] = 0;
        Timer::interruptTimer1[5].emit();
    }
}

void TIMER2_IRQHandler() {
    if (NRF_TIMER2->EVENTS_COMPARE[0] == 1) {
        NRF_TIMER2->EVENTS_COMPARE[0] = 0;
        Timer::interruptTimer2[0].emit();
    }
    if (NRF_TIMER2->EVENTS_COMPARE[1] != 0) {
        NRF_TIMER2->EVENTS_COMPARE[1] = 0;
        Timer::interruptTimer2[1].emit();
    }
    if (NRF_TIMER2->EVENTS_COMPARE[2] != 0) {
        NRF_TIMER2->EVENTS_COMPARE[2] = 0;
        Timer::interruptTimer2[2].emit();
    }
    if (NRF_TIMER2->EVENTS_COMPARE[3] != 0) {
        NRF_TIMER2->EVENTS_COMPARE[3] = 0;
        Timer::interruptTimer2[3].emit();
    }
    if (NRF_TIMER2->EVENTS_COMPARE[4] != 0) {
        NRF_TIMER2->EVENTS_COMPARE[4] = 0;
        Timer::interruptTimer2[4].emit();
    }
    if (NRF_TIMER2->EVENTS_COMPARE[5] != 0) {
        NRF_TIMER2->EVENTS_COMPARE[5] = 0;
        Timer::interruptTimer2[5].emit();
    }
}

}  // namespace microhal
}  // namespace nrf51
