/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      GPIO interface
 *
 * @authors    Pawel Okas
 * created on: 11-09-2018
 *
 * @copyright Copyright (c) 2018-2021, Pawel Okas
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
#include <utils/microhal_expected.h>
#include <cstdint>

namespace microhal {
/* ************************************************************************************************
 * CLASS
 */
class GPIO {
 public:
    enum class State : uint_fast8_t { Low = 0, High = 1 };

    enum class Direction : uint_fast8_t {
        Input = 0,  //!< Configure as input
        Output = 1  //!< Configure as output
    };
    /**
     * Possible pin output types
     */
    enum class OutputType : uint_fast8_t {
        PushPull = 0,      //!< Push pull output
        OpenDrain = 1,     //!< Open drain output
        OpenCollector = 2  //!< Open collector output
    };
    /**
     * Possible pull type
     */
    enum class PullType : uint_fast8_t {
        NoPull = 0,   //!< No pull up or down resistors enabled
        PullUp = 1,   //!< Pull up resistor active
        PullDown = 2  //!< Pull down resistor active
    };

    enum class Error : int_fast8_t { None = 0, GPIONotOutput = -1, GPIONotInput = -2, UnsupportedOutputType = -3 };

    using PinStateReturnType = Expected<State, UnexpectedNegativeValue<Error>>;

    // disable copying
    GPIO() = default;
    GPIO(const GPIO &) = delete;
    GPIO &operator=(const GPIO &gpio) = delete;

    virtual ~GPIO() = default;

    /** Set pin to high state
     * @return Operation state
     * @retval Error::None on success
     * @retval Error code if an error occurred.
     */
    virtual Error set() noexcept = 0;

    /** Set pin to high state
     * @return Operation state
     * @retval Error::None on success
     * @retval Error code if an error occurred.
     */
    Error set(bool state) noexcept {
        if (state)
            return set();
        else
            return reset();
    }

    /** Set pin to high state
     * @return Operation state
     * @retval Error::None on success
     * @retval Error code if an error occurred.
     */
    Error set(State state) noexcept {
        if (state == State::Low)
            return reset();
        else
            return set();
    }
    /** Set pin to low state
     * @return Operation state
     * @retval Error::None on success
     * @retval Error code if an error occurred.
     */
    virtual Error reset() noexcept = 0;
    /** Set pin to opposite state
     * @return Operation state
     * @retval Error::None on success
     * @retval Error code if an error occurred.
     */
    Error toggle() noexcept {
        const auto state = get();
        if (state) return (state.value() == State::Low) ? set() : reset();
        return state.error();  // error occurred
    }
    /**
     * @brief Get actual pin state
     * @return Current pin state or error, if has_value() method of return type is true then function result contain value and can be accessed by
     * calling value() method.
     *
     * example:
     *
     * // check if gpioState contain value or error
     * if (const auto gpioState = gpio.get();)
     * {
     *		// gpioState contain gpio state, you can check current state by calling value() method
     *		gpioState.value(); // access pin state
     * } else {
     * 		// an error occurred during checking of GPIO state. You can check error type by calling error() method. Possible errors are specyfied
     *      // in Error enum
     *      Error errorCode = gpioState.error(); // access error code
     * }
     */
    virtual PinStateReturnType get() const noexcept = 0;
    /**
     * @brief Get output state, this can be used with @ref get method to check if pin is shorted to ground or power lines.
     * @return Programmed pin state or error, set by @ref set or @ref reset methods
     * 		   Check get() method documentation to understand return values.
     */
    virtual PinStateReturnType getOutputState() const noexcept = 0;

    Error configureAsInput(PullType pullUpOrDown) { return configure(Direction::Input, OutputType::PushPull, pullUpOrDown); }
    Error configureAsOutput(OutputType outputType, PullType pullUpOrDown) { return configure(Direction::Output, outputType, pullUpOrDown); }
    /**
     * @brief Check current pin direction
     *
     * @return pin Direction
     */
    virtual Expected<Direction, UnexpectedNegativeValue<Error>> getDirection() const noexcept = 0;
    /**
     * @brief Returns pin output configuration, if pin is configured as input return value will be invalid.
     *
     */
    virtual Expected<OutputType, UnexpectedNegativeValue<Error>> getOutputType() const noexcept = 0;
    /**
     * @brief Returns pin pull configuration
     *
     */
    virtual Expected<PullType, UnexpectedNegativeValue<Error>> getPullType() const noexcept = 0;

 private:
    virtual Error configure(Direction, OutputType, PullType) = 0;
};

}  // namespace microhal

#endif /* _MICROHAL_GPIO_INTERFACE_H_ */
