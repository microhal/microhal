/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 19-10-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#ifndef _MICROHAL_MATH_PID_H_
#define _MICROHAL_MATH_PID_H_

#include <cstdint>

namespace microhal {
namespace math {

class PIDBase {
 public:
    void setLimits(float min, float max) {
        outputLimitMin = min;
        outputLimitMax = max;
    }

 protected:
    float outputLimitMin;
    float outputLimitMax;
    float integral = 0.0f;

    PIDBase(float outputLimitMin, float outputLimitMax) : outputLimitMin(outputLimitMin), outputLimitMax(outputLimitMax) {}

    float limit(float value) {
        if (value > outputLimitMax) return outputLimitMax;
        if (value < outputLimitMin) return outputLimitMin;
        return value;
    }

    void limitIntegral(float piOut) {
        // integral = limit(integral);

        float minValue = min(outputLimitMin - piOut, 0);
        float maxValue = max(outputLimitMax - piOut, 0);
        if (integral < minValue) integral = minValue;
        if (integral > maxValue) integral = maxValue;
    }

    float min(float a, float b) { return a < b ? a : b; }
    float max(float a, float b) { return a > b ? a : b; }
};

class PID : public PIDBase {
 public:
    PID(float dt, float Kp, float Ki, float Kd, float outputLimitMin, float outputLimitMax)
        : PIDBase(outputLimitMin, outputLimitMax), dt(dt), Kp(Kp), Ki(Ki), Kd(Kd) {}

    float pid(float setValue, float actualValue) {
        float error = setValue - actualValue;
        integral += error * dt;
        float derivative = (error - previousError) / dt;
        previousError = error;

        float output = Kp * error + Ki * integral + Kd * derivative;
        limitIntegral(output);
        return limit(output);
    }

 private:
    const float dt;
    float previousError = 0.0f;
    const float Kp;
    const float Ki;
    const float Kd;
};

class PIseries : public PIDBase {
 public:
    /**
     *
     * @param dt
     * @param Ka - gain, Ka = Kp
     * @param Kb - integral part gain, define zero position, Kb = Ki/Kp
     * @param outputLimitMin
     * @param outputLimitMax
     */
    PIseries(float dt, float Ka, float Kb, float outputLimitMin, float outputLimitMax)
        : PIDBase(outputLimitMin, outputLimitMax), Ka(Ka), Kbdt(Kb * dt) {}

    float pid(float setValue, float newSample) {
        float error = setValue - newSample;
        float proportionalTerm = Ka * error;
        integral += Kbdt * proportionalTerm;
        float piOut = proportionalTerm + integral;
        limitIntegral(piOut);
        return limit(piOut);
    }

 private:
    const float Ka;
    const float Kbdt;
    float integral = 0.0f;
};

}  // namespace math
}  // namespace microhal

#endif /* _MICROHAL_MATH_PID_H_ */
