/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      GPIO port driver
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2014-2021, Pawel Okas
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

#include "ports/stmCommon/gpio/gpio_v2/gpio_stmCommon.h"
#if _MICROHAL_PORT_STM_GPIO_DRIVER_VERSION == 2

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

bool GPIO::configure(microhal::GPIO::Direction dir, microhal::GPIO::OutputType type, microhal::GPIO::PullType pull) {
    port.enableClock();

    if (dir == Direction::Input) {
        uint8_t cnf = pull == PullType::NoPull ? 0b0100 : 0b1000;
        port.configurePin(pinNo, cnf);
        // set pullup
        if (pull == PullType::PullUp) {
            port.setMask(pinMask);
        } else {
            port.resetMask(pinMask);
        }
    } else {
        uint8_t cnf;
        switch (type) {
            case OutputType::OpenDrain:
                cnf = 0b0100 | MediumSpeed;
                break;
            case OutputType::PushPull:
                cnf = 0b0000 | MediumSpeed;
                break;
            default:
                return false;
        }
        port.configurePin(pinNo, cnf);
    }
    return true;
}

bool GPIO::getConfiguration(Direction &dir, OutputType &otype, PullType &pull) const {
    auto config = port.getPinConfiguration(pinNo);
    if (config & 0b11) {
        dir = Direction::Output;
        if (config & 0b0100)
            otype = OutputType::OpenDrain;
        else {
            otype = OutputType::PushPull;
        }
    } else {
        dir = Direction::Input;
        if ((config & 0b1100) == 0b1000) {
            if (port.getOdr() & pinMask) {
                pull = PullType::PullUp;
            } else {
                pull = PullType::PullDown;
            }
        } else {
            pull = PullType::NoPull;
        }
    }
    return true;
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
#endif  //_MICROHAL_PORT_STM_GPIO_DRIVER_VERSION == 2
