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
     * This enum contain port list.
     */
    typedef enum { PortA } Port;
    /**
     *
     */
    typedef uint_fast8_t Pin;
    /**
     *
     */

    Port port;
    Pin pin;
    constexpr IOPin(const Port port, const Pin pin) : port(port), pin(pin) {}
};
}  // namespace linux
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STM32F4XX_IOPIN_H_ */
