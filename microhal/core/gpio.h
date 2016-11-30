/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      GPIO driver
 *
 * @authors    Michal Karwatowski, Pawel Okas
 * created on: 20-01-2014
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

#ifndef _MICROHAL_GPIO_H_
#define _MICROHAL_GPIO_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "ports/manager/gpio_port.h"

namespace microhal {
/**
 * @addtogroup microHAL Interfaces
 * @{
 * @class GPIO
 * @}
 * \brief   This class provides basic GPIO functions.
 */
/* ************************************************************************************************
 * CLASS
 */
class GPIO {
 public:
    typedef activePort::GPIO::IOPin IOPin;
    typedef activePort::GPIO::Direction Direction;
    /**
     *
     */
    typedef enum {
        PushPull = activePort::GPIO::PushPull,    //!< PUSH_PULL
        OpenDrain = activePort::GPIO::OpenDrain,  //!< OPEN_DRAIN
    } OutputType;
    /**
     *
     */
    typedef enum {
        NoPull = activePort::GPIO::NoPull,  //!< NO_PULL
        PullUp = activePort::GPIO::PullUp,  //!< PULL_UP
    } PullType;
//---------------------------------------- constructors ---------------------------------------
    GPIO(const IOPin ioPin, const Direction dir, const PullType pull = NoPull,
            const OutputType type = PushPull) __attribute__((always_inline)):
    pin(ioPin.port, ioPin.pin, dir, static_cast<activePort::GPIO::PullType>(pull), static_cast<activePort::GPIO::OutputType>(type)) {
    }

    ~GPIO() {
    }
    //------------------------------------------ functions ----------------------------------------
    /** This function set pin to high state
     *
     * @param pin - pin number
     */
    static inline void set(const IOPin pin) __attribute__((always_inline)) {
        activePort::GPIO::set(pin.port, pin.pin);
    }
    /** This function set pin to high state*/
    inline void set() __attribute__((always_inline)) {
        pin.set();
    }

    static inline void reset(const IOPin pin) __attribute__((always_inline)) {
        activePort::GPIO::reset(pin.port, pin.pin);
    }

    inline void reset() __attribute__((always_inline)) {
        pin.reset();
    }

    static inline bool get(const IOPin pin) __attribute__((always_inline)) {
        return activePort::GPIO::get(pin.port, pin.pin);
    }

    inline bool get() const __attribute__((always_inline)) {
        return pin.get();
    }
    /**
     * @brief Check if pin is set as high
     *
     * @param pin - IOpin descriptor
     *
     * @retval true - if pin is high
     * @retval false - if pin is low
     */
    static inline bool isSet(const IOPin pin) __attribute__((always_inline)) {
        return activePort::GPIO::isSet(pin.port, pin.pin);
    }
    /**
     * @brief Check if pin is set as high
     *
     * @retval true - if pin is high
     * @retval false - if pin is low
     */
    inline bool isSet() const __attribute__((always_inline)) {
        return pin.isSet();
    }
    /**
     * @brief Check if pin is set as low
     *
     * @param pin - IOpin descriptor
     *
     * @retval true - if pin is low
     * @retval false - if pin is high
     */
    static inline bool isReset(const IOPin pin) __attribute__((always_inline)) {
        return activePort::GPIO::isReset(pin.port, pin.pin);
    }
    /**
     * @brief Check if pin is set as low
     *
     * @retval true - if pin is low
     * @retval false - if pin is high
     */
    inline bool isReset() const __attribute__((always_inline)) {
        return pin.isReset();
    }
    /**
     * Set pin state to opposite
     */
    static inline void toggle(const IOPin pin) __attribute__((always_inline)) {
        activePort::GPIO::toggle(pin.port, pin.pin);
    }
    /**
     * Set pin state to opposite
     */
    inline void toggle() __attribute__((always_inline)) {
        pin.toggle();
    }
    /**
     *
     * @param port
     * @param pin
     * @param direction
     */
    static inline void setDirection(const IOPin pin, const Direction direction) __attribute__((always_inline)) {
        activePort::GPIO::setDirection(pin.port, pin.pin, direction);
    }

    inline void setDirection(const Direction direction) __attribute__((always_inline)) {
        pin.setDirection(direction);
    }

    static inline void setPullType(const IOPin pin, const PullType pullType) {
        activePort::GPIO::setPullType(pin.port, pin.pin, static_cast<activePort::GPIO::PullType>(pullType));
    }

    inline void setPullType(const PullType pullType) {
        pin.setPullType(static_cast<activePort::GPIO::PullType>(pullType));
    }
    /**
     * @brief Returns state of pin
     *
     * @retval true - if pin is high
     * @retval false - if pin is low
     */
    operator bool() __attribute__((always_inline)) {
        return get();
    }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

    /* This is nonstandard usage of that operators!*/

    /**
     * @brief Sets pin state to state
     *
     * @param state - value that is to be set on pin
     */
    void operator=(bool state) __attribute__((always_inline)) {
        state ? set() : reset();
    }
    /**
     * @brief Sets pin state to state
     *
     * @param state - value that is to be set on pin
     */
    void operator=(const GPIO &state) __attribute__((always_inline)) {
        state.get() ? set() : reset();
    }
#pragma GCC diagnostic pop
    /**
     * @brief Returns negated state of pin
     *
     * @retval true - if pin is low
     * @retval false - if pin is high
     */
    bool operator!() __attribute__((always_inline)) {
        return isReset();
    }
    /**
     * @brief Compares state of pins
     * @param state
     * @retval true - if pins are equal
     * @retval false - if pin are not equal
     */
    bool operator==(bool state) __attribute__((always_inline)) {
        return (get() == state);
    }
    /**
     * @brief Compares state of pins
     *
     * @param state
     * @retval true - if pins not are equal
     * @retval false - if pin are equal
     */
    bool operator!=(bool state) __attribute__((always_inline)) {
        return (get() != state);
    }
    /** This function provides access to not portable function of actual GPIO port.
     *
     * @return reference to actual port GPIO
     */
    activePort::GPIO & useNotPortableFunction() __attribute__((always_inline)) {
        return pin;
    }

 private:
    activePort::GPIO pin;
};

} // namespace microhal

#endif // _MICROHAL_GPIO_H_
