/**
 * @file
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      board support package for nucleo-f411re board
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
#include "nucleo_f411re_extension_v0_1_0.h"
#include "ports/stm32f4xx/IOManager_stm32f4xx.h"
#include "ports/stm32f4xx/clockManager.h"
#include "ports/stm32f4xx/timer_stm32f4xx.h"
#include "ports/stmCommon/clockManager/dmaClock.h"

using namespace microhal;
using namespace microhal::stm32f4xx;

using Timer = stm32f4xx::Timer;

namespace bsp {
namespace detail {
constexpr IOPin ld2_pin(microhal::stm32f4xx::GPIO::Port::PortA, 5);  // green LED

stm32f4xx::GPIO greenLed(ld2_pin, stm32f4xx::GPIO::Direction::Output);
}  // namespace detail

microhal::GPIO &gpio = detail::greenLed;

void configureSerialPort(microhal::SerialPort &serial, microhal::SerialPort::BaudRate baudrate) {
    serial.setBaudRate(baudrate);
    serial.setDataBits(microhal::SerialPort::Data8);
    serial.setStopBits(microhal::SerialPort::OneStop);
    serial.setParity(microhal::SerialPort::NoParity);
    serial.open(microhal::SerialPort::ReadWrite);
}

bool init() {
    ClockManager::enableDMA(2, ClockManager::PowerMode::Normal);
    IOManager::routeSerial<2, Txd, IOPin{IOPin::PortA, 2}>();
    IOManager::routeSerial<2, Rxd, IOPin{IOPin::PortA, 3}>();

    configureSerialPort(debugPort, microhal::SerialPort::Baud115200);

    return true;
}

}  // namespace bsp

extern "C" ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t nbyte) {
    return bsp::debugPort.write((const char *)buf, nbyte);
}

extern "C" int _isatty(int file) {
    return 1;
}

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

extern "C" void low_level_init_0(void) {
    stm32f4xx::Core::fpu_enable();
}

extern "C" void preInit(void) {}

void hardwareConfig(void) {
    //  stm32f4xx::Core::fpu_enable();
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

    xTaskCreate(run_main, "main", 1000, NULL, tskIDLE_PRIORITY, &xHandle);

    vTaskStartScheduler();
}
