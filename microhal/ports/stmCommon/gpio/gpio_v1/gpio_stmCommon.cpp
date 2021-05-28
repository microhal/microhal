/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      GPIO port driver
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2014-2020, Pawel Okas
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

#include <ports/stmCommon/gpio/gpio_v1/gpio_stmCommon.h>
#if _MICROHAL_PORT_STM_GPIO_DRIVER_VERSION == 1
#include <ports/stmCommon/clockManager/gpioClock.h>

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

void GPIO::pinInitialize(PinConfiguration config) {
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::enableGPIO(port.getGpioHandle(), ClockManager::PowerMode::Normal);
#else
    ClockManager::enableGPIO(port.getGpioHandle());
#endif

    auto afr = port.getGpioHandle().afr[pinNo / 8].volatileLoad();
    afr &= ~(0b1111 << ((pinNo % 8) * 4));                    // clear old configuration
    afr |= ((0xF0 & config.mode) >> 4) << ((pinNo % 8) * 4);  // set new configuration
    port.getGpioHandle().afr[pinNo / 8].volatileStore(afr);

    // set mode -> config.mode is split to 2 part 4MSB bit
    //      contain alternate function and 4LSB bit contain mode
    auto moder = port.getGpioHandle().moder.volatileLoad();
    moder &= ~((0b11) << (pinNo * 2));             // clear old configuration
    moder |= (0x03 & config.mode) << (pinNo * 2);  // set new configuration
    port.getGpioHandle().moder.volatileStore(moder);
    // set type
    setDirection(static_cast<Direction>(config.type));
    setPullType(static_cast<PullType>(config.pull));
    setSpeed(config.speed);
}

bool GPIO::getConfiguration(Direction &dir, OutputType &otype, PullType &pull) const {
    dir = static_cast<Direction>((port.getDirection() >> pinNo) & 0b1);
    pull = static_cast<PullType>((port.getPullConfig() >> (2 * pinNo)) & 0b11);
    return true;
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
#endif  // _MICROHAL_PORT_STM_GPIO_DRIVER_VERSION == 1
