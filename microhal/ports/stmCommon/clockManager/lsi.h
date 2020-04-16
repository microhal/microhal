/*
 * lsiClock.h
 *
 *  Created on: Nov 24, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_LSI_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_LSI_H_

#include <cstdint>
#include "../stmCommonDefines.h"
#include "clockTypes.h"
#include _MICROHAL_INCLUDE_PORT_CONFIG

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
struct LSI {
    /**
     * @brief This function return LSI frequency.
     *
     * @return LSI frequency in [Hz].
     */
    static constexpr Frequency frequency() noexcept { return 32000; }

    static void enable() noexcept {
        auto csr = registers::rcc->csr.volatileLoad();
        csr.LSION.set();
        registers::rcc->csr.volatileStore(csr);
    }
    static void disable() noexcept {
        auto csr = registers::rcc->csr.volatileLoad();
        csr.LSION.clear();
        registers::rcc->csr.volatileStore(csr);
    }
    static bool isReady() noexcept { return registers::rcc->csr.volatileLoad().LSIRDY; }
    static bool isEnabled() noexcept { return registers::rcc->csr.volatileLoad().LSION; }
};

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_LSI_H_ */
