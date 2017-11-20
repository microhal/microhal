/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 27-12-2016
 * last modification: 27-12-2016
 *
 * @copyright Copyright (c) 2016, microHAL
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

#ifndef _MICROHAL_GPIO_NRF51_H_
#define _MICROHAL_GPIO_NRF51_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "device/nrf.h"

namespace microhal {
template <class Derived>
class GPIO_Interface {};
namespace nrf51 {
/* ************************************************************************************************
 * CLASS
 */

class GPIO : public GPIO_Interface<nrf51::GPIO> {
    using GPIO_Type = NRF_GPIO_Type;

 public:
    //---------------------------------------- typedefs -----------------------------------------//
    /**
     * This enum contain port list.
     */
    typedef enum {
        P0 = NRF_GPIO_BASE,  //!< Port0
    } Port;
    /**
     *
     */
    typedef uint_fast8_t Pin;
    /**
     *
     */
    struct IOPin {
        const Port port;
        const Pin pin;
        constexpr IOPin(const Port port, const Pin pin) : port(port), pin(pin) {}
    };
    /**
     * @brief Possible pin directions
     */
    typedef enum : uint8_t {
        Input = 0,  //!< INPUT
        Output = 1  //!< OUTPUT
    } Direction;
    /**
     * Possible pin output types
     */
    typedef enum : uint16_t {
        PushPull = 0,       //!< PUSH_PULL
        OpenDrain = 6 << 8  //!< OPEN_DRAIN
    } OutputType;
    /**
     * Possible pull type
     */
    typedef enum : uint8_t {
        NoPull = 0,        //!< NO_PULL
        PullUp = 3 << 2,   //!< PULL_UP
        PullDown = 1 << 2  //!< PULL_DOWN
    } PullType;

 private:
    /**
     *
     */
    typedef struct __attribute__((packed)) {
        uint8_t mode;
        uint8_t type;
        uint16_t pull;
    } PinConfiguration;
    static_assert(sizeof(PinConfiguration) == 4, "");

 public:
    //--------------------------------------- constructors --------------------------------------//
    /**
     * @brief Constructor of GPIO class
     *
     * @param port - port to be used
     * @param pin - pin on used port
     * @param dir - direction of port
     * @param pull - pull up setting
     * @param type - output type setting
     */
    inline GPIO(const Port port, const Pin pin, const Direction dir, const PullType pull = NoPull, const OutputType type = PushPull)
        __attribute__((always_inline))
        : pin(pin), port(port) {
        pinInitialize(port, pin, PinConfiguration{dir, type, pull});
    }

    ~GPIO() {}
    //------------------------------------ static functions -------------------------------------//
    /** @brief This function set pins to high state.
     *
     * @param port - port name
     * @param mask - if bit in mask is set then corresponding pin will be set
     */
    static inline void setMask(Port port, uint32_t mask) __attribute__((always_inline)) {
        reinterpret_cast<volatile GPIO_Type *>(port)->OUTSET = mask;
    }
    /** @brief This function set pins to low state.
     *
     * @param port - port name
     * @param mask - if bit in mask is set then corresponding pin will be reset
     */
    static inline void resetMask(Port port, uint32_t mask) __attribute__((always_inline)) {
        reinterpret_cast<volatile GPIO_Type *>(port)->OUTCLR = mask;
    }
    /** @brief This function return port state.
     *
     * @param port - port name
     * @return - read value of pins. If pin zero is set then LSB in returned value will be set.
     */
    static inline uint32_t getMask(Port port) __attribute__((always_inline)) { return reinterpret_cast<volatile GPIO_Type *>(port)->IN; }
    /** This function set pin to high state.
     *
     * @param port - port name
     * @param pin - pin number
     */
    static inline void set(Port port, Pin pin) { setMask(port, 1 << pin); }
    /** This function set pin to high state. */
    inline void set() { set(port, pin); }
    /** This function set pin to low state.
     *
     * @param port - port name
     * @param pin - pin number
     */
    static inline void reset(Port port, Pin pin) { resetMask(port, 1 << pin); }
    /** This function set pin to low state.*/
    inline void reset() { reset(port, pin); }
    /** This function read pin state
     *
     * @param port - port name
     * @param pin - pin number
     * @return
     */
    static inline bool get(Port port, Pin pin) { return (getMask(port) & static_cast<uint32_t>(1 << pin)); }
    /** This function read pin state*/
    inline bool get() const { return get(port, pin); }
    /** This function check for pin set.
     *
     * @param port - port name
     * @param pin - pin number
     * @return
     */
    static inline bool isSet(Port port, Pin pin) { return get(port, pin); }
    /** This function check for pin set.*/
    inline bool isSet() const { return isSet(port, pin); }
    /** This function check for pin reset.
     *
     * @param port - port name
     * @param pin - pin number
     * @return
     */
    static inline bool isReset(Port port, Pin pin) { return !get(port, pin); }
    /** This function check for pin reset.*/
    inline bool isReset() const { return isReset(port, pin); }
    /** Sets pin to opposite state
     *
     * @param port - port name
     * @param pin - pin number
     */
    static inline void toggle(Port port, Pin pin) { (isSet(port, pin)) ? (reset(port, pin)) : (set(port, pin)); }
    /** Sets pin to opposite state*/
    inline void toggle() { toggle(port, pin); }
    /** This function set pin direction.
     *
     * @param port - port name
     * @param pin - pin number
     * @param direction - pin direction
     */
    static inline void setDirection(const Port port, const Pin pin, const Direction direction) {
        if (direction == Direction::Input) {
            reinterpret_cast<volatile GPIO_Type *>(port)->DIRCLR = 1 << pin;
        } else {
            reinterpret_cast<volatile GPIO_Type *>(port)->DIRSET = 1 << pin;
        }
    }
    /** This function set pin direction.
     *
     * @param direction - pin direction
     */
    inline void setDirection(const Direction direction) { setDirection(port, pin, direction); }
    /** This function set pin pull type
     *
     * @param port
     * @param pin
     * @param pullType
     */
    static inline void setPullType(Port port, Pin pin, PullType pullType) {
        uint32_t tmp = reinterpret_cast<volatile GPIO_Type *>(port)->PIN_CNF[pin];
        tmp &= ~(1 << 2 | 1 << 3);
        tmp |= pullType;
        reinterpret_cast<volatile GPIO_Type *>(port)->PIN_CNF[pin] = tmp;
    }
    /** This function set pin pull type
     *
     * @param pullType
     */
    inline void setPullType(const PullType pullType) { setPullType(port, pin, pullType); }
    //----------------------------- not portable functions -----------------------------------

 protected:
    const Pin pin;
    const Port port;

    static void pinInitialize(Port port, Pin pin, PinConfiguration configuration) {
        setDirection(port, pin, static_cast<Direction>(configuration.mode));
        setPullType(port, pin, static_cast<PullType>(configuration.pull));
        uint32_t tmp = reinterpret_cast<volatile GPIO_Type *>(port)->PIN_CNF[pin];
        tmp &= ~(1 << 10 | 1 << 9 | 1 << 8);
        tmp |= configuration.type;
        reinterpret_cast<volatile GPIO_Type *>(port)->PIN_CNF[pin] = tmp;
    }
    //----------------------------------------- friends -----------------------------------------//
    friend class IOManager;
};

}  // namespace nrf51
}  // namespace microhal

#endif  // _MICROHAL_GPIO_NRF51_H_
