/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      GPIO port driver
 *
 * @authors    Michal Karwatowski, Pawel Okas
 * created on: 17-01-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014 - 2018, Pawel Okas
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

#ifndef _MICROHAL_GPIO_STM32F4XX_H_
#define _MICROHAL_GPIO_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <interfaces/gpio_interface.h>
#include <cstdint>
#include "IOPin.h"
#include "device/stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

/* ************************************************************************************************
 * CLASS
 */
class GPIO : public microhal::GPIO {
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
        uint8_t speed;
    } PinConfiguration;
    static_assert(sizeof(PinConfiguration) == 4, "");
    /**
     *
     */
    typedef enum : uint8_t {
        Timer_1_2 = 1,
        Timer_3_4_5 = 2,
        Serial = 0x07,  //!< Serial
        Serial_4_5_6 = 0x08,
        SPI = 0x05,    //!< Alternate function for SPI 1 and 2
        SPI_3 = 0x06,  //!< Alternate function for SPI 3
        I2C = 0x04,    //!< I2C
        USB = 0x0A
    } AlternateFunction;

 public:
    using Port = IOPin::Port;
    using Pin = IOPin::Pin;
    //--------------------------------------- constructors --------------------------------------//
    GPIO(IOPin pin) : pin(pin) {}
    /**
     * @brief Constructor of GPIO class
     *
     * @param port - port to be used
     * @param pin - pin on used port
     * @param dir - direction of port
     * @param pull - pull up setting
     * @param type - output type setting
     */
    GPIO(IOPin pin, Direction dir, PullType pull = NoPull, OutputType type = PushPull, Speed speed = HighSpeed) : pin(pin) {
        pinInitialize(pin.port, pin.pin, PinConfiguration{dir, type, pull, speed});
    }

    ~GPIO() {}

    bool setState(bool value) final {
        if (value) {
            set(pin);
        } else {
            reset(pin);
        }
        return true;
    }

    bool get() const final { return get(pin); }

    bool configure(microhal::GPIO::Direction dir, microhal::GPIO::OutputType type, microhal::GPIO::PullType pull) final {
        pinInitialize(pin.port, pin.pin, PinConfiguration{dir, type, pull, NoPull});
        return true;
    }
    //------------------------------------ static functions -------------------------------------//
    /** @brief This function set pins to high state.
     *
     * @param port - port name
     * @param mask - if bit in mask is set then corresponding pin will be set
     */
    static void setMask(Port port, uint16_t mask) { reinterpret_cast<volatile GPIO_TypeDef *>(port)->BSRR = mask; }
    /** @brief This function set pins to low state.
     *
     * @param port - port name
     * @param mask - if bit in mask is set then corresponding pin will be reset
     */
    static void resetMask(Port port, uint16_t mask) { reinterpret_cast<volatile GPIO_TypeDef *>(port)->BSRR = mask << 16; }
    /** @brief This function return port state.
     *
     * @param port - port name
     * @return - read value of pins. If pin zero is set then LSB in returned value will be set.
     */
    static uint16_t getMask(Port port) { return reinterpret_cast<volatile GPIO_TypeDef *>(port)->IDR; }
    /** This function set pin to high state.
     *
     * @param port - port name
     * @param pin - pin number
     */
    static void set(IOPin pin) { setMask(pin.port, 1 << pin.pin); }
    /** This function set pin to low state.
     *
     * @param port - port name
     * @param pin - pin number
     */
    static void reset(IOPin pin) { resetMask(pin.port, 1 << pin.pin); }
    /** This function read pin state
     *
     * @param port - port name
     * @param pin - pin number
     * @return
     */
    static bool get(IOPin pin) { return (getMask(pin.port) & static_cast<uint16_t>(1 << pin.pin)); }
    /** This function check for pin set.
     *
     * @param port - port name
     * @param pin - pin number
     * @return
     */
    static bool isSet(IOPin pin) { return get(pin); }
    /** This function check for pin reset.
     *
     * @param port - port name
     * @param pin - pin number
     * @return
     */
    static bool isReset(IOPin pin) { return !get(pin); }
    /** Sets pin to opposite state
     *
     * @param port - port name
     * @param pin - pin number
     */
    static void toggle(IOPin pin) { (isSet(pin)) ? (reset(pin)) : (set(pin)); }
    using microhal::GPIO::toggle;
    /** This function set pin direction.
     *
     * @param port - port name
     * @param pin - pin number
     * @param direction - pin direction
     */
    static void setDirection(IOPin pin, Direction direction) { reinterpret_cast<volatile GPIO_TypeDef *>(pin.port)->OTYPER |= direction << pin.pin; }
    /** This function set pin direction.
     *
     * @param direction - pin direction
     */
    void setDirection(Direction direction) { setDirection(pin, direction); }
    /** This function set pin pull type
     *
     * @param port
     * @param pin
     * @param pullType
     */
    static void setPullType(IOPin pin, PullType pullType) { reinterpret_cast<volatile GPIO_TypeDef *>(pin.port)->PUPDR |= pullType << (pin.pin * 2); }
    /** This function set pin pull type
     *
     * @param pullType
     */
    inline void setPullType(PullType pullType) { setPullType(pin, pullType); }
    //----------------------------- not portable functions -----------------------------------
    /** This function set pin speed
     *
     * @param pin -
     * @param speed - pin speed
     */
    static void setSpeed(IOPin pin, Speed speed) { reinterpret_cast<volatile GPIO_TypeDef *>(pin.port)->OSPEEDR |= speed << (pin.pin); }
    /** This function set pin speed
     *
     * @param speed - pin speed
     */
    void setSpeed(const Speed speed) { setSpeed(pin, speed); }

 protected:
    const IOPin pin;

    static void pinInitialize(Port port, uint_fast8_t pin, PinConfiguration configuration);
    /**
     *
     * @param port
     * @param pin
     * @param function
     * @param pull
     * @param type
     * @param speed
     */
    static void setAlternateFunction(Port port, Pin pin, AlternateFunction function, PullType pull = NoPull, OutputType type = PushPull,
                                     Speed speed = HighSpeed) {
        // 0x02 in mode enable alternate function
        pinInitialize(port, pin, PinConfiguration{static_cast<uint8_t>(0x02 | (function << 4)), type, pull, speed});
    }

    static void setAnalogFunction(Port port, Pin pin, PullType pull = NoPull, Speed speed = HighSpeed) {
        // 0x03 in mode enable analog function
        pinInitialize(port, pin, PinConfiguration{static_cast<uint8_t>(0x03), 0x00, pull, speed});
    }
    //----------------------------------------- friends -----------------------------------------//
    friend class IOManager;
    friend class DataBus;
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // _MICROHAL_GPIO_STM32F4XX_H_
