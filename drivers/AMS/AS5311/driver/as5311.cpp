/*
 * as5311.cpp
 *
 *  Created on: Nov 14, 2019
 *      Author: pokas
 */
#include "as5311.h"

namespace microhal {
bool AS5311::update() {
    uint8_t data[3];
    if (spi.readBuffer(data, sizeof(data), true)) {
        uint32_t frame = (uint32_t)data[0] << 11;
        frame |= (uint32_t)data[1] << 3;
        frame |= (uint32_t)data[2] >> 5;
        uint_fast8_t ones = 0;
        for (uint_fast8_t i = 1; i < 18; i++) {
            if (frame & (1 << i)) {
                ones++;
            }
        }
        position = frame >> 6;
        status = static_cast<Status>((frame >> 1) & 0b11111);
        return (ones & 0b1) == (frame & 0b1);
    }
    return false;
}
}  // namespace microhal
