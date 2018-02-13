/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      24c512 example

 * @authors    Pawel Okas
 * created on: 15-12-2015
 * last modification: <DD-MM-YYYY>

 * @copyright Copyright (c) 2016, Pawel Okas
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 * conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "m24c16.h"
#include "microhal.h"

using namespace microhal;
using namespace diagnostic;

M24C16 m24c16(sensorI2C);

int main(void) {
    debugPort.write("\n\r----------------- M24C16 Demo -----------------\n\r");
    diagChannel.setOutputDevice(debugPort);

    uint8_t data[27];
    sensorI2C.read(0xA2, data, sizeof(data));
    m24c16.read(0x00, data, sizeof(data));

    for (uint8_t &x : data) {
        diagChannel << lock << Debug << "value: " << x << endl << unlock;
    }

    if (data[0] == 0xFF) {
        const uint8_t txt[] = "ala ma kota. Kot ma ale...";
        m24c16.write(0x00, txt, sizeof(txt));
        // wait for write
        std::this_thread::sleep_for(std::chrono::milliseconds{10});
    }

    while (1) {
        m24c16.read(0x00, data, sizeof(data));
        diagChannel << lock << Debug << "EEPROM: " << (char *)data << endl << unlock;
        std::this_thread::sleep_for(std::chrono::milliseconds{2000});
    }

    return -1;  // indicate error
}
