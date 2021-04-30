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

#include "display.h"
#include "font.h"

namespace microhal {
namespace graphics {

float max(float a, float b) {
    return a > b ? a : b;
}

bool Display::drawLine(Point begin, Point end, Color color) {
    int_fast16_t dx = std::abs(end.x - begin.x);
    int_fast16_t sx = begin.x < end.x ? 1 : -1;
    int_fast16_t dy = -abs(end.y - begin.y);
    int_fast16_t sy = begin.y < end.y ? 1 : -1;
    int_fast16_t err = dx + dy; /* error value e_xy             */
    Point pixel = begin;

    for (;;) { /* loop */
        setPixel(pixel, color);
        if (pixel.x == end.x && pixel.y == end.y) break;
        int_fast16_t e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            pixel.x += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx) {
            err += dx;
            pixel.y += sy;
        } /* e_xy+e_y < 0 */
    }
    return true;
}

bool Display::drawLineWidth(Point begin, Point end, uint_fast16_t width, Color color) {
    uint_fast8_t widthHalf = width / 2;
    drawFilledCircle(begin, widthHalf, color);
    drawFilledCircle(end, widthHalf, color);

    if (begin.x == end.x) {
        drawFilledRectangle(Point{begin.x - widthHalf, begin.y}, Point{end.x + widthHalf + (width % 2), end.y}, color);
    } else if (begin.y == end.y) {
        drawFilledRectangle(Point{begin.x, begin.y - widthHalf}, Point{end.x, end.y + widthHalf + (width % 2)}, color);
    } else {
        // TODO implement
        return false;
    }
    return true;
}

bool Display::drawTriangle(const Triangle& triangle) {
    if (triangle.visible()) {
        Point v1, v2, v3;
        triangle.getCoordinates_to(v1, v2, v3);
        const Color color = triangle.color();
        drawLine(v1, v2, color);
        drawLine(v2, v3, color);
        drawLine(v1, v3, color);
    }
    return true;
}

bool Display::drawRectangle(Point begin, Point end, Color color) {
    Point corner1 = begin;
    Point corner2 = {begin.x, end.y};
    Point corner3 = end;
    Point corner4 = {end.x, begin.y};
    drawLine(corner1, corner2, color);
    drawLine(corner2, corner3, color);
    drawLine(corner3, corner4, color);
    drawLine(corner4, corner1, color);
    return true;
}

bool Display::drawFilledRectangle(Point begin, Point end, Color color) {
    if (begin.x > end.x) std::swap(begin.x, end.x);
    if (begin.y > end.y) std::swap(begin.y, end.y);
    for (uint16_t x = begin.x; x <= end.x; x++) {
        for (uint16_t y = begin.y; y <= end.y; y++) {
            setPixel({x, y}, color);
        }
    }
    return true;
}

bool Display::drawRoundedRectangle(Point begin, Point end, uint16_t radius, Color color) {
    if (begin.x > end.x) std::swap(begin.x, end.x);
    if (begin.y > end.y) std::swap(begin.y, end.y);
    uint16_t x1 = begin.x + radius;
    uint16_t x2 = end.x - radius;
    uint16_t y1 = begin.y + radius;
    uint16_t y2 = end.y - radius;
    drawLine(Point{begin.x, y1}, Point{begin.x, y2}, color);
    drawLine(Point{end.x, y1}, Point{end.x, y2}, color);
    drawLine(Point{x1, begin.y}, Point{x2, begin.y}, color);
    drawLine(Point{x1, end.y}, Point{x2, end.y}, color);
    drawPartialCircle(Point{x1, y1}, radius, color, 0b0000'1000);
    drawPartialCircle(Point{x2, y1}, radius, color, 0b0000'0100);
    drawPartialCircle(Point{x1, y2}, radius, color, 0b0000'0010);
    drawPartialCircle(Point{x2, y2}, radius, color, 0b0000'0001);
    return true;
}

bool Display::drawFilledRoundedRectangle(Point begin, Point end, uint16_t radius, Color color) {
    uint16_t x1 = begin.x + radius;
    uint16_t x2 = end.x - radius;
    uint16_t y1 = begin.y + radius;
    uint16_t y2 = end.y - radius;

    drawFilledRectangle(Point{begin.x, y1}, Point{end.x, y2}, color);
    drawFilledRectangle(Point{x1, begin.y}, Point{x2, y1}, color);
    drawFilledRectangle(Point{x1, y2}, Point{x2, end.y}, color);

    drawFilledCircle(Point{x1, y1}, radius, color);
    drawFilledCircle(Point{x2, y1}, radius, color);
    drawFilledCircle(Point{x1, y2}, radius, color);
    drawFilledCircle(Point{x2, y2}, radius, color);
    return true;
}

bool Display::drawCircle(Point center, uint16_t radius, Color color) {
    drawPartialCircle(center, radius, color, 0xFF);
    return true;
}

bool Display::drawFilledCircle(Point center, uint16_t radius, Color color) {
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    drawFilledCircle_helper(center, x, y, color);
    while (y >= x) {
        // for each pixel we will
        // draw all eight pixels

        x++;

        // check for decision parameter
        // and correspondingly
        // update d, x, y
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else
            d = d + 4 * x + 6;
        drawFilledCircle_helper(center, x, y, color);
    }

    return true;
}

void Display::drawPartialCircle(Point center, uint16_t radius, Color color, uint8_t parts) {
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    drawCircle_helper(center, x, y, color, parts);
    while (y >= x) {
        // for each pixel we will
        // draw all eight pixels

        x++;

        // check for decision parameter
        // and correspondingly
        // update d, x, y
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else
            d = d + 4 * x + 6;
        drawCircle_helper(center, x, y, color, parts);
    }
}

bool Display::drawBitmap(Point begin, size_t width, size_t height, Color bitmap[]) {
    for (size_t h = 0; h < height; h++) {
        for (size_t w = 0; w < width; w++) {
            setPixel(Point{begin.x + w, begin.y + h}, bitmap[w + (h * width)]);
        }
    }
    return true;
}

void Display::drawCircle_helper(Point center, int x, int y, Color color, uint8_t parts) {
    if (parts & 0b0000'0001) setPixel(Point{center.x + x, center.y + y}, color);
    if (parts & 0b0000'0010) setPixel(Point{center.x - x, center.y + y}, color);
    if (parts & 0b0000'0100) setPixel(Point{center.x + x, center.y - y}, color);
    if (parts & 0b0000'1000) setPixel(Point{center.x - x, center.y - y}, color);
    if (parts & 0b0001'0000) setPixel(Point{center.x + y, center.y + x}, color);
    if (parts & 0b0010'0000) setPixel(Point{center.x - y, center.y + x}, color);
    if (parts & 0b0100'0000) setPixel(Point{center.x + y, center.y - x}, color);
    if (parts & 0b1000'0000) setPixel(Point{center.x - y, center.y - x}, color);
}

void Display::drawFilledCircle_helper(Point center, uint16_t x, uint16_t y, Color color) {
    drawLine(Point{center.x + x, center.y + y}, Point{center.x - x, center.y + y}, color);
    drawLine(Point{center.x + x, center.y - y}, Point{center.x - x, center.y - y}, color);
    drawLine(Point{center.x + y, center.y + x}, Point{center.x - y, center.y + x}, color);
    drawLine(Point{center.x + y, center.y - x}, Point{center.x - y, center.y - x}, color);
}

}  // namespace graphics
}  // namespace microhal
