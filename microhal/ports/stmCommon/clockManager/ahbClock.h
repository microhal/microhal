/*
 * ahbClock.h
 *
 *  Created on: Nov 15, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_AHBCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_AHBCLOCK_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "rcc_register_select.h"
#include "sysclk.h"

namespace microhal {
namespace ClockManager {
namespace ClockManagerDetail {}

struct AHB {
    static uint32_t prescaler() {
        uint32_t hpre = registers::rcc->cfgr.volatileLoad().HPRE;
        if (hpre & 0b1000) {
            static constexpr const uint16_t dividers[] = {2, 4, 8, 16, 64, 128, 256, 512};
            return dividers[hpre & 0b111];
        }
        return 1;
    }
    static uint32_t frequency() { return SYSCLK::frequency() / prescaler(); }

    static bool prescaler(uint32_t prescaler) {
        auto cfgr = registers::rcc->cfgr.volatileLoad();
        switch (prescaler) {
            case 1:
                cfgr.HPRE = 0b0000;
                break;
            case 2:
                cfgr.HPRE = 0b1000;
                break;
            case 4:
                cfgr.HPRE = 0b1001;
                break;
            case 8:
                cfgr.HPRE = 0b1010;
                break;
            case 16:
                cfgr.HPRE = 0b1011;
                break;
            case 64:
                cfgr.HPRE = 0b1100;
                break;
            case 128:
                cfgr.HPRE = 0b1101;
                break;
            case 256:
                cfgr.HPRE = 0b1110;
                break;
            case 512:
                cfgr.HPRE = 0b1111;
                break;
            default:
                return false;
        }
        registers::rcc->cfgr.volatileStore(cfgr);
        return true;
    }
};

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_AHBCLOCK_H_ */
