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
#ifndef _MICROHAL_I2C_DMA_STMCOMMON_H_
#define _MICROHAL_I2C_DMA_STMCOMMON_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <ports/stmCommon/i2c_v2/i2c_stmCommon.h>
#include <cstdint>
#include "microhal_semaphore.h"

#include _MICROHAL_INCLUDE_PORT_clockManager
#include _MICROHAL_INCLUDE_PORT_DMA

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

extern "C" {
void I2C1_EV_IRQHandler(void);
void I2C2_EV_IRQHandler(void);
void I2C3_EV_IRQHandler(void);

void I2C1_ER_IRQHandler(void);
void I2C2_ER_IRQHandler(void);
void I2C3_ER_IRQHandler(void);

void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
class I2C_dma : public _MICROHAL_ACTIVE_PORT_NAMESPACE::I2C {
 public:
//---------------------------------------- variables ----------------------------------------//
#ifdef MICROHAL_USE_I2C1_DMA
    static I2C_dma i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_DMA
    static I2C_dma i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_DMA
    static I2C_dma i2c3;
#endif
 protected:
    Error writeRead(DeviceAddress address, const uint8_t *write, size_t writeLength, uint8_t *read, size_t readLength) noexcept final;

    Error write(DeviceAddress address, const uint8_t *write, size_t writeLength) noexcept final;
    Error write(DeviceAddress address, const uint8_t *write, size_t writeLength, const uint8_t *writeB, size_t writeBLength) noexcept final;

    Error read(DeviceAddress address, uint8_t *data, size_t length) noexcept final;
    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept final;

 private:
    enum class Mode { Receive = 0x01, Transmit = 0x02, TransmitReceive = 0x04, ReceiveDoubleBuffer = 0x11, TransmitDoubleBuffer = 0x12 };
    //---------------------------------- variables ----------------------------------
    volatile I2C::Error error;
    DMA::Channel &rxStream;
    DMA::Channel &txStream;
    os::Semaphore semaphore;

    struct Buffer {
        void *ptr;
        size_t length;
    };
    struct {
        Mode mode;
        size_t txLength;
        size_t rxLength;
        Buffer bufferB;
    } transfer;
    //---------------------------------- constructors -------------------------------
    I2C_dma(registers::I2C &i2c, DMA::Channel &rxStream, DMA::Channel &txStream)
        : I2C(i2c), error(), rxStream(rxStream), txStream(txStream), semaphore{}, transfer() {
        ClockManager::enableI2C(getNumber());
        init();
        switch (reinterpret_cast<uint32_t>(&i2c)) {
            case reinterpret_cast<uint32_t>(I2C1_BASE):
                NVIC_EnableIRQ(I2C1_EV_IRQn);
                NVIC_SetPriority(I2C1_EV_IRQn, 0);
                NVIC_EnableIRQ(I2C1_ER_IRQn);
                NVIC_SetPriority(I2C1_ER_IRQn, 0);
                break;
                //            case reinterpret_cast<uint32_t>(I2C2):
                //                NVIC_EnableIRQ(I2C2_EV_IRQn);
                //                NVIC_SetPriority(I2C2_EV_IRQn, 0);
                //                NVIC_EnableIRQ(I2C2_ER_IRQn);
                //                NVIC_SetPriority(I2C2_ER_IRQn, 0);
                //                break;
                //            case reinterpret_cast<uint32_t>(I2C3):
                //                NVIC_EnableIRQ(I2C3_EV_IRQn);
                //                NVIC_SetPriority(I2C3_EV_IRQn, 0);
                //                NVIC_EnableIRQ(I2C3_ER_IRQn);
                //                NVIC_SetPriority(I2C3_ER_IRQn, 0);
                //                break;
        }
    }

    //---------------------------------- functions ----------------------------------
    void init(void);
    static void IRQFunction(I2C_dma &obj, registers::I2C *i2c);
    void IRQErrorFunction();
    //---------------------------------- friends ------------------------------------

    friend void I2C1_EV_IRQHandler(void);
    friend void I2C2_EV_IRQHandler(void);
    friend void I2C3_EV_IRQHandler(void);

    friend void I2C1_ER_IRQHandler(void);
    friend void I2C2_ER_IRQHandler(void);
    friend void I2C3_ER_IRQHandler(void);

    friend void DMA1_Stream0_IRQHandler(void);
    friend void DMA1_Stream2_IRQHandler(void);
    friend void DMA1_Stream3_IRQHandler(void);
    friend void DMA1_Stream4_IRQHandler(void);
    friend void DMA1_Stream5_IRQHandler(void);
    friend void DMA1_Stream6_IRQHandler(void);
    friend void DMA1_Stream7_IRQHandler(void);
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // _MICROHAL_I2C_DMA_STMCOMMON_H_
