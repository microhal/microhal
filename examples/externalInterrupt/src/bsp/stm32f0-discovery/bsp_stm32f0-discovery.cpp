/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      board support package for stm32f4-Discovery board
 *
 * @authors    Pawel
 * created on: 16-04-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014, microHAL
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

namespace bsp {
namespace detail {

using namespace microhal::stm32f0xx;

constexpr IOPin led3_pin(microhal::stm32f0xx::GPIO::Port::PortC, 9);
constexpr IOPin led4_pin(microhal::stm32f0xx::GPIO::Port::PortC, 8);

constexpr IOPin led5_pin(microhal::stm32f0xx::GPIO::Port::PortC, 10);
constexpr IOPin led6_pin(microhal::stm32f0xx::GPIO::Port::PortC, 11);

GPIO redLed(led3_pin, GPIO::Direction::Output);
GPIO greenLed(led4_pin, GPIO::Direction::Output);
GPIO blueLed(led5_pin, GPIO::Direction::Output);
GPIO orangeLed(led6_pin, GPIO::Direction::Output);

}  // namespace detail

microhal::GPIO &redLed = detail::redLed;
microhal::GPIO &greenLed = detail::greenLed;
microhal::GPIO &blueLed = detail::blueLed;
microhal::GPIO &orangeLed = detail::orangeLed;
}  // namespace bsp

void hardwareConfig(void) {}
