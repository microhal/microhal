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

#include "diagnostic/diagnostic.h"
#include "lis2dh12.h"
#include "microhal.h"
#include "microhal_bsp.h"

using namespace microhal;
using namespace diagnostic;

LIS2DH12 lis2dh12(sensorI2C, LIS2DH12::I2C_ADDRESS_0);

int main(void) {
    debugPort.write("\n\r----------------- LIS2DH12 Demo -----------------\n\r");
    diagChannel.setOutputDevice(debugPort);

    do {
        if (!lis2dh12.present()) {
            diagChannel << lock << MICROHAL_ERROR << "Can not find LIS2DH12 device." << endl << unlock;
            break;
        }
        lis2dh12.enableAxis(LIS2DH12::XYZ);

        lis2dh12.setODR(LIS2DH12::ODR_100Hz);

        if (!lis2dh12.setMode(LIS2DH12::Mode::Normal)) {
            diagChannel << lock << MICROHAL_ERROR << "Cannot set operation mode." << endl << unlock;
            break;
        }
        LIS2DH12::Acceleration acceleration;
        Temperature temperature;
        while (1) {
            std::this_thread::sleep_for(std::chrono::milliseconds{250});
            if (lis2dh12.readAcceleration(acceleration)) {
                diagChannel << lock << MICROHAL_DEBUG << "X[g]: " << acceleration[0] << ", Y[g]: " << acceleration[1] << ", Z[g]: " << acceleration[2]
                            << endl
                            << unlock;
            }
            //            if (lis2dh12.readTemperature(temperature)) {
            //                diagChannel << lock << DEBUG << "Temperature " << temperature.getCelsius() << endl << unlock;
            //            }
        }
    } while (0);

    while (1) {
    }

    return 0;
}
