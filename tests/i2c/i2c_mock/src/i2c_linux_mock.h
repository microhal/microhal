/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      I2C implementation for STM32F4xx microcontroler
 *
 * @authors    Pawel Okas
 * created on: 2017
 * last modification: 18-10-2017
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

#ifndef _MICROHAL_I2C_LINUX_MOCK_H_
#define _MICROHAL_I2C_LINUX_MOCK_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "interfaces/i2c_interface.h"
#include "mockDevice.h"

namespace microhal {
class I2CDevice;
namespace linux {
/* ************************************************************************************************
 * CLASS
 */
/**
 * @addtogroup stm32f4xx_port
 * @{
 * @class I2C
 * @}
 * \brief   This class implements I2C functions.
 */
class I2C : public microhal::I2C {
 public:
    //---------------------------------- variables
    //----------------------------------
    //#if (defined MICROHAL_USE_I2C1_MOCK)
    static microhal::I2C &mocked;
    //#endif
    //---------------------------------- functions ----------------------------------
    /**
     * @brief This function change I2C peripheral speed and mode. Changing mode is only
     * possible when I2C peripheral is disabled.
     *
     * @retval true if mode was set.
     * @retval false if I2C is enabled and mode can't be changed.
     */
    bool speed(Speed speed, Mode mode) noexcept final { return false; }
    Speed speed() noexcept final {}
    void busReset() noexcept final {}

    Error writeRead(DeviceAddress deviceAddress, const uint8_t *write, size_t write_size, uint8_t *read, size_t read_size) noexcept final {
        for (auto &dev : devices) {
            if (dev->addr() == deviceAddress) {
                dev->writeRead(write, write_size, read, read_size);
                return I2C::Error::None;
            }
        }
        return I2C::Error::AcknowledgeFailure;
    }

    Error write(DeviceAddress deviceAddress, const uint8_t *data, size_t size) noexcept final {
        for (auto &dev : devices) {
            if (dev->addr() == deviceAddress) {
                dev->write(data, size);
                return I2C::Error::None;
            }
        }
        return I2C::Error::AcknowledgeFailure;
    }
    Error write(DeviceAddress deviceAddress, const uint8_t *dataA, size_t sizeA, const uint8_t *dataB, size_t sizeB) noexcept final {
        for (auto &dev : devices) {
            if (dev->addr() == deviceAddress) {
                dev->write(dataA, sizeA, dataB, sizeB);
                return I2C::Error::None;
            }
        }
        return I2C::Error::AcknowledgeFailure;
    }

    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t size) noexcept final {
        for (auto &dev : devices) {
            if (dev->addr() == deviceAddress) {
                dev->read(data, size);
                return I2C::Error::None;
            }
        }
        return I2C::Error::AcknowledgeFailure;
    }
    Error read(DeviceAddress deviceAddress, uint8_t *dataA, size_t sizeA, uint8_t *dataB, size_t sizeB) noexcept final {
        if (deviceAddress == 12) return I2C::Error::None;
        return I2C::Error::AcknowledgeFailure;
    }

    void addDevice(Device *dev) { devices.push_back(dev); }

// protected:
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        I2C() {
    }

 private:
    //----------------------------------------- friends -----------------------------------------//
    friend microhal::I2CDevice;
    std::vector<Device *> devices;
};

}  // namespace linux
}  // namespace microhal
#endif  // _MICROHAL_I2C_LINUX_MOCK_H_
