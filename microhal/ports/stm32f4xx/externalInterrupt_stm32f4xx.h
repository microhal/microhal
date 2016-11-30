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

#ifndef _MICROHAL_EXTERNALINTERRUPT_STM32F4XX_H_
#define _MICROHAL_EXTERNALINTERRUPT_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */

#include <cstdint>

#include "stm32f4xx.h"
#include "gpio_stm32f4xx.h"

#include "signalSlot/signalSlot.h"

namespace microhal {
namespace stm32f4xx {

extern "C" {
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
class ExternalInterrupt {
 public:
    typedef enum {
        TriggerOnRising, TriggerOnFalling, TriggerOnEdge
    } Trigger;

    ExternalInterrupt() = delete;

    static void init();

    template<typename T>
    static inline bool connect(const T &slot, const typename T::type &object, Trigger trigger, const GPIO::Port port, const GPIO::Pin pinNumber) {
        if (signals[pinNumber].connect(slot, object)) {
            configure(pinNumber, trigger);
            SYSCFG->EXTICR[pinNumber / 4] |= ((port - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)) << (4 * (pinNumber % 4));
            return true;
        }
        return false;
    }

    static inline bool connect(void (*interruptFunction)(void), Trigger trigger, const GPIO::Port port, const GPIO::Pin pinNumber) {
        if (signals[pinNumber].connect(interruptFunction)) {
            configure(pinNumber, trigger);


            SYSCFG->EXTICR[pinNumber / 4] |= ((port - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)) << (4 * (pinNumber % 4));
            return true;
        }
        return false;
    }

    static inline bool disconnect(void (*interruptFunction)(void), const GPIO::Port port, const GPIO::Pin pinNumber) {
        if (signals[pinNumber].disconnect(interruptFunction)) {
            SYSCFG->EXTICR[pinNumber / 4] &= ~(((port - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)) << (4 * (pinNumber % 4)));
            return true;
        }
        return false;
    }

    static inline bool enable(const GPIO::Port port, const GPIO::Pin pinNumber) __attribute__((always_inline)) {
        if ( (SYSCFG->EXTICR[pinNumber / 4] >> (4 * (pinNumber % 4))) == ((port - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)) ) {
            EXTI->IMR |= 1 << pinNumber;
            return true;
        }
        return false;
    }
    static inline bool disable(const GPIO::Port port, const GPIO::Pin pinNumber) __attribute__((always_inline)) {
        if ( (SYSCFG->EXTICR[pinNumber / 4] >> (4 * (pinNumber % 4))) == ((port - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)) ) {
            EXTI->IMR &= ~(1 << pinNumber);
            return true;
        }
        return false;
    }

    static inline bool isEnabled(const GPIO::Port port, const GPIO::Pin pinNumber) {
        if ( (SYSCFG->EXTICR[pinNumber / 4] >> (4 * (pinNumber % 4))) == ((port - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)) ) {
            return EXTI->IMR & (1 << pinNumber);
        }
        return false;
    }

 private:
    static Signal<void> signals[16];

    friend void EXTI0_IRQHandler(void);
    friend void EXTI1_IRQHandler(void);
    friend void EXTI2_IRQHandler(void);
    friend void EXTI3_IRQHandler(void);
    friend void EXTI4_IRQHandler(void);
    friend void EXTI9_5_IRQHandler(void);
    friend void EXTI15_10_IRQHandler(void);

    static void configure(uint8_t vectorNumber, Trigger trigger) {
        const uint32_t bitMask = 1 << vectorNumber;

        switch (trigger) {
        case TriggerOnRising:
            EXTI->RTSR |= bitMask;
            EXTI->FTSR &= ~bitMask;
            break;
        case TriggerOnFalling:
            EXTI->FTSR |= bitMask;
            EXTI->RTSR &= ~bitMask;
            break;
        case TriggerOnEdge:
            EXTI->RTSR |= bitMask;
            EXTI->FTSR |= bitMask;
            break;
        }
    }
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // _MICROHAL_EXTERNALINTERRUPT_STM32F4XX_H_
