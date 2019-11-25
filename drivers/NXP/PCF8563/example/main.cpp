/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 18-02-2017
 * last modification: 18-02-2017
 *
 * @copyright Copyright (c) 2017, microHAL
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */

#include "bsp.h"
#include "pcf8563.h"

using namespace microhal;
using namespace diagnostic;
using namespace std::literals::chrono_literals;

int main() {
    serialPort.setDataBits(SerialPort::Data8);
    serialPort.setStopBits(SerialPort::OneStop);
    serialPort.setParity(SerialPort::NoParity);
    serialPort.open(SerialPort::ReadWrite);
    serialPort.setBaudRate(SerialPort::Baud115200);

    serialPort.write("\n\r------------------- PCF8563 Demo -------------------------\n\r");

    diagChannel.setOutputDevice(serialPort);

    PCF8563 pcf(i2c);

    PCF8563::Time time;
    time.hour = 17;
    time.min = 13;
    time.sec = 10;
    time.year = 16;
    time.month = 2;
    time.day = 22;
    pcf.setTime(time);

    while (1) {
        PCF8563::Time time;
        if (pcf.getTime(time) == PCF8563::Result::SUCCESS) {
            diagChannel << Debug << "time: " << time.hour << ":" << time.min << ":" << time.sec << endl
                        << "day: " << time.day << endl
                        << "month: " << time.month << endl;
        }

        std::this_thread::sleep_for(1s);
    }
}
