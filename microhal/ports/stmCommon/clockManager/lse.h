/*
 * lse.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_LSE_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_LSE_H_

#include <cstdint>

#include "../stmCommonDefines.h"
#include _MICROHAL_INCLUDE_PORT_CONFIG
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

struct LSE {
    static Frequency frequency() noexcept {
        if constexpr (externalLSEPresent == false) {
            std::terminate();
        }
        return externalLSEFrequency;
    }

    static void enable() noexcept {
        auto bdcr = registers::rcc->bdcr.volatileLoad();
        bdcr.LSEON.set();
        registers::rcc->bdcr.volatileStore(bdcr);
    }
    static void disable() noexcept {
        auto bdcr = registers::rcc->bdcr.volatileLoad();
        bdcr.LSEON.clear();
        registers::rcc->bdcr.volatileStore(bdcr);
    }
    static bool isReady() noexcept { return registers::rcc->bdcr.volatileLoad().LSERDY; }
    static bool isEnabled() noexcept { return registers::rcc->bdcr.volatileLoad().LSEON; }
    static void enableBypas() noexcept {
        auto bdcr = registers::rcc->bdcr.volatileLoad();
        bdcr.LSEBYP.set();
        registers::rcc->bdcr.volatileStore(bdcr);
    }
    static void disableBypas() noexcept {
        auto bdcr = registers::rcc->bdcr.volatileLoad();
        bdcr.LSEBYP.clear();
        registers::rcc->bdcr.volatileStore(bdcr);
    }
};

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_LSE_H_ */
