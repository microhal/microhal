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
#include "rcc_register_select.h"

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
