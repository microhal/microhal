/*
 * quaternion.cpp
 *
 *  Created on: 14 gru 2013
 *      Author: pawel
 */

#include "../quaternion.h"
#include <math.h>

namespace hal {

Quaternion::~Quaternion() {
    // TODO Auto-generated destructor stub
}

Quaternion::Quaternion(float phi, float theta, float psi) {
    this->set(phi, theta, psi);
}

void Quaternion::set(float w, float x, float y, float z)
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

void Quaternion::set(float phi, float theta, float psi)
{
    float cos_z_2 = cosf(0.5*psi);
    float cos_y_2 = cosf(0.5*theta);
    float cos_x_2 = cosf(0.5*phi);

    float sin_z_2 = sinf(0.5*psi);
    float sin_y_2 = sinf(0.5*theta);
    float sin_x_2 = sinf(0.5*psi);

    // and now compute quaternion
    w = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
    x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
    y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
    z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;
}

float Quaternion::getPsi() {
    return atan2f(2 * x * y - 2 * w * z, 2 * w * w + 2 * x * x - 1);   // psi
}
float Quaternion::getTheta() {
    return -asinf(2 * x * z + 2 * w * y);                              // theta
}
float Quaternion::getPhi() {
    return atan2f(2 * y * z - 2 * w * x, 2 * w * w + 2 * z * z - 1); // phi
}

} /* namespace hal */


