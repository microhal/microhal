/*
 * Vector.h
 *
 *  Created on: 02-10-2013
 *      Author: Pawel
 */

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include <cmath>
#include <cstdint>
/**
 * \brief This class provides 3d vector basic functions
 */
class Vector3D {
    //---------------------------------- variables ----------------------------------
    float x;
    float y;
    float z;

 public:
    //---------------------------------- constructors -------------------------------
    constexpr Vector3D() : x(0), y(0), z(0) {}
    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3D(const Vector3D &v) : x(v.x), y(v.y), z(v.z) {}

    //---------------------------------- functions ----------------------------------
    /**
     * \brief This function set data to vector
     * @param x - x parameter
     * @param y - y parameter
     * @param z - z parameter
     */
    void set(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    /**
     * \brief This function convert data from int16_t and set to vector
     * @param x - x parameter
     * @param y - y parameter
     * @param z - z parameter
     */
    void set(int16_t x, int16_t y, int16_t z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    /**
     *\brief This function return x value of vector.
     * @return vector value for x
     */
    float getX(void) const { return x; }
    /**
     *\brief This function return y value of vector.
     * @return vector value for y
     */
    float getY(void) const { return y; }
    /**
     *\brief This function return z value of vector.
     * @return vector value for z
     */
    float getZ(void) const { return z; }
    /**
     * \brief This function return modulus of vector.
     * @return absolute value of vector.
     */
    float abs(void) { return std::sqrt(x * x + y * y + z * z); }
    /**
     * \brief This function return angle between vector and x axis (Roll)
     * @return Phi angle
     */
    float getPhi(void) {  // obrot wokol osi x (Roll)
        return atan2f(y, z);
    }
    /**
     * \brief This function return angle between vector and y axis (Pitch)
     * @return Theta angle
     */
    float getTheta(void) {  // obrot wokol osi y	(Pitch)
        return atan2f(x, z);
    }
    /**
     * \brief This function return angle between vector and z axis (Yaw)
     * @return Psi angle
     */
    float getPsi(void) {  // obrot wokol osi z	(Yaw)
        return atan2f(y, x);
    }
    /**
     * \brief Rotate vector around x axis
     * @param angle Angle of rotation in radians
     */
    void rotatePhi(float angle) {  // obrut wzgledem osi x
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        y = y * cos - z * sin;
        z = y * sin + z * cos;
    }
    /**
     * \brief Rotate vector around y axis
     * @param angle Angle of rotation in radians
     */
    void rotateTheta(float angle) {  // obrot wzgledem osi y
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        x = x * cos - z * sin;
        z = x * sin + z * cos;
    }
    /**
     * \brief Rotate vector around z axis
     * @param angle Angle of rotation in radians
     */
    void rotatePsi(float angle) {
        float cos = std::cos(angle);
        float sin = std::sin(angle);
        y = y * cos - x * sin;
        x = y * sin + x * cos;
    }
    /**
     * \brief Rotate vector simultaneously around x and y axes
     * @param theta Angle of rotation in radians
     * @param phi Angle of rotation in radians
     */
    void rotateTethaPhi(float theta, float phi) {
        float tmpX, tmpY;
        float cosTheta = std::cos(theta);
        float sinTheta = std::sin(theta);
        float cosPhi = std::cos(phi);
        float sinPhi = std::sin(phi);

        tmpX = cosTheta * x + sinTheta * sinPhi * y + sinTheta * cosPhi * z;
        tmpY = cosPhi * y - sinPhi * z;
        z = cosTheta * cosPhi * y + cosPhi * cosTheta * z - sinTheta * x;
        x = tmpX;
        y = tmpY;
    }
    //---------------------------------- operators ----------------------------------
    Vector3D operator*(float i) const {
        Vector3D v(x * i, y * i, z * i);
        return v;
    }

    Vector3D operator/(float i) const {
        Vector3D v(x / i, y / i, z / i);
        return v;
    }
    /**
     * \brief This function return sum of vectors
     * @param v - vector witch will be added
     * @return sum of vectors
     */
    Vector3D operator+(Vector3D v) const {
        Vector3D i(x + v.x, y + v.y, z + v.z);
        return i;
    }
    /**
     * \brief This function return subtract of vectors
     * @param v - vector witch will be subtracted
     * @return subtract of vectors
     */
    Vector3D operator-(Vector3D v) const {
        Vector3D i(x - v.x, y - v.y, z - v.z);
        return i;
    }
    Vector3D &operator+=(Vector3D &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    void operator-=(Vector3D v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }
    void operator*=(float i) {
        x *= i;
        y *= i;
        z *= i;
    }
};

#endif /* VECTOR3D_H_ */
