/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2021, Pawel Okas
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

#ifndef SRC_THIRD_PARTY_GRAPHICS_TRIANGLE_H_
#define SRC_THIRD_PARTY_GRAPHICS_TRIANGLE_H_

#include "color.h"
#include "point.h"

namespace microhal {
namespace graphics {

class Triangle {
 public:
    Triangle(Point a, Point b, Point c, Color color);
    virtual ~Triangle();

    void translate(int16_t x, int16_t y) {
        a.x += x;
        a.y += y;
        b.x += x;
        b.y += y;
        c.x += x;
        c.y += y;
    }

    void getCoordinates_to(Point &pointA, Point &pointB, Point &pointC) const {
        pointA = a;
        pointB = b;
        pointC = c;
    }

    const Color &color() const { return m_color; }
    void color(Color col) { m_color = col; }

    bool visible() const { return m_visible; }
    void visible(bool visible_) { m_visible = visible_; }

    bool hidden() const { return !visible(); }

 private:
    Point a;
    Point b;
    Point c;
    Color m_color;
    bool m_visible = true;
};

}  // namespace graphics
}  // namespace microhal

#endif /* SRC_THIRD_PARTY_GRAPHICS_TRIANGLE_H_ */
