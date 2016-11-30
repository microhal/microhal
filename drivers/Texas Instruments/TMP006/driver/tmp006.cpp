/* ========================================================================================================================== *//**
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

 *//* ========================================================================================================================== */

#include "tmp006.h"

using namespace microhal;
/**
 * @brief this function read and update temperature.
 * @return
 */
bool TMP006::update() {
    constexpr float a1 = 0.00175;
    constexpr float a2 = -0.00001678;
    const Temperature Tref(298.15f);
    constexpr float b0 = -0.0000294;
    constexpr float b1 = -0.00000057;
    constexpr float b2 = 0.00000000463;
    constexpr float c2 = 13.4;

    int16_t temp;
    int16_t voltage;
    if (readRegister(T_AMBIENT, (uint16_t&) temp, microhal::BigEndian) == false) return false;
    if (readRegister(V_OBJECT, (uint16_t&) voltage, microhal::BigEndian) == false) return false;

    //convert to kelvin
    ambientTemperature = Temperature {static_cast<float>(temp / 128 + 273)};
    //convert to volt
    float Vobj = (float) voltage * 0.00000015625;

    //calculating temperature according to TMP006 User's Guide (SBOU107 – May 2011) chapter 5.1
    const Temperature deltaT = ambientTemperature - Tref;
    const float deltaTsquare = deltaT.getKelvin() * deltaT.getKelvin();

    const float S = S0 + S0 * (deltaT.getKelvin() * a1 + deltaTsquare * a2);
    const float Vos = b0 + b1 * deltaT.getKelvin() + b2 * deltaTsquare;

    float deltaV = Vobj - Vos;
    float fVobj = deltaV + c2 * deltaV * deltaV;

    //we need ambient temperature to power of 4
    const float tmp = ambientTemperature.getKelvin() * ambientTemperature.getKelvin();
    const float ambjentTempToFour = tmp * tmp;

    //calculate object temperature in kelvin
    objectTemperature = Temperature ( sqrtf(sqrtf(ambjentTempToFour + (Vobj / S))) );
    return true;
}
