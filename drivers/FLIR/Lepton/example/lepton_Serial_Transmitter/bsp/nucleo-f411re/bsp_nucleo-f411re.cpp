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

using namespace microhal;
using namespace stm32f4xx;

microhal::stm32f4xx::GPIO leptonCS({microhal::stm32f4xx::GPIO::Port::PortA, 15});
microhal::stm32f4xx::GPIO leptonPower({microhal::stm32f4xx::GPIO::Port::PortC, 11});
microhal::stm32f4xx::GPIO leptonReset({microhal::stm32f4xx::GPIO::Port::PortC, 10});

extern "C" int main(int, void *);
static void run_main(void *) {
    main(0, nullptr);
}

void hardwareConfig(void) {
    Core::pll_start(8000000, 100'000'000);
    Core::fpu_enable();

    IOManager::routeSerial<1, Txd, IOPin::PortB, 6>();
    IOManager::routeSerial<1, Rxd, IOPin::PortA, 10>();

    IOManager::routeSerial<2, Txd, IOPin::PortA, 2>();
    IOManager::routeSerial<2, Rxd, IOPin::PortA, 3>();

    IOManager::routeI2C<1, SDA, IOPin::PortB, 9>();
    IOManager::routeI2C<1, SCL, IOPin::PortB, 8>();

    IOManager::routeSPI<3, SCK, IOPin::PortB, 3>();
    IOManager::routeSPI<3, MISO, IOPin::PortB, 4>();
    IOManager::routeSPI<3, MOSI, IOPin::PortB, 5>();

    stm32f4xx::SPI::spi3.init(stm32f4xx::SPI::Mode3, stm32f4xx::SPI::Prescaler8);
    stm32f4xx::SPI::spi3.enable();

    stm32f4xx::I2C::i2c1.init();
    stm32f4xx::I2C::i2c1.speed(100000, microhal::I2C::Mode::Standard);
    stm32f4xx::I2C::i2c1.enable();

    debugPort.setDataBits(stm32f4xx::SerialPort::Data8);
    debugPort.setStopBits(stm32f4xx::SerialPort::OneStop);
    debugPort.setParity(stm32f4xx::SerialPort::NoParity);
    debugPort.open(stm32f4xx::SerialPort::ReadWrite);
    debugPort.setBaudRate(stm32f4xx::SerialPort::Baud115200);

    cameraPort.setDataBits(stm32f4xx::SerialPort::Data8);
    cameraPort.setStopBits(stm32f4xx::SerialPort::OneStop);
    cameraPort.setParity(stm32f4xx::SerialPort::NoParity);
    cameraPort.open(stm32f4xx::SerialPort::ReadWrite);
    cameraPort.setBaudRate(500000);

    TaskHandle_t xHandle = NULL;
    xTaskCreate(run_main, "Main", 512, NULL, tskIDLE_PRIORITY, &xHandle);
    vTaskStartScheduler();
}
