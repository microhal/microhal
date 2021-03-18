/*
 * apbClock.h
 *
 *  Created on: Nov 15, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_APBCLOCK_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_APBCLOCK_H_

#include <cstdint>
#include <exception>
#include <type_traits>
#include "ahbClock.h"
#include "clockTypes.h"
#include "rcc_register_select.h"

namespace microhal {
namespace ClockManager {
namespace ClockManagerDetail {
CREATE_SET_CLEAR_FUNCTION(PPRE2)
}  // namespace ClockManagerDetail

struct APB1 {
    [[maybe_unused]] static uint32_t prescaler() {
        uint32_t ppre1 = registers::rcc->cfgr.volatileLoad().PPRE1;
        if (ppre1 & 0b100) {
            static constexpr const uint8_t dividers[] = {2, 4, 8, 16};
            return dividers[ppre1 & 0b11];
        }
        return 1;
    }

    [[maybe_unused]] static bool prescaler(uint_fast8_t prescaler) {
        auto cfgr = registers::rcc->cfgr.volatileLoad();
        switch (prescaler) {
            case 1:
                cfgr.PPRE1 = 0b000;
                break;
            case 2:
                cfgr.PPRE1 = 0b100;
                break;
            case 4:
                cfgr.PPRE1 = 0b101;
                break;
            case 8:
                cfgr.PPRE1 = 0b110;
                break;
            case 16:
                cfgr.PPRE1 = 0b111;
                break;
            default:
                return false;
        }
        registers::rcc->cfgr.volatileStore(cfgr);
        return true;
    }

    [[maybe_unused]] static uint32_t frequency() { return AHB::frequency() / prescaler(); }
};

#ifdef _MICROHAL_REGISTERS_RCC_CFGR_HAS_PPRE2

#define _MICROHAL_CLOCKMANAGER_HAS_APB2

struct APB2 {
    [[maybe_unused]] static uint32_t prescaler() {
        auto cfgr = registers::rcc->cfgr.volatileLoad();
        uint32_t ppre2 = ClockManagerDetail::get_PPRE2_ifExist(cfgr);
        if (ppre2 & 0b100) {
            static constexpr const uint8_t dividers[] = {2, 4, 8, 16};
            return dividers[ppre2 & 0b11];
        }

        return 1;
    }

    [[maybe_unused]] static bool prescaler(uint_fast8_t prescaler) {
        auto cfgr = registers::rcc->cfgr.volatileLoad();
        switch (prescaler) {
            case 1:
                cfgr.PPRE2 = 0b000;
                break;
            case 2:
                cfgr.PPRE2 = 0b100;
                break;
            case 4:
                cfgr.PPRE2 = 0b101;
                break;
            case 8:
                cfgr.PPRE2 = 0b110;
                break;
            case 16:
                cfgr.PPRE2 = 0b111;
                break;
            default:
                return false;
        }
        registers::rcc->cfgr.volatileStore(cfgr);
        return true;
    }

    [[maybe_unused]] static uint32_t frequency() { return AHB::frequency() / prescaler(); }
};
#endif  //  _MICROHAL_REGISTERS_RCC_CFGR_HAS_PPRE2

}  // namespace ClockManager
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_APBCLOCK_H_ */
