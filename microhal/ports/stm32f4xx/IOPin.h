/*
 * IOPin.h
 *
 *  Created on: Sep 12, 2018
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STM32F4XX_IOPIN_H_
#define SRC_MICROHAL_PORTS_STM32F4XX_IOPIN_H_

#include <cstdint>
#include "device/stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

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
#if defined(GPIOF_BASE)
        PortF = GPIOF_BASE,  //!< PortF
#endif
#if defined(GPIOG_BASE)
        PortG = GPIOG_BASE,  //!< PortG
#endif
#if defined(GPIOH_BASE)
        PortH = GPIOH_BASE,  //!< PortG
#endif
#if defined(GPIOI_BASE)
        PortI = GPIOI_BASE,  //!< PortI
#endif
    } Port;
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
    constexpr bool operator==(const IOPin &b) const {
        if (port != b.port) return false;
        if (pin != b.pin) return false;
        return true;
    }
};
}  // namespace stm32f4xx
}  // namespace microhal

#endif /* SRC_MICROHAL_PORTS_STM32F4XX_IOPIN_H_ */
