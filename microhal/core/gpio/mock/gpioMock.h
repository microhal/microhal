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
    int set() noexcept final {
        state = true;
        return 1;
    }
    /** Set pin to low state */
    int reset() noexcept final {
        state = false;
        return 1;
    }

    int get() const noexcept final { return state; }

    int getOutputState() const noexcept { return state; }

 private:
    bool state = 0;
    Direction direction = Direction::Input;
    OutputType outputType = OutputType::PushPull;
    PullType pullType = PullType::NoPull;

    bool configure(Direction, OutputType, PullType) final;
    bool getConfiguration(Direction &dir, OutputType &otype, PullType &pull) const final;
};

}  // namespace mock
}  // namespace microhal

#endif /* _MICROHAL_GPIO_MOCK_H_ */
