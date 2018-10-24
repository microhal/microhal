/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 12-02-2017
 * last modification: 12-02-2017
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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
#include "diagnostic/diagnostic.h"
#include "microhal.h"
#include "mpl115a2.h"

using namespace microhal;
using namespace diagnostic;
using namespace std::literals::chrono_literals;

int main() {
    bsp::debugPort.setDataBits(SerialPort::Data8);
    bsp::debugPort.setStopBits(SerialPort::OneStop);
    bsp::debugPort.setParity(SerialPort::NoParity);
    bsp::debugPort.open(SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(SerialPort::Baud115200);

    bsp::debugPort.write("\n\r------------------- MPL115A2 Demo -------------------------\n\r");

    diagChannel.setOutputDevice(bsp::debugPort);

    MPL115A2 mpl(bsp::mpl115a2::i2c);
    bsp::mpl115a2::mplReset.set();

    diagChannel << lock << MICROHAL_DEBUG << "Initializing MPL115A2..." << unlock;
    if (mpl.init() == true) {
        diagChannel << Debug << "OK" << endl;
    } else {
        diagChannel << lock << MICROHAL_ERROR << "Cannot initialize MPL115A2. Maybe unconnected?" << unlock;
    }

    while (1) {
        mpl.startConversion();
        std::this_thread::sleep_for(mpl.maxConversionTime());

        if (auto pressure = mpl.pressure()) {
            diagChannel << Debug << "Pressure: " << *pressure << endl;
        } else {
            diagChannel << lock << MICROHAL_ERROR << "Unable to read pressure." << unlock;
        }

        std::this_thread::sleep_for(500ms);
    }

    return 0;
}
