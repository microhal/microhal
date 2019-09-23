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

#ifndef _MICROHAL_MATH_PARKTRANSFORMATION_H_
#define _MICROHAL_MATH_PARKTRANSFORMATION_H_

#include <cmath>
#include "math/alphaBeta.h"
#include "math/dq.h"

namespace microhal {
namespace math {

DQ parkTransform(AlphaBeta ab, float angle) {
    DQ dq;
    float sin = std::sin(angle);
    float cos = std::cos(angle);
    dq.d = ab.a * cos + ab.b * sin;
    dq.q = ab.b * cos - ab.a * sin;
    return dq;
}

DQ parkTransform(AlphaBeta ab, float sin, float cos) {
    DQ dq;
    dq.d = ab.a * cos + ab.b * sin;
    dq.q = ab.b * cos - ab.a * sin;
    return dq;
}

AlphaBeta inverseParkTransform(DQ dq, float angle) {
    AlphaBeta ab;
    float cos = std::cos(angle);
    float sin = std::sin(angle);
    ab.a = dq.d * cos - dq.q * sin;
    ab.b = dq.q * cos + dq.d * sin;
    return ab;
}

AlphaBeta inverseParkTransform(DQ dq, float sin, float cos) {
    AlphaBeta ab;
    ab.a = dq.d * cos - dq.q * sin;
    ab.b = dq.q * cos + dq.d * sin;
    return ab;
}

}  // namespace math
}  // namespace microhal

#endif /* _MICROHAL_MATH_PARKTRANSFORMATION_H_ */
