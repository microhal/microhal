/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      GPIO port driver for:
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2014-2021, Pawel Okas
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
#include <gpio/gpio_interface.h>
#include <cstdint>
#include <type_traits>
#include "../../IOPin.h"
#include "../../registers/gpio_v1.h"
#include "../../stmCommonDefines.h"
#include "gpioPort_stmCommon.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif
/* **************************************************************************************************************************************************
 * CLASS
 */
namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

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
        Speed speed;
    } PinConfiguration;
    static_assert(sizeof(PinConfiguration) == 4, "");

    enum class AlternateFunction : uint8_t {
        AF0 = 0,
        AF1 = 1,
        AF2 = 2,
        AF3 = 3,
        AF4 = 4,
        AF5 = 5,
        AF6 = 6,
        AF7 = 7,
        AF8 = 8,
        AF9,
        AF10,
        AF11,
        AF12,
        AF13,
        AF14,
        AF15
    };

 public:
    using Port = IOPin::Port;
    //--------------------------------------- constructors --------------------------------------//
    constexpr GPIO(IOPin pin) : port(reinterpret_cast<microhal::registers::GPIO *>(pin.port)), pinNo(pin.pin) {}
    /**
     * @brief Constructor of GPIO class
     *
     * @param port - port to be used
     * @param pin - pin on used port
     * @param dir - direction of port
     * @param pull - pull up setting
     * @param type - output type setting
     */
    constexpr GPIO(IOPin pin, Direction dir, PullType pull = PullType::NoPull, OutputType type = OutputType::PushPull, Speed speed = Speed::HighSpeed)
        : port(reinterpret_cast<microhal::registers::GPIO *>(pin.port)), pinNo(pin.pin) {
        pinInitialize(static_cast<uint8_t>(dir), type, pull, speed);
    }

    int set() noexcept final {
        port.setMask(1 << pinNo);
        return 1;
    }
    int reset() noexcept final {
        port.resetMask(1 << pinNo);
        return 1;
    }
    /** This function read pin state*/
    int get() const noexcept final {
        uint16_t io = port.get();
        return io & (1 << pinNo);
    }

    int getOutputState() const noexcept final {
        uint16_t odr = port.getOdr();
        return odr & (1 << pinNo);
    }

    bool configure(microhal::GPIO::Direction dir, microhal::GPIO::OutputType type, microhal::GPIO::PullType pull) final {
        pinInitialize(static_cast<uint8_t>(dir), type, pull, MediumSpeed);
        return true;
    }

    bool getConfiguration(Direction &dir, OutputType &otype, PullType &pull) const final;
    /**
     * This function set pin pull type
     *
     * @param pullType
     */
    void setPullType(PullType pullType) {
        auto pupdr = port.getPullConfig();
        pupdr &= ~(0b11 << (pinNo * 2));                          // clear old configuration
        pupdr |= static_cast<uint16_t>(pullType) << (pinNo * 2);  // set new configuration
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

    void pinInitialize(uint8_t mode, OutputType outputType, PullType pull, Speed speed);

    /**
     * This function set pin direction.
     *
     * @param direction - pin direction
     */
    void setOutputType(OutputType outputType) {
        uint16_t dir = port.getOutputType();
        dir &= ~(1 << pinNo);                               // clear old configuration
        dir |= static_cast<uint16_t>(outputType) << pinNo;  // set new configuration
        port.setOutputType(dir);
    }

    Direction getDirection() const {
        uint32_t mode = port.getMode();
        mode >>= (2 * pinNo);
        mode &= 0b11;
        return static_cast<Direction>(mode);
    }

    /**
     *
     * @param function
     * @param pull
     * @param type
     * @param speed
     */
    void setAlternateFunction(AlternateFunction function, PullType pull = PullType::NoPull, OutputType type = OutputType::PushPull,
                              Speed speed = HighSpeed) {
        // 0x02 in mode enable alternate function
        pinInitialize(static_cast<uint8_t>(0x02U | (static_cast<std::underlying_type_t<AlternateFunction>>(function) << 4)), type, pull, speed);
    }

    void setAnalogFunction(PullType pull = PullType::NoPull, Speed speed = HighSpeed) {
        // 0x03 in mode enable analog function
        pinInitialize(0x03U, OutputType::PushPull, pull, speed);
    }
    //----------------------------------------- friends -----------------------------------------//
    friend class IOManager;
    friend class DataBus;
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // _MICROHAL_GPIO_STM_COMMON_H_
#endif  // _MICROHAL_PORT_STM_GPIO_DRIVER_VERSION == 1
