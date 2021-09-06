/*
 * GPIO.h
 *
 *  Created on: Dec 2, 2020
 *      Author: pokas
 */

#ifndef _MICROHAL_GPIO_MOCK_H_
#define _MICROHAL_GPIO_MOCK_H_

#include "../gpio_interface.h"

namespace microhal {
namespace mock {

class GPIO : public microhal::GPIO {
 public:
    GPIO();
    virtual ~GPIO();

    /** Set pin to high state */
    Error set() noexcept final {
        state = State::High;
        return Error::None;
    }
    /** Set pin to low state */
    Error reset() noexcept final {
        state = State::Low;
        return Error::None;
    }

    PinStateReturnType get() const noexcept final { return state; }

    PinStateReturnType getOutputState() const noexcept { return state; }

 private:
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
