/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      MAG3110 example

 * @authors    Kubaszek Mateusz
 * created on: 10-01-2017
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

#include "lis331hh.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"
#include "microhal_bsp.h"
#include "units/acceleration.h"

#include <thread>

using namespace microhal;
using namespace diagnostic;
using namespace std::literals::chrono_literals;

LIS331HH lis331hh(sensorI2C, LIS331HH::Address::I2CaddressLowSA0);

int main(void) {
    debugPort.write("\n\r----------------- LIS331HH demo -----------------\n\r");
    diagChannel.setOutputDevice(debugPort);

    if (false == lis331hh.init(LIS331HH::Axis::all, LIS331HH::PowerMode::lowPower_10Hz, LIS331HH::Sensitivity::sensitivity24g)) {
        diagChannel << lock << MICROHAL_ERROR << "Initialization error" << unlock;
    }
    LIS331HH::Axis axis;
    Acceleration x, y, z;
    while (1) {
        if (lis331hh.dataAvailable(axis)) {
            if ((axis & LIS331HH::Axis::all) == LIS331HH::Axis::all) {
                if (lis331hh.getAcceleration(x, y, z)) {
                    diagChannel << lock << Notice << "\n\rx=" << x << " y=" << y << " z=" << z << unlock;
                } else {
                    diagChannel << lock << MICROHAL_ERROR << "Transaction error" << unlock;
                }
            }
        } else {
            diagChannel << lock << MICROHAL_ERROR << "Transaction error" << unlock;
        }
    }

    return 0;
}
