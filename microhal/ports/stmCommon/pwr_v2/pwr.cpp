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

void PWR::clearWakeupFlag(WakeupFlag flag) {
    auto scr = registers::pwr->scr.volatileLoad();
    scr |= static_cast<unsigned>(flag);
    registers::pwr->scr.volatileStore(scr);
}

static void configuerPinPullUpDown(registers::PWR::PUCRx &pucr, registers::PWR::PDCRx &pdcr, IOPin pin, PWR::Pull pull) {
    switch (pull) {
        case PWR::Pull::None:
            pdcr &= ~(1 << pin.pin);
            pucr &= ~(1 << pin.pin);
            break;
        case PWR::Pull::Up:
            pdcr &= ~(1 << pin.pin);
            pucr |= 1 << pin.pin;
            break;
        case PWR::Pull::Down:
            pdcr |= 1 << pin.pin;
            pucr &= ~(1 << pin.pin);
            break;
    }
}

void PWR::configurePin(IOPin pin, Pull pull) {
    if (pin.port == IOPin::PortA) {
        auto pdcr = registers::pwr->pdcra.volatileLoad();
        auto pucr = registers::pwr->pucra.volatileLoad();
        configuerPinPullUpDown(pucr, pdcr, pin, pull);
        registers::pwr->pdcra.volatileStore(pdcr);
        registers::pwr->pucra.volatileStore(pucr);
    } else if (pin.port == IOPin::PortB) {
        auto pdcr = registers::pwr->pdcrb.volatileLoad();
        auto pucr = registers::pwr->pucrb.volatileLoad();
        configuerPinPullUpDown(pucr, pdcr, pin, pull);
        registers::pwr->pdcrb.volatileStore(pdcr);
        registers::pwr->pucrb.volatileStore(pucr);
    } else if (pin.port == IOPin::PortC) {
        auto pdcr = registers::pwr->pdcrc.volatileLoad();
        auto pucr = registers::pwr->pucrc.volatileLoad();
        configuerPinPullUpDown(pucr, pdcr, pin, pull);
        registers::pwr->pdcrc.volatileStore(pdcr);
        registers::pwr->pucrc.volatileStore(pucr);
    } else if (pin.port == IOPin::PortD) {
        auto pdcr = registers::pwr->pdcrd.volatileLoad();
        auto pucr = registers::pwr->pucrd.volatileLoad();
        configuerPinPullUpDown(pucr, pdcr, pin, pull);
        registers::pwr->pdcrd.volatileStore(pdcr);
        registers::pwr->pucrd.volatileStore(pucr);
    }
#if defined(_MICROHAL_GPIOE_BASE_ADDRESS)
    else if (pin.port == IOPin::PortE) {
        auto pdcr = registers::pwr->pdcre.volatileLoad();
        auto pucr = registers::pwr->pucre.volatileLoad();
        configuerPinPullUpDown(pucr, pdcr, pin, pull);
        registers::pwr->pdcre.volatileStore(pdcr);
        registers::pwr->pucre.volatileStore(pucr);
    }
#endif
#if defined(_MICROHAL_GPIOF_BASE_ADDRESS)
    else if (pin.port == IOPin::PortF) {
        auto pdcr = registers::pwr->pdcrf.volatileLoad();
        auto pucr = registers::pwr->pucrf.volatileLoad();
        configuerPinPullUpDown(pucr, pdcr, pin, pull);
        registers::pwr->pdcrf.volatileStore(pdcr);
        registers::pwr->pucrf.volatileStore(pucr);
    }
#endif
}

void PWR::enablePullConfiguration() {
    auto cr3 = registers::pwr->cr3.volatileLoad();
    cr3.APC.set();
    registers::pwr->cr3.volatileStore(cr3);
}

void PWR::disablePullConfiguration() {
    auto cr3 = registers::pwr->cr3.volatileLoad();
    cr3.APC.clear();
    registers::pwr->cr3.volatileStore(cr3);
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
