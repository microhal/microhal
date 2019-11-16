/*
 * as5311.h
 *
 *  Created on: Nov 13, 2019
 *      Author: pokas
 */

#ifndef SRC_AS5311_AS5311_H_
#define SRC_AS5311_AS5311_H_

#include <cstdint>
#include "SPIDevice/SPIDevice.h"
#include "microhal.h"

namespace microhal {

namespace as5311Detail {
enum class Status {
    OffsetCompensationFinished = 0b10000,
    CordicOverflow = 0b01000,
    LinearityAlarm = 0b00100,
};

constexpr Status operator|(Status a, Status b) {
    auto tmp = static_cast<uint8_t>(a) | static_cast<uint8_t>(b);
    return static_cast<Status>(tmp);
}

constexpr Status operator&(Status a, Status b) {
    auto tmp = static_cast<uint8_t>(a) & static_cast<uint8_t>(b);
    return static_cast<Status>(tmp);
}
}  // namespace as5311Detail

using as5311Detail::operator&;
using as5311Detail::operator|;

class AS5311 {
 public:
    using Status = as5311Detail::Status;

    AS5311(SPI &spi, GPIO &cs) : spi(spi, cs) {}

    bool update();

    uint16_t getPosition() const { return position; }
    bool isPositionValid() const {
        return (status & (Status::OffsetCompensationFinished | Status::CordicOverflow | Status::LinearityAlarm)) ==
               Status::OffsetCompensationFinished;
    }
    Status getStatus() const { return status; }

 private:
    SPIDevice spi;
    uint16_t position = 0;
    Status status = {};
};

}  // namespace microhal
#endif /* SRC_AS5311_AS5311_H_ */
