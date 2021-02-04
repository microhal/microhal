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
#include "hsi.h"
#include "lse.h"
#include "rcc_register_select.h"

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

enum UsartClockSource {
    PCLK = 0,  // PCLK means APB clock
    SYSCLK = 1,
#ifdef MCU_TYPE_STM32G0XX
    HSI16 = 2,
    LSE = 3
#else
    LSE = 2,
    HSI = 3
#endif
};

#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
void enableUSART(uint8_t number, PowerMode mode);
void disableUSART(uint8_t number, PowerMode mode);

[[maybe_unused]] constexpr UsartClockSource USARTClockSource(uint8_t number) {
    return UsartClockSource::PCLK;
}
#else

void enableUSART(uint8_t number);
void disableUSART(uint8_t number);

#ifdef MCU_TYPE_STM32F1XX
[[maybe_unused]] constexpr UsartClockSource USARTClockSource([[maybe_unused]] uint8_t number) {
    return UsartClockSource::PCLK;
}
#else
#ifdef MCU_TYPE_STM32G0XX
[[maybe_unused]] inline UsartClockSource USARTClockSource(uint8_t number) {
    if (number == 1)
        return static_cast<UsartClockSource>(registers::rcc->ccipr.volatileLoad().USART1SEL.get());
    else if (number == 2)
        return static_cast<UsartClockSource>(registers::rcc->ccipr.volatileLoad().USART2SEL.get());

    else if (number == 3)
        return static_cast<UsartClockSource>(registers::rcc->ccipr.volatileLoad().USART3SEL.get());
    else {
        while (1)
            ;
    }
}

[[maybe_unused]] static void USARTClockSource(uint8_t usartNumber, UsartClockSource source) {
    auto ccipr = registers::rcc->ccipr.volatileLoad();
    if (usartNumber == 1)
        ccipr.USART1SEL = static_cast<uint32_t>(source);
    else if (usartNumber == 2) {
        ccipr.USART2SEL = static_cast<uint32_t>(source);
    } else if (usartNumber == 3) {
        ccipr.USART3SEL = static_cast<uint32_t>(source);
        if (source != UsartClockSource::PCLK) {
            while (1)
                ;
        }
    } else {
        while (1)
            ;
    }
}
#else
[[maybe_unused]] static UsartClockSource USARTClockSource(uint8_t number) {
    if (number == 1)
        return static_cast<UsartClockSource>(registers::rcc->cfgr3.volatileLoad().USART1SW.get());
    else if (number == 2)
        if constexpr (false) {
            // return static_cast<UsartClockSource>(registers::rcc->cfgr3.volatileLoad().USART2SW.get());
        } else {
            return UsartClockSource::PCLK;
        }
    else if (number == 3)
        // return static_cast<UsartClockSource>(registers::rcc->cfgr3.volatileLoad().USART3SW.get());
        return UsartClockSource::PCLK;
    else {
        while (1)
            ;
    }
}

[[maybe_unused]] static void USARTClockSource(uint8_t usartNumber, UsartClockSource source) {
    auto cfgr3 = registers::rcc->cfgr3.volatileLoad();
    if (usartNumber == 1)
        cfgr3.USART1SW = static_cast<uint32_t>(source);
    else if (usartNumber == 2) {
        // cfgr3.USART2SW = static_cast<uint32_t>(source);
        if (source != UsartClockSource::PCLK) {
            while (1)
                ;
        }
    } else if (usartNumber == 3) {
        // cfgr3.USART3SW = static_cast<uint32_t>(source);
        if (source != UsartClockSource::PCLK) {
            while (1)
                ;
        }
    } else {
        while (1)
            ;
    }
}
#endif  // MCU_TYPE_STM32G0XX
#endif
#endif

[[maybe_unused]] static uint32_t USARTFrequency(uint8_t number) {
    UsartClockSource usartClockSource = USARTClockSource(number);

    switch (usartClockSource) {
        case PCLK:
            if (number == 1 || number == 6) {
                return APB2::frequency();
            } else {
                return APB1::frequency();
            }
        case SYSCLK:
            return SYSCLK::frequency();
        case LSE:
            return LSE::frequency();
            break;
#ifdef MCU_TYPE_STM32G0XX
        case HSI16:
            return HSI::frequency();
            break;
#else
        case HSI:
            return HSI::frequency();
#endif
    }
    std::terminate();
}

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_USARTCLOCK_H_ */
