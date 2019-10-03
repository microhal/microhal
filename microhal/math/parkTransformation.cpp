/*
 * parkTransformation.cpp
 *
 *  Created on: Sep 25, 2019
 *      Author: pokas
 */

#include "parkTransformation.h"
#include <cmath>

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
