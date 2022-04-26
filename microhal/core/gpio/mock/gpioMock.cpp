/*
 * GPIO.cpp
 *
 *  Created on: Dec 2, 2020
 *      Author: pokas
 */

#include "gpioMock.h"
#include "diagnostic/diagnostic.h"

using namespace microhal::diagnostic;

namespace microhal {
namespace mock {

GPIO::GPIO() {}

GPIO::GPIO(std::string name, Direction dir) : m_name(name), direction(dir) {}

GPIO::~GPIO() {}

GPIO::Error GPIO::set() noexcept {
    diagChannel << lock << MICROHAL_INFORMATIONAL << "MOCK GPIO " << m_name << " changing state to High" << unlock;
    state = State::High;
    return Error::None;
}

GPIO::Error GPIO::reset() noexcept {
    diagChannel << lock << MICROHAL_INFORMATIONAL << "MOCK GPIO " << m_name << " changing state to Low" << unlock;
    state = State::Low;
    return Error::None;
}

GPIO::Error GPIO::configure(Direction dir, OutputType otype, PullType pull) {
    diagChannel << lock << MICROHAL_INFORMATIONAL << "MOCK GPIO " << m_name << " changing configuration " << to_string(direction) << unlock;

    direction = dir;
    if (direction == Direction::Output) {
        outputType = otype;
    }
    pullType = pull;
    return Error::None;
}

// bool GPIO::getConfiguration(Direction &dir, OutputType &otype, PullType &pull) const {
//    dir = direction;
//    otype = outputType;
//    pull = pullType;
//    return true;
//}

}  // namespace mock
}  // namespace microhal
