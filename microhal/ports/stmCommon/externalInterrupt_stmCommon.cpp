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
#include _MICROHAL_INCLUDE_PORT_DEVICE  // stmCommonDefines.h have to be included before this
#undef EXTI

#if defined(_MICROHAL_AFIO_BASE_ADDRESS)
#include "clockManager/afioClock.h"
#endif
#if defined(_MICROHAL_SYSCFG_BASE_ADDRESS) || defined(_MICROHAL_SYSCFG_COMP_OPAMP_BASE_ADDRESS) || defined(_MICROHAL_SYSCFG_COMP_BASE_ADDRESS)
#include "clockManager/syscfgClock.h"
#endif

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
#if defined(_MICROHAL_SYSCFG_BASE_ADDRESS) || defined(_MICROHAL_SYSCFG_COMP_OPAMP_BASE_ADDRESS) || defined(_MICROHAL_SYSCFG_COMP_BASE_ADDRESS)
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::enableSYSCFG(ClockManager::PowerMode::Normal);
#else
    ClockManager::enableSYSCFG();
#endif
#endif
#if defined(_MICROHAL_AFIO_BASE_ADDRESS)
    ClockManager::enableAFIO();
#endif

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
void externalInterrupt0Irq() {
    registers::EXTI::PR toClear{};
    toClear.PR0.set();
    registers::exti1->pr.volatileStore(toClear);

    ExternalInterrupt::signals[0].emit();
}

void externalInterrupt1Irq() {
    registers::EXTI::PR toClear{};
    toClear.PR1.set();
    registers::exti1->pr.volatileStore(toClear);

    ExternalInterrupt::signals[1].emit();
}

void externalInterrupt2Irq() {
    registers::EXTI::PR toClear{};
    toClear.PR2.set();
    registers::exti1->pr.volatileStore(toClear);

    ExternalInterrupt::signals[2].emit();
}

void externalInterrupt3Irq() {
    registers::EXTI::PR toClear{};
    toClear.PR3.set();
    registers::exti1->pr.volatileStore(toClear);

    ExternalInterrupt::signals[3].emit();
}

void externalInterrupt4Irq() {
    registers::EXTI::PR toClear{};
    toClear.PR4.set();
    registers::exti1->pr.volatileStore(toClear);

    ExternalInterrupt::signals[4].emit();
}

void externalInterrupt9_5Irq(registers::EXTI::PR pr) {
    registers::EXTI::PR toClear;
    toClear = 0;
    if (pr.PR5) {
        toClear.PR5.set();
        ExternalInterrupt::signals[5].emit();
    }
    if (pr.PR6) {
        toClear.PR6.set();
        ExternalInterrupt::signals[6].emit();
    }
    if (pr.PR7) {
        toClear.PR7.set();
        ExternalInterrupt::signals[7].emit();
    }
    if (pr.PR8) {
        toClear.PR8.set();
        ExternalInterrupt::signals[8].emit();
    }
    if (pr.PR9) {
        toClear.PR9.set();
        ExternalInterrupt::signals[9].emit();
    }
    registers::exti1->pr.volatileStore(toClear);
}

void externalInterrupt15_10Irq(registers::EXTI::PR pr) {
    registers::EXTI::PR toClear;
    toClear = 0;
    if (pr.PR10) {
        toClear.PR10.set();
        ExternalInterrupt::signals[10].emit();
    }
    if (pr.PR11) {
        toClear.PR11.set();
        ExternalInterrupt::signals[11].emit();
    }
    if (pr.PR12) {
        toClear.PR12.set();
        ExternalInterrupt::signals[12].emit();
    }
    if (pr.PR13) {
        toClear.PR13.set();
        ExternalInterrupt::signals[13].emit();
    }
    if (pr.PR14) {
        toClear.PR14.set();
        ExternalInterrupt::signals[14].emit();
    }
    if (pr.PR15) {
        toClear.PR15.set();
        ExternalInterrupt::signals[15].emit();
    }
    registers::exti1->pr.volatileStore(toClear);
}
//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
#if defined(MCU_TYPE_STM32F0XX)
extern "C" void EXTI0_1_IRQHandler(void) {
    auto pr = registers::exti1->pr.volatileLoad();
    if (pr.PR0) {
        externalInterrupt0Irq();
    }
    if (pr.PR1) {
        externalInterrupt1Irq();
    }
}

extern "C" void EXTI2_3_IRQHandler(void) {
    auto pr = registers::exti1->pr.volatileLoad();
    if (pr.PR2) {
        externalInterrupt2Irq();
    }
    if (pr.PR3) {
        externalInterrupt3Irq();
    }
}

extern "C" void EXTI4_15_IRQHandler(void) {
    auto pr = registers::exti1->pr.volatileLoad();
    if (pr.PR4) {
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
    externalInterrupt9_5Irq(registers::exti1->pr.volatileLoad());
}
extern "C" void EXTI15_10_IRQHandler(void) {
    externalInterrupt15_10Irq(registers::exti1->pr.volatileLoad());
}
#endif
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
