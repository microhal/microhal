/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      external interrupt implementation version 2 for STM32
 *
 * @authors    Pawel Okas
 * created on: 06-02-2021
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

#ifndef _MICROHAL_PORTS_EXTERNALINTERRUPT_STMCOMMON_H_
#define _MICROHAL_PORTS_EXTERNALINTERRUPT_STMCOMMON_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include <exception>

#include "IOPin.h"
#include "registers/exti_registers_v2.h"
#include "signalSlot/signalSlot.h"
#include "stmCommonDefines.h"

#undef EXTI

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
/* ************************************************************************************************
 * CLASS
 */
class ExternalInterrupt {
 public:
    typedef enum { TriggerOnRising, TriggerOnFalling, TriggerOnEdge } Trigger;

    ExternalInterrupt() = delete;

    static void init(uint32_t priority);

    template <typename T>
    static inline bool connect(const T &slot, const typename T::type &object, Trigger trigger, IOPin pin) {
        if (signals[pin.pin].connect(slot, object)) {
            configure(pin.pin, trigger);
            selectEXTILinePort(pin);
            return true;
        }
        return false;
    }

    static inline bool connect(void (*interruptFunction)(void), Trigger trigger, IOPin pin) {
        if (signals[pin.pin].connect(interruptFunction)) {
            configure(pin.pin, trigger);
            selectEXTILinePort(pin);
            return true;
        }
        return false;
    }

    static inline bool disconnect(void (*interruptFunction)(void), IOPin pin) {
        if (signals[pin.pin].disconnect(interruptFunction)) {
            deselectEXTILinePort(pin);
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
            auto imr = registers::exti1->imr1.volatileLoad();
            imr |= 1 << pin.pin;
            registers::exti1->imr1.volatileStore(imr);
            return true;
        }
        return false;
    }
    static inline bool disable(IOPin pin) {
        if (getConfiguration(pin) == pin.port) {
            auto imr = registers::exti1->imr1.volatileLoad();
            imr &= ~(1 << pin.pin);
            registers::exti1->imr1.volatileStore(imr);
            return true;
        }
        return false;
    }

    static bool isEnabled(IOPin pin) {
        if (getConfiguration(pin) == pin.port) {
            auto imr = registers::exti1->imr1.volatileLoad();
            return imr & uint32_t(1 << pin.pin);
        }
        return false;
    }

 private:
    static Signal<void> signals[16];

    friend void externalInterrupt0Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr);
    friend void externalInterrupt1Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr);
    friend void externalInterrupt2Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr);
    friend void externalInterrupt3Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr);
    friend void externalInterrupt4Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr);
    friend void externalInterrupt9_5Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr);
    friend void externalInterrupt15_10Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr);

    static void configure(uint_fast8_t vectorNumber, Trigger trigger) {
        const uint32_t bitMask = 1 << vectorNumber;

        auto rtsr = registers::exti1->rtsr1.volatileLoad();
        auto ftsr = registers::exti1->ftsr1.volatileLoad();
        switch (trigger) {
            case TriggerOnRising:
                rtsr |= bitMask;
                ftsr &= ~bitMask;
                break;
            case TriggerOnFalling:
                ftsr |= bitMask;
                rtsr &= ~bitMask;
                break;
            case TriggerOnEdge:
                rtsr |= bitMask;
                ftsr |= bitMask;
                break;
        }
        registers::exti1->rtsr1.volatileStore(rtsr);
        registers::exti1->ftsr1.volatileStore(ftsr);
    }

    static void selectEXTILinePort(IOPin pin) {
        auto exticr = registers::exti1->exticr[pin.pin / 4].volatileLoad();
        exticr &= ~(0b1111'1111 << (8 * (pin.pin % 4)));
        exticr |= ((pin.port - _MICROHAL_GPIOA_BASE_ADDRESS) / (_MICROHAL_GPIOB_BASE_ADDRESS - _MICROHAL_GPIOA_BASE_ADDRESS)) << (8 * (pin.pin % 4));
        registers::exti1->exticr[pin.pin / 4].volatileStore(exticr);
    }

    static void deselectEXTILinePort(IOPin pin) {
        auto exticr = registers::exti1->exticr[pin.pin / 4].volatileLoad();
        exticr &= ~(0b1111'1111 << (8 * (pin.pin % 4)));
        registers::exti1->exticr[pin.pin / 4].volatileStore(exticr);
    }

    static IOPin::Port getConfiguration(IOPin pin) {
        uint32_t exticr = registers::exti1->exticr[pin.pin / 4].volatileLoad();
        exticr >>= 8 * (pin.pin % 4);
        exticr &= 0b1111'1111;
        switch (exticr) {
            case 0:
                return IOPin::PortA;
            case 1:
                return IOPin::PortB;
            case 2:
                return IOPin::PortC;
#if defined(_MICROHAL_GPIOD_BASE_ADDRESS)
            case 3:
                return IOPin::PortD;
#endif
#if defined(_MICROHAL_GPIOE_BASE_ADDRESS)
            case 4:
                return IOPin::PortE;
#endif
#if defined(_MICROHAL_GPIOF_BASE_ADDRESS)
            case 5:
                return IOPin::PortF;
#endif
        }
        std::terminate();
    }
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // _MICROHAL_PORTS_EXTERNALINTERRUPT_STMCOMMON_H_
