/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      GPIO interface
 *
 * @authors    Pawel Okas
 * created on: 11-09-2018
 * last modification: 11-09-2018
 *
 * @copyright Copyright (c) 2018-2020, Pawel Okas
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

#ifndef _MICROHAL_GPIO_INTERFACE_H_
#define _MICROHAL_GPIO_INTERFACE_H_

/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>

namespace microhal {
/* ************************************************************************************************
 * CLASS
 */
class GPIO {
 public:
    typedef enum {
        Input = 0,  //!< Configure as input
        Output = 1  //!< Configure as output
    } Direction;
    /**
     * Possible pin output types
     */
    typedef enum {
        PushPull = 0,      //!< Push pull output
        OpenDrain = 1,     //!< Open drain output
        OpenCollector = 2  //!< Open collector output
    } OutputType;
    /**
     * Possible pull type
     */
    typedef enum {
        NoPull = 0,   //!< No pull up or down resistors enabled
        PullUp = 1,   //!< Pull up resistor active
        PullDown = 2  //!< Pull down resistor active
    } PullType;

    // disable copying
    GPIO() = default;
    GPIO(const GPIO &) = delete;
    GPIO &operator=(const GPIO &gpio) = delete;

    virtual ~GPIO() = default;

    /** Set pin to high state */
    virtual bool set() = 0;
    /** Set pin to low state */
    virtual bool reset() = 0;
    /** Set pin to opposite state */
    void toggle() { get() ? reset() : set(); }
    /**
     * @brief Get actual pin state
     * @return Current pin state
     * @retval true if pin is in high state
     * @retval false if pin is in low state
     */
    virtual bool get() const = 0;
    /**
     * @brief Get output state, this can be used with @ref get method to check if pin is shorted to ground or power lines.
     * @return Programmed pin state, set by @ref set or @ref reset methods
     * @retval true if pin should be in high state
     * @retval false if pin should be in low state
     */
    virtual bool getOutputState() const = 0;
    /**
     * @brief Check if pin is set as high
     *
     * @retval true - if pin is high
     * @retval false - if pin is low
     */
    bool isSet() { return get(); }
    /**
     * @brief Check if pin is set as low
     *
     * @retval true - if pin is low
     * @retval false - if pin is high
     */
    bool isReset() const { return !get(); }
    /**
     * @brief Check if pin is configured as output
     *
     * @retval true - if pin is configured as Output
     * @retval false - if pin is configured as Input
     */
    bool isInput() const { return getDirection() == Direction::Input; }
    /**
     * @brief Check if pin is configured as input
     *
     * @retval true - if pin is configured as Input
     * @retval false - if pin is configured as Output
     */
    bool isOutput() const { return getDirection() == Direction::Output; }

    bool configureAsInput(PullType pullUpOrDown) { return configure(Direction::Input, OutputType::PushPull, pullUpOrDown); }
    bool configureAsOutput(OutputType outputType, PullType pullUpOrDown) { return configure(Direction::Output, outputType, pullUpOrDown); }
    /**
     * @brief Check current pin direction
     *
     * @return pin Direction
     */
    Direction getDirection() const;
    /**
     * @brief Returns pin output configuration, if pin is configured as input return value will be invalid.
     *
     */
    OutputType getOutputType() const;
    /**
     * @brief Returns pin pull configuration
     *
     */
    PullType getPullType() const;
    /**
     * @brief Returns state of pin
     *
     * @retval true - if pin is high
     * @retval false - if pin is low
     */
    operator bool() { return get(); }
    /**
     * @brief Sets pin state to state
     *
     * @param state - value that is to be set on pin
     */
    GPIO &operator=(bool state) {
        state ? set() : reset();
        return *this;
    }

    /**
     * @brief Returns negated state of pin
     *
     * @retval true - if pin is low
     * @retval false - if pin is high
     */
    bool operator!() { return isReset(); }
    /**
     * @brief Compares state of pins
     * @param state
     * @retval true - if pins are equal
     * @retval false - if pin are not equal
     */
    bool operator==(bool state) { return get() == state; }
    /**
     * @brief Compares state of pins
     *
     * @param state
     * @retval true - if pins not are equal
     * @retval false - if pin are equal
     */
    bool operator!=(bool state) { return get() != state; }

 private:
    virtual bool configure(Direction, OutputType, PullType) = 0;
    virtual bool getConfiguration(Direction &dir, OutputType &otype, PullType &pull) const = 0;
};

}  // namespace microhal

#endif /* _MICROHAL_GPIO_INTERFACE_H_ */