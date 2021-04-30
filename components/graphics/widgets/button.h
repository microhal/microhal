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

#ifndef SRC_GRAPHICS_WIDGETS_BUTTON_H_
#define SRC_GRAPHICS_WIDGETS_BUTTON_H_

#include <algorithm>
#include <string_view>
#include "display.h"
#include "font.h"
#include "fonts/defaultFont.h"

namespace microhal {
namespace graphics {

class Button {
 public:
    enum class Mode { SquareNoBorder, SquareWithBorder, RoundedNoBorder, RoundedWithBorder };
    Button(Point begin, Point end, std::string_view text) : begin(begin), end(end), text(text) {}
    void setMode(Mode m) { mode = m; }

    void fillColor(Color color) { m_fillColor = color; }
    const Color& fillColor() const { return m_fillColor; }
    void borderColor(Color color) { m_borderColor = color; }
    const Color& borderColor() const { return m_borderColor; }

    void draw(Display& display) {
        if (mode == Mode::SquareNoBorder) {
            display.drawFilledRectangle(begin, end, m_fillColor);
        } else if (mode == Mode::SquareWithBorder) {
            display.drawFilledRectangle(Point(begin.x + 1, begin.y + 1), Point(end.x - 1, end.y - 1), m_fillColor);
            display.drawRectangle(begin, end, m_borderColor);
        } else if (mode == Mode::RoundedNoBorder) {
            display.drawFilledRoundedRectangle(begin, end, 5, m_fillColor);
        } else if (mode == Mode::RoundedWithBorder) {
            display.drawFilledRoundedRectangle(begin, end, 5, m_fillColor);
            display.drawRoundedRectangle(begin, end, 5, m_borderColor);
        }

        uint_fast16_t textOffsetFromCenter = (text.length() * 5) / 2;
        Point textBegin = centerPoint();
        textBegin.x -= textOffsetFromCenter;
        textBegin.y -= _font->getHeight() / 2 + 1;
        _font->draw(display, textBegin, 0, text);
    }

    Point centerPoint() {
        Point tmpBegin = begin;
        Point tmpEnd = end;
        if (tmpBegin.x > tmpEnd.x) std::swap(tmpBegin.x, tmpEnd.x);
        if (tmpBegin.y > tmpEnd.y) std::swap(tmpBegin.y, tmpEnd.y);
        Point diff = end - begin;
        Point center(tmpBegin.x + diff.x / 2, tmpBegin.y + diff.y / 2);
        return center;
    }

    Font* const font() { return _font; }

 private:
    Point begin;
    Point end;
    std::string_view text;
    Font* _font = new DefaultFont();
    Color m_fillColor = {128, 128, 0};
    Color m_borderColor = {0, 128, 128};
    Mode mode = Mode::RoundedWithBorder;

    Color gradient(Color color, uint8_t step) {
        uint32_t startColor = color.r + (color.g << 8) + (color.b << 16);
        startColor += step;

        int x = startColor % 256;

        int r = 0, g = 0, b = 0;

        switch (startColor / 256) {
                // clang-format off
	  case 0: r = 255; g = x;       break;
	  case 1: g = 255; r = 255 - x; break;
	  case 2: g = 255; b = x;       break;
	  case 3: b = 255; g = 255 - x; break;
	  case 4: b = 255; r = x;       break;
	  case 5: r = 255; b = 255 - x; break;
                // clang-format on
        }
        return Color{r, g, b};
    }
};

}  // namespace graphics
}  // namespace microhal

#endif /* SRC_GRAPHICS_WIDGETS_BUTTON_H_ */
