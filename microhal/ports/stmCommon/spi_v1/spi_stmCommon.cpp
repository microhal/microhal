/*
 * spi_stmCommon.cpp
 *
 *  Created on: Nov 13, 2019
 *      Author: pokas
 */

#include "spi_stmCommon.h"
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#if _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1                                      // Check if driver is compatible with selected MCU
#if ENABLED_ANY_SPI(POLLING) || ENABLED_ANY_SPI(INTERRUPT) || ENABLED_ANY_SPI(DMA)  // Check if driver is enabled in microhal port config

#include <ports/stmCommon/interruptController/interruptController.h>

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

static const int32_t prescalerValues[] = {2, 4, 8, 16, 32, 64, 128, 256};

SPI::Prescaler SPI::findClosestPrescaler(uint32_t prescaler) {
    uint32_t bestMatch = std::numeric_limits<uint32_t>::max();
    size_t bestPos = 0;
    for (size_t i = 0; i < sizeof(prescalerValues) / sizeof(prescalerValues[0]); i++) {
        uint32_t distance = std::abs(prescalerValues[i] - static_cast<int32_t>(prescaler));
        if (bestMatch > distance) {
            bestMatch = distance;
            bestPos = i;
        }
    }
    return static_cast<Prescaler>(bestPos);
}

SPI::Prescaler SPI::findLargerOrEqualPrescaler(uint32_t prescaler) {
    for (size_t i = 0; i < sizeof(prescalerValues) / sizeof(prescalerValues[0]); i++) {
        if (prescalerValues[i] >= prescaler) return static_cast<Prescaler>(i);
    }
    return static_cast<Prescaler>((sizeof(prescalerValues) / sizeof(prescalerValues[0])) - 1);
}

void SPI::enableGlobalInterrupt(uint32_t priority) {
    const auto spiNumber = getNumber();
    enableSPIInterrupt(spiNumber, priority);
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // ENABLED_ANY_SPI(POLLING) || ENABLED_ANY_SPI(INTERRUPT) || ENABLED_ANY_SPI(DMA)
#endif  // _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1
