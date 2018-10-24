/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      board support package for nucleo-f334r8 board
 *
 * @authors    Pawel Okas
 * created on: 12-03-2017
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2017, Paweł Okas
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

using namespace microhal;
using namespace stm32f3xx;
using namespace diagnostic;

void hardwareConfig(void) {
    (void)bsp::ds2782::i2c;
    (void)bsp::debugPort;
    Core::fpu_enable();
    stm32f3xx::ClockManager::PLL::clockSource(stm32f3xx::ClockManager::PLL::ClockSource::HSIDiv2);
    stm32f3xx::ClockManager::PLL::frequency(51200000);
    stm32f3xx::ClockManager::SYSCLK::source(stm32f3xx::ClockManager::SYSCLK::Source::PLL);
    while (stm32f3xx::ClockManager::SYSCLK::source() != stm32f3xx::ClockManager::SYSCLK::Source::PLL)
        ;

    IOManager::routeSerial<2, Txd, stm32f3xx::IOPin::PortA, 2>();
    IOManager::routeSerial<2, Rxd, stm32f3xx::IOPin::PortA, 3>();

    IOManager::routeI2C<1, SDA, stm32f3xx::IOPin::PortB, 9>();
    IOManager::routeI2C<1, SCL, stm32f3xx::IOPin::PortB, 8>();

    stm32f3xx::I2C::i2c1.init();
    stm32f3xx::I2C::i2c1.speed(100000, microhal::I2C::Mode::Standard);
    stm32f3xx::I2C::i2c1.enable();

    SysTick_Config(512000000 / 1000);
}
uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
