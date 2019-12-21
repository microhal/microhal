/*
 * spiClock.h
 *
 *  Created on: Nov 15, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_SPICLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_SPICLOCK_H_
#include <cstdint>
#include <exception>
#include <type_traits>
#include "apbClock.h"
#include "clockTypes.h"

#ifdef MCU_TYPE_STM32F0XX
#include "ports/stm32f0xx/RCC_2.h"
#endif
#ifdef MCU_TYPE_STM32F1XX
#include "ports/stm32f1xx/rcc_stm32f103.h"
#endif
#ifdef MCU_TYPE_STM32F3XX
#include "ports/stm32f3xx/rcc_3x4.h"
#endif
#ifdef MCU_TYPE_STM32F4XX
#ifdef STM32F411xE
#include "ports/stm32f4xx/rcc_411.h"
#else
#include "ports/stm32f4xx/rcc_407.h"
#endif
#endif

namespace microhal {
namespace ClockManager {
namespace ClockManagerDetail {
CREATE_SET_CLEAR_FUNCTION(SPI1EN)
CREATE_SET_CLEAR_FUNCTION(SPI2EN)
CREATE_SET_CLEAR_FUNCTION(SPI3EN)
CREATE_SET_CLEAR_FUNCTION(SPI4EN)
CREATE_SET_CLEAR_FUNCTION(SPI5EN)
CREATE_SET_CLEAR_FUNCTION(SPI6EN)

CREATE_SET_CLEAR_FUNCTION(SPI1LPEN)
CREATE_SET_CLEAR_FUNCTION(SPI2LPEN)
CREATE_SET_CLEAR_FUNCTION(SPI3LPEN)
CREATE_SET_CLEAR_FUNCTION(SPI4LPEN)
CREATE_SET_CLEAR_FUNCTION(SPI5LPEN)
CREATE_SET_CLEAR_FUNCTION(SPI6LPEN)
}  // namespace ClockManagerDetail

#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
[[maybe_unused]] static void enableSPI(uint8_t number, PowerMode mode) {
    if (number == 2 || number == 3) {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
        if (number == 2) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI2EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI2LPEN_ifExist(apb1lpenr);
        } else if (number == 3) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI3EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI3LPEN_ifExist(apb1lpenr);
        } else {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->apb1enr.volatileStore(apb1enr);
        registers::rcc->apb1lpenr.volatileStore(apb1lpenr);
    } else {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
        if (number == 1) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI1EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI1LPEN_ifExist(apb2lpenr);
        } else if (number == 4) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI4EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI4LPEN_ifExist(apb2lpenr);
        } else if (number == 5) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI5EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI5LPEN_ifExist(apb2lpenr);
        } else if (number == 6) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI6EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI6LPEN_ifExist(apb2lpenr);
        } else {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->apb2enr.volatileStore(apb2enr);
        registers::rcc->apb2lpenr.volatileStore(apb2lpenr);
    }
}

[[maybe_unused]] static void disableSPI(uint8_t number, PowerMode mode) {
    if (number == 2 || number == 3) {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        auto apb1lpenr = registers::rcc->apb1lpenr.volatileLoad();
        if (number == 2) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI2EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI2LPEN_ifExist(apb1lpenr);
        } else if (number == 3) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI3EN_ifExist(apb1enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI3LPEN_ifExist(apb1lpenr);
        } else {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->apb1enr.volatileStore(apb1enr);
        registers::rcc->apb1lpenr.volatileStore(apb1lpenr);
    } else {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        auto apb2lpenr = registers::rcc->apb2lpenr.volatileLoad();
        if (number == 1) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI1EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI1LPEN_ifExist(apb2lpenr);
        } else if (number == 4) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI4EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI4LPEN_ifExist(apb2lpenr);
        } else if (number == 5) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI5EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI5LPEN_ifExist(apb2lpenr);
        } else if (number == 6) {
            if (isEnabled(mode, PowerMode::Normal)) ClockManagerDetail::set_SPI6EN_ifExist(apb2enr);
            if (isEnabled(mode, PowerMode::Sleep)) ClockManagerDetail::set_SPI6LPEN_ifExist(apb2lpenr);
        } else {
            std::terminate();  // Error should newer go there
        }
        registers::rcc->apb2enr.volatileStore(apb2enr);
        registers::rcc->apb2lpenr.volatileStore(apb2lpenr);
    }
}
#else
[[maybe_unused]] static void enableSPI(uint8_t number) {
    if (number == 2 || number == 3) {
        auto apb1enr = registers::rcc->apb1enr.volatileLoad();
        switch (number) {
            case 2:
                ClockManagerDetail::set_SPI2EN_ifExist(apb1enr);
                break;
            case 3:
                ClockManagerDetail::set_SPI3EN_ifExist(apb1enr);
                break;
        }
        registers::rcc->apb1enr.volatileStore(apb1enr);
    } else {
        auto apb2enr = registers::rcc->apb2enr.volatileLoad();
        switch (number) {
            case 1:
                ClockManagerDetail::set_SPI1EN_ifExist(apb2enr);
                break;
            case 4:
                ClockManagerDetail::set_SPI4EN_ifExist(apb2enr);
                break;
            case 5:
                ClockManagerDetail::set_SPI5EN_ifExist(apb2enr);
                break;
            case 6:
                ClockManagerDetail::set_SPI6EN_ifExist(apb2enr);
                break;
        }
        registers::rcc->apb2enr.volatileStore(apb2enr);
    }
}
#endif

static uint32_t SPIFrequency(uint8_t number) {
    if (number == 2 || number == 3) {
        return APB1::frequency();
    }
    return APB2::frequency();
}
}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_SPICLOCK_H_ */
