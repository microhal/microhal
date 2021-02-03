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
#include "ports/stm32f3xx/clockManager.h"

using namespace microhal;
using namespace stm32f3xx;
using namespace diagnostic;

extern "C" ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t nbyte) {
    return bsp::debugPort.write((const char *)buf, nbyte);
}

namespace bsp {
microhal::stm32f3xx::SPI_polling &spi1 = microhal::stm32f3xx::SPI_polling::create<1, {IOPin::PortA, 6}, {IOPin::PortA, 7}, {IOPin::PortA, 5}>();
microhal::SPI &wsSpi = spi1;
}  // namespace bsp

void hardwareConfig(void) {
    (void)bsp::wsSpi;
    Core::fpu_enable();
    ClockManager::PLL::clockSource(ClockManager::PLL::ClockSource::HSIDiv2);
    ClockManager::PLL::frequency(51200000);
    ClockManager::SYSCLK::source(ClockManager::SYSCLK::Source::PLL);
    while (ClockManager::SYSCLK::source() != ClockManager::SYSCLK::Source::PLL)
        ;

    IOManager::routeSerial<2, Txd, stm32f3xx::IOPin::PortA, 2>();
    IOManager::routeSerial<2, Rxd, stm32f3xx::IOPin::PortA, 3>();

    bsp::debugPort.setDataBits(stm32f3xx::SerialPort::Data8);
    bsp::debugPort.setStopBits(stm32f3xx::SerialPort::OneStop);
    bsp::debugPort.setParity(stm32f3xx::SerialPort::NoParity);
    bsp::debugPort.open(stm32f3xx::SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(stm32f3xx::SerialPort::Baud115200);
    diagChannel.setOutputDevice(bsp::debugPort);

    //    stm32f3xx::IOManager::routeSPI<1, SCK, stm32f3xx::GPIO::PortA, 5>();
    //    stm32f3xx::IOManager::routeSPI<1, MISO, stm32f3xx::GPIO::PortA, 6>();
    stm32f3xx::IOManager::routeSPI<1, MOSI, stm32f3xx::IOPin::PortA, 7>();

    bsp::spi1.init(stm32f3xx::SPI::Mode1, stm32f3xx::SPI::Prescaler8);
    bsp::spi1.enable();

    diagChannel << Notice << "SPI frequency: " << bsp::spi1.speed() << endl;

    SysTick_Config(512000000 / 1000);
}
uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
