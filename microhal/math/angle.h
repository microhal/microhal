/*
 * Angle.h
 *
 *  Created on: 17 sty 2014
 *      Author: skoczek
 */

#ifndef ANGLE_H_
#define ANGLE_H_

#include <stdint.h>
/**
 * \brief This class provides basic angle conversion
 */
class Angle {
private:
    constexpr static float pi =
            3.1415926535897932384626433832795028841971693993751058209749445923078164062;
    Angle(){}
public:
    //---------------------------------- functions ----------------------------------
    /**
     * Radians to degrees conversion
     * @param rad Angle in radians
     * @return Angle in degrees
     */
    static float radToDeg(float rad)
    {
        return rad * 180.0 / pi;
    }
    /**
     * Degrees to radians conversion
     * @param deg Angle in degrees
     * @return Angle in radians
     */
    static float degToRad(float deg)
    {
        return deg * pi / 180.0;
    }
};

#endif /* ANGLE_H_ */
