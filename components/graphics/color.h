/*
 * color.h
 *
 *  Created on: Apr 7, 2019
 *      Author: pokas
 */

#ifndef _MICROHAL_GRAPHICS_COLOR_H_
#define _MICROHAL_GRAPHICS_COLOR_H_

#include <cstdint>
namespace microhal {
namespace graphics {
class Color {
 public:
    constexpr Color() : r(0), g(0), b(0) {}
    constexpr Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    constexpr bool operator==(const Color &c) const {
        if (r != c.r) return false;
        if (g != c.g) return false;
        if (b != c.b) return false;
        return true;
    }

    uint8_t r;
    uint8_t g;
    uint8_t b;
};
}  // namespace graphics
}  // namespace microhal
#endif /* _MICROHAL_GRAPHICS_COLOR_H_ */
