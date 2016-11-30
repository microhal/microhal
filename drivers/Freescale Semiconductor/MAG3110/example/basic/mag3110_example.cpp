/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      MAG3110 example

 * @authors    Koszyk
 * created on: 15-12-2015
 * last modification: <DD-MM-YYYY>

 * @copyright Copyright (c) 2015, microHAL
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

#include "microhal.h"
#include "diagnostic/diagnostic.h"
#include "mag3110/mag3110.h"
#include "microhal_bsp.h"


using namespace microhal;
using namespace diagnostic;

// MAG3110 object
MAG3110 mag3110(sensorI2C, MAG3110::I2C_ADDRESS_0);

int main(void) {
    debugPort.write("\n\r----------------- MAG3110 Demo -----------------\n\r");
    diagChannel.setOutputDevice(debugPort);

    do {
        if (!mag3110.init()) {
            diagChannel << lock << ERROR<< "Cannot initialize MG3110" << endl << unlock;
            break;
        }
        mag3110.enableAxis(MAG3110::XYZ);

        mag3110.setODR_OSR(MAG3110::ODR_10Hz_OSR_128);

        if (!mag3110.setMode(MAG3110::ACTIVE_RAW)) {
            diagChannel<< lock << ERROR << "Cannot set operation mode."<< endl << unlock;
            break;
        }
        MAG3110::MagneticVector mag;
        Temperature temperature;
        while (1) {
            std::this_thread::sleep_for(std::chrono::milliseconds {250});
            if (mag3110.getMagnetic(&mag)) {
                diagChannel << lock << DEBUG << "X[g]: " << mag[0] << ", Y[g]: " << mag[1] << ", Z[g]: " << mag[2] << endl << unlock;
            }
            if (mag3110.getDieTemperature(&temperature)) {
                diagChannel << lock << DEBUG << "Die Temperature " << temperature.getCelsius() << endl << unlock;
            }
        }
    } while (0);

    while (1) {
        {}
    }

    return 0;
}
