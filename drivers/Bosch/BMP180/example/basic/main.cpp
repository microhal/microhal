/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 16-04-2014
 * last modification: 20-03-2018
 *
 * @copyright Copyright (c) 2014-2018, Pawel Okas
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

#include <chrono>
#include <cstdio>
#include "bmp180.h"
#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"

using namespace microhal;
using namespace diagnostic;
using namespace std::chrono_literals;

int main(void) {
    bsp::init();

    bsp::debugPort.write("\n\r------------------- BMP180 Demo -------------------------\n\r");

    diagChannel.setOutputDevice(bsp::debugPort);

    BMP180 bmp(bsp::bmp180::i2c);

    diagChannel << lock << Informational << "BMP180 ID = " << toHex(bmp.getDeviceID()) << endl << unlock;

    if (bmp.init() == false) {
        diagChannel << lock << MICROHAL_ERROR << "BMP initialize error." << unlock;
    }

    while (1) {
        if (auto tempAndPress = bmp.getTemperatureAndPressure(BMP180::Oversampling::None)) {
            diagChannel << lock << Debug << "Temperature: " << tempAndPress->temperature / 10 << " Celcius, pressure: " << tempAndPress->pressure
                        << " Pa" << endl
                        << unlock;
        }

        std::this_thread::sleep_for(1s);
    }
}
