/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 02-08-2018
 * last modification: 02-08-2018
 *
 * @copyright Copyright (c) 2018, Pawel Okas
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

#ifndef _MICROHAL_GPIO_STM32F0XX_H_
#define _MICROHAL_GPIO_STM32F0XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "device/stm32f0xx.h"

namespace microhal {
namespace stm32f0xx {

/* **************************************************************************************************************************************************
 * CLASS
 */
class GPIO {
 public:
    /**
     * This enum contain port list.
     */
    typedef enum {
        PortA = GPIOA_BASE,  //!< PortA
        PortB = GPIOB_BASE,  //!< PortB
        PortC = GPIOC_BASE,  //!< PortC
#if defined(GPIOD_BASE)
        PortD = GPIOD_BASE,  //!< PortD
#endif
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
    typedef enum : uint8_t {
        PushPull = 0,  //!< PUSH_PULL
        OpenDrain = 1  //!< OPEN_DRAIN
    } OutputType;
    /**
     * Possible pull type
     */
    typedef enum : uint8_t {
        NoPull = 0,   //!< NO_PULL
        PullUp = 1,   //!< PULL_UP
        PullDown = 2  //!< PULL_DOWN
    } PullType;

    /**
     * @brief Possible pin speed
     */
    typedef enum : uint8_t {
        LowSpeed = 0x00,     //!< LOW_SPEED
        MediumSpeed = 0x01,  //!< MEDIUM_SPEED
        HighSpeed = 0x03     //!< HIGH_SPEED
    } Speed;

 private:
    /**
     *
     */
    typedef struct __attribute__((packed)) {
        uint8_t mode;
        uint8_t type;
        uint8_t pull;
        uint8_t speed;
    } PinConfiguration;
    static_assert(sizeof(PinConfiguration) == 4, "");
    /**
     *
     */
    typedef enum : uint8_t {
        Serial = 0x01,  //!< Serial
        I2C = 0x01,     //!< I2C
    } AlternateFunction;

 public:
    inline GPIO(const Port port, const Pin pin, const Direction dir, const PullType pull = NoPull, const OutputType type = PushPull,
                Speed speed = HighSpeed) __attribute__((always_inline))
    : pin(pin), port(port) {
        pinInitialize(port, pin, PinConfiguration{dir, type, pull, speed});
    }

    ~GPIO() {}

    /** @brief This function set pins to high state.
     *
     * @param port - port name
     * @param mask - if bit in mask is set then corresponding pin will be set
     */
    static inline void setMask(Port port, uint32_t mask) { reinterpret_cast<volatile GPIO_TypeDef *>(port)->BSRR = mask; }
    /** @brief This function set pins to low state.
     *
     * @param port - port name
     * @param mask - if bit in mask is set then corresponding pin will be reset
     */
    static inline void resetMask(Port port, uint32_t mask) __attribute__((always_inline)) {
        reinterpret_cast<volatile GPIO_TypeDef *>(port)->BRR = mask;
    }
    /** @brief This function return port state.
     *
     * @param port - port name
     * @return - read value of pins. If pin zero is set then LSB in returned value
     * will be set.
     */
    static inline uint16_t getMask(Port port) __attribute__((always_inline)) { return reinterpret_cast<volatile GPIO_TypeDef *>(port)->IDR; }
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
    static inline bool get(Port port, Pin pin) { return (getMask(port) & static_cast<uint16_t>(1 << pin)); }
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
    static inline void setDirection(Port port, Pin pin, Direction direction) {
        volatile GPIO_TypeDef *port_ptr = reinterpret_cast<volatile GPIO_TypeDef *>(port);
        uint32_t moder = port_ptr->MODER;
        moder &= ~((0b11) << (pin * 2));           // clear old configuration
        moder |= (0x03 & direction) << (pin * 2);  // set new configuration
        port_ptr->MODER = moder;
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
    static inline void setPullType(const Port port, const Pin pin, const PullType pullType) {
        volatile GPIO_TypeDef *port_ptr = reinterpret_cast<volatile GPIO_TypeDef *>(port);
        uint32_t pupdr = port_ptr->PUPDR;
        pupdr &= ~(0b11 << (pin * 2));   // clear old configuration
        pupdr |= pullType << (pin * 2);  // set new configuration
        port_ptr->PUPDR = pupdr;
    }
    /** This function set pin pull type
     *
     * @param pullType
     */
    inline void setPullType(const PullType pullType) { setPullType(port, pin, pullType); }
    //----------------------------- not portable functions
    //-----------------------------------
    /** This function is not portable, when called set pin speed
     *
     * @param port - port name
     * @param pin - pin number
     * @param speed - pin speed
     */
    static inline void setSpeed(const Port port, const Pin pin, const Speed speed) {
        reinterpret_cast<volatile GPIO_TypeDef *>(port)->OSPEEDR |= speed << (pin * 2);
    }
    /** This function set pin speed
     *
     * @param pin -
     * @param speed - pin speed
     */
    static inline void setSpeed(IOPin pin, const Speed speed) { setSpeed(pin.port, pin.pin, speed); }
    /** This function set pin speed
     *
     * @param speed - pin speed
     */
    inline void setSpeed(const Speed speed) { setSpeed(port, pin, speed); }

 protected:
    const Pin pin;
    const Port port;

    static void pinInitialize(const Port port, const uint_fast8_t pin, const PinConfiguration configuration);

    static void setAlternateFunction(Port port, Pin pin, AlternateFunction function, PullType pull = NoPull, OutputType type = PushPull,
                                     Speed speed = HighSpeed) __attribute__((always_inline)) {
        // 0x02 in mode enable alternate function
        pinInitialize(port, pin, PinConfiguration{static_cast<uint8_t>(0x02 | (function << 4)), type, pull, speed});
    }

    friend class IOManager;
};

}  // namespace stm32f0xx
}  // namespace microhal

#endif  // _MICROHAL_GPIO_STM32F0XX_H_
