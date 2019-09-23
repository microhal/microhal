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

#ifndef _MICROHAL_MATH_CLARKETRANSFORMATION_H_
#define _MICROHAL_MATH_CLARKETRANSFORMATION_H_

#include <cstdint>
#include "math/alphaBeta.h"

namespace microhal {
namespace math {

AlphaBeta clarkieTransform(float a, float b, float c) {
    static constexpr const float sqrt3Over2 = 0.866025404f;
    static constexpr const float oneOverSqrt3 = 0.577350269f;
    enum class Implementations { Generic, AAxisSuperpossedWidgAlfa };

    constexpr Implementations implementation = Implementations::Generic;

    AlphaBeta ab;
    if constexpr (implementation == Implementations::Generic) {
        ab.a = 2.0f / 3.0f * a - (2.0f / 3.0f) * 0.5f * (b - c);
        ab.b = 2.0f / 3.0f * sqrt3Over2 * (b - c);
    } else if (implementation == Implementations::AAxisSuperpossedWidgAlfa) {
        ab.a = a;
        ab.b = oneOverSqrt3 * (b - c);
    }
    return ab;
}

AlphaBeta clarkieTransform(float a, float b) {
    static constexpr const float oneOverSqrt3 = 0.577350269f;

    AlphaBeta ab;
    ab.a = a;
    ab.b = oneOverSqrt3 * (a + 2 * b);
    return ab;
}
}  // namespace math
}  // namespace microhal

#endif /* _MICROHAL_MATH_CLARKETRANSFORMATION_H_ */
