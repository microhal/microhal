/*
 * quaternion.h
 *
 *  Created on: 14 gru 2013
 *      Author: pawel
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include <stdint.h>
//#include <cmath>

namespace hal {

/**
 * \brief Structure that represent rotation in three dimensions.
 */
class Quaternion {
    //---------------------------------- variables ----------------------------------
    float w;    ///< W component of Quaternion
    float x;    ///< X component of Quaternion
    float y;    ///< Y component of Quaternion
    float z;    ///< Z component of Quaternion
public:
    //---------------------------------- constructors -------------------------------
    /**
     *\brief Initialize a new Quaternion.
     */
    Quaternion() :
        w(0.0), x(0.0), y(0.0), z(0.0) {
    }
    /**
     * \brief Initialize a new Quaternion with given x,y,z,w components.
     * @param _w W component of Quaternion
     * @param _x X component of Quaternion
     * @param _y Y component of Quaternion
     * @param _z Z component of Quaternion
     */
    Quaternion(float _w, float _x, float _y, float _z) :
        w(_w), x(_x), y(_y), z(_z) {
    }
    /**
     * \brief Initialize a new Quaterion form Euler angle
     * @param phi   Angle rotation around x axis
     * @param theta Angle rotation around y axis
     * @param psi   Angle rotation around z axis
     */
    Quaternion(float phi, float theta, float psi);
    Quaternion(Quaternion &q) :
        w(q.w), x(q.x), y(q.y), z(q.z){
    }
    virtual ~Quaternion();

    //---------------------------------- functions ----------------------------------

    /**
     * Set quaterninion components
     * @param w W component of Quaternion
     * @param x X component of Quaternion
     * @param y Y component of Quaternion
     * @param z Z component of Quaternion
     */
    void set(float w, float x, float y, float z);
    /**
     * Set quaterninion components from Euler angle
     * @param phi   Angle rotation around x axis
     * @param theta Angle rotation around y axis
     * @param psi   Angle rotation around z axis
     */
    void set(float phi, float theta, float psi);

    float getW() const{
        return w;
    }
    float getX() const{
        return x;
    }
    float getY() const{
        return y;
    }
    float getZ() const{
        return z;
    }

    float getPsi();
    float getTheta();
    float getPhi();
};

} /* namespace hal */

#endif /* QUATERNION_H_ */
