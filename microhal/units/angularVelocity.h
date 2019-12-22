/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      angular velocity unit

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

#ifndef _MICROHAL_ANGULARVELOCITY_H_
#define _MICROHAL_ANGULARVELOCITY_H_

#include <cstdint>

namespace microhal {
namespace units {

class AngularVelocity {
 public:
    explicit AngularVelocity(const float vel = 0.0) : velocity(vel) {}
    explicit AngularVelocity(const int32_t vel) : velocity(static_cast<float>(vel)) {}
    ~AngularVelocity() {}

    template <typename T>
    T get(void) const {
        if constexpr (std::is_same_v<T, float>) {
            return velocity;
        } else {
            static_assert(std::is_same_v<T, float>, "Unsupported type");
        }
    }

    AngularVelocity operator+(const AngularVelocity& vel) const { return AngularVelocity(velocity + vel.velocity); }

    friend AngularVelocity operator-(AngularVelocity lhs, const AngularVelocity& rhs) {
        lhs -= rhs;
        return lhs;
    }

    AngularVelocity operator*(float mul) const { return AngularVelocity(velocity * mul); }

    AngularVelocity operator/(float div) const { return AngularVelocity(velocity / div); }

    AngularVelocity& operator+=(const AngularVelocity& vel) {
        velocity += vel.velocity;
        return *this;
    }

    AngularVelocity& operator-=(const AngularVelocity& vel) {
        velocity -= vel.velocity;
        return *this;
    }

    AngularVelocity& operator*=(float mul) {
        velocity *= mul;
        return *this;
    }

    AngularVelocity& operator/=(float div) {
        velocity /= div;
        return *this;
    }

 private:
    float velocity;
};

}  // namespace units
}  // namespace microhal

#endif  // _MICROHAL_ANGULARVELOCITY_H_
