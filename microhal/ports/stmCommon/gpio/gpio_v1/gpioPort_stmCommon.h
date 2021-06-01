/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      GPIO port driver for:
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2021, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_STMCOMMON_GPIOPORT_V1_H_
#define _MICROHAL_STMCOMMON_GPIOPORT_V1_H_

#include "../../registers/gpio_v1.h"
#include "../../stmCommonDefines.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif
/* **************************************************************************************************************************************************
 * CLASS
 */
namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

class GPIOPort {
 public:
    constexpr GPIOPort(microhal::registers::GPIO *gpio) : gpio(*gpio) {}

    uint16_t get() const { return (uint32_t)gpio.idr.volatileLoad(); }
    uint16_t getOdr() const { return gpio.odr.volatileLoad(); }
    void set(uint16_t value) { gpio.odr.volatileStore(value); }
    void setMask(uint16_t bitsToSet) { gpio.bsrr.volatileStore(bitsToSet); }
    void resetMask(uint16_t bitsToReset) {
#ifdef _MICROHAL_GPIO_REGISTER_HAS_BRR
        gpio.brr.volatileStore(bitsToReset);
#else
        gpio.bsrr.volatileStore(bitsToReset << 16);
#endif
    }
    void setResetMask(uint16_t values, uint16_t mask) {
        uint32_t toSet = values & mask;
        uint32_t toReset = (~values) & mask;
        registers::GPIO::BSRR bsrr;
        bsrr.BS = toSet;
        bsrr.BR = toReset;
        gpio.bsrr.volatileStore(bsrr);
    }

    uint32_t getMode() const { return (uint32_t)gpio.moder.volatileLoad(); }
    // void setDirection(uint16_t direction) { gpio.otyper.volatileStore(direction); }
    uint16_t getOutputType() const { return (uint32_t)gpio.otyper.volatileLoad(); }
    void setOutputType(uint16_t direction) { gpio.otyper.volatileStore(direction); }
    uint32_t getPullConfig() const { return gpio.pupdr.volatileLoad(); }
    void setPullConfig(uint32_t pullConfig) { gpio.pupdr.volatileStore(pullConfig); }
    uint32_t getSpeed() const { return gpio.ospeedr.volatileLoad(); }
    void setSpeed(uint32_t speed) { gpio.ospeedr.volatileStore(speed); }

    microhal::registers::GPIO &getGpioHandle() { return gpio; }

 private:
    microhal::registers::GPIO &gpio;
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* _MICROHAL_STMCOMMON_GPIOPORT_V1_H_ */
