/*
 * pwr.cpp
 *
 *  Created on: Mar 3, 2021
 *      Author: pokas
 */

#include "pwr.h"
#include <cassert>

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

void PWR::configureWakeup(WakeupPin pinNumber, Edge edge) {
    auto cr4 = registers::pwr->cr4.volatileLoad();
    cr4 |= static_cast<unsigned>(edge) << static_cast<unsigned>(pinNumber);
    registers::pwr->cr4.volatileStore(cr4);
}

void PWR::enableWakeup(WakeupPin pinNumber) {
    auto cr3 = registers::pwr->cr3.volatileLoad();
    cr3 |= 1 << static_cast<unsigned>(pinNumber);
    registers::pwr->cr3.volatileStore(cr3);
}

void PWR::disableWakeup(WakeupPin pinNumber) {
    auto cr3 = registers::pwr->cr3.volatileLoad();
    cr3 &= ~(1 << static_cast<unsigned>(pinNumber));
    registers::pwr->cr3.volatileStore(cr3);
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
