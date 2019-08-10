/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    buleks
 * created on: 19-07-2016
 * last modification: 19-07-2016
 *
 * @copyright Copyright (c) 2016, microHAL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MICROHAL_GPIO_STM32F3XX_H_
#define _MICROHAL_GPIO_STM32F3XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <interfaces/gpio_interface.h>
#include <cstdint>
#include "IOPin.h"
#include "device/stm32f3xx.h"
/* **************************************************************************************************************************************************
 * CLASS
 */
namespace microhal {
namespace stm32f3xx {

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
        AF2 = 0x02,     //! TIM1, TIM3, TIM15, TIM16
        AF4 = 0x04,     //!< I2C, TIM1
        Serial = 0x07,  //!< Serial
        AF5 = 0x05,
        AF6 = 0x06,
        SPI_2 = 0x05,
        SPI_3 = 0x06,
        // I2C = 0x04,  //!< I2C
        CAN_TIM1_TIM15 = 0x09,
        Usb = 0x0A
    } AlternateFunction;

 public:
    using Port = IOPin::Port;
    using Pin = IOPin::Pin;
    //--------------------------------------- constructors --------------------------------------//
    constexpr GPIO(IOPin pin) : pin(pin) {}
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
            setMask(pin.port, 1 << pin.pin);
        } else {
            resetMask(pin.port, 1 << pin.pin);
        }
        return true;
    }
    /** This function read pin state*/
    bool get() const final { return getMask(pin.port) & static_cast<uint16_t>(1 << pin.pin); }

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
     *  This function set pin direction.
     *
     * @param port - port name
     * @param pin - pin number
     * @param direction - pin direction
     */
    static inline void setDirection(IOPin pin, const Direction direction) {
        reinterpret_cast<volatile GPIO_TypeDef *>(pin.port)->OTYPER |= direction << pin.pin;
    }
    /**
     * This function set pin direction.
     *
     * @param direction - pin direction
     */
    inline void setDirection(Direction direction) { setDirection(pin, direction); }
    /**
     * This function set pin pull type
     *
     * @param port
     * @param pin
     * @param pullType
     */
    static inline void setPullType(IOPin pin, const PullType pullType) {
        reinterpret_cast<volatile GPIO_TypeDef *>(pin.port)->PUPDR |= pullType << (pin.pin * 2);
    }
    /**
     * This function set pin pull type
     *
     * @param pullType
     */
    void setPullType(PullType pullType) { setPullType(pin, pullType); }
    //----------------------------- not portable functions
    /** This function is not portable, when called set pin speed
     *
     * @param port - port name
     * @param pin - pin number
     * @param speed - pin speed
     */
    static void setSpeed(IOPin pin, const Speed speed) { reinterpret_cast<volatile GPIO_TypeDef *>(pin.port)->OSPEEDR |= speed << (pin.pin); }
    /** This function set pin speed
     *
     * @param speed - pin speed
     */
    void setSpeed(const Speed speed) { setSpeed(pin, speed); }

    void __setIOPin(IOPin pin) { this->pin == pin; }

 protected:
    IOPin pin;

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
                                     Speed speed = HighSpeed) __attribute__((always_inline)) {
        // 0x02 in mode enable alternate function
        pinInitialize(port, pin, PinConfiguration{static_cast<uint8_t>(0x02 | (function << 4)), type, pull, speed});
    }

    static void setAnalogFunction(Port port, Pin pin, PullType pull = NoPull, Speed speed = HighSpeed) __attribute__((always_inline)) {
        // 0x03 in mode enable analog function
        pinInitialize(port, pin, PinConfiguration{static_cast<uint8_t>(0x03), 0x00, pull, speed});
    }
    //----------------------------------------- friends -----------------------------------------//
    friend class IOManager;
    friend class DataBus;
};
}  // namespace stm32f3xx
}  // namespace microhal

#endif  // _MICROHAL_GPIO_STM32F3XX_H_
