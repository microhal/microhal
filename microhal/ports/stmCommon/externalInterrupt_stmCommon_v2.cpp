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
/* ************************************************************************************************
 * INCLUDES
 */
#include "externalInterrupt_stmCommon_v2.h"
#include <type_traits>
#include "clockManager/syscfgClock.h"
#include _MICROHAL_INCLUDE_PORT_DEVICE  // stmCommonDefines.h have to be included before this

#undef EXTI

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
namespace detail {
#define _MICROHAL_HAS_IRQn(x)                                                  \
    template <class T>                                                         \
    struct has_##x {                                                           \
        typedef char yes;                                                      \
        typedef yes (&no)[2];                                                  \
        template <class U>                                                     \
        static yes test(decltype(U::x) *);                                     \
        template <class U>                                                     \
        static no test(...);                                                   \
        static constexpr bool const value = sizeof(test<T>(0)) == sizeof(yes); \
    }

#define _MICROHAL_INIT_EXTI(x, y)         \
    template <typename T>                 \
    void init##x(uint32_t priority) {     \
        NVIC_SetPriority(T::y, priority); \
        NVIC_EnableIRQ(T::y);             \
    }

#define _MICROHAL_INIT_EXTI_ENABLE_IF_EXIST(x, y)                                  \
    template <typename T>                                                          \
    std::enable_if_t<detail::has_##y<T>::value, void> init##x(uint32_t priority) { \
        NVIC_SetPriority(T::y, priority);                                          \
        NVIC_EnableIRQ(T::y);                                                      \
    }

// functions for stm32f0xx port
_MICROHAL_INIT_EXTI(Exti0_1, EXTI0_1_IRQn)
_MICROHAL_INIT_EXTI(Exti2_3, EXTI2_3_IRQn)
_MICROHAL_INIT_EXTI(Exti4_15, EXTI4_15_IRQn)
// functions for stm32f3xx and stm32f4xx
_MICROHAL_INIT_EXTI(Exti0, EXTI0_IRQn)
_MICROHAL_INIT_EXTI(Exti1, EXTI1_IRQn)

_MICROHAL_INIT_EXTI(Exti3, EXTI3_IRQn)
_MICROHAL_INIT_EXTI(Exti4, EXTI4_IRQn)
_MICROHAL_INIT_EXTI(Exti9_5, EXTI9_5_IRQn)
_MICROHAL_INIT_EXTI(Exti15_10, EXTI15_10_IRQn)

_MICROHAL_HAS_IRQn(EXTI0_1_IRQn);
_MICROHAL_HAS_IRQn(EXTI2_IRQn);
_MICROHAL_HAS_IRQn(EXTI2_TSC_IRQn);
_MICROHAL_HAS_IRQn(EXTI2_3_IRQn);
_MICROHAL_HAS_IRQn(EXTI4_15_IRQn);

_MICROHAL_INIT_EXTI_ENABLE_IF_EXIST(Exti2, EXTI2_IRQn)
_MICROHAL_INIT_EXTI_ENABLE_IF_EXIST(Exti2, EXTI2_TSC_IRQn)

template <typename T>
void initExti2(...) {}

}  // namespace detail

Signal<void> ExternalInterrupt::signals[16];

void ExternalInterrupt::init(uint32_t priority) {
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
void externalInterrupt0Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr) {
    if (rpr.RPIF0) {
        registers::EXTI::RPR1 toClear{};
        toClear.RPIF0.set();
        registers::exti1->rpr1.volatileStore(toClear);
    }
    if (fpr.FPIF0) {
        registers::EXTI::FPR1 toClear{};
        toClear.FPIF0.set();
        registers::exti1->fpr1.volatileStore(toClear);
    }
    ExternalInterrupt::signals[0].emit();
}

void externalInterrupt1Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr) {
    if (rpr.RPIF1) {
        registers::EXTI::RPR1 toClear{};
        toClear.RPIF1.set();
        registers::exti1->rpr1.volatileStore(toClear);
    }
    if (fpr.FPIF1) {
        registers::EXTI::FPR1 toClear{};
        toClear.FPIF1.set();
        registers::exti1->fpr1.volatileStore(toClear);
    }

    ExternalInterrupt::signals[1].emit();
}

void externalInterrupt2Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr) {
    if (rpr.RPIF2) {
        registers::EXTI::RPR1 toClear{};
        toClear.RPIF2.set();
        registers::exti1->rpr1.volatileStore(toClear);
    }
    if (fpr.FPIF2) {
        registers::EXTI::FPR1 toClear{};
        toClear.FPIF2.set();
        registers::exti1->fpr1.volatileStore(toClear);
    }
    ExternalInterrupt::signals[2].emit();
}

void externalInterrupt3Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr) {
    if (rpr.RPIF3) {
        registers::EXTI::RPR1 toClear{};
        toClear.RPIF3.set();
        registers::exti1->rpr1.volatileStore(toClear);
    }
    if (fpr.FPIF3) {
        registers::EXTI::FPR1 toClear{};
        toClear.FPIF3.set();
        registers::exti1->fpr1.volatileStore(toClear);
    }

    ExternalInterrupt::signals[3].emit();
}

void externalInterrupt4Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr) {
    if (rpr.RPIF4) {
        registers::EXTI::RPR1 toClear{};
        toClear.RPIF4.set();
        registers::exti1->rpr1.volatileStore(toClear);
    }
    if (fpr.FPIF4) {
        registers::EXTI::FPR1 toClear{};
        toClear.FPIF4.set();
        registers::exti1->fpr1.volatileStore(toClear);
    }

    ExternalInterrupt::signals[4].emit();
}

void externalInterrupt9_5Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr) {
    registers::EXTI::RPR1 raiseToClear{};
    registers::EXTI::FPR1 fallingToClear{};
    if (rpr.RPIF5 || fpr.FPIF5) {
        if (rpr.RPIF5)
            raiseToClear.RPIF5.set();
        else
            fallingToClear.FPIF5.set();

        ExternalInterrupt::signals[5].emit();
    }
    if (rpr.RPIF6 || fpr.FPIF6) {
        if (rpr.RPIF6)
            raiseToClear.RPIF6.set();
        else
            fallingToClear.FPIF6.set();
        ExternalInterrupt::signals[6].emit();
    }
    if (rpr.RPIF7 || fpr.FPIF7) {
        if (rpr.RPIF7)
            raiseToClear.RPIF7.set();
        else
            fallingToClear.FPIF7.set();
        ExternalInterrupt::signals[7].emit();
    }
    if (rpr.RPIF8 || fpr.FPIF8) {
        if (rpr.RPIF8)
            raiseToClear.RPIF8.set();
        else
            fallingToClear.FPIF8.set();
        ExternalInterrupt::signals[8].emit();
    }
    if (rpr.RPIF9 || fpr.FPIF9) {
        if (rpr.RPIF9)
            raiseToClear.RPIF9.set();
        else
            fallingToClear.FPIF9.set();
        ExternalInterrupt::signals[9].emit();
    }
    registers::exti1->rpr1.volatileStore(raiseToClear);
    registers::exti1->fpr1.volatileStore(fallingToClear);
}

void externalInterrupt15_10Irq(registers::EXTI::RPR1 rpr, registers::EXTI::FPR1 fpr) {
    registers::EXTI::RPR1 raiseToClear{};
    registers::EXTI::FPR1 fallingToClear{};
    if (rpr.RPIF10 || fpr.FPIF10) {
        if (rpr.RPIF10)
            raiseToClear.RPIF10.set();
        else
            fallingToClear.FPIF10.set();
        ExternalInterrupt::signals[10].emit();
    }
    if (rpr.RPIF11 || fpr.FPIF11) {
        if (rpr.RPIF11)
            raiseToClear.RPIF11.set();
        else
            fallingToClear.FPIF11.set();
        ExternalInterrupt::signals[11].emit();
    }
    if (rpr.RPIF12 || fpr.FPIF12) {
        if (rpr.RPIF12)
            raiseToClear.RPIF12.set();
        else
            fallingToClear.FPIF12.set();
        ExternalInterrupt::signals[12].emit();
    }
    if (rpr.RPIF13 || fpr.FPIF13) {
        if (rpr.RPIF13)
            raiseToClear.RPIF13.set();
        else
            fallingToClear.FPIF13.set();
        ExternalInterrupt::signals[13].emit();
    }
    if (rpr.RPIF14 || fpr.FPIF14) {
        if (rpr.RPIF14)
            raiseToClear.RPIF14.set();
        else
            fallingToClear.FPIF14.set();
        ExternalInterrupt::signals[14].emit();
    }
    if (rpr.RPIF15 || fpr.FPIF15) {
        if (rpr.RPIF15)
            raiseToClear.RPIF15.set();
        else
            fallingToClear.FPIF15.set();
        ExternalInterrupt::signals[15].emit();
    }
    registers::exti1->rpr1.volatileStore(raiseToClear);
    registers::exti1->fpr1.volatileStore(fallingToClear);
}
//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
extern "C" void EXTI0_1_IRQHandler(void) {
    auto fpr = registers::exti1->fpr1.volatileLoad();
    auto rpr = registers::exti1->rpr1.volatileLoad();
    if (rpr.RPIF0 || fpr.FPIF0) {
        externalInterrupt0Irq(rpr, fpr);
    }
    if (rpr.RPIF1 || fpr.FPIF1) {
        externalInterrupt1Irq(rpr, fpr);
    }
}

extern "C" void EXTI2_3_IRQHandler(void) {
    auto fpr = registers::exti1->fpr1.volatileLoad();
    auto rpr = registers::exti1->rpr1.volatileLoad();
    if (rpr.RPIF2 || fpr.FPIF2) {
        externalInterrupt2Irq(rpr, fpr);
    }
    if (rpr.RPIF3 || fpr.FPIF3) {
        externalInterrupt3Irq(rpr, fpr);
    }
}

extern "C" void EXTI4_15_IRQHandler(void) {
    auto fpr = registers::exti1->fpr1.volatileLoad();
    auto rpr = registers::exti1->rpr1.volatileLoad();
    if (rpr.RPIF4 || fpr.FPIF4) {
        externalInterrupt4Irq(rpr, fpr);
    }
    externalInterrupt9_5Irq(rpr, fpr);
    externalInterrupt15_10Irq(rpr, fpr);
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
