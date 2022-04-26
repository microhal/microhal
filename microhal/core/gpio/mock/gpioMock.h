/*
 * GPIO.h
 *
 *  Created on: Dec 2, 2020
 *      Author: pokas
 */

#ifndef _MICROHAL_GPIO_MOCK_H_
#define _MICROHAL_GPIO_MOCK_H_

#include <string>
#include "../gpio_interface.h"

namespace microhal {
namespace mock {

class GPIO : public microhal::GPIO {
 public:
    GPIO();
    GPIO(GPIO&& gpio)
        : m_name(std::move(gpio.m_name)), state(gpio.state), direction(gpio.direction), outputType(gpio.outputType), pullType(gpio.pullType) {}
    GPIO(std::string name, Direction dir);
    virtual ~GPIO();

    using microhal::GPIO::set;

    /** Set pin to high state */
    Error set() noexcept final;
    /** Set pin to low state */
    Error reset() noexcept final;

    PinStateReturnType get() const noexcept final { return state; }

    PinStateReturnType getOutputState() const noexcept { return state; }

    Expected<Direction, UnexpectedNegativeValue<Error>> getDirection() const noexcept { return direction; }

    Expected<OutputType, UnexpectedNegativeValue<Error>> getOutputType() const noexcept { return outputType; }

    Expected<PullType, UnexpectedNegativeValue<Error>> getPullType() const noexcept { return pullType; }

 private:
    std::string m_name;
    State state = State::Low;
    Direction direction = Direction::Input;
    OutputType outputType = OutputType::PushPull;
    PullType pullType = PullType::NoPull;

    Error configure(Direction, OutputType, PullType) final;
    // bool getConfiguration(Direction &dir, OutputType &otype, PullType &pull) const final;
};

}  // namespace mock
}  // namespace microhal

#endif /* _MICROHAL_GPIO_MOCK_H_ */
