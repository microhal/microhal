/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      GPIO port driver
 *
 * @authors    Pawel Okas
 * created on: 17-04-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014-2017, Pawel Okas
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

#include "gpio_stm32f4xx.h"
#include "clockManager.h"

namespace microhal {
namespace stm32f4xx {

void GPIO::pinInitialize(const Port port_, const uint_fast8_t pin, const PinConfiguration config) {
	volatile GPIO_TypeDef *port = reinterpret_cast<volatile GPIO_TypeDef *>(port_);

	ClockManager::enable(*reinterpret_cast<const GPIO_TypeDef *>(port_));

    uint32_t afr = port->AFR[pin / 8];
    afr &= ~(0b1111 << ((pin % 8) * 4)); // clear old configuration
    afr |= ((0xF0 & config.mode) >> 4) << ((pin % 8) * 4); // set new configuration
    port->AFR[pin / 8] = afr;

    // set mode -> config.mode is split to 2 part 4MSB bit
    //      contain alternate function and 4LSB bit contain mode
    uint32_t moder = port->MODER;
    moder &= ~((0b11) << (pin * 2));  // clear old configuration
    moder |= (0x03 & config.mode) << (pin * 2);  // set new configuration
    port->MODER = moder;
    // set type
    uint32_t otyper = port->OTYPER;
    otyper &= ~(0b1 << pin);  // clear old configuration
    otyper |= config.type << pin;  // set new configuration
    port->OTYPER = otyper;
    // set pullup
    uint32_t pupdr = port->PUPDR;
    pupdr &= ~(0b11 << (pin * 2));  // clear old configuration
    pupdr |= config.pull << (pin * 2);  // set new configuration
    port->PUPDR = pupdr;
    // set speed
    uint32_t ospeedr = port->OSPEEDR;
    ospeedr &= ~(0b11 << (pin*2));  // clear old configuration
    ospeedr |= config.speed << (pin * 2);  // set new configuration
    port->OSPEEDR = ospeedr;
}

}  // namespace stm32f4xx
}  // namespace microhal
