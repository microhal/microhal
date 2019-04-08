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
    constexpr Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
}  // namespace graphics
}  // namespace microhal
#endif /* _MICROHAL_GRAPHICS_COLOR_H_ */
