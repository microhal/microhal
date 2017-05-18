/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 17-04-2014
 * last modification: 29-12-2016
 *
 * @copyright Copyright (c) 2014-2016, Pawel Okas
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
#ifndef I2C_INTERRUPT_STM32F4XX_H_
#define I2C_INTERRUPT_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "../clockManager.h"
#include "../device/stm32f4xx.h"
#include "../i2c_stm32f4xx.h"
#include "microhal_semaphore.h"

namespace microhal {
namespace stm32f4xx {

class I2C_interrupt : public stm32f4xx::I2C {
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
 private:
    enum class Mode { Receive = 0x01, Transmit = 0x02, TransmitReceive = 0x04, ReceiveDoubleBuffer = 0x11, TransmitDoubleBuffer = 0x12 };
    struct Buffer {
        uint8_t *ptr;
        size_t length;
    };
    typedef struct {
        uint8_t deviceAddress;
        Buffer bufferA;
        Buffer bufferB;
        Mode mode;
    } Transfer;

    Transfer transfer;
    volatile I2C::Error error;
    os::Semaphore semaphore;
    //---------------------------------------- constructors ---------------------------------------
    I2C_interrupt(I2C_TypeDef &i2c) : I2C(i2c), transfer(), error(), semaphore() {
        ClockManager::enable(i2c);
        const uint32_t priority = 0;
        enableEventInterrupt(priority);
        enableErrorInterrupt(priority);
    }
    //---------------------------------------- functions ----------------------------------------//
    Error writeRead(DeviceAddress address, const uint8_t *write, size_t writeLength, uint8_t *read_, size_t readLength) noexcept final;

    Error write(DeviceAddress deviceAddress, const uint8_t *data, size_t length) noexcept final;
    Error write(DeviceAddress deviceAddress, const uint8_t *data, size_t dataLength, const uint8_t *dataB, size_t dataBLength) noexcept final;

    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t length) noexcept final;
    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept final;

    static void IRQFunction(I2C_interrupt &obj, I2C_TypeDef *i2c);
    //------------------------------------------- friends -----------------------------------------
    friend void I2C1_ER_IRQHandler(void);
    friend void I2C1_EV_IRQHandler(void);
    friend void I2C2_ER_IRQHandler(void);
    friend void I2C2_EV_IRQHandler(void);
    friend void I2C3_ER_IRQHandler(void);
    friend void I2C3_EV_IRQHandler(void);
};

}  // namsepace stm32f4xx
}  // namespace microhal

#endif  // I2C_INTERRUPT_STM32F4XX_H_
