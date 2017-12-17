/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      nrf-51 RTC main example file
 *
 * @authors    Pawel Okas
 * created on: 17.12.2017
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014-2017, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
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

#include <string.h>

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"
#include "ports/nrf51/rtc_nrf51.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

void configureSerialPort(SerialPort &serial) {
    serial.open(SerialPort::ReadWrite);
    serial.setBaudRate(SerialPort::Baud38400);
    serial.setDataBits(SerialPort::Data8);
    serial.setStopBits(SerialPort::OneStop);
    serial.setParity(SerialPort::NoParity);
}

int main(void) {
    bsp::init();
    configureSerialPort(bsp::debugPort);
    diagChannel.setOutputDevice(bsp::debugPort);

    diagChannel << lock << MICROHAL_EMERGENCY << "------------------- nRF51 RTC module demo -------------------------" << endl << endl << unlock;

    nrf51::ClockManager::LFCLK::source(nrf51::ClockManager::LFCLK::Source::Xtal);
    nrf51::ClockManager::LFCLK::enable();
    nrf51::RTC rtc(*NRF_RTC0);
    rtc.prescaler(0);
    rtc.start();
    rtc.enableInterrupt(nrf51::RTC::Interrupt::Overflow);
    rtc.enableModuleInterrupts(3);

    // -------------------- setting time
    // this should be 01.01.2018
    std::tm timeToSet;
    timeToSet.tm_sec = 0;
    timeToSet.tm_min = 0;
    timeToSet.tm_hour = 0;
    timeToSet.tm_mday = 1;
    timeToSet.tm_mon = 0;
    timeToSet.tm_year = 2018 - 1900;
    timeToSet.tm_isdst = -1;

    std::time_t basetime = std::mktime(&timeToSet);

    nrf51::RTC::time_point time(nrf51::RTC::from_time_t(basetime));
    rtc.setTime(time);

    while (1) {
        std::time_t t = nrf51::RTC::to_time_t(rtc.now());

        diagChannel << lock << Debug << "Current RTC time is: " << std::ctime(&t) << endl << endl << unlock;

        std::this_thread::sleep_for(50ms);
    }

    return 0;
}
