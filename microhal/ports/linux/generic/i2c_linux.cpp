/*
 * i2c_linux.cpp
 *
 *  Created on: Apr 11, 2019
 *      Author: pokas
 */

#include "i2c_linux.h"
#include <errno.h>
#include <linux/i2c-dev.h>
#include "diagnostic/diagnostic.h"

namespace microhal {
namespace linux {

using namespace diagnostic;

I2C::Error I2C::read(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept {
    if (::ioctl(file, I2C_SLAVE, deviceAddress >> 1) < 0) return Error::Unknown;
    auto buffer = std::make_unique<uint8_t[]>(dataLength + dataBLength);
    auto readStatus = ::read(file, buffer.get(), dataLength + dataBLength);
    if (readStatus >= 0) {
        // we know that writeStatus is positive so it is safe to cast it into unsigned data
        if (static_cast<size_t>(readStatus) == dataLength + dataBLength) {
            std::copy_n(buffer.get(), dataLength, data);
            std::copy_n(buffer.get() + dataLength, dataBLength, dataB);
            return Error::None;
        }
    } else {
    }
    return Error::Unknown;
}

I2C::Error I2C::writeRead(DeviceAddress deviceAddress, const uint8_t *write, size_t writeLength, uint8_t *read, size_t readLength) noexcept {
    if (::ioctl(file, I2C_SLAVE, deviceAddress >> 1) < 0) return Error::Unknown;
    auto writeStatus = ::write(file, write, writeLength);
    if (writeStatus >= 0) {
        // we know that writeStatus is positive so it is safe to cast it into unsigned data
        if (static_cast<size_t>(writeStatus) == writeLength) {
            auto readStatus = ::read(file, read, readLength);
            if (readStatus >= 0) {
                // we know that readStatus is positive so it is safe to cast it into unsigned data
                if (static_cast<size_t>(readStatus) == readLength) {
                    return Error::None;
                } else {
                    diagChannel << MICROHAL_DEBUG << "I2C data read error, expected to read:" << readLength << " bytes, but we read: " << readStatus
                                << endl;
                }
            } else {
                diagChannel << MICROHAL_DEBUG << "I2C data read error, errno value: " << strerror(errno) << endl;
            }
        } else {
            diagChannel << MICROHAL_DEBUG << "I2C data write error, expected to write:" << writeLength << " bytes, but we write: " << writeStatus
                        << endl;
        }
    } else {
        diagChannel << MICROHAL_DEBUG << "I2C data write error, errno value: " << strerror(errno) << endl;
    }

    return Error::Unknown;
}

}  // namespace linux
}  // namespace microhal
