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
#include "ports/stm32f4xx/clockManager.h"

#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"

using namespace microhal;
using namespace stm32f4xx;
using namespace diagnostic;

namespace bsp {

namespace moduleA {
namespace detail {
stm32f4xx::GPIO csDat({microhal::stm32f4xx::GPIO::Port::PortC, 11}, stm32f4xx::GPIO::Direction::Input);  // CON1A GPIO1
stm32f4xx::GPIO csCon({microhal::stm32f4xx::GPIO::Port::PortA, 15}, stm32f4xx::GPIO::Direction::Input);  // CON1A SS
stm32f4xx::GPIO RESET({microhal::stm32f4xx::GPIO::Port::PortD, 2}, stm32f4xx::GPIO::Direction::Input);   // CON1A GPIO3
}  // namespace detail
microhal::stm32f4xx::SPI_interrupt &spi =
    microhal::stm32f4xx::SPI_interrupt::create<1, IOPin{IOPin::PortA, 6}, IOPin{IOPin::PortA, 7}, IOPin{IOPin::PortA, 5}>();
microhal::GPIO &csDat = detail::csDat;
microhal::GPIO &csCon = detail::csCon;
microhal::GPIO &RESET = detail::RESET;
}  // namespace moduleA

namespace moduleB {
namespace detail {
stm32f4xx::GPIO csDat({microhal::stm32f4xx::GPIO::Port::PortB, 0}, stm32f4xx::GPIO::Direction::Input);  // CON1C GPIO1
stm32f4xx::GPIO csCon({microhal::stm32f4xx::GPIO::Port::PortC, 1}, stm32f4xx::GPIO::Direction::Input);  // CON1C SS
stm32f4xx::GPIO RESET({microhal::stm32f4xx::GPIO::Port::PortC, 3}, stm32f4xx::GPIO::Direction::Input);  // CON1C GPIO3
}  // namespace detail
microhal::stm32f4xx::SPI_interrupt &spi =
    microhal::stm32f4xx::SPI_interrupt::create<3, IOPin{IOPin::PortB, 4}, IOPin{IOPin::PortB, 5}, IOPin{IOPin::PortB, 3}>();
microhal::GPIO &csDat = detail::csDat;
microhal::GPIO &csCon = detail::csCon;
microhal::GPIO &RESET = detail::RESET;
}  // namespace moduleB
}  // namespace bsp

extern "C" int main(int, void *);

static void run_main(void *) {
    main(0, nullptr);
}

void hardwareConfig(void) {
    (void)bsp::moduleA::spi;
    (void)bsp::moduleB::spi;
    (void)bsp::debugPort;
    Core::fpu_enable();
    Core::flash_latency(100000000);
    ClockManager::PLL::clockSource(ClockManager::PLL::ClockSource::HSI);
    ClockManager::PLL::MDivider(4);
    ClockManager::PLL::Main::enable();
    ClockManager::PLL::Main::N(50);
    ClockManager::PLL::Main::P(2);
    ClockManager::PLL::Main::Q(4);
    while (!ClockManager::PLL::Main::isReady()) {
    }
    //  ClockManager::SYSCLK::source(ClockManager::SYSCLK::Source::PLL);
    // while (ClockManager::SYSCLK::source() != ClockManager::SYSCLK::Source::PLL)
    //    ;

    IOManager::routeSerial<2, Txd, stm32f4xx::IOPin::PortA, 2>();
    IOManager::routeSerial<2, Rxd, stm32f4xx::IOPin::PortA, 3>();

    bsp::debugPort.setDataBits(stm32f4xx::SerialPort::Data8);
    bsp::debugPort.setStopBits(stm32f4xx::SerialPort::OneStop);
    bsp::debugPort.setParity(stm32f4xx::SerialPort::NoParity);
    bsp::debugPort.open(stm32f4xx::SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(stm32f4xx::SerialPort::Baud115200);
    diagChannel.setOutputDevice(bsp::debugPort);

    //    stm32f4xx::IOManager::routeSPI<1, SCK, stm32f4xx::IOPin::PortA, 5>();
    //    stm32f4xx::IOManager::routeSPI<1, MISO, stm32f4xx::IOPin::PortA, 6>();
    //    stm32f4xx::IOManager::routeSPI<1, MOSI, stm32f4xx::IOPin::PortA, 7>();

    //    stm32f4xx::IOManager::routeSPI<3, SCK, stm32f4xx::IOPin::PortB, 3>();
    //    stm32f4xx::IOManager::routeSPI<3, MISO, stm32f4xx::IOPin::PortB, 4>();
    //    stm32f4xx::IOManager::routeSPI<3, MOSI, stm32f4xx::IOPin::PortB, 5>();

    bsp::moduleA::spi.init(stm32f4xx::SPI::Mode0, stm32f4xx::SPI::Prescaler256);
    bsp::moduleA::spi.enable();

    bsp::moduleB::spi.init(stm32f4xx::SPI::Mode0, stm32f4xx::SPI::Prescaler256);
    bsp::moduleB::spi.enable();

    xTaskHandle mainHandle;
    xTaskCreate(run_main, (const char *)"main", (11 * 1024), 0, tskIDLE_PRIORITY, &mainHandle);

    vTaskStartScheduler();
}
