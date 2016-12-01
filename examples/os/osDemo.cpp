/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      Operating system example main file

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

 *//* ========================================================================================================================== */

#include <thread>
#include <chrono>

#include "microhal.h"
#include "microhal_bsp.h"

using namespace microhal;
using namespace std::literals::chrono_literals;

void task1() {
    GPIO redLed(RedLed, GPIO::Direction::Output);

    std::chrono::milliseconds dura(1000);
    while (1) {
        redLed.toggle();

        std::this_thread::sleep_for(dura);
    }
}

void task2() {
    GPIO greenLed(GreenLed, GPIO::Direction::Output);

    while (1) {
        greenLed.toggle();

        std::this_thread::sleep_for(2s);
    }
}

// when using os main function is a thread
int main(void) {
    serialPort.open(SerialPort::ReadWrite);
    serialPort.setBaudRate(SerialPort::Baud115200);
    serialPort.setDataBits(SerialPort::Data8);
    serialPort.setStopBits(SerialPort::OneStop);
    serialPort.setParity(SerialPort::NoParity);

    serialPort.write("\n\r----------------------------- OS DEMO -----------------------------\n\r");

    std::thread first(task1);
    std::thread first2(task2);

    while (1) {
    }
    return 0;
}
