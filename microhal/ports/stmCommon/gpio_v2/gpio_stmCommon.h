/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      GPIO port driver for: STM32F102, STM32F107, STM32F103, STM32F101, STM32F100
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2019-2020, Pawel Okas
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
#include "../IOPin.h"
#include "../stmCommonDefines.h"
#include "ports/stmCommon/registers/gpio_v2.h"

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
    uint16_t getOdr() const { return (uint32_t)gpio.odr.volatileLoad(); }
    void set(uint16_t value) { gpio.odr.volatileStore(value); }
    void setMask(uint16_t bitsToSet) { gpio.bsrr.volatileStore(bitsToSet); }
    void resetMask(uint16_t bitsToReset) { gpio.brr.volatileStore(bitsToReset); }
    void setResetMask(uint16_t values, uint16_t mask) {
        uint32_t toSet = values & mask;
        uint32_t toReset = (~values) & mask;
        registers::GPIO::BSRR bsrr;
        bsrr.BSR = toSet;
        bsrr.BR = toReset;
        gpio.bsrr.volatileStore(bsrr);
    }

    /**
     *
     * @param pinNumber from 0 to 15
     * @param configuration 4 bit configuration value 0b0000xxyy, where xx are CNF bits and yy are MODE bits.
     */
    void configurePin(uint8_t pinNumber, uint8_t configuration) {
        if (pinNumber < 8) {
            auto crl = gpio.crl.volatileLoad();
            crl &= ~(0b1111 << (pinNumber * 4));
            crl |= configuration << (pinNumber * 4);
            gpio.crl.volatileStore(crl);
        } else {
            auto crh = gpio.crh.volatileLoad();
            crh &= ~(0b1111 << ((pinNumber - 8) * 4));
            crh |= configuration << ((pinNumber - 8) * 4);
            gpio.crh.volatileStore(crh);
        }
    }
    /**
     *
     * @param pinNumber from 0 to 15
     * @return
     */
    uint8_t getPinConfiguration(uint8_t pinNumber) const {
        if (pinNumber < 8) {
            auto crl = gpio.crl.volatileLoad();
            uint32_t tmp = crl;
            tmp >>= pinNumber * 4;
            return tmp & 0b1111;
        } else {
            auto crh = gpio.crh.volatileLoad();
            uint32_t tmp = crh;
            tmp >>= (pinNumber - 8) * 4;
            return tmp & 0b1111;
        }
    }

    void enableClock();
    void disableClock();

 private:
    microhal::registers::GPIO &gpio;
};

class GPIOCommonBase : public microhal::GPIO {
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
    constexpr GPIOCommonBase(IOPin pin) : port(reinterpret_cast<registers::GPIO *>(pin.port)), pinNo(pin.pin) {}
    /**
     * @brief Constructor of GPIO class
     *
     * @param pin - pin on used port
     * @param dir - direction of port
     * @param pull - pull up setting
     * @param type - output type setting
     */
    constexpr GPIOCommonBase(IOPin pin, Direction dir, PullType pull = NoPull, OutputType type = PushPull, Speed speed = HighSpeed)
        : port(reinterpret_cast<registers::GPIO *>(pin.port)), pinNo(pin.pin) {
        configure(dir, type, pull);
        setSpeed(speed);
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

    bool getOutputState() const final {
        uint16_t io = port.getOdr();
        return io & (1 << pinNo);
    }

    bool configure(microhal::GPIO::Direction dir, microhal::GPIO::OutputType type, microhal::GPIO::PullType pull) final;
    /** This function set pin speed
     *
     * @param speed - pin speed
     */
    bool setSpeed(Speed speed) {
        auto config = port.getPinConfiguration(pinNo);
        if (config & 0b11) {
            config &= ~0b11;  // clean mode
            config |= speed;
            port.configurePin(pinNo, config);
            return true;
        }
        return false;
    }

    bool getConfiguration(Direction &dir, OutputType &otype, PullType &pull) const final;

 protected:
    GPIOPort port;
    uint8_t pinNo;
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
    void setAlternateFunctionOutput(AlternateFunction function, PullType pull = NoPull, OutputType type = PushPull, Speed speed = HighSpeed) {
        port.enableClock();
        uint8_t cnf;
        switch (type) {
            case OutputType::OpenCollector:
                return;
            case OutputType::OpenDrain:
                cnf = 0b1100 | speed;
                break;
            case OutputType::PushPull:
                cnf = 0b1000 | speed;
                break;
        }
        port.configurePin(pinNo, cnf);
    }
    //
    //    void setAnalogFunction(PullType pull = NoPull, Speed speed = HighSpeed) __attribute__((always_inline)) {
    //        // 0x03 in mode enable analog function
    //        pinInitialize(PinConfiguration{static_cast<uint8_t>(0x03), 0x00, pull, speed});
    //    }
    //----------------------------------------- friends -----------------------------------------//
    friend class IOManager;
    friend class DataBus;
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // _MICROHAL_GPIO_STMCOMMON_H_
#endif  //_MICROHAL_PORT_STM_GPIO_DRIVER_VERSION == 1
