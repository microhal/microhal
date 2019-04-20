/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 06-04-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#ifndef _MICROHAL_GRAPHICS_DISPLAY_H_
#define _MICROHAL_GRAPHICS_DISPLAY_H_

#include <cmath>
#include <cstdint>
#include "color.h"
#include "display.h"
#include "point.h"

namespace microhal {
namespace graphics {

class Display {
 public:
    virtual ~Display() {}

    virtual bool setPixel(Point, Color color) = 0;
    virtual bool drawLine(Point begin, Point end, Color color);
    bool drawLineWidth(Point begin, Point end, uint_fast16_t width, Color color);
    bool drawRectangle(Point begin, Point end, Color);
    virtual bool drawFilledRectangle(Point begin, Point end, Color);
    bool drawCircle(Point center, uint16_t radius, Color);
    bool drawFilledCircle(Point center, uint16_t radius, Color);
    bool drawTriangle(Point v1, Point v2, Point v3, Color);
    bool drawRoundedRectangle(Point begin, Point end, uint16_t radius, Color);
    bool drawFilledRoundedRectangle(Point begin, Point end, uint16_t radius, Color color);

 private:
    void drawCircle_helper(Point center, int x, int y, Color color, uint8_t parts);
    void drawFilledCircle_helper(Point center, uint16_t x, uint16_t y, Color color);
    void drawPartialCircle(Point center, uint16_t radius, Color, uint8_t parts);
};
}  // namespace graphics
}  // namespace microhal

#endif /* _MICROHAL_GRAPHICS_DISPLAY_H_ */
