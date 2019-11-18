/*
 * timerClock.h
 *
 *  Created on: Nov 15, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_TIMERCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_TIMERCLOCK_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "apbClock.h"
#include "clockTypes.h"

#ifdef MCU_TYPE_STM32F1XX
#include "ports/stm32f1xx/rcc_stm32f103.h"
#endif
#ifdef MCU_TYPE_STM32F3XX
#include "ports/stm32f3xx/rcc_3x4.h"
#endif
#ifdef MCU_TYPE_STM32F4XX
#include "ports/stm32f4xx/rcc_411.h"
#endif

namespace microhal {
namespace ClockManager {
namespace ClockManagerDetail {
CREATE_SET_CLEAR_FUNCTION(TIM1EN)
CREATE_SET_CLEAR_FUNCTION(TIM2EN)
CREATE_SET_CLEAR_FUNCTION(TIM3EN)
CREATE_SET_CLEAR_FUNCTION(TIM4EN)
CREATE_SET_CLEAR_FUNCTION(TIM5EN)
CREATE_SET_CLEAR_FUNCTION(TIM6EN)
CREATE_SET_CLEAR_FUNCTION(TIM7EN)
CREATE_SET_CLEAR_FUNCTION(TIM8EN)
CREATE_SET_CLEAR_FUNCTION(TIM9EN)
CREATE_SET_CLEAR_FUNCTION(TIM10EN)
CREATE_SET_CLEAR_FUNCTION(TIM11EN)
CREATE_SET_CLEAR_FUNCTION(TIM12EN)
CREATE_SET_CLEAR_FUNCTION(TIM13EN)
CREATE_SET_CLEAR_FUNCTION(TIM14EN)

CREATE_SET_CLEAR_FUNCTION(TIM1LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM2LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM3LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM4LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM5LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM6LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM7LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM8LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM9LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM10LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM11LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM12LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM13LPEN)
CREATE_SET_CLEAR_FUNCTION(TIM14LPEN)
}  // namespace ClockManagerDetail

constexpr uint8_t getTimerApb(int8_t timerNumber) {
    if (timerNumber == 1 || (timerNumber >= 8 && timerNumber <= 11)) return 2;
    return 1;
}

#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
static void enableTimer(uint8_t timerNumber, PowerMode mode) {
    if (timerNumber == 1 || (timerNumber >= 8 && timerNumber <= 11)) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
        if (timerNumber == 1) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM1EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM1LPEN_ifExist(apb2lpenr);
        } else if (timerNumber == 8) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM8EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM8LPEN_ifExist(apb2lpenr);
        } else if (timerNumber == 9) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM9EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM9LPEN_ifExist(apb2lpenr);
        } else if (timerNumber == 10) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM10EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM10LPEN_ifExist(apb2lpenr);
        } else if (timerNumber == 11) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM11EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM11LPEN_ifExist(apb2lpenr);
        }
        registers::rcc->apb2lpenr.volatileStore(apb2lpenr);
        registers::rcc->apb2enr.volatileStore(apb2enr);
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
        if (timerNumber == 2) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM2EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM2LPEN_ifExist(apb1enr);
        } else if (timerNumber == 3) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM3EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM3LPEN_ifExist(apb1enr);
        } else if (timerNumber == 4) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM4EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM4LPEN_ifExist(apb1enr);
        } else if (timerNumber == 5) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM5EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM5LPEN_ifExist(apb1enr);
        } else if (timerNumber == 6) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM6EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM6LPEN_ifExist(apb1enr);
        } else if (timerNumber == 7) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM7EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM7LPEN_ifExist(apb1enr);
        } else if (timerNumber == 12) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM12EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM12LPEN_ifExist(apb1enr);
        } else if (timerNumber == 13) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM13EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM13LPEN_ifExist(apb1enr);
        } else if (timerNumber == 14) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_TIM14EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_TIM14LPEN_ifExist(apb1enr);
        } else {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->apb1lpenr.volatileStore(apb1lpenr);
        registers::rcc->apb1enr.volatileStore(apb1enr);
    }
}

static void disableTimer(uint8_t timerNumber, PowerMode mode) {
    if (timerNumber == 1 || (timerNumber >= 8 && timerNumber <= 11)) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
        if (timerNumber == 1) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM1EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM1LPEN_ifExist(apb2lpenr);
        } else if (timerNumber == 8) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM8EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM8LPEN_ifExist(apb2lpenr);
        } else if (timerNumber == 9) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM9EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM9LPEN_ifExist(apb2lpenr);
        } else if (timerNumber == 10) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM10EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM10LPEN_ifExist(apb2lpenr);
        } else if (timerNumber == 11) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM11EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM11LPEN_ifExist(apb2lpenr);
        }
        registers::rcc->apb2lpenr.volatileStore(apb2lpenr);
        registers::rcc->apb2enr.volatileStore(apb2enr);
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
        if (timerNumber == 2) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM2EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM2LPEN_ifExist(apb1enr);
        } else if (timerNumber == 3) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM3EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM3LPEN_ifExist(apb1enr);
        } else if (timerNumber == 4) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM4EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM4LPEN_ifExist(apb1enr);
        } else if (timerNumber == 5) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM5EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM5LPEN_ifExist(apb1enr);
        } else if (timerNumber == 6) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM6EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM6LPEN_ifExist(apb1enr);
        } else if (timerNumber == 7) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM7EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM7LPEN_ifExist(apb1enr);
        } else if (timerNumber == 12) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM12EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM12LPEN_ifExist(apb1enr);
        } else if (timerNumber == 13) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM13EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM13LPEN_ifExist(apb1enr);
        } else if (timerNumber == 14) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_TIM14EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_TIM14LPEN_ifExist(apb1enr);
        } else {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->apb1lpenr.volatileStore(apb1lpenr);
        registers::rcc->apb1enr.volatileStore(apb1enr);
    }
}
#else
[[maybe_unused]] static void enableTimer(uint8_t number) {
    if (getTimerApb(number) == 2) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        switch (number) {
            case 1:
                ClockManagerDetail::set_TIM1EN_ifExist(apb2enr);  // equal to apb2enr.TIM1EN.set();
                break;
            case 8:
                ClockManagerDetail::set_TIM8EN_ifExist(apb2enr);
                break;
            case 9:
                ClockManagerDetail::set_TIM9EN_ifExist(apb2enr);
                break;
            case 10:
                ClockManagerDetail::set_TIM10EN_ifExist(apb2enr);
                break;
            case 11:
                ClockManagerDetail::set_TIM11EN_ifExist(apb2enr);
                break;
        }
        registers::rcc->apb2enr.volatileStore(apb2enr);
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        switch (number) {
            case 2:
                ClockManagerDetail::set_TIM2EN_ifExist(apb1enr);
                break;
            case 3:
                ClockManagerDetail::set_TIM3EN_ifExist(apb1enr);
                break;
            case 4:
                ClockManagerDetail::set_TIM4EN_ifExist(apb1enr);
                break;
            case 5:
                ClockManagerDetail::set_TIM5EN_ifExist(apb1enr);
                break;
            case 6:
                ClockManagerDetail::set_TIM6EN_ifExist(apb1enr);
                break;
            case 7:
                ClockManagerDetail::set_TIM7EN_ifExist(apb1enr);
                break;
            case 12:
                ClockManagerDetail::set_TIM12EN_ifExist(apb1enr);
                break;
            case 13:
                ClockManagerDetail::set_TIM13EN_ifExist(apb1enr);
                break;
            case 14:
                ClockManagerDetail::set_TIM14EN_ifExist(apb1enr);
                break;
        }
        registers::rcc->apb1enr.volatileStore(apb1enr);
    }
}

[[maybe_unused]] static void disableTimer(uint8_t number) {
    if (getTimerApb(number) == 2) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        switch (number) {
            case 1:
                ClockManagerDetail::clear_TIM1EN_ifExist(apb2enr);
                break;
            case 8:
                ClockManagerDetail::clear_TIM8EN_ifExist(apb2enr);
                break;
            case 9:
                ClockManagerDetail::clear_TIM9EN_ifExist(apb2enr);
                break;
            case 10:
                ClockManagerDetail::clear_TIM10EN_ifExist(apb2enr);
                break;
            case 11:
                ClockManagerDetail::clear_TIM11EN_ifExist(apb2enr);
                break;
        }
        registers::rcc->apb2enr.volatileStore(apb2enr);
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        switch (number) {
            case 2:
                ClockManagerDetail::clear_TIM2EN_ifExist(apb1enr);
                break;
            case 3:
                ClockManagerDetail::clear_TIM3EN_ifExist(apb1enr);
                break;
            case 4:
                ClockManagerDetail::clear_TIM4EN_ifExist(apb1enr);
                break;
            case 5:
                ClockManagerDetail::clear_TIM5EN_ifExist(apb1enr);
                break;
            case 6:
                ClockManagerDetail::clear_TIM6EN_ifExist(apb1enr);
                break;
            case 7:
                ClockManagerDetail::clear_TIM7EN_ifExist(apb1enr);
                break;
            case 12:
                ClockManagerDetail::clear_TIM12EN_ifExist(apb1enr);
                break;
            case 13:
                ClockManagerDetail::clear_TIM13EN_ifExist(apb1enr);
                break;
            case 14:
                ClockManagerDetail::clear_TIM14EN_ifExist(apb1enr);
                break;
        }
        registers::rcc->apb1enr.volatileStore(apb1enr);
    }
}
#endif

[[maybe_unused]] static uint32_t TimerFrequency(uint8_t number) {
    if (number == 1 || (number >= 8 && number <= 11)) {
        return apb2Frequency();
    }
    return apb1Frequency();
}

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_TIMERCLOCK_H_ */
