/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      serial port example main file
 *
 * @authors    Pawel Okas
 * created on: 20-01-2014
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

#include "bsp.h"
#include "microhal.h"

using namespace microhal;

void configureSerialPort(SerialPort &serial) {
    serial.setBaudRate(SerialPort::Baud115200);
    serial.setDataBits(SerialPort::Data8);
    serial.setStopBits(SerialPort::OneStop);
    serial.setParity(SerialPort::NoParity);
    serial.open(SerialPort::ReadWrite);
}

int main(void) {
    bsp::init();
    configureSerialPort(bsp::serialPortA);

    bsp::serialPortA.write("\n\r----------------------------- SerialPort DEMO -----------------------------\n\r");

    char buffer[100];
    while (1) {
        auto availableBytes = bsp::serialPortA.availableBytes();

        // if some data available
        if (availableBytes != 0) {
            // prevent buffer overflow
            if (availableBytes > sizeof(buffer)) {
                availableBytes = sizeof(buffer);
            }
            // make echo
            bsp::serialPortA.write(buffer, availableBytes);
        }
    }

    return 0;
}
