/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      board support package for nucleo-f411re board
 *
 * @authors    Pawel Okas
 * created on: 18-11-2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, Paweł Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 	   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * 	      documentation and/or other materials provided with the distribution.
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

using namespace microhal::stm32f4xx;

constexpr IOPin ld2_pin(microhal::stm32f4xx::GPIO::Port::PortA, 5);  // green LED
// constexpr IOPin led2_pin(microhal::stm32f3xx::GPIO::Port::PortF, 6);   // LED on nucleo expansion board
// constexpr IOPin led3_pin(microhal::stm32f3xx::GPIO::Port::PortF, 7);   // LED on nucleo expansion board
constexpr IOPin led4_pin(microhal::stm32f4xx::GPIO::Port::PortA, 13);  // LED on nucleo expansion board
constexpr IOPin led5_pin(microhal::stm32f4xx::GPIO::Port::PortA, 14);  // LED on nucleo expansion board
constexpr IOPin led7_pin(microhal::stm32f4xx::GPIO::Port::PortA, 15);

GPIO redLed(led5_pin, GPIO::Direction::Output);
GPIO greenLed(ld2_pin, GPIO::Direction::Output);
GPIO blueLed(led4_pin, GPIO::Direction::Output);
GPIO orangeLed(led7_pin, GPIO::Direction::Input);

}  // namespace detail

microhal::GPIO &redLed = detail::redLed;
microhal::GPIO &greenLed = detail::greenLed;
microhal::GPIO &blueLed = detail::blueLed;
microhal::GPIO &orangeLed = detail::orangeLed;
}  // namespace bsp

void hardwareConfig(void) {
    microhal::stm32f4xx::Core::fpu_enable();

    SysTick_Config(16000000 / 1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
