/*
 * IOPin.h
 *
 *  Created on: Sep 12, 2018
 *      Author: pokas
 */

#ifndef _MICROHAL_IOPIN_LINUX_H_
#define _MICROHAL_IOPIN_LINUX_H_

#include <cstdint>

namespace microhal {
namespace linux {

struct IOPin {
    /**
     *
     */
    typedef int_fast16_t Pin;
    /**
     *
     */

    Pin pin;
    constexpr IOPin(const Pin pin) : pin(pin) {}
    constexpr IOPin() : pin(-1) {}
};
}  // namespace linux
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STM32F4XX_IOPIN_H_ */
