/*
 * clarkeTransformation.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: pokas
 */
#include "clarkeTransformation.h"

namespace microhal {
namespace math {

AlphaBeta clarkieTransform(float a, float b, float c) {
    static constexpr const float sqrt3Over2 = 0.866025404f;
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
}  // namespace math
}  // namespace microhal
