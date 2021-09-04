/**
 * @file
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      board support package for nucleo-g071rb board
 *
 * @authors    Pawel Okas
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
#include "nucleo_f103rb_extension_v0_1_0.h"
#include "ports/stm32g0xx/IOManager_stm32g0xx.h"
#include "ports/stm32g0xx/clockManager.h"
#include "ports/stm32g0xx/spi_stm32g0xx.h"
#include "ports/stm32g0xx/timer_stm32g0xx.h"
#include "ports/stmCommon/clockManager/dmaClock.h"

using namespace microhal;
using namespace microhal::stm32g0xx;

using Timer = stm32g0xx::Timer;

namespace bsp {

auto &spi1Detail = SPI_interrupt::create<1, bsp::con1::a::miso, bsp::con1::a::mosi, bsp::con1::a::sck>();
// auto &spi1Detail = SPI_dma::create<1, bsp::con1::a::miso, bsp::con1::a::mosi, bsp::con1::a::sck>();
stm32g0xx::GPIO cs_stm(con1::a::cs, stm32g0xx::GPIO::Direction::Output);

microhal::SPI &sdCardSpi = spi1Detail;
microhal::GPIO &sdCardCs = cs_stm;

void configureSerialPort(microhal::SerialPort &serial, microhal::SerialPort::BaudRate baudrate) {
    serial.setBaudRate(baudrate);
    serial.setDataBits(microhal::SerialPort::Data8);
    serial.setStopBits(microhal::SerialPort::OneStop);
    serial.setParity(microhal::SerialPort::NoParity);
    serial.open(microhal::SerialPort::ReadWrite);
}

bool init() {
    //   ClockManager::enableDMA(2, ClockManager::PowerMode::Normal);
    IOManager::routeSerial<2, Txd, IOPin{IOPin::PortA, 2}>();
    IOManager::routeSerial<2, Rxd, IOPin{IOPin::PortA, 3}>();

    configureSerialPort(debugPort, microhal::SerialPort::Baud115200);
    spi1Detail.init(microhal::SPI::Mode0, stm32g0xx::SPI::Prescaler256);
    spi1Detail.enable();

    return true;
}

}  // namespace bsp

Timer timer(registers::tim2);

extern "C" void RTOS_AppConfigureTimerForRuntimeStats(void) {
    timer.setClockSource(Timer::ClockSource::Internal);
    timer.setPrescaler(6400);
    timer.setMode(Timer::Mode::EdgeAligned);
    timer.setDirection(Timer::Direction::UpCounter);
    timer.setValue(0);
    timer.generateEvent(Timer::Event::Update);
    timer.enable();
}

extern "C" uint32_t RTOS_AppGetRuntimeCounterValueFromISR(void) {
    return (uint32_t)registers::tim2->cnt.volatileLoad();
}

extern "C" int main(int, void *);
static void run_main(void *) {
    main(0, nullptr);
}

void hardwareConfig(void) {
    // ClockManager::HSE::enableBypas();
    // ClockManager::HSE::enable();
    // while (!ClockManager::HSE::isReady()) {
    // }
    //    ClockManager::PLL::clockSource(ClockManager::PLL::ClockSource::HSE);
    //    ClockManager::PLL::MDivider(4);
    //    ClockManager::PLL::Main::N(64);
    //    ClockManager::PLL::Main::P(8);
    //    ClockManager::PLL::Main::enable();
    //    while (!ClockManager::PLL::Main::isReady()) {
    //    }
    // ClockManager::SYSCLK::source(ClockManager::SYSCLK::Source::HSE);

    TaskHandle_t xHandle = NULL;

    xTaskCreate(run_main, "main", 500, NULL, tskIDLE_PRIORITY, &xHandle);

    vTaskStartScheduler();
}
