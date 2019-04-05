/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 16-04-2014
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

#include "bmp180.h"
#include <tuple>

/**
 *
 * @return
 */
BMP180::Error BMP180::readCalibrationData() {
    // we are reading callibration coefitiens only once at BMP180 initialization. Lets read callibration coeffictients twice to be sure the reading
    // was correct. To check this we will check if both reading are equal, if not we will return error.
    std::tuple<int16_t, int16_t, int16_t, uint16_t, uint16_t, uint16_t, int16_t, int16_t, int16_t, int16_t, int16_t> tmp[2];
    Error result = Error::None;
    for (auto &x : tmp) {
        result = readMultipleRegisters(x, Register::AC1, Register::AC2, Register::AC3, Register::AC4, Register::AC5, Register::AC6, Register::B1,
                                       Register::B2, Register::MB, Register::MC, Register::MD);
        if (result != Error::None) return result;
    }

    if (tmp[0] == tmp[1]) {
        calibrationCoefs.AC1 = std::get<0>(tmp[0]);
        calibrationCoefs.AC2 = std::get<1>(tmp[0]);
        calibrationCoefs.AC3 = std::get<2>(tmp[0]);
        calibrationCoefs.AC4 = std::get<3>(tmp[0]);
        calibrationCoefs.AC5 = std::get<4>(tmp[0]);
        calibrationCoefs.AC6 = std::get<5>(tmp[0]);
        calibrationCoefs.B1 = std::get<6>(tmp[0]);
        calibrationCoefs.B2 = std::get<7>(tmp[0]);
        calibrationCoefs.MB = std::get<8>(tmp[0]);
        calibrationCoefs.MC = std::get<9>(tmp[0]);
        calibrationCoefs.MD = std::get<10>(tmp[0]);
    } else {
        return Error::Unknown;
    }

    return result;
}

#if 0
// test for values form datasheat
constexpr BMP180::CalibrationCoefficients calibrationCoefs = {408, -72, -14383, 32741, 32757, 23153, 6190, 4, -32768, -8711, 2868};

static_assert(calibrationCoefs.AC1 == 408, "");
static_assert(calibrationCoefs.AC2 == -72, "");
static_assert(calibrationCoefs.AC3 == -14383, "");
static_assert(calibrationCoefs.AC4 == 32741, "");
static_assert(calibrationCoefs.AC5 == 32757, "");
static_assert(calibrationCoefs.AC6 == 23153, "");

static_assert(calibrationCoefs.MC == -8711, "");
static_assert(calibrationCoefs.MD == 2868, "");

template <int32_t val>
struct test {
    static_assert(val == 150, "");
};

test<BMP180::calculateTrueTemperature(calibrationCoefs, 27898)> a;
test<BMP180::calculateTruePressure(calibrationCoefs, 23843, 0, 2399)> b;
#endif
