/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 22-03-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#ifndef _MICROHAL_GPIO_LPC43XX_H_
#define _MICROHAL_GPIO_LPC43XX_H_

/* ************************************************************************************************
 * INCLUDES
 */
#include <interfaces/gpio_interface.h>
#include <cstdint>
#include "IOPin.h"
#include "hardware_lpc43xx.h"
#include "scu.h"

namespace microhal {
namespace lpc43xx {

/* ************************************************************************************************
 * CLASS
 */
class GPIO : public microhal::GPIO {
    struct GpioPin {
        uint8_t port;
        uint8_t pin;
    };

 public:
    using Port = uint8_t;
    using Pin = uint8_t;
    using DriveStrength = SCU::DriveStrength;
    //--------------------------------------- constructors --------------------------------------//
    GPIO(IOPin pin) : pin(getGpioPinFromIOPin(pin)) { scu.enableInputBuffer(pin); }
    /**
     * @brief Constructor of GPIO class
     *
     * @param pin - pin
     * @param dir - direction of pin
     */
    GPIO(IOPin pin, Direction dir) : GPIO(pin) { setDirection(dir); }

    ~GPIO() {}

    bool configure(microhal::GPIO::Direction dir, microhal::GPIO::OutputType, microhal::GPIO::PullType pullType) final {
        setDirection(dir);
        setPullType(pullType);
        return true;
    }

    bool setState(bool value) final {
        if (value) {
            registers->B[pin.port][pin.pin] = 1;
        } else {
            registers->B[pin.port][pin.pin] = 0;
        }
        return true;
    }

    bool get() const final { return registers->B[pin.port][pin.pin]; }

    void toggle() { registers->NOT[pin.port] = 1 << pin.pin; }

    void setDirection(Direction direction) {
        if (direction == Direction::Output)
            registers->DIR[pin.port] |= 1 << pin.pin;
        else
            registers->DIR[pin.port] &= ~(1 << pin.pin);
    }

    /** This function set pin pull type
     *
     * @param pullType
     */
    void setPullType(PullType pullType) {
        IOPin ioPin = getIOPinFromGpioPin(pin);
        switch (pullType) {
            case PullType::NoPull:
                scu.disablePullDown(ioPin);
                scu.disablePullUp(ioPin);
                break;
            case PullType::PullUp:
                scu.enablePullUp(ioPin);
                scu.disablePullDown(ioPin);
                break;
            case PullType::PullDown:
                scu.enablePullDown(ioPin);
                scu.disablePullUp(ioPin);
                break;
        }
    }

    void setDriveStrength(DriveStrength strength) { scu.setDriveStrength(getIOPinFromGpioPin(pin), strength); }

 protected:
    typedef struct {                 /*!< GPIO_PORT Structure */
        volatile uint8_t B[128][32]; /*!< Offset 0x0000: Byte pin registers ports 0 to n; pins PIOn_0 to PIOn_31 */
        volatile uint32_t W[32][32]; /*!< Offset 0x1000: Word pin registers port 0 to n */
        volatile uint32_t DIR[32];   /*!< Offset 0x2000: Direction registers port n */
        volatile uint32_t MASK[32];  /*!< Offset 0x2080: Mask register port n */
        volatile uint32_t PIN[32];   /*!< Offset 0x2100: Portpin register port n */
        volatile uint32_t MPIN[32];  /*!< Offset 0x2180: Masked port register port n */
        volatile uint32_t SET[32];   /*!< Offset 0x2200: Write: Set register for port n Read: output bits for port n */
        volatile uint32_t CLR[32];   /*!< Offset 0x2280: Clear port n */
        volatile uint32_t NOT[32];   /*!< Offset 0x2300: Toggle port n */
    } Registers;
    const GpioPin pin;
    static Registers* const registers;
    static constexpr uint16_t gpio_to_Pin[][16] = {{
                                                       0 * 32 + 0,   // GPIO0[0]
                                                       0 * 32 + 1,   // GPIO0[1]
                                                       1 * 32 + 15,  // GPIO0[2]
                                                       1 * 32 + 16,  // GPIO0[3]
                                                       1 * 32 + 0,   // GPIO0[4]
                                                       6 * 32 + 6,   // GPIO0[5]
                                                       3 * 32 + 6,   // GPIO0[6]
                                                       2 * 32 + 7,   // GPIO0[7]
                                                       1 * 32 + 1,   // GPIO0[8]
                                                       1 * 32 + 2,   // GPIO0[9]
                                                       1 * 32 + 3,   // GPIO0[10]
                                                       1 * 32 + 4,   // GPIO0[11]
                                                       1 * 32 + 17,  // GPIO0[12]
                                                       1 * 32 + 18,  // GPIO0[13]
                                                       2 * 32 + 10,  // GPIO0[14]
                                                       1 * 32 + 20   // GPIO0[15]
                                                   },
                                                   {
                                                       1 * 32 + 7,   // GPIO1[0]
                                                       1 * 32 + 8,   // GPIO1[1]
                                                       1 * 32 + 9,   // GPIO1[2]
                                                       1 * 32 + 10,  // GPIO1[3]
                                                       1 * 32 + 11,  // GPIO1[4]
                                                       1 * 32 + 12,  // GPIO1[5]
                                                       1 * 32 + 13,  // GPIO1[6]
                                                       1 * 32 + 14,  // GPIO1[7]
                                                       1 * 32 + 5,   // GPIO1[8]
                                                       1 * 32 + 6,   // GPIO1[9]
                                                       2 * 32 + 9,   // GPIO1[10]
                                                       2 * 32 + 11,  // GPIO1[11]
                                                       2 * 32 + 12,  // GPIO1[12]
                                                       2 * 32 + 13,  // GPIO1[13]
                                                       3 * 32 + 4,   // GPIO1[14]
                                                       3 * 32 + 5    // GPIO1[15]

                                                   },
                                                   {
                                                       4 * 32 + 0,   // GPIO2[0]
                                                       4 * 32 + 1,   // GPIO2[1]
                                                       4 * 32 + 2,   // GPIO2[2]
                                                       4 * 32 + 3,   // GPIO2[3]
                                                       4 * 32 + 4,   // GPIO2[4]
                                                       4 * 32 + 5,   // GPIO2[5]
                                                       4 * 32 + 6,   // GPIO2[6]
                                                       5 * 32 + 7,   // GPIO2[7]
                                                       6 * 32 + 12,  // GPIO2[8]
                                                       5 * 32 + 0,   // GPIO2[9]
                                                       5 * 32 + 1,   // GPIO2[10]
                                                       5 * 32 + 2,   // GPIO2[11]
                                                       5 * 32 + 3,   // GPIO2[12]
                                                       5 * 32 + 4,   // GPIO2[13]
                                                       5 * 32 + 5,   // GPIO2[14]
                                                       5 * 32 + 6,   // GPIO2[15]
                                                   },
                                                   {
                                                       6 * 32 + 1,   // GPIO3[0]
                                                       6 * 32 + 2,   // GPIO3[1]
                                                       6 * 32 + 3,   // GPIO3[2]
                                                       6 * 32 + 4,   // GPIO3[3]
                                                       6 * 32 + 5,   // GPIO3[4]
                                                       6 * 32 + 9,   // GPIO3[5]
                                                       6 * 32 + 10,  // GPIO3[6]
                                                       6 * 32 + 11,  // GPIO3[7]
                                                       7 * 32 + 0,   // GPIO3[8]
                                                       7 * 32 + 1,   // GPIO3[9]
                                                       7 * 32 + 2,   // GPIO3[10]
                                                       7 * 32 + 3,   // GPIO3[11]
                                                       7 * 32 + 4,   // GPIO3[12]
                                                       7 * 32 + 5,   // GPIO3[13]
                                                       7 * 32 + 6,   // GPIO3[14]
                                                       7 * 32 + 7,   // GPIO3[15]
                                                   },
                                                   {
                                                       8 * 32 + 0,   // GPIO4[0]
                                                       8 * 32 + 1,   // GPIO4[1]
                                                       8 * 32 + 2,   // GPIO4[2]
                                                       8 * 32 + 3,   // GPIO4[3]
                                                       8 * 32 + 4,   // GPIO4[4]
                                                       8 * 32 + 5,   // GPIO4[5]
                                                       8 * 32 + 6,   // GPIO4[6]
                                                       8 * 32 + 7,   // GPIO4[7]
                                                       10 * 32 + 1,  // GPIO4[8]
                                                       10 * 32 + 2,  // GPIO4[9]
                                                       10 * 32 + 3,  // GPIO4[10]
                                                       9 * 32 + 6,   // GPIO4[11]
                                                       9 * 32 + 0,   // GPIO4[12]
                                                       9 * 32 + 1,   // GPIO4[13]
                                                       9 * 32 + 2,   // GPIO4[14]
                                                       9 * 32 + 3,   // GPIO4[15]
                                                   }};

    static GpioPin getGpioPinFromIOPin(IOPin pin) {
        for (uint_fast8_t gpioPort = 0; gpioPort < 5; gpioPort++) {
            for (uint_fast8_t gpioPin = 0; gpioPin < 16; gpioPin++) {
                if (gpio_to_Pin[gpioPort][gpioPin] == (pin.port * 32 + pin.pin)) {
                    return {static_cast<uint8_t>(gpioPort), static_cast<uint8_t>(gpioPin)};
                }
            }
        }

        // trying to use non GPIO capable pin
        hardware::Device::breakpoint();
        std::terminate();
    }

    static constexpr IOPin getIOPinFromGpioPin(GpioPin pin) {
        uint16_t value = gpio_to_Pin[pin.port][pin.pin];
        return {static_cast<IOPin::Port>(value / 32), static_cast<IOPin::Pin>(value % 32)};
    }
};

}  // namespace lpc43xx
}  // namespace microhal

#endif  // _MICROHAL_GPIO_LPC43XX_H_
