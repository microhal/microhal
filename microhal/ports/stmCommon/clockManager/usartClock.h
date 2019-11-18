/*
 * usartClock.h
 *
 *  Created on: Nov 15, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_USARTCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_USARTCLOCK_H_

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
CREATE_SET_CLEAR_FUNCTION(USART1EN)
CREATE_SET_CLEAR_FUNCTION(USART2EN)
CREATE_SET_CLEAR_FUNCTION(USART3EN)
CREATE_SET_CLEAR_FUNCTION(UART4EN)
CREATE_SET_CLEAR_FUNCTION(UART5EN)
CREATE_SET_CLEAR_FUNCTION(USART6EN)
CREATE_SET_CLEAR_FUNCTION(UART7EN)
CREATE_SET_CLEAR_FUNCTION(UART8EN)

CREATE_SET_CLEAR_FUNCTION(USART1LPEN)
CREATE_SET_CLEAR_FUNCTION(USART2LPEN)
CREATE_SET_CLEAR_FUNCTION(USART3LPEN)
CREATE_SET_CLEAR_FUNCTION(USART4LPEN)
CREATE_SET_CLEAR_FUNCTION(USART5LPEN)
CREATE_SET_CLEAR_FUNCTION(USART6LPEN)
CREATE_SET_CLEAR_FUNCTION(USART7LPEN)
CREATE_SET_CLEAR_FUNCTION(USART8LPEN)
}  // namespace ClockManagerDetail

#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
[[maybe_unused]] static void enableUSART(uint8_t number, PowerMode mode) {
    if (number == 1 || number == 6) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
        if (number == 1) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_USART1EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART1LPEN_ifExist(apb2lpenr);
        } else if (number == 6) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_USART6EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART6LPEN_ifExist(apb2lpenr);
        } else {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->ahb2lpenr.volatileStore(apb2lpenr);
        registers::rcc->apb2enr.volatileStore(apb2enr);
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
        if (number == 2) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_USART2EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART2LPEN_ifExist(apb1lpenr);
        } else if (number == 3) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_USART3EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART3LPEN_ifExist(apb1lpenr);
        } else if (number == 4) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_UART4EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART4LPEN_ifExist(apb1lpenr);
        } else if (number == 5) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_UART5EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART5LPEN_ifExist(apb1lpenr);
        } else if (number == 7) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_UART7EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART7LPEN_ifExist(apb1lpenr);
        } else if (number == 8) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_UART8EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART8LPEN_ifExist(apb1lpenr);
        } else {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->ahb2lpenr.volatileStore(apb1lpenr);
        registers::rcc->apb2enr.volatileStore(apb1enr);
    }
}

[[maybe_unused]] static void disableUSART(uint8_t number, PowerMode mode) {
    if (number == 1 || number == 6) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
        if (number == 1) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_USART1EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART1LPEN_ifExist(apb2lpenr);
        } else if (number == 6) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_USART6EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART6LPEN_ifExist(apb2lpenr);
        } else {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->ahb2lpenr.volatileStore(apb2lpenr);
        registers::rcc->apb2enr.volatileStore(apb2enr);
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
        if (number == 2) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_USART2EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART2LPEN_ifExist(apb1lpenr);
        } else if (number == 3) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_USART3EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART3LPEN_ifExist(apb1lpenr);
        } else if (number == 4) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_UART4EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART4LPEN_ifExist(apb1lpenr);
        } else if (number == 5) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_UART5EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART5LPEN_ifExist(apb1lpenr);
        } else if (number == 7) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_UART7EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART7LPEN_ifExist(apb1lpenr);
        } else if (number == 8) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_UART8EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART8LPEN_ifExist(apb1lpenr);
        } else {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->ahb2lpenr.volatileStore(apb1lpenr);
        registers::rcc->apb2enr.volatileStore(apb1enr);
    }
}
#else
[[maybe_unused]] static void enableUSART(uint8_t number) {
    if (number == 1) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        apb2enr.USART1EN.set();
        registers::rcc->apb2enr.volatileStore(apb2enr);
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        switch (number) {
            case 2:
                ClockManagerDetail::set_USART2EN_ifExist(apb1enr);
                break;
            case 3:
                ClockManagerDetail::set_USART3EN_ifExist(apb1enr);
                break;
            case 4:
                ClockManagerDetail::set_UART4EN_ifExist(apb1enr);
                break;
            case 5:
                ClockManagerDetail::set_UART5EN_ifExist(apb1enr);
                break;
            case 6:
                ClockManagerDetail::set_USART6EN_ifExist(apb1enr);
                break;
            default:
                std::terminate();
        }
        registers::rcc->apb1enr.volatileStore(apb1enr);
    }
}

[[maybe_unused]] static void disableUSART(uint8_t number) {
    if (number == 1) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        apb2enr.USART1EN.clear();
        registers::rcc->apb2enr.volatileStore(apb2enr);
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        switch (number) {
            case 2:
                ClockManagerDetail::clear_USART2EN_ifExist(apb1enr);
                break;
            case 3:
                ClockManagerDetail::clear_USART3EN_ifExist(apb1enr);
                break;
            case 4:
                ClockManagerDetail::clear_UART4EN_ifExist(apb1enr);
                break;
            case 5:
                ClockManagerDetail::clear_UART5EN_ifExist(apb1enr);
                break;
            case 6:
                ClockManagerDetail::clear_USART6EN_ifExist(apb1enr);
                break;
            default:
                std::terminate();
        }
        registers::rcc->apb1enr.volatileStore(apb1enr);
    }
}
#endif

[[maybe_unused]] static uint32_t USARTFrequency(uint8_t number) {
    if (number == 1) {
        return apb2Frequency();
    } else {
        return apb1Frequency();
    }
}

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_USARTCLOCK_H_ */
