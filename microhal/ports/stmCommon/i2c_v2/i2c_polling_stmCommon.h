/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 20-02-2017
 * last modification: 24-02-2017
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

#ifndef _MICROHAL_I2C_POLLING_STMCOMMON_H_
#define _MICROHAL_I2C_POLLING_STMCOMMON_H_
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#include "i2c_stmCommon.h"
#if _MICROHAL_PORT_STM_I2C_DRIVER_VERSION == 2  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_I2C(POLLING)                    // Check if driver is enabled in microhal port config
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "i2c_stmCommon.h"
#include "ports/stmCommon/clockManager/i2cClock.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
/* ************************************************************************************************
 * CLASS
 */
class I2C_polling : public _MICROHAL_ACTIVE_PORT_NAMESPACE::I2C {
 public:
//---------------------------------------- variables ----------------------------------------//
#ifdef MICROHAL_USE_I2C1_POLLING
    static I2C_polling i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_POLLING
    static I2C_polling i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_POLLING
    static I2C_polling i2c3;
#endif

    Error writeRead(DeviceAddress address, const uint8_t *write, size_t write_size, uint8_t *read, size_t read_size) noexcept final;

    Error write(DeviceAddress deviceAddress, const uint8_t *write, size_t write_size) noexcept final;
    Error write(DeviceAddress deviceAddress, const uint8_t *write_data, size_t write_data_size, const uint8_t *write_dataB,
                size_t write_data_sizeB) noexcept final;

    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength) noexcept final {
        Error error = read_implementation(deviceAddress, data, dataLength, nullptr, 0);
        waitForStopSend();
        return error;
    }
    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept final {
        Error error = read_implementation(deviceAddress, data, dataLength, dataB, dataBLength);
        waitForStopSend();
        return error;
    }

 private:
    //---------------------------------------- constructors ---------------------------------------
    I2C_polling(registers::I2C &i2c) : I2C(i2c) { ClockManager::enableI2C(getNumber()); }
    //---------------------------------------- functions ----------------------------------------//
    Error write(const uint8_t *data, size_t length, size_t &toRead);
    Error readInto(uint8_t *data, size_t length, size_t &toRead);
    Error write_implementation(DeviceAddress deviceAddress, const uint8_t *write_data, size_t write_data_size, const uint8_t *write_dataB,
                               size_t write_data_sizeB);
    Error read_implementation(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept;
    void waitForStopSend();
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif
#endif
#endif  // _MICROHAL_I2C_POLLING_STMCOMMON_H_
