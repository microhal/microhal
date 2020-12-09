/*
 * Angle.h
 *
 *  Created on: 17 sty 2014
 *      Author: skoczek
 */

#ifndef ANGLE_H_
#define ANGLE_H_

#include <cmath>
#include <cstdint>
#include "diagnostic/diagnostic.h"
#include "math/constants.h"

namespace microhal {
namespace math {
/**
 * \brief This class provides basic angle conversion
 */
class Angle {
 public:
    Angle() = default;
    constexpr Angle(const float& angle) : angle(std::fmod(angle, maxValue)) {}

    //---------------------------------- functions ----------------------------------
    /**
     * Radians to degrees conversion
     * @param rad Angle in radians
     * @return Angle in degrees
     */
    static constexpr float radToDeg(float rad) { return rad * 180.0f / pi; }
    /**
     * Degrees to radians conversion
     * @param deg Angle in degrees
     * @return Angle in radians
     */
    static constexpr float degToRad(float deg) { return deg * pi / 180.0f; }

    float toRad() const { return angle; }
    float toDeg() const { return radToDeg(angle); }

    bool isEqual(Angle b, Angle error) const {
        if (angle < (b.angle + error.angle) && angle > (b.angle - error.angle)) {
            return true;
        }
        return false;
    }

    Angle distance(Angle b) {
        float diff = angle - b.angle;
        float minDiff = std::abs(diff) < std::abs(2 * pi + diff) ? 2 * pi - diff : -2 * pi - diff;
        return Angle{minDiff};
    }

    Angle operator+(Angle b) { return {angle + b.angle}; }
    Angle operator-(Angle b) { return {angle - b.angle}; }

    Angle& operator+=(Angle b) {
        angle = std::fmod(angle + b.angle, maxValue);
        return *this;
    }

    Angle& operator-=(Angle b) {
        angle = std::fmod(angle - b.angle, maxValue);
        return *this;
    }

    Angle& operator=(Angle b) {
        angle = b.angle;
        return *this;
    }

    bool operator==(Angle b) const {
        static const Angle error(allowedCompareDifference);
        return isEqual(b, error);
    };

    bool operator!=(Angle b) const {
        static const Angle error(allowedCompareDifference);
        return isEqual(b, error);
    };

 private:
    static constexpr float maxValue = 2.0f * pi;
    static constexpr const float allowedCompareDifference = 0.0001f;
    float angle;
};

template <microhal::diagnostic::LogLevel level, bool B>
inline microhal::diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel, Angle angle) {
    return logChannel << angle.toRad() << " rad";
}

}  // namespace math
}  // namespace microhal
#endif /* ANGLE_H_ */
