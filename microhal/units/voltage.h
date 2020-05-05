/*
 * voltage.h
 *
 *  Created on: Oct 10, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_UNITS_VOLTAGE_H_
#define SRC_MICROHAL_UNITS_VOLTAGE_H_

#include <cstdint>

namespace microhal {
namespace units {

template <typename T = float>
class Voltage {
 public:
    explicit Voltage(T voltage) : voltage(voltage) {}

    template <typename S = T>
    S get() const {
        return voltage;
    }

 private:
    T voltage;
};

}  // namespace units

template <microhal::diagnostic::LogLevel level, bool B, typename T>
inline microhal::diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel,
                                                                  const units::Voltage<T> &voltage) {
    return logChannel << voltage.get() << " V";
}
}  // namespace microhal

#endif /* SRC_MICROHAL_UNITS_VOLTAGE_H_ */
