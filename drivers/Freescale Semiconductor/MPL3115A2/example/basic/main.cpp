/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 18-11-2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016-2018, Pawel Okas
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
#include "diagnostic/diagnostic.h"
#include "microhal.h"
#include "mpl3115.h"

using namespace microhal;
using namespace diagnostic;
using namespace std::chrono_literals;

int main() {
    bsp::init();
    bsp::debugPort.write("\n\r------------------- MPL3115A2 Demo -------------------------\n\r");

    diagChannel.setOutputDevice(bsp::debugPort);

    MPL3115 mpl(bsp::mpl3115a2::i2c);

    diagChannel << MICROHAL_DEBUG << "Initializing MPL3115A...";
    if (mpl.init() == true) {
        diagChannel << Debug << "OK" << endl;
    } else {
        diagChannel << MICROHAL_ERROR << "Cannot initialize MPL3115A. Maybe unconnected?";
    }

    mpl.setMode(MPL3115::Mode::Active);
    mpl.setSensingMode(MPL3115::SensingMode::Barometer);
    mpl.setOSR(MPL3115::OutputSampleRate::OSR_6ms);
    mpl.enableEvent(MPL3115::Event::OnNewPressureOrAltitude | MPL3115::Event::OnNewTemperature);

    while (1) {
        if (mpl.isPressureOrAltitudeReady()) {
            if (auto data = mpl.read()) {
                diagChannel << lock << Debug << "Temperature: " << std::get<MPL3115::Temperature>(*data)
                            << ", Pressure: " << std::get<MPL3115::Pressure>(*data) << endl
                            << unlock;
            }
        }
        std::this_thread::sleep_for(1s);
    }

    return 0;
}
