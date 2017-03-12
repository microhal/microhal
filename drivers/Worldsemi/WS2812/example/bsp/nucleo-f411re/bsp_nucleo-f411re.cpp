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

#include "microhal.h"
#include "bsp.h"

using namespace microhal;
using namespace stm32f4xx;
using namespace diagnostic;

void hardwareConfig(void) {
	stm32f4xx::ClockManager::PLL::clockSource(stm32f4xx::ClockManager::PLL::ClockSource::HSI);
    Core::pll_start(8000000, 102400000);
    Core::fpu_enable();

    IOManager::routeSerial<2, Txd, stm32f4xx::GPIO::PortA, 2>();
    IOManager::routeSerial<2, Rxd, stm32f4xx::GPIO::PortA, 3>();

    bsp::debugPort.setDataBits(stm32f4xx::SerialPort::Data8);
    bsp::debugPort.setStopBits(stm32f4xx::SerialPort::OneStop);
    bsp::debugPort.setParity(stm32f4xx::SerialPort::NoParity);
    bsp::debugPort.open(stm32f4xx::SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(stm32f4xx::SerialPort::Baud115200);
    diagChannel.setOutputDevice(bsp::debugPort);

//    stm32f4xx::IOManager::routeSPI<1, SCK, stm32f4xx::GPIO::PortA, 5>();
//    stm32f4xx::IOManager::routeSPI<1, MISO, stm32f4xx::GPIO::PortA, 6>();
    stm32f4xx::IOManager::routeSPI<1, MOSI, stm32f4xx::GPIO::PortA, 7>();

    stm32f4xx::SPI::spi1.init(stm32f4xx::SPI::Mode1, stm32f4xx::SPI::PRESCALER_8);
    stm32f4xx::SPI::spi1.enable();

    diagChannel << Notice << "SPI frequency: " << stm32f4xx::SPI::spi1.frequency() << endl;

    SysTick_Config(102400000/1000);
}
uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void)
{
	SysTick_time++;
}
