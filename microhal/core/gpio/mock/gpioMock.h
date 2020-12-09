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
    bool set() final {
        state = true;
        return true;
    }
    /** Set pin to low state */
    bool reset() final {
        state = false;
        return true;
    }

    bool get() const final { return state; }

    bool getOutputState() const { return state; }

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
