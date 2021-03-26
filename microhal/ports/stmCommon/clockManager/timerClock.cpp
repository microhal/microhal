/*
 * timerClock.cpp
 *
 *  Created on: Nov 15, 2019
 *      Author: pokas
 */

#include "timerClock.h"

namespace microhal {
namespace ClockManager {

#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1

#else

void enableTimer(uint8_t number) {
#ifdef _MICROHAL_REGISTERS_RCC_HAS_APBENR2
    auto apb1enr = registers::rcc->apbenr1.volatileLoad();
    auto apb2enr = registers::rcc->apbenr2.volatileLoad();
#else
    auto apb1enr = registers::rcc->apb1enr.volatileLoad();
    auto apb2enr = registers::rcc->apb2enr.volatileLoad();
#endif

    switch (number) {
#ifdef _MICROHAL_TIM1_BASE_ADDRESS
        case 1:
            apb2enr.TIM1EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM2_BASE_ADDRESS
        case 2:
            apb1enr.TIM2EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM3_BASE_ADDRESS
        case 3:
            apb1enr.TIM3EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM4_BASE_ADDRESS
        case 4:
            apb1enr.TIM4EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM5_BASE_ADDRESS
        case 5:
            apb1enr.TIM5EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM6_BASE_ADDRESS
        case 6:
            apb1enr.TIM6EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM7_BASE_ADDRESS
        case 7:
            apb1enr.TIM7EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM8_BASE_ADDRESS
        case 8:
            apb2enr.TIM8EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM9_BASE_ADDRESS
        case 9:
            apb2enr.TIM9EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM10_BASE_ADDRESS
        case 10:
            apb2enr.TIM10EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM11_BASE_ADDRESS
        case 11:
            apb2enr.TIM11EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM12_BASE_ADDRESS
        case 12:
            apb1enr.TIM12EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM13_BASE_ADDRESS
        case 13:
            apb1enr.TIM13EN.set();
            break;
#endif
#ifdef _MICROHAL_TIM14_BASE_ADDRESS
        case 14:
#ifdef _MICROHAL_REGISTERS_RCC_APB2ENR_HAS_TIM14
            apb2enr.TIM14EN.set();
#else
            apb1enr.TIM14EN.set();
#endif
            break;
#endif
    }

#ifdef _MICROHAL_REGISTERS_RCC_HAS_APBENR2
    registers::rcc->apbenr1.volatileStore(apb1enr);
    registers::rcc->apbenr2.volatileStore(apb2enr);
#else
    registers::rcc->apb1enr.volatileStore(apb1enr);
    registers::rcc->apb2enr.volatileStore(apb2enr);
#endif
}

#endif

}  // namespace ClockManager
}  // namespace microhal
