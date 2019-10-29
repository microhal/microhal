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
/* ************************************************************************************************
 * INCLUDES
 */
#include "externalInterrupt_stmCommon.h"
#include <type_traits>

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
namespace detail {
#define _MICROHAL_HAS_IRQn(x)                                        \
    template <class T>                                               \
    struct has_##x {                                                 \
        typedef char yes;                                            \
        typedef yes (&no)[2];                                        \
        template <class U>                                           \
        static yes test(decltype(U::x) *);                           \
        template <class U>                                           \
        static no test(...);                                         \
        static bool const value = sizeof(test<T>(0)) == sizeof(yes); \
    }

#define _MICROHAL_INIT_EXTI(x, y)         \
    template <typename T>                 \
    void init##x(uint32_t priority) {     \
        NVIC_SetPriority(T::y, priority); \
        NVIC_EnableIRQ(T::y);             \
    }

#define _MICROHAL_INIT_EXTI_ENABLE_IF_EXIST(x, y)             \
    template <typename T>                                     \
    std::enable_if_t<T::y, void> init##x(uint32_t priority) { \
        NVIC_SetPriority(T::y, priority);                     \
        NVIC_EnableIRQ(T::y);                                 \
    }

template <typename T>
std::enable_if_t<T::EXTI2_3_IRQn, void> initExti2(uint32_t) {
    std::terminate();
}

// functions for stm32f0xx port
_MICROHAL_INIT_EXTI(Exti0_1, EXTI0_1_IRQn)
_MICROHAL_INIT_EXTI(Exti2_3, EXTI2_3_IRQn)
_MICROHAL_INIT_EXTI(Exti4_15, EXTI4_15_IRQn)
// functions for stm32f3xx and stm32f4xx
_MICROHAL_INIT_EXTI(Exti0, EXTI0_IRQn)
_MICROHAL_INIT_EXTI(Exti1, EXTI1_IRQn)

_MICROHAL_INIT_EXTI_ENABLE_IF_EXIST(Exti2, EXTI2_IRQn)
_MICROHAL_INIT_EXTI_ENABLE_IF_EXIST(Exti2, EXTI2_TSC_IRQn)

_MICROHAL_INIT_EXTI(Exti3, EXTI3_IRQn)
_MICROHAL_INIT_EXTI(Exti4, EXTI4_IRQn)
_MICROHAL_INIT_EXTI(Exti9_5, EXTI9_5_IRQn)
_MICROHAL_INIT_EXTI(Exti15_10, EXTI15_10_IRQn)

_MICROHAL_HAS_IRQn(EXTI0_1_IRQn);
_MICROHAL_HAS_IRQn(EXTI2_3_IRQn);
_MICROHAL_HAS_IRQn(EXTI4_15_IRQn);
}  // namespace detail

Signal<void> ExternalInterrupt::signals[16];

void ExternalInterrupt::init(uint32_t priority) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    if constexpr (detail::has_EXTI0_1_IRQn<IRQn_Type>::value) {
        detail::initExti0_1<IRQn_Type>(priority);
    } else {
        detail::initExti0<IRQn_Type>(priority);
        detail::initExti1<IRQn_Type>(priority);
    }

    if constexpr (detail::has_EXTI2_3_IRQn<IRQn_Type>::value) {
        detail::initExti2_3<IRQn_Type>(priority);
    } else {
        detail::initExti2<IRQn_Type>(priority);
        detail::initExti3<IRQn_Type>(priority);
    };

    if constexpr (detail::has_EXTI4_15_IRQn<IRQn_Type>::value) {
        detail::initExti4_15<IRQn_Type>(priority);
    } else {
        detail::initExti4<IRQn_Type>(priority);
        detail::initExti9_5<IRQn_Type>(priority);
        detail::initExti15_10<IRQn_Type>(priority);
    }
}
//***********************************************************************************************//
//                                     Interrupt functions                                       //
//***********************************************************************************************//
static void externalInterrupt0Irq() {
    // clear interrupt
    EXTI->PR |= EXTI_PR_PR0;

    ExternalInterrupt::signals[0].emit();
}

static void externalInterrupt1Irq() {
    // clear interrupt
    EXTI->PR |= EXTI_PR_PR1;

    ExternalInterrupt::signals[1].emit();
}

static void externalInterrupt2Irq() {
    // clear interrupt
    EXTI->PR |= EXTI_PR_PR2;

    ExternalInterrupt::signals[2].emit();
}

static void externalInterrupt3Irq() {
    // clear interrupt
    EXTI->PR |= EXTI_PR_PR3;

    ExternalInterrupt::signals[3].emit();
}

static void externalInterrupt4Irq() {
    // clear interrupt
    EXTI->PR |= EXTI_PR_PR4;

    ExternalInterrupt::signals[4].emit();
}

static void externalInterrupt9_5Irq(uint32_t pr) {
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

static void externalInterrupt15_10Irq(uint32_t pr) {
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
//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
#if defined(MCU_TYPE_STM32F0XX)
extern "C" void EXTI0_1_IRQHandler(void) {
    uint32_t pr = EXTI->PR;
    if (pr & EXTI_PR_PR0) {
        externalInterrupt0Irq();
    }
    if (pr & EXTI_PR_PR1) {
        externalInterrupt1Irq();
    }
}

extern "C" void EXTI2_3_IRQHandler(void) {
    uint32_t pr = EXTI->PR;
    if (pr & EXTI_PR_PR2) {
        externalInterrupt2Irq();
    }
    if (pr & EXTI_PR_PR3) {
        externalInterrupt3Irq();
    }
}

extern "C" void EXTI4_15_IRQHandler(void) {
    uint32_t pr = EXTI->PR;
    if (pr & EXTI_PR_PR4) {
        externalInterrupt4Irq();
    }
    externalInterrupt9_5Irq(pr);
    externalInterrupt15_10Irq(pr);
}

#else
extern "C" void EXTI0_IRQHandler(void) {
    externalInterrupt0Irq();
}
extern "C" void EXTI1_IRQHandler(void) {
    externalInterrupt1Irq();
}
extern "C" void EXTI2_IRQHandler(void) {
    externalInterrupt2Irq();
}
extern "C" void EXTI3_IRQHandler(void) {
    externalInterrupt3Irq();
}
extern "C" void EXTI4_IRQHandler(void) {
    externalInterrupt4Irq();
}
extern "C" void EXTI9_5_IRQHandler(void) {
    externalInterrupt9_5Irq(EXTI->PR);
}
extern "C" void EXTI15_10_IRQHandler(void) {
    externalInterrupt15_10Irq(EXTI->PR);
}
#endif
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
