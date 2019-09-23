/*
 * Angle.h
 *
 *  Created on: 17 sty 2014
 *      Author: skoczek
 */

#ifndef ANGLE_H_
#define ANGLE_H_

#include <cstdint>
#include "math/constants.h"

namespace microhal {
namespace math {
/**
 * \brief This class provides basic angle conversion
 */
class Angle {
 private:
    Angle() {}

 public:
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
};
}  // namespace math
}  // namespace microhal
#endif /* ANGLE_H_ */
