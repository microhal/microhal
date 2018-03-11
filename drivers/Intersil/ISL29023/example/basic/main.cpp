/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 22-02-2014
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

 */ /* ==========================================================================================================================
                                                                                                                                      */

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "isl29023.h"
#include "microhal.h"

#include <cstdio>

using namespace microhal;
using namespace diagnostic;

int main(void) {
    bsp::debugPort.clear();

    bsp::debugPort.setDataBits(SerialPort::Data8);
    bsp::debugPort.setStopBits(SerialPort::OneStop);
    bsp::debugPort.setParity(SerialPort::NoParity);
    bsp::debugPort.setBaudRate(SerialPort::Baud115200);
    bsp::debugPort.open(SerialPort::ReadWrite);

    bsp::debugPort.write("\n\r------------------- ISL29023 Demo -------------------------\n\r");

    diagChannel.setOutputDevice(bsp::debugPort);

    ISL29023 isl(bsp::isl29023::i2c);

    // isl.reset();

    diagChannel << lock << MICROHAL_DEBUG << "Setting mode...";
    if (isl.setMode(ISL29023::Mode_AlsContinuous) != I2C::Error::None) {
        diagChannel << MICROHAL_ERROR << "unable to set mode." << unlock;
    } else {
        diagChannel << Debug << "OK" << endl << unlock;
    }

    diagChannel << lock << MICROHAL_DEBUG << "Setting range...";
    if (isl.setRange(ISL29023::Range2) == false) {
        diagChannel << MICROHAL_ERROR << "unable to set range." << unlock;
    } else {
        diagChannel << Debug << "OK" << endl << unlock;
    }

    diagChannel << lock << MICROHAL_DEBUG << "Setting resolution...";
    if (isl.setResolution(ISL29023::Resolution_8bit) == false) {
        diagChannel << MICROHAL_ERROR << "unable to set resolution." << unlock;
    } else {
        diagChannel << Debug << "OK" << endl << unlock;
    }

    float lux;
    while (1) {
        if (isl.getLux(lux) == true) {
            diagChannel << lock << Debug << "lux = " << lux << endl << unlock;
        } else {
            diagChannel << MICROHAL_ERROR << "unable to read light intensity." << unlock;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds{1000});
    }

    return 0;
}
