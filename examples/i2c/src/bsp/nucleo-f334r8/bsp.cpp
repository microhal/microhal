/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      board support package for stm32f4Discovery board

 @authors    Pawel Okas
 created on: 16-04-2014
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 	 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 	 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 	 	in the documentation and/or other materials provided with the distribution.
 	 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 	 	from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */ /* ==========================================================================================================================
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           */

#include "bsp.h"
#include "microhal.h"

using namespace microhal;
using namespace stm32f3xx;

constexpr microhal::IOPin ld2_pin(microhal::stm32f3xx::GPIO::Port::PortA, 5);
constexpr microhal::IOPin led3_pin(microhal::stm32f3xx::GPIO::Port::PortD, 13);
constexpr microhal::IOPin led5_pin(microhal::stm32f3xx::GPIO::Port::PortD, 14);
constexpr microhal::IOPin led6_pin(microhal::stm32f3xx::GPIO::Port::PortD, 15);

constexpr microhal::IOPin button_pin(microhal::stm32f3xx::GPIO::Port::PortC, 13);

stm32f3xx::GPIO greenport(ld2_pin, stm32f3xx::GPIO::Direction::Output);
stm32f3xx::GPIO redport(led5_pin, stm32f3xx::GPIO::Direction::Output);
stm32f3xx::GPIO butport(button_pin, stm32f3xx::GPIO::Direction::Input);
microhal::GPIO& greenLed = greenport;
microhal::GPIO& redLed = redport;
microhal::GPIO& button = butport;

void hardwareConfig(void) {
    // Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    SysTick_Config(8000000 / 1000);
}

namespace bsp {
void init() {
    //    IOManager::routeSerial<2, Txd, stm32f3xx::IOPin::PortA, 2>();
    //    IOManager::routeSerial<2, Rxd, stm32f3xx::IOPin::PortA, 3>();

    debugPort.open(IODevice::ReadWrite);
    debugPort.setBaudRate(stm32f3xx::SerialPort::Baud115200);
    debugPort.setDataBits(stm32f3xx::SerialPort::Data8);
    debugPort.setStopBits(stm32f3xx::SerialPort::OneStop);
    debugPort.setParity(stm32f3xx::SerialPort::NoParity);

    IOManager::routeI2C<1, SDA, stm32f3xx::IOPin::PortB, 9>();
    IOManager::routeI2C<1, SCL, stm32f3xx::IOPin::PortB, 8>();

    //    IOManager::routeI2C<2, SDA, stm32f3xx::IOPin::PortB, 11>();
    //    IOManager::routeI2C<2, SCL, stm32f3xx::IOPin::PortB, 10>();

    stm32f3xx::I2C::i2c1.init();
    stm32f3xx::I2C::i2c1.speed(100000, microhal::I2C::Mode::Standard);
    stm32f3xx::I2C::i2c1.enable();
    //
    //    stm32f3xx::I2C::i2c2.init();
    //    stm32f3xx::I2C::i2c2.speed(100000, microhal::I2C::Mode::Standard);
    //    stm32f3xx::I2C::i2c2.enable();
}
}  // namespace bsp

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}