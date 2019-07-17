/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      GPIO port driver
 *
 * @authors    Pawel Okas
 * created on: 17-01-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014 - 2017, Pawel Okas
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
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <cstdio>

#include "IOPin.h"
#include "diagnostic/diagnostic.h"
#include "interfaces/gpio_interface.h"

namespace microhal {
namespace linux {

/* ************************************************************************************************
 * CLASS
 */
class GPIO : public microhal::GPIO {
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
    GPIO(IOPin pin, Direction dir, PullType pull = NoPull, OutputType type = PushPull) : pin(pin) {
        char buf[100];
        auto fd = open("/sys/class/gpio/export", O_WRONLY);
        if (fd > 0) {
            sprintf(buf, "%d", pin.pin);
            write(fd, buf, strlen(buf));
            close(fd);

            setDirection(dir);
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "GPIO error, unable to open gpio/export file"
                                              << microhal::diagnostic::unlock;
            std::terminate();
        }
    }

    ~GPIO() {
        setDirection(Direction::Input);
        auto fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (fd > 0) {
            char buf[100];
            sprintf(buf, "%d", pin.pin);
            write(fd, buf, strlen(buf));

            close(fd);
        }
    }
    //------------------------------------ static functions -------------------------------------//
    /** This function set pin to high state.
     *
     * @param port - port name
     * @param pin - pin number
     */
    void set() {
        char buf[100];
        sprintf(buf, "/sys/class/gpio/gpio%d/value", pin.pin);
        auto fd = ::open(buf, O_WRONLY);
        if (fd > 0) {
            // Set GPIO high status
            ::write(fd, "1", 1);
            ::close(fd);
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "GPIO error, unable to open gpio file"
                                              << microhal::diagnostic::unlock;
            std::terminate();
        }
    }
    // using microhal::GPIO::reset;
    // using microhal::GPIO::set;
    /** This function set pin to high state. */
    // void set() { set(pin); }
    /** This function set pin to low state.
     *
     * @param port - port name
     * @param pin - pin number
     */
    void reset() {
        char buf[100];
        sprintf(buf, "/sys/class/gpio/gpio%d/value", pin.pin);
        auto fd = open(buf, O_WRONLY);
        if (fd > 0) {
            // Set GPIO low status
            write(fd, "0", 1);
            close(fd);
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "GPIO error, unable to open gpio file"
                                              << microhal::diagnostic::unlock;
            std::terminate();
        }
    }
    /** This function set pin to low state.*/
    // void reset() { reset(pin); }
    /** This function read pin state
     *
     * @param port - port name
     * @param pin - pin number
     * @return
     */
    bool get() const final {
        char buf[100];
        sprintf(buf, "/sys/class/gpio/gpio%d/value", pin.pin);

        auto fd = open(buf, O_RDONLY);
        if (fd > 0) {
            char value;
            read(fd, &value, 1);
            bool pinState = (value == '0') ? 0 : 1;

            close(fd);
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY << "Pin value " << value
                                              << microhal::diagnostic::unlock;
            return pinState;
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY
                                              << "GPIO error, unable to open gpio file, GPIO number: " << pin.pin << microhal::diagnostic::unlock;
            std::terminate();
        }
    }
    /** This function read pin state*/
    // bool get() const final { return get(pin); }
    /** This function check for pin set.
     *
     * @param port - port name
     * @param pin - pin number
     * @return
     */
    // static inline bool isSet(IOPin pin) { return get(pin); }
    /** This function check for pin set.*/
    // inline bool isSet() const { return isSet(pin); }
    /** This function check for pin reset.
     *
     * @param port - port name
     * @param pin - pin number
     * @return
     */
    //  static inline bool isReset(IOPin pin) { return !get(pin); }
    /** This function check for pin reset.*/
    //  inline bool isReset() const { return isReset(pin); }
    /** Sets pin to opposite state
     *
     * @param port - port name
     * @param pin - pin number
     */
    // static inline void toggle(IOPin pin) { (isSet(pin)) ? (reset(pin)) : (set(pin)); }
    /** Sets pin to opposite state*/
    // inline void toggle() { toggle(pin); }
    /** This function set pin direction.
     *
     * @param port - port name
     * @param pin - pin number
     * @param direction - pin direction
     */
    void setDirection(Direction direction) {
        char buf[100];
        sprintf(buf, "/sys/class/gpio/gpio%d/direction", pin.pin);
        auto fd = open(buf, O_WRONLY);
        if (fd > 0) {
            switch (direction) {
                case Direction::Output:
                    write(fd, "out", 3);
                    break;
                case Direction::Input:
                    write(fd, "in", 2);
                    break;
            }
            ::close(fd);
        } else {
            microhal::diagnostic::diagChannel << microhal::diagnostic::lock << MICROHAL_EMERGENCY
                                              << "GPIO error, unable to open gpio file, GPIO number: " << pin.pin << microhal::diagnostic::unlock;
        }
    }
    /** This function set pin direction.
     *
     * @param direction - pin direction
     */
    // inline void setDirection(const Direction direction) { setDirection(pin, direction); }
    /** This function set pin pull type
     *
     * @param port
     * @param pin
     * @param pullType
     */
    void setPullType(PullType pullType) {
        //  reinterpret_cast<volatile GPIO_TypeDef *>(port)->PUPDR |= pullType << (pin * 2);
    }
    /** This function set pin pull type
     *
     * @param pullType
     */
    // void setPullType(PullType pullType) { setPullType(pin, pullType); }

 protected:
    const IOPin pin;

    bool setState(bool state) final {
        if (state)
            set();
        else
            reset();
        return true;
    }

    bool configure(Direction direction, OutputType outputType, PullType pullType) final {
        setDirection(direction);
        (void)outputType;
        setPullType(pullType);
        return true;
    }
};

}  // namespace linux
}  // namespace microhal

namespace microhal {
namespace activePort = linux;
}

#endif  // _MICROHAL_GPIO_LINUX_H_
