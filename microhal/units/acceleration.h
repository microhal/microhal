/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      acceleration unit

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

#ifndef _MICROHAL_ACCELERATION_H_
#define _MICROHAL_ACCELERATION_H_

#include <cstdint>
#include "diagnostic/diagnostic.h"
namespace microhal {

class Acceleration {
 public:
  explicit Acceleration(const float acc = 0.0) : acceleration(acc) { }
  explicit Acceleration(const int32_t acc) : acceleration(static_cast <float>(acc)) { }
  ~Acceleration() {}

  float getAcceleration(void) const {
    return acceleration;
  }

  Acceleration operator+(const Acceleration& acc) const {
    return Acceleration(acceleration + acc.acceleration);
  }

  Acceleration operator-(const Acceleration& acc) const {
    return Acceleration(acceleration - acc.acceleration);
  }

  Acceleration operator*(const float mul) const {
    return Acceleration(acceleration * mul);
  }

  Acceleration operator/(const float div) const {
    return Acceleration(acceleration / div);
  }

  Acceleration& operator+=(const Acceleration &acc) {
      acceleration += acc.acceleration;
      return *this;
  }

  Acceleration& operator-=(const Acceleration &acc) {
      acceleration -= acc.acceleration;
      return *this;
  }

  Acceleration& operator*=(const float mul) {
      acceleration *= mul;
      return *this;
  }

  Acceleration& operator/=(const float div) {
      acceleration /= div;
      return *this;
  }

  template <microhal::diagnostic::LogLevel level, bool B>
  friend inline microhal::diagnostic::LogLevelChannel<level, B> operator<< (microhal::diagnostic::LogLevelChannel<level, B> logChannel, const Acceleration& acc);

 private:
  float acceleration;
};

template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> operator<< (microhal::diagnostic::LogLevelChannel<level, B> logChannel, const Acceleration& acc) {
    return logChannel << acc.getAcceleration() << "[g]";
}
}  // namespace microhal

#endif  // _MICROHAL_ACCELERATION_H_

