/**
 * @file
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      board support package for nucleo-g071rb board
 *
 * @authors    Pawel Okas
 * created on: 02-02-2021
 *
 * @copyright Copyright (c) 2021, Pawel Okas
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
using namespace stm32g0xx;

extern "C" ssize_t _write_r([[maybe_unused]] struct _reent *r, [[maybe_unused]] int file, const void *buf, size_t nbyte) {
    return bsp::debugPort.write((const char *)buf, nbyte);
}

namespace bsp {

void init() {
    IOManager::routeSerial<2, Txd, IOPin{IOPin::PortA, 2}>();
    IOManager::routeSerial<2, Rxd, IOPin{IOPin::PortA, 3}>();

    IOManager::routeI2C<1, SDA, IOPin{IOPin::PortB, 9}>();
    IOManager::routeI2C<1, SCL, IOPin{IOPin::PortB, 8}>();

    bsp::debugPort.open(IODevice::ReadWrite);
    bsp::debugPort.setBaudRate(stm32g0xx::SerialPort::Baud115200);
    bsp::debugPort.setDataBits(stm32g0xx::SerialPort::Data8);
    bsp::debugPort.setStopBits(stm32g0xx::SerialPort::OneStop);
    bsp::debugPort.setParity(stm32g0xx::SerialPort::NoParity);

    stm32g0xx::I2C::i2c1.init();
    stm32g0xx::I2C::i2c1.speed(400000, microhal::I2C::Mode::Fast);
    stm32g0xx::I2C::i2c1.enable();
}

}  // namespace bsp

void hardwareConfig(void) {
    // SysTick_Config(8000000 / 1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
