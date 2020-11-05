/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 21-02-2017
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
#ifndef _MICROHAL_I2C_INTERRUPT_STMCOMMON_H_
#define _MICROHAL_I2C_INTERRUPT_STMCOMMON_H_
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#include "i2c_stmCommon.h"
#if _MICROHAL_PORT_STM_I2C_DRIVER_VERSION == 2  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_I2C(INTERRUPT)                  // Check if driver is enabled in microhal port config
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "i2c_stmCommon.h"
#include "microhal_semaphore.h"
#include _MICROHAL_INCLUDE_PORT_CONFIG

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

class I2C_interrupt : public _MICROHAL_ACTIVE_PORT_NAMESPACE::I2C {
 public:
//---------------------------------------- variables ----------------------------------------//
#ifdef MICROHAL_USE_I2C1_INTERRUPT
    static I2C_interrupt i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_INTERRUPT
    static I2C_interrupt i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_INTERRUPT
    static I2C_interrupt i2c3;
#endif

    //---------------------------------------- functions ----------------------------------------//
    Error writeRead(DeviceAddress address, const uint8_t *write, size_t writeLength, uint8_t *read_, size_t readLength) noexcept final;

    Error write(DeviceAddress deviceAddress, const uint8_t *data, size_t length) noexcept final;
    Error write(DeviceAddress deviceAddress, const uint8_t *data, size_t dataLength, const uint8_t *dataB, size_t dataBLength) noexcept final;

    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t length) noexcept final;
    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept final;

 private:
    enum class Mode { Receive = 0x01, Transmit = 0x02, TransmitReceive = 0x04, ReceiveDoubleBuffer = 0x11, TransmitDoubleBuffer = 0x12 };
    struct Buffer {
        uint8_t *ptr;
        size_t length;
    };
    typedef struct {
        Buffer bufferA;
        Buffer bufferB;
        Mode mode;
    } Transfer;

    Transfer transfer;
    volatile I2C::Error error;
    os::Semaphore semaphore;
    //---------------------------------------- constructors ---------------------------------------
    I2C_interrupt(registers::I2C &i2c);
    //---------------------------------------- functions ----------------------------------------//
    void IRQFunction();
    void ErrorIRQFunction();
    //------------------------------------------- friends -----------------------------------------
    friend void I2C1_ER_IRQHandler(void);
    friend void I2C1_EV_IRQHandler(void);
    friend void I2C2_ER_IRQHandler(void);
    friend void I2C2_EV_IRQHandler(void);
    friend void I2C3_ER_IRQHandler(void);
    friend void I2C3_EV_IRQHandler(void);
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif
#endif
#endif  // _MICROHAL_I2C_INTERRUPT_STMCOMMON_H_
