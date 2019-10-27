/*
 * IOPin.h
 *
 *  Created on: Sep 12, 2018
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_STMCOMMON_IOPIN_H_
#define _MICROHAL_PORTS_STMCOMMON_IOPIN_H_

#include <cstdint>
#include "stmCommonDefines.h"

#include _MICROHAL_INCLUDE_PORT_DEVICE  // stmCommonDefines.h have to be included before this

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

namespace ioPinDetail {
#define _MICROHAL_HAS_PORT(x)                                        \
    template <class T>                                               \
    struct has_##x {                                                 \
        typedef char yes;                                            \
        typedef yes (&no)[2];                                        \
        template <class U>                                           \
        static yes test(decltype(U::x) *);                           \
        template <class U>                                           \
        static no test(...);                                         \
        static bool const value = sizeof(test<T>(0)) == sizeof(yes); \
    };

_MICROHAL_HAS_PORT(PortA)
_MICROHAL_HAS_PORT(PortB)
_MICROHAL_HAS_PORT(PortC)
_MICROHAL_HAS_PORT(PortD)
_MICROHAL_HAS_PORT(PortE)
_MICROHAL_HAS_PORT(PortF)
_MICROHAL_HAS_PORT(PortG)
_MICROHAL_HAS_PORT(PortH)
_MICROHAL_HAS_PORT(PortI)
}  // namespace ioPinDetail

struct IOPin {
    /**
     * This enum contain port list.
     */
    typedef enum {
#if defined(GPIOA_BASE)
        PortA = GPIOA_BASE,  //!< PortA
#endif
#if defined(GPIOB_BASE)
        PortB = GPIOB_BASE,  //!< PortB
#endif
#if defined(GPIOC_BASE)
        PortC = GPIOC_BASE,  //!< PortC
#endif
#if defined(GPIOD_BASE)
        PortD = GPIOD_BASE,  //!< PortD
#endif
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

    constexpr IOPin(Port port, Pin pin) : port(port), pin(pin) {}
    constexpr bool operator==(const IOPin &b) const {
        if (port != b.port) return false;
        if (pin != b.pin) return false;
        return true;
    }
    constexpr static bool isPortAPresent() { return ioPinDetail::has_PortA<Port>::value; }
    constexpr static bool isPortBPresent() { return ioPinDetail::has_PortB<Port>::value; }
    constexpr static bool isPortCPresent() { return ioPinDetail::has_PortC<Port>::value; }
    constexpr static bool isPortDPresent() { return ioPinDetail::has_PortD<Port>::value; }
    constexpr static bool isPortEPresent() { return ioPinDetail::has_PortE<Port>::value; }
    constexpr static bool isPortFPresent() { return ioPinDetail::has_PortF<Port>::value; }
    constexpr static bool isPortGPresent() { return ioPinDetail::has_PortG<Port>::value; }
    constexpr static bool isPortHPresent() { return ioPinDetail::has_PortH<Port>::value; }
    constexpr static bool isPortIPresent() { return ioPinDetail::has_PortI<Port>::value; }
};
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_IOPIN_H_ */
