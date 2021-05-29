/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      GPIO port driver for: STM32F102, STM32F107, STM32F103, STM32F101, STM32F100
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2019-2021, Pawel Okas
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
#if _MICROHAL_PORT_STM_GPIO_DRIVER_VERSION == 2
#ifndef _MICROHAL_GPIO_STMCOMMON_H_
#define _MICROHAL_GPIO_STMCOMMON_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <gpio/gpio_interface.h>
#include <cstdint>
#include <type_traits>
#include "../../IOPin.h"
#include "../../stmCommonDefines.h"
#include "gpioPort_stmCommon.h"
#include "ports/stmCommon/registers/gpio_v2.h"

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
        LowSpeed = 0b10,     //!< LOW_SPEED
        MediumSpeed = 0b01,  //!< MEDIUM_SPEED
        HighSpeed = 0b11     //!< HIGH_SPEED
    } Speed;

 public:
    //--------------------------------------- constructors --------------------------------------//
    constexpr GPIO(IOPin pin) : port(reinterpret_cast<registers::GPIO *>(pin.port)), pinMask(1 << pin.pin), pinNo(pin.pin) {}
    /**
     * @brief Constructor of GPIO class
     *
     * @param pin - pin on used port
     * @param dir - direction of port
     * @param pull - pull up setting
     * @param type - output type setting
     * @param speed - pin speed
     */
    constexpr GPIO(IOPin pin, Direction dir, PullType pull = PullType::NoPull, OutputType type = OutputType::PushPull, Speed speed = Speed::HighSpeed)
        : port(reinterpret_cast<registers::GPIO *>(pin.port)), pinMask(1 << pin.pin), pinNo(pin.pin) {
        configure(dir, type, pull);
        setSpeed(speed);
    }

    constexpr virtual ~GPIO() = default;

    int set() noexcept final {
        port.setMask(pinMask);
        return 1;
    }
    int reset() noexcept final {
        port.resetMask(pinMask);
        return 1;
    }
    /** This function read pin state*/
    int get() const noexcept final {
        const uint16_t io = port.get();
        return io & pinMask;
    }

    int getOutputState() const noexcept final {
        uint16_t io = port.getOdr();
        return io & pinMask;
    }

    bool configure(microhal::GPIO::Direction dir, microhal::GPIO::OutputType type, microhal::GPIO::PullType pull) final;
    /** This function set pin speed
     *
     * @param speed - pin speed
     */
    bool setSpeed(Speed speed);
    bool getConfiguration(Direction &dir, OutputType &otype, PullType &pull) const final;

 protected:
    GPIOPort port;
    uint16_t pinMask;
    uint8_t pinNo;

    /**
     *
     * @param pull
     * @param type
     * @param speed
     */
    void setAlternateFunctionOutput(PullType pull = PullType::NoPull, OutputType type = OutputType::PushPull, Speed speed = Speed::HighSpeed);

    void configureAsAnalogInput() {
        port.enableClock();
        port.configurePin(pinNo, 0);
    }

    //----------------------------------------- friends -----------------------------------------//
    friend class IOManager;
    friend class DataBus;
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // _MICROHAL_GPIO_STMCOMMON_H_
#endif  //_MICROHAL_PORT_STM_GPIO_DRIVER_VERSION == 1
