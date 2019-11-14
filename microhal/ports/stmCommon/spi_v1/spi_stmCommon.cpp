/*
 * spi_stmCommon.cpp
 *
 *  Created on: Nov 13, 2019
 *      Author: pokas
 */

#include "spi_stmCommon.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

SPI::Prescaler SPI::findClosestPrescaler(uint32_t prescaler) {
    const int32_t prescalerValues[] = {2, 4, 8, 16, 32, 64, 128, 256};
    uint32_t bestMatch = std::numeric_limits<uint32_t>::max();
    size_t bestPos = 0;
    for (size_t i = 0; i < sizeof(prescalerValues) / sizeof(prescalerValues[0]); i++) {
        uint32_t distance = std::abs(prescalerValues[i] - static_cast<int32_t>(prescaler));
        if (bestMatch > distance) {
            bestMatch = distance;
            bestPos = i;
        }
    }
    return static_cast<Prescaler>(bestPos << 3);
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
