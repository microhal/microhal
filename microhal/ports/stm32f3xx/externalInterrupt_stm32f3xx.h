/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      external interrupt implementation for stm32f3xx
 *
 * @authors    Pawel Okas
 * created on: 19-02-2017
 * last modification: <DD-MM-YYYY>
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

#ifndef _MICROHAL_EXTERNALINTERRUPT_STM32F3XX_H_
#define _MICROHAL_EXTERNALINTERRUPT_STM32F3XX_H_
/* ************************************************************************************************
 * INCLUDES
 */

#include <cstdint>

#include "IOPin.h"
#include "device/stm32f3xx.h"
#include "gpio_stm32f3xx.h"

#include "signalSlot/signalSlot.h"

namespace microhal {
namespace stm32f3xx {

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
    typedef enum { TriggerOnRising, TriggerOnFalling, TriggerOnEdge } Trigger;

    ExternalInterrupt() = delete;

    static void init();

    template <typename T>
    static inline bool connect(const T &slot, const typename T::type &object, Trigger trigger, IOPin::Port port, IOPin::Pin pinNumber) {
        if (signals[pinNumber].connect(slot, object)) {
            configure(pinNumber, trigger);
            selectEXTILinePort({port, pinNumber});
            return true;
        }
        return false;
    }

    static inline bool connect(void (*interruptFunction)(void), Trigger trigger, const GPIO::Port port, const GPIO::Pin pinNumber) {
        if (signals[pinNumber].connect(interruptFunction)) {
            configure(pinNumber, trigger);
            selectEXTILinePort({port, pinNumber});
            return true;
        }
        return false;
    }

    static inline bool disconnect(void (*interruptFunction)(void), const GPIO::Port port, const GPIO::Pin pinNumber) {
        if (signals[pinNumber].disconnect(interruptFunction)) {
            deselectEXTILinePort({port, pinNumber});
            return true;
        }
        return false;
    }

    template <typename T>
    static inline bool disconnect(const T &slot, const typename T::type &object, Trigger trigger, IOPin::Port port, IOPin::Pin pinNumber) {
        if (signals[pinNumber].connect(slot, object)) {
            deselectEXTILinePort({port, pinNumber});
            return true;
        }
        return false;
    }

    static bool enable(IOPin pin) {
        if (getConfiguration(pin) == pin.port) {
            volatile auto exti = EXTI;
            exti->IMR |= 1 << pin.pin;
            return true;
        }
        return false;
    }
    static inline bool disable(GPIO::Port port, GPIO::Pin pinNumber) {
        if (getConfiguration({port, pinNumber}) == port) {
            volatile auto exti = EXTI;
            exti->IMR &= ~(1 << pinNumber);
            return true;
        }
        return false;
    }

    static bool isEnabled(IOPin pin) {
        if (getConfiguration(pin) == pin.port) {
            volatile auto exti = EXTI;
            return exti->IMR & (1 << pin.pin);
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

    static void configure(uint_fast8_t vectorNumber, Trigger trigger) {
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

    static void selectEXTILinePort(IOPin pin) {
        uint32_t exticr = SYSCFG->EXTICR[pin.pin / 4];
        exticr &= ~(0b1111 << (4 * (pin.pin % 4)));
        exticr |= ((pin.port - GPIOA_BASE) / (GPIOB_BASE - GPIOA_BASE)) << (4 * (pin.pin % 4));
        SYSCFG->EXTICR[pin.pin / 4] = exticr;
    }

    static void deselectEXTILinePort(IOPin pin) {
        uint32_t exticr = SYSCFG->EXTICR[pin.pin / 4];
        exticr &= ~(0b1111 << (4 * (pin.pin % 4)));
        SYSCFG->EXTICR[pin.pin / 4] = exticr;
    }

    static IOPin::Port getConfiguration(IOPin pin) {
        //        constexpr IOPin::Port map[] = {IOPin::PortA, IOPin::PortB, IOPin::PortC, IOPin::PortD, IOPin::PortE};
        uint32_t exticr = SYSCFG->EXTICR[pin.pin / 4];
        exticr >>= 4 * (pin.pin % 4);
        exticr &= 0b1111;
        switch (exticr) {
            case 0:
                return IOPin::PortA;
            case 1:
                return IOPin::PortB;
            case 2:
                return IOPin::PortC;
            case 3:
                return IOPin::PortD;
#if defined(GPIOE_BASE)
            case 4:
                return IOPin::PortE;
#endif
        }
        std::terminate();
    }
};

}  // namespace stm32f3xx
}  // namespace microhal

#endif  // _MICROHAL_EXTERNALINTERRUPT_STM32F3XX_H_
