/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      board support package for stm32f4Discovery board
 *
 * @authors    Pawel Okas
 * created on: 16-04-2014
 * last modification: <DD-MM-YYYY>
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
#include "bsp.h"
#include "microhal.h"

using namespace microhal;
using namespace stm32f4xx;

namespace bsp {
namespace detail {
using Port = microhal::stm32f4xx::GPIO::Port;

constexpr IOPin led3_pin(microhal::stm32f4xx::GPIO::Port::PortD, 13);
constexpr IOPin led4_pin(microhal::stm32f4xx::GPIO::Port::PortD, 12);
constexpr IOPin led5_pin(microhal::stm32f4xx::GPIO::Port::PortD, 14);
constexpr IOPin led6_pin(microhal::stm32f4xx::GPIO::Port::PortD, 15);

constexpr IOPin button_pin(microhal::stm32f4xx::GPIO::Port::PortA, 0);

stm32f4xx::GPIO greenLed(led4_pin, stm32f4xx::GPIO::Direction::Output);
stm32f4xx::GPIO redLed(led3_pin, stm32f4xx::GPIO::Direction::Output);
stm32f4xx::GPIO button(button_pin, stm32f4xx::GPIO::Direction::Input);
}  // namespace detail

microhal::GPIO& greenLed = detail::greenLed;
microhal::GPIO& redLed = detail::redLed;
microhal::GPIO& button = detail::button;

}  // namespace bsp

void hardwareConfig(void) {
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    SysTick_Config(168000000 / 1000);
}

extern "C" void preInit() {}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
