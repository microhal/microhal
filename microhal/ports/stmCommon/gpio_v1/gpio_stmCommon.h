/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      GPIO port driver for:
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2014-2020, Pawel Okas
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

#include <ports/stmCommon/driverConfiguration_stmCommon.h>
#if _MICROHAL_PORT_STM_GPIO_DRIVER_VERSION == 1
#ifndef _MICROHAL_GPIO_STM_COMMON_H_
#define _MICROHAL_GPIO_STM_COMMON_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <interfaces/gpio_interface.h>
#include <cstdint>
#include <type_traits>
#include "../IOPin.h"
#include "../registers/gpio_v1.h"
#include "../stmCommonDefines.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif
/* **************************************************************************************************************************************************
 * CLASS
 */
namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

class GPIOPort {
 public:
    constexpr GPIOPort(microhal::registers::GPIO *gpio) : gpio(*gpio) {}

    uint16_t get() const { return (uint32_t)gpio.idr.volatileLoad(); }
    void set(uint16_t value) { gpio.odr.volatileStore(value); }
    void setMask(uint16_t bitsToSet) { gpio.bsrr.volatileStore(bitsToSet); }
    void resetMask(uint16_t bitsToReset) {
#ifdef _MICROHAL_GPIO_REGISTER_HAS_BRR
        gpio.brr.volatileStore(bitsToReset);
#else
        gpio.bsrr.volatileStore(bitsToReset << 16);
#endif
    }
    void setResetMask(uint16_t values, uint16_t mask) {
        uint32_t toSet = values & mask;
        uint32_t toReset = (~values) & mask;
        registers::GPIO::BSRR bsrr;
        bsrr.BS = toSet;
        bsrr.BR = toReset;
        gpio.bsrr.volatileStore(bsrr);
    }

    uint16_t getDirection() { return (uint32_t)gpio.otyper.volatileLoad(); }
    void setDirection(uint16_t direction) { gpio.otyper.volatileStore(direction); }
    uint32_t getPullConfig() { return gpio.pupdr.volatileLoad(); }
    void setPullConfig(uint32_t pullConfig) { gpio.pupdr.volatileStore(pullConfig); }
    uint32_t getSpeed() { return gpio.ospeedr.volatileLoad(); }
    void setSpeed(uint32_t speed) { gpio.ospeedr.volatileStore(speed); }

    microhal::registers::GPIO &getGpioHandle() { return gpio; }

 private:
    microhal::registers::GPIO &gpio;
};

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
    using GPIO = microhal::registers::GPIO;
    //--------------------------------------- constructors --------------------------------------//
    constexpr GPIOCommonBase(IOPin pin) : port(reinterpret_cast<microhal::registers::GPIO *>(pin.port)), pinNo(pin.pin) {}
    /**
     * @brief Constructor of GPIO class
     *
     * @param port - port to be used
     * @param pin - pin on used port
     * @param dir - direction of port
     * @param pull - pull up setting
     * @param type - output type setting
     */
    constexpr GPIOCommonBase(IOPin pin, Direction dir, PullType pull = NoPull, OutputType type = PushPull, Speed speed = HighSpeed)
        : port(reinterpret_cast<microhal::registers::GPIO *>(pin.port)), pinNo(pin.pin) {
        pinInitialize(PinConfiguration{dir, type, pull, speed});
    }

    virtual ~GPIOCommonBase() {}

    bool set() final {
        port.setMask(1 << pinNo);
        return true;
    }
    bool reset() final {
        port.resetMask(1 << pinNo);
        return true;
    }
    /** This function read pin state*/
    bool get() const final {
        uint16_t io = port.get();
        return io & (1 << pinNo);
    }

    bool configure(microhal::GPIO::Direction dir, microhal::GPIO::OutputType type, microhal::GPIO::PullType pull) final {
        pinInitialize(PinConfiguration{dir, type, pull, MediumSpeed});
        return true;
    }
    /**
     * This function set pin pull type
     *
     * @param pullType
     */
    void setPullType(PullType pullType) {
        auto pupdr = port.getPullConfig();
        pupdr &= ~(0b11 << (pinNo * 2));   // clear old configuration
        pupdr |= pullType << (pinNo * 2);  // set new configuration
        port.setPullConfig(pupdr);
    }
    //----------------------------- not portable functions
    /** This function set pin speed
     *
     * @param speed - pin speed
     */
    void setSpeed(Speed speed) {
        auto ospeedr = port.getSpeed();
        ospeedr &= ~(0b11 << (pinNo * 2));  // clear old configuration
        ospeedr |= speed << (pinNo * 2);    // set new configuration
        port.setSpeed(speed);
    }

 protected:
    GPIOPort port;
    uint8_t pinNo;

    void pinInitialize(PinConfiguration configuration);

    /**
     * This function set pin direction.
     *
     * @param direction - pin direction
     */
    void setDirection(Direction direction) {
        uint16_t dir = port.getDirection();
        dir &= ~(1 << pinNo);       // clear old configuration
        dir |= direction << pinNo;  // set new configuration
        port.setDirection(dir);
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
#endif  // _MICROHAL_PORT_STM_GPIO_DRIVER_VERSION == 1
