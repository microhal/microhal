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

//#include "ports/stm32g0xx/clockManager.h"

#include "FreeRTOS.h"
#include "task.h"

using namespace microhal;
using namespace stm32g0xx;
using namespace diagnostic;

namespace bsp {
namespace moduleA {
namespace detail {
stm32g0xx::GPIO csDat({microhal::stm32g0xx::GPIO::Port::PortC, 11}, stm32g0xx::GPIO::Direction::Input);  // CON1A GPIO1
stm32g0xx::GPIO csCon({microhal::stm32g0xx::GPIO::Port::PortA, 15}, stm32g0xx::GPIO::Direction::Input);  // CON1A SS
stm32g0xx::GPIO RESET({microhal::stm32g0xx::GPIO::Port::PortD, 2}, stm32g0xx::GPIO::Direction::Input);   // CON1A GPIO3
}  // namespace detail
microhal::stm32g0xx::SPI_interrupt &spi =
    microhal::stm32g0xx::SPI_interrupt::create<1, IOPin{IOPin::PortA, 6}, IOPin{IOPin::PortA, 7}, IOPin{IOPin::PortA, 5}>();
microhal::GPIO &csDat = detail::csDat;
microhal::GPIO &csCon = detail::csCon;
microhal::GPIO &RESET = detail::RESET;
}  // namespace moduleA

namespace moduleB {
namespace detail {
stm32g0xx::GPIO csDat({microhal::stm32g0xx::GPIO::Port::PortC, 8}, stm32g0xx::GPIO::Direction::Input);  // CON1C GPIO1
stm32g0xx::GPIO csCon({microhal::stm32g0xx::GPIO::Port::PortB, 2}, stm32g0xx::GPIO::Direction::Input);  // CON1C SS
stm32g0xx::GPIO RESET({microhal::stm32g0xx::GPIO::Port::PortB, 1}, stm32g0xx::GPIO::Direction::Input);  // CON1C GPIO3
}  // namespace detail
microhal::stm32g0xx::SPI_interrupt &spi = moduleA::spi;
// microhal::stm32g0xx::SPI_interrupt::create<2, IOPin{IOPin::PortB, 4}, IOPin{IOPin::PortB, 5}, IOPin{IOPin::PortB, 3}>();
microhal::GPIO &csDat = detail::csDat;
microhal::GPIO &csCon = detail::csCon;
microhal::GPIO &RESET = detail::RESET;
}  // namespace moduleB

void init() {
    IOManager::routeSerial<2, Txd, IOPin{IOPin::PortA, 2}>();
    IOManager::routeSerial<2, Rxd, IOPin{IOPin::PortA, 3}>();
}

}  // namespace bsp

extern "C" int main(int, void *);

static void run_main(void *) {
    main(0, nullptr);
}

void hardwareConfig(void) {
    IOManager::routeSerial<2, Txd, IOPin{stm32g0xx::IOPin::PortA, 2}>();
    IOManager::routeSerial<2, Rxd, IOPin{stm32g0xx::IOPin::PortA, 3}>();

    bsp::debugPort.setDataBits(stm32g0xx::SerialPort::Data8);
    bsp::debugPort.setStopBits(stm32g0xx::SerialPort::OneStop);
    bsp::debugPort.setParity(stm32g0xx::SerialPort::NoParity);
    bsp::debugPort.open(stm32g0xx::SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(stm32g0xx::SerialPort::Baud115200);
    diagChannel.setOutputDevice(bsp::debugPort);

    bsp::moduleA::spi.init(stm32g0xx::SPI::Mode0, stm32g0xx::SPI::Prescaler256);
    bsp::moduleA::spi.enable();

    // stm32g0xx::GPIO pin(IOPin{IOPin::PortA, 6}, stm32g0xx::GPIO::Direction::Input);
    // stm32g0xx::IOManager::routeSPI<1, MISO, IOPin{IOPin::PortA, 6}>();

    //    bsp::moduleB::spi.init(stm32g0xx::SPI::Mode0, stm32g0xx::SPI::Prescaler256);
    //    bsp::moduleB::spi.enable();

    xTaskHandle mainHandle;
    xTaskCreate(run_main, (const char *)"main", (2.5 * 1024), 0, tskIDLE_PRIORITY, &mainHandle);

    vTaskStartScheduler();
}
