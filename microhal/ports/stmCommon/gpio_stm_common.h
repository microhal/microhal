/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      GPIO port driver
 *
 * @authors    Pawel Okas
 * created on: 17-04-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014-2019, Pawel Okas
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

#ifndef _MICROHAL_GPIO_STM_COMMON_H_
#define _MICROHAL_GPIO_STM_COMMON_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <interfaces/gpio_interface.h>
#include <cstdint>
#include <type_traits>
#include "IOPin.h"
#include "stmCommonDefines.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif
/* **************************************************************************************************************************************************
 * CLASS
 */
namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

class GPIOCommonBase : public microhal::GPIO {
 public:
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
        Speed speed;
    } PinConfiguration;
    static_assert(sizeof(PinConfiguration) == 4, "");

 public:
    using Port = IOPin::Port;
    //--------------------------------------- constructors --------------------------------------//
    constexpr GPIOCommonBase(IOPin pin) : pin(pin) {}
    /**
     * @brief Constructor of GPIO class
     *
     * @param port - port to be used
     * @param pin - pin on used port
     * @param dir - direction of port
     * @param pull - pull up setting
     * @param type - output type setting
     */
    constexpr GPIOCommonBase(IOPin pin, Direction dir, PullType pull = NoPull, OutputType type = PushPull, Speed speed = HighSpeed) : pin(pin) {
        pinInitialize(PinConfiguration{dir, type, pull, speed});
    }

    virtual ~GPIOCommonBase() {}

    bool set() final {
        setMask(pin.port, 1 << pin.pin);
        return true;
    }
    bool reset() final {
        resetMask(pin.port, 1 << pin.pin);
        return true;
    }
    /** This function read pin state*/
    bool get() const final {
        uint32_t value = reinterpret_cast<volatile GPIO_TypeDef *>(pin.port)->IDR;
        return value & static_cast<uint32_t>(1 << pin.pin);
    }

    bool configure(microhal::GPIO::Direction dir, microhal::GPIO::OutputType type, microhal::GPIO::PullType pull) final {
        pinInitialize(PinConfiguration{dir, type, pull, MediumSpeed});
        return true;
    }
    //------------------------------------ static functions -------------------------------------//
    /** @brief This function set pins to high state.
     *
     * @param port - port name
     * @param mask - if bit in mask is set then corresponding pin will be set
     */
    static inline void setMask(Port port, uint16_t mask) { reinterpret_cast<volatile GPIO_TypeDef *>(port)->BSRR = mask; }
    /** @brief This function set pins to low state.
     *
     * @param port - port name
     * @param mask - if bit in mask is set then corresponding pin will be reset
     */
    static inline void resetMask(Port port, uint16_t mask) {
        reinterpret_cast<volatile GPIO_TypeDef *>(port)->BSRR = static_cast<uint32_t>(mask) << 16;
    }
    /**
     * @brief This function return port state.
     *
     * @param port - port name
     * @return - read value of pins. If pin zero is set then LSB in returned value
     * will be set.
     */
    static uint16_t getMask(Port port) __attribute__((always_inline)) { return reinterpret_cast<volatile GPIO_TypeDef *>(port)->IDR; }

    /**
     * This function set pin pull type
     *
     * @param pullType
     */
    void setPullType(PullType pullType) {
        volatile GPIO_TypeDef *port = reinterpret_cast<volatile GPIO_TypeDef *>(pin.port);
        uint32_t pupdr = port->PUPDR;
        pupdr &= ~(0b11 << (pin.pin * 2));   // clear old configuration
        pupdr |= pullType << (pin.pin * 2);  // set new configuration
        port->PUPDR = pupdr;
    }
    //----------------------------- not portable functions
    /** This function set pin speed
     *
     * @param speed - pin speed
     */
    void setSpeed(Speed speed) {
        volatile GPIO_TypeDef *port = reinterpret_cast<volatile GPIO_TypeDef *>(pin.port);
        uint32_t ospeedr = port->OSPEEDR;
        ospeedr &= ~(0b11 << (pin.pin * 2));  // clear old configuration
        ospeedr |= speed << (pin.pin * 2);    // set new configuration
        port->OSPEEDR = ospeedr;
    }

    void __setIOPin(IOPin pin) { this->pin == pin; }

 protected:
    IOPin pin;

    void pinInitialize(PinConfiguration configuration);

    /**
     * This function set pin direction.
     *
     * @param direction - pin direction
     */
    void setDirection(Direction direction) {
        volatile GPIO_TypeDef *port = reinterpret_cast<volatile GPIO_TypeDef *>(pin.port);
        uint32_t otyper = port->OTYPER;
        otyper &= ~(0b1 << pin.pin);     // clear old configuration
        otyper |= direction << pin.pin;  // set new configuration
        port->OTYPER = otyper;
    }
};

template <typename Enum>
class GPIOCommon : public GPIOCommonBase {
 public:
    using GPIOCommonBase::GPIOCommonBase;

 protected:
    using AlternateFunction = Enum;
    /**
     *
     * @param function
     * @param pull
     * @param type
     * @param speed
     */
    void setAlternateFunction(AlternateFunction function, PullType pull = NoPull, OutputType type = PushPull, Speed speed = HighSpeed)
        __attribute__((always_inline)) {
        // 0x02 in mode enable alternate function
        pinInitialize(PinConfiguration{static_cast<uint8_t>(0x02 | (static_cast<std::underlying_type_t<AlternateFunction>>(function) << 4)), type,
                                       pull, speed});
    }

    void setAnalogFunction(PullType pull = NoPull, Speed speed = HighSpeed) __attribute__((always_inline)) {
        // 0x03 in mode enable analog function
        pinInitialize(PinConfiguration{static_cast<uint8_t>(0x03), 0x00, pull, speed});
    }
    //----------------------------------------- friends -----------------------------------------//
    friend class IOManager;
    friend class DataBus;
};
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // _MICROHAL_GPIO_STM_COMMON_H_
