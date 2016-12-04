/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      hardware definitions

 @authors    Pawel
 created on: 11-08-2015
 last modification: <DD-MM-YYYY>

 Copyright (c) 2014, microHAL
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
#include "SPIDevice/SPIDevice.h"
#include "microhal.h"
#include "microhal_bsp.h"

//#include "FreeRTOS.h"
//#include "task.h"

using namespace microhal;
using namespace stm32f4xx;

void main(void *);

void hardwareConfig(void) {
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    IOManager::routeSerial<1, Txd, stm32f4xx::GPIO::PortB, 6>();
    IOManager::routeSerial<1, Rxd, stm32f4xx::GPIO::PortB, 7>();

    //    IOManager::routeSerial<2, Txd, stm32f4xx::GPIO::PortA, 2>();
    //    IOManager::routeSerial<2, Rxd, stm32f4xx::GPIO::PortA, 3>();

    IOManager::routeSPI<1, SCK, stm32f4xx::GPIO::PortA, 5>();
    IOManager::routeSPI<1, MISO, stm32f4xx::GPIO::PortB, 4>();
    IOManager::routeSPI<1, MOSI, stm32f4xx::GPIO::PortB, 5>();

    IOManager::routeSPI<2, SCK, stm32f4xx::GPIO::PortB, 13>();
    IOManager::routeSPI<2, MISO, stm32f4xx::GPIO::PortB, 14>();
    IOManager::routeSPI<2, MOSI, stm32f4xx::GPIO::PortB, 15>();

    // pull down RF modules interrupt pins
    stm32f4xx::GPIO::setPullType(rfModule1_IRQ0.port, rfModule1_IRQ0.pin, stm32f4xx::GPIO::PullDown);
    stm32f4xx::GPIO::setPullType(rfModule1_IRQ1.port, rfModule1_IRQ1.pin, stm32f4xx::GPIO::PullDown);

    stm32f4xx::GPIO::setPullType(rfModule2_IRQ0.port, rfModule2_IRQ0.pin, stm32f4xx::GPIO::PullDown);
    stm32f4xx::GPIO::setPullType(rfModule2_IRQ1.port, rfModule2_IRQ1.pin, stm32f4xx::GPIO::PullDown);

    // configure Serial Port interfaces
    stm32f4xx::SerialPort::Serial1.clear();

    stm32f4xx::SerialPort::Serial1.setDataBits(stm32f4xx::SerialPort::Data8);
    stm32f4xx::SerialPort::Serial1.setStopBits(stm32f4xx::SerialPort::OneStop);
    stm32f4xx::SerialPort::Serial1.setParity(stm32f4xx::SerialPort::NoParity);
    stm32f4xx::SerialPort::Serial1.setBaudRate(stm32f4xx::SerialPort::Baud115200);
    stm32f4xx::SerialPort::Serial1.open(stm32f4xx::SerialPort::ReadWrite);

    //	stm32f4xx::SerialPort::Serial2.clear();
    //
    //	stm32f4xx::SerialPort::Serial2.setDataBits(stm32f4xx::SerialPort::Data8);
    //	stm32f4xx::SerialPort::Serial2.setStopBits(stm32f4xx::SerialPort::OneStop);
    //	stm32f4xx::SerialPort::Serial2.setParity(stm32f4xx::SerialPort::NoParity);
    //	stm32f4xx::SerialPort::Serial2.setBaudRate(stm32f4xx::SerialPort::Baud115200, stm32f4xx::SerialPort::Direction::AllDirections);
    //	stm32f4xx::SerialPort::Serial2.open(stm32f4xx::SerialPort::ReadWrite);

    stm32f4xx::SPI::spi1.init(stm32f4xx::SPI::Mode1, stm32f4xx::SPI::PRESCALER_256);
    stm32f4xx::SPI::spi1.enable();

    stm32f4xx::SPI::spi2.init(stm32f4xx::SPI::Mode1, stm32f4xx::SPI::PRESCALER_256);
    stm32f4xx::SPI::spi2.enable();

    //    xTaskHandle mainHandle;
    //    xTaskCreate(main, (const char* )"main", (10*1024), 0, tskIDLE_PRIORITY, &mainHandle);
    //
    //    vTaskStartScheduler();
}

uint64_t SysTick_time = 0;
