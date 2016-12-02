/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      Main file for LIS302DL example

 * @authors    Pawel
 * created on: 10-11-2015
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
#include "microhal_bsp.h"

#include "lis302.h"

using namespace microhal;
using namespace diagnostic;
// create accelerometer sensor object
LIS302DL lis302(lis302_SPI, lis302_CS);

int main(void) {
    // show example information
    debugPort.write("\n\r----- LIS302 Demo -----\n\r");
    // set default diagChannel output device.
    diagChannel.setOutputDevice(debugPort);

    do {
        if (!lis302.init()) {
            diagChannel << lock << MICROHAL_ERROR <<
                    "Error while initializing" << endl << unlock;
            break;
        }

        if (!lis302.enableAxis(LIS302DL::Axis::XYZ)) {
            diagChannel << lock << MICROHAL_ERROR <<
                    "Error while setting axis" << endl << unlock;
            break;
        }
        if (!lis302.setODR(LIS302DL::Odr::ODR_100Hz)) {
            diagChannel << lock << MICROHAL_ERROR <<
                    "Error while setting ODR" << endl << unlock;
            break;
        }
        if (!lis302.setRange(LIS302DL::Range::Range_2g)) {
            diagChannel << lock << MICROHAL_ERROR <<
                    "Error while setting range" << endl << unlock;
            break;
        }

        if (!lis302.setMode(LIS302DL::Mode::Active)) {
            diagChannel << lock << MICROHAL_ERROR <<
                    "Error while setting mode" << endl << unlock;
            break;
        }

        LIS302DL::AccelerationVector acc;
        while (1) {
            std::this_thread::sleep_for(std::chrono::milliseconds {250});
            if (lis302.getAcceleration(&acc)) {
                diagChannel << lock << MICROHAL_DEBUG <<
                         "X: " << acc[0] <<
                         ", Y: " << acc[1] <<
                         ", Z: " << acc[2] <<
                         endl << unlock;
            }
        }
    } while (0);

    while (1) {
        {;}
    }

    return 0;
}
