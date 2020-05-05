/*
 * sin.cpp
 *
 *  Created on: Sep 28, 2019
 *      Author: pokas
 */

#include <math/sincos.h>
#include <array>
#include <cmath>
#include "math/constants.h"

namespace microhal {
namespace math {

constexpr const size_t sinTableLength = 2000;

const constexpr std::array<float, sinTableLength> generateSinTable() {
    std::array<float, sinTableLength> table = {};

    for (size_t i = 0; i < table.size(); i++) {
        table[i] = std::sin(((2.0 * pi) / table.size()) * i);
    }
    return table;
}

static const constexpr std::array<float, sinTableLength> sinTab = generateSinTable();

float sin(float angle) {
    angle = std::fmod(angle, 2.0f * pi);
    constexpr const float angleConverter = sinTab.size() / (2 * pi);
    int32_t index = angle * angleConverter;
    return index >= 0 ? sinTab[index] : sinTab[sinTab.size() + index];
}

float cos(float angle) {
    return sin((pi / 2.0f) + angle);
}

}  // namespace math
}  // namespace microhal
