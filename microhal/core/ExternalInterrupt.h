/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      ExternalInterrupt
 *
 * @authors    Pawel
 * created on: 20-03-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015, microHAL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_EXTERNALINTERRUPT_H_
#define _MICROHAL_EXTERNALINTERRUPT_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "gpio.h"
#include "platform.h"
#include "ports/manager/externalInterrupt_port.h"
#include "signalSlot/signalSlot.h"

namespace microhal {
/**
 * @addtogroup microHAL Interfaces
 * @{
 * @class ExternalInterrupt
 * @}
 * \brief   This class manage External interrupt .
 */
/* ************************************************************************************************
 * CLASS
 */
class ExternalInterrupt {
 public:
    enum class Trigger {
        OnFallingEdge = activePort::ExternalInterrupt::TriggerOnFalling,
        OnRisingEdge = activePort::ExternalInterrupt::TriggerOnRising,
        OnAnyEdge = activePort::ExternalInterrupt::TriggerOnEdge
    };
//---------------------------------------- constructors -----------------------------------------//
    explicit constexpr ExternalInterrupt(GPIO::IOPin ioPin) noexcept __attribute__((always_inline))
        : ioPin(ioPin) {
    }
//------------------------------------------ functions ------------------------------------------//
    inline static void init() noexcept {
        activePort::ExternalInterrupt::init();
    }
    /**
     *
     * @param interruptFunction - pointer to interrupt function
     * @param trigger - kind of event when interrupt will be called.
     * @param ioPin - external pin of interrupt.
     *
     * @retval true if interrupt was successfully connected.
     * @retval false if this interrupt is already connected to another function.
     */
    static inline bool connect(void (*interruptFunction)(void), Trigger trigger, GPIO::IOPin ioPin) noexcept __attribute__ ((always_inline)) {
        return activePort::ExternalInterrupt::connect(interruptFunction, static_cast<activePort::ExternalInterrupt::Trigger>(trigger), ioPin.port, ioPin.pin);
    }
    /**
     *
     * @param interruptFunction - pointer to interrupt function
     * @param trigger - kind of event when interrupt will be called.
     *
     * @retval true if interrupt was successfully connected.
     * @retval false if this interrupt is already connected to another function.
     */
    inline bool connect(void (*interruptFunction)(void), Trigger trigger) noexcept __attribute__ ((always_inline))  {
        return connect(interruptFunction, trigger, ioPin);
    }

    template<typename T>
    static inline bool connect(const T &slot, const typename T::type &object, Trigger trigger, GPIO::IOPin ioPin) noexcept {
        return activePort::ExternalInterrupt::connect(slot, object, static_cast<activePort::ExternalInterrupt::Trigger>(trigger), ioPin.port, ioPin.pin);
    }

    template<typename T>
    inline bool connect(const T &slot, const typename T::type &object, Trigger trigger) noexcept {
        return connect(slot, object, trigger, ioPin);
    }
    /**
     * @brief This function enable external interrupt. If interrupt is not connected the function return false;
     *
     * @retval true if interrupt is connected and was enabled.
     * @retval false if interrupt is unconnected.
     */
    static inline void enable(microhal::GPIO::IOPin ioPin) noexcept __attribute__((always_inline)) {
        activePort::ExternalInterrupt::enable(ioPin.port, ioPin.pin);
    }
    /**
     * @brief This function enable external interrupt. If interrupt is not connected the function return false;
     *
     * @retval true if interrupt is connected and was enabled.
     * @retval false if interrupt is unconnected.
     */
    inline bool enable() noexcept __attribute__((always_inline)) {
        enable(ioPin);
        return true;
    }
    /**
	 * @brief This function enable external interrupt. If interrupt is not connected the function return false;
	 *
	 * @retval true if interrupt is connected and was enabled.
	 * @retval false if interrupt is unconnected.
	 */
    static inline bool disable(microhal::GPIO::IOPin ioPin) noexcept __attribute__((always_inline)) {
        activePort::ExternalInterrupt::disable(ioPin.port, ioPin.pin);
        return true;
    }
    /**
     * @brief This function enable external interrupt. If interrupt is not connected the function return false;
     *
     * @retval true if interrupt is connected and was enabled.
     * @retval false if interrupt is unconnected.
     */
    inline bool disable() noexcept __attribute__((always_inline)) {
        return disable(ioPin);
    }

    static inline bool isEnabled(microhal::GPIO::IOPin ioPin) noexcept __attribute__((always_inline)) {
        return activePort::ExternalInterrupt::isEnabled(ioPin.port, ioPin.pin);
    }

    inline bool isEnabled() noexcept __attribute__((always_inline)) {
        return isEnabled(ioPin);
    }

 private:
//------------------------------------------- variables -----------------------------------------//
    GPIO::IOPin ioPin;
};

}  // namespace microhal

#endif  // _MICROHAL_EXTERNALINTERRUPT_H_
