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

#include <cstdint>
#include <filesystem>

#include "IOPin.h"
#include "gpio/gpio_interface.h"

namespace microhal {
namespace linux {

/* ************************************************************************************************
 * CLASS
 */
class GPIO : public microhal::GPIO {
 public:
    //--------------------------------------- constructors --------------------------------------//
    GPIO() = default;
    GPIO(const GPIO &) = delete;
    GPIO(GPIO &&b)
        : pin(b.pin),
          output_state_requested(b.output_state_requested),
          value_file_path(std::move(b.value_file_path)),
          direction_file_path(std::move(b.direction_file_path)) {
        b.pin.pin = -1;
    }
    /**
     * @brief Constructor of GPIO class
     *
     * @param port - port to be used
     * @param pin - pin on used port
     * @param dir - direction of port
     * @param pull - pull up setting
     * @param type - output type setting
     */
    GPIO(IOPin pin, Direction dir, PullType pull = PullType::NoPull, OutputType type = OutputType::PushPull);
    ~GPIO() override;

    void operator=(const GPIO &) = delete;
    void operator=(GPIO &&) = delete;
    //------------------------------------ static functions -------------------------------------//
    using microhal::GPIO::set;
    /** This function set pin to high state.
     *
     */
    Error set() noexcept final;
    /** This function set pin to low state.
     *
     */
    Error reset() noexcept final;
    /** This function read pin state
     *
     * @return
     */
    PinStateReturnType get() const noexcept final;

    PinStateReturnType getOutputState() const noexcept final;
    /** This function set pin direction.
     *
     * @param port - port name
     * @param pin - pin number
     * @param direction - pin direction
     */
    void setDirection(Direction direction);

    Expected<Direction, UnexpectedNegativeValue<Error>> getDirection() const noexcept final;

    Expected<OutputType, UnexpectedNegativeValue<Error>> getOutputType() const noexcept final;

    Expected<PullType, UnexpectedNegativeValue<Error>> getPullType() const noexcept final;

    /** This function set pin pull type
     *
     * @param port
     * @param pin
     * @param pullType
     */
    void setPullType(PullType pullType) {}

 protected:
    IOPin pin{};
    State output_state_requested{};
    std::filesystem::path value_file_path;
    std::filesystem::path direction_file_path;

    Error configure(Direction direction, OutputType outputType, PullType pullType) final {
        setDirection(direction);
        (void)outputType;
        setPullType(pullType);
        return Error::None;
    }

    void init_file_path();
};

}  // namespace linux
}  // namespace microhal

namespace microhal {
namespace activePort = linux;
}

#endif  // _MICROHAL_GPIO_LINUX_H_
