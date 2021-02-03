/*
 * usartClock.cpp
 *
 *  Created on: Dec 15, 2020
 *      Author: pokas
 */
#include "usartClock.h"

namespace microhal {
namespace ClockManager {

#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
void enableUSART(uint8_t number, PowerMode mode) {
    if (number == 1 || number == 6) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
#if defined(_MICROHAL_USART1_BASE_ADDRESS) || defined(_MICROHAL_UART1_BASE_ADDRESS)
        if (number == 1) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_USART1EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART1LPEN_ifExist(apb2lpenr);
        } else
#endif
#if defined(_MICROHAL_USART6_BASE_ADDRESS) || defined(_MICROHAL_UART6_BASE_ADDRESS)
            if (number == 6) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_USART6EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART6LPEN_ifExist(apb2lpenr);
        } else
#endif
        {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->ahb2lpenr.volatileStore(apb2lpenr);
        registers::rcc->apb2enr.volatileStore(apb2enr);
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
#if defined(_MICROHAL_USART2_BASE_ADDRESS) || defined(_MICROHAL_UART2_BASE_ADDRESS)
        if (number == 2) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_USART2EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART2LPEN_ifExist(apb1lpenr);
        } else
#endif
#if defined(_MICROHAL_USART3_BASE_ADDRESS) || defined(_MICROHAL_UART3_BASE_ADDRESS)
            if (number == 3) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_USART3EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART3LPEN_ifExist(apb1lpenr);
        } else
#endif
#if defined(_MICROHAL_USART4_BASE_ADDRESS) || defined(_MICROHAL_UART4_BASE_ADDRESS)
            if (number == 4) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_UART4EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART4LPEN_ifExist(apb1lpenr);
        } else
#endif
#if defined(_MICROHAL_USART5_BASE_ADDRESS) || defined(_MICROHAL_UART5_BASE_ADDRESS)
            if (number == 5) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_UART5EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART5LPEN_ifExist(apb1lpenr);
        } else
#endif
#if defined(_MICROHAL_USART7_BASE_ADDRESS) || defined(_MICROHAL_UART7_BASE_ADDRESS)
            if (number == 7) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_UART7EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART7LPEN_ifExist(apb1lpenr);
        } else
#endif
#if defined(_MICROHAL_USART8_BASE_ADDRESS) || defined(_MICROHAL_UART8_BASE_ADDRESS)
            if (number == 8) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_UART8EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_USART8LPEN_ifExist(apb1lpenr);
        } else
#endif
        {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->ahb1lpenr.volatileStore(apb1lpenr);
        registers::rcc->apb1enr.volatileStore(apb1enr);
    }
}

void disableUSART(uint8_t number, PowerMode mode) {
    if (number == 1 || number == 6) {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
#if defined(_MICROHAL_USART1_BASE_ADDRESS) || defined(_MICROHAL_UART1_BASE_ADDRESS)
        if (number == 1) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_USART1EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART1LPEN_ifExist(apb2lpenr);
        } else
#endif
#if defined(_MICROHAL_USART6_BASE_ADDRESS) || defined(_MICROHAL_UART6_BASE_ADDRESS)
            if (number == 6) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_USART6EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART6LPEN_ifExist(apb2lpenr);
        } else
#endif
        {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->ahb2lpenr.volatileStore(apb2lpenr);
        registers::rcc->apb2enr.volatileStore(apb2enr);
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
#if defined(_MICROHAL_USART2_BASE_ADDRESS) || defined(_MICROHAL_UART2_BASE_ADDRESS)
        if (number == 2) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_USART2EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART2LPEN_ifExist(apb1lpenr);
        } else
#endif
#if defined(_MICROHAL_USART3_BASE_ADDRESS) || defined(_MICROHAL_UART3_BASE_ADDRESS)
            if (number == 3) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_USART3EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART3LPEN_ifExist(apb1lpenr);
        } else
#endif
#if defined(_MICROHAL_USART4_BASE_ADDRESS) || defined(_MICROHAL_UART4_BASE_ADDRESS)
            if (number == 4) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_UART4EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART4LPEN_ifExist(apb1lpenr);
        } else
#endif
#if defined(_MICROHAL_USART5_BASE_ADDRESS) || defined(_MICROHAL_UART5_BASE_ADDRESS)
            if (number == 5) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_UART5EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART5LPEN_ifExist(apb1lpenr);
        } else
#endif
#if defined(_MICROHAL_USART7_BASE_ADDRESS) || defined(_MICROHAL_UART7_BASE_ADDRESS)
            if (number == 7) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_UART7EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART7LPEN_ifExist(apb1lpenr);
        } else
#endif
#if defined(_MICROHAL_USART8_BASE_ADDRESS) || defined(_MICROHAL_UART8_BASE_ADDRESS)
            if (number == 8) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::clear_UART8EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::clear_USART8LPEN_ifExist(apb1lpenr);
        } else
#endif
        {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->ahb1lpenr.volatileStore(apb1lpenr);
        registers::rcc->apb1enr.volatileStore(apb1enr);
    }
}

#else

void enableUSART(uint8_t number) {
    if (number == 1) {
#ifdef _MICROHAL_USART1_BASE_ADDRESS
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        apb2enr.USART1EN.set();
        registers::rcc->apb2enr.volatileStore(apb2enr);
#else
        std::terminate();
#endif
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        switch (number) {
#ifdef _MICROHAL_USART2_BASE_ADDRESS
            case 2:
                apb1enr.USART2EN.set();
                break;
#endif
#ifdef _MICROHAL_USART3_BASE_ADDRESS
            case 3:
                apb1enr.USART3EN.set();
                break;
#endif
#ifdef _MICROHAL_USART4_BASE_ADDRESS
            case 4:
                apb1enr.USART4EN.set();
                break;
#endif
#ifdef _MICROHAL_UART4_BASE_ADDRESS
            case 4:
                apb1enr.UART4EN.set();
                break;
#endif
#ifdef _MICROHAL_USART5_BASE_ADDRESS
            case 5:
                apb1enr.USART5EN.set();
                break;
#endif
#ifdef _MICROHAL_UART5_BASE_ADDRESS
            case 5:
                apb1enr.UART5EN.set();
                break;
#endif
#ifdef _MICROHAL_USART6_BASE_ADDRESS
            case 6:
                apb1enr.USART6EN.set();
                break;
#endif
            default:
                std::terminate();
        }
        registers::rcc->apb1enr.volatileStore(apb1enr);
    }
}

void disableUSART(uint8_t number) {
    if (number == 1) {
#ifdef _MICROHAL_USART1_BASE_ADDRESS
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        apb2enr.USART1EN.clear();
        registers::rcc->apb2enr.volatileStore(apb2enr);
#else
        std::terminate();
#endif
    } else {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        switch (number) {
#ifdef _MICROHAL_USART2_BASE_ADDRESS
            case 2:
                apb1enr.USART2EN.clear();
                break;
#endif
#ifdef _MICROHAL_USART3_BASE_ADDRESS
            case 3:
                apb1enr.USART3EN.clear();
                break;
#endif
#ifdef _MICROHAL_USART4_BASE_ADDRESS
            case 4:
                apb1enr.USART4EN.clear();
                break;
#endif
#ifdef _MICROHAL_UART4_BASE_ADDRESS
            case 4:
                apb1enr.UART4EN.clear();
                break;
#endif
#ifdef _MICROHAL_USART5_BASE_ADDRESS
            case 5:
                apb1enr.USART5EN.clear();
                break;
#endif
#ifdef _MICROHAL_UART5_BASE_ADDRESS
            case 5:
                apb1enr.UART5EN.clear();
                break;
#endif
#ifdef _MICROHAL_USART6_BASE_ADDRESS
            case 6:
                apb1enr.USART6EN.clear();
                break;
#endif
            default:
                std::terminate();
        }
        registers::rcc->apb1enr.volatileStore(apb1enr);
    }
}
#endif

}  // namespace ClockManager
}  // namespace microhal
