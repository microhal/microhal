/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 19-11-2017
 * last modification: 19-11-2017
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

#ifndef _MICROHAL_TIMER_H_
#define _MICROHAL_TIMER_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */

#include <cstddef>
#include <cstdint>
#include <exception>
#include "device/nrf.h"

#include "signalSlot/signalSlot.h"

namespace microhal {
namespace nrf51 {

extern "C" {
void TIMER0_IRQHandler();
void TIMER1_IRQHandler();
void TIMER2_IRQHandler();
}
/* **************************************************************************************************************************************************
 * CLASS
 */
class Timer {
 public:
    enum class Mode : uint32_t { Timer = 0, Counter = 1, LowPowerCounter = 2 };
    enum class BitMode : uint32_t { Width_16Bits = 0, Width_8Bits = 1, Width_24Bits = 2, Width_32Bits = 3 };

    Timer(NRF_TIMER_Type &timer)
        : captureCompare({CC{timer, 0}, CC(timer, 1), CC(timer, 2), CC(timer, 3), CC(timer, 4), CC(timer, 5)}), timer(timer) {}

    void mode(Mode mode) { timer.MODE = static_cast<uint32_t>(mode); }
    Mode mode() { return static_cast<Mode>(timer.MODE); }
    uint32_t prescaler(uint32_t presc) {
        if (presc > 9) presc = 9;
        timer.PRESCALER = presc;
        return presc;
    }
    uint32_t prescaler() { return timer.PRESCALER; }
    uint32_t frequency() { return 16000000 / (2 << prescaler()); }
    void bitMode(BitMode bitMode) { timer.BITMODE = static_cast<uint32_t>(bitMode); }
    BitMode bitMode() { return static_cast<BitMode>(timer.BITMODE); }

    void start() { timer.TASKS_START = 1; }
    void stop() { timer.TASKS_STOP = 1; }
    void shutdown() { timer.TASKS_SHUTDOWN = 1; }
    void clear() { timer.TASKS_CLEAR = 1; }

    void enableInterrupt() { NVIC_EnableIRQ(getIRQNumber()); }
    void disableInterrupt() { NVIC_DisableIRQ(getIRQNumber()); }

    struct CC {
        CC(NRF_TIMER_Type &timer, size_t ccNumber) : timer(timer), ccNumber(ccNumber) {}
        void value(uint32_t value) { timer.CC[ccNumber] = value; }
        uint32_t value() { return timer.CC[ccNumber]; }
        void enableInterrupt() { timer.INTENSET = 1 << (16 + ccNumber); }
        void disableInterrupt() { timer.INTENCLR = 1 << (16 + ccNumber); }

        void enableTimerClearOnCompare() { timer.SHORTS |= 1 << ccNumber; }
        void disableTimerClearOnCompare() { timer.SHORTS &= ~(1 << ccNumber); }

        void enableTimerStopOnCompare() { timer.SHORTS |= 1 << (8 + ccNumber); }
        void disableTimerStopOnCompare() { timer.SHORTS &= ~(1 << (8 + ccNumber)); }

        bool connectInterrupt(void (*func)(void)) noexcept {
            if (&timer == NRF_TIMER0) return interruptTimer0[ccNumber].connect(func);
            if (&timer == NRF_TIMER1) return interruptTimer1[ccNumber].connect(func);
            if (&timer == NRF_TIMER2) return interruptTimer2[ccNumber].connect(func);
            std::terminate();  // critical error
        }
        template <typename T>
        bool connectInterrupt(const T &slot, const typename T::type &object) noexcept {
            if (&timer == NRF_TIMER0) return interruptTimer0[ccNumber].connect(slot.function, &object);
            if (&timer == NRF_TIMER1) return interruptTimer1[ccNumber].connect(slot.function, &object);
            if (&timer == NRF_TIMER2) return interruptTimer2[ccNumber].connect(slot.function, &object);
            std::terminate();  // critical error
        }

     private:
        NRF_TIMER_Type &timer;
        size_t ccNumber;
    };

    CC captureCompare[6];

 private:
    NRF_TIMER_Type &timer;
    static Signal<void> interruptTimer0[6];
    static Signal<void> interruptTimer1[6];
    static Signal<void> interruptTimer2[6];

    IRQn_Type getIRQNumber() {
        if (&timer == NRF_TIMER0) return TIMER0_IRQn;
        if (&timer == NRF_TIMER1) return TIMER1_IRQn;
        if (&timer == NRF_TIMER2) return TIMER2_IRQn;
        std::terminate();  // critical error, should newer go here
    }

    friend void TIMER0_IRQHandler();
    friend void TIMER1_IRQHandler();
    friend void TIMER2_IRQHandler();
};
}  // namespace nrf51
}  // namespace microhal

#endif  // _MICROHAL_TIMER_H_
