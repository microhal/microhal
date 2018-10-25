/*
 * IOPin.h
 *
 *  Created on: Sep 12, 2018
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STM32F3XX_IOPIN_H_
#define SRC_MICROHAL_PORTS_STM32F3XX_IOPIN_H_

#include <cstdint>
#include "device/stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {

struct IOPin {
    /**
     * This enum contain port list.
     */
    typedef enum {
        PortA = GPIOA_BASE,  //!< PortA
        PortB = GPIOB_BASE,  //!< PortB
        PortC = GPIOC_BASE,  //!< PortC
        PortD = GPIOD_BASE,  //!< PortD
#if defined(GPIOE_BASE)
        PortE = GPIOE_BASE,  //!< PortE
#endif
        PortF = GPIOF_BASE  //!< PortF
    } Port;
    /**
     *
     */
    typedef uint_fast8_t Pin;
    /**
     *
     */

    const Port port;
    const Pin pin;
    constexpr IOPin(const Port port, const Pin pin) : port(port), pin(pin) {}
};
}  // namespace stm32f3xx
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STM32F3XX_IOPIN_H_ */