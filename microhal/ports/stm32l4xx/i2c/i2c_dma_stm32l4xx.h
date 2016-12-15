/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 17-09-2016
 * last modification: 17-09-2016
 *
 * @copyright Copyright (c) 2016, microHAL
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

#ifndef _MICROHAL_I2C_DMA_STM32L4XX_H_
#define _MICROHAL_I2C_DMA_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "../device/stm32l4xx.h"
#include "../i2c_stm32l4xx.h"
#include "../dma_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {
extern "C" {
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);

void DMA2_Channel6_IRQHandler(void);
void DMA2_Channel7_IRQHandler(void);
}
/* **************************************************************************************************************************************************
 * CLASS
 */
class I2C_dma: public stm32l4xx::I2C {
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
    I2C::Error write(uint8_t deviceAddress, uint8_t data) override final;
    I2C::Error write(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data) override final;
    I2C::Error write(uint8_t deviceAddress, uint8_t registerAddress, const void *data, size_t length) override final;

    I2C::Error read(uint8_t deviceAddress, uint8_t &data) override final;
    I2C::Error read(uint8_t deviceAddress, uint8_t registerAddress, uint8_t &data) override final;
    I2C::Error read(uint8_t deviceAddress, uint8_t registerAddress, void *data, size_t length) override final;
private:
    typedef enum {
        Receive = 0x01,
        ReceiveFromRegister = 0x02,
        Transmit = 0x04
    } Mode;
    //---------------------------------- variables ----------------------------------
    volatile I2C::Error errorSemaphore;
    DMA::Channel &rxChannel;
    DMA::Channel &txChannel;

    struct {
		Mode mode;
		uint8_t deviceAddress;
		uint8_t registerAddress;
		size_t length;
    } transfer;
    //---------------------------------- constructors -------------------------------
    I2C_dma(I2C_TypeDef &i2c, DMA::Channel & rxChannel, DMA::Channel & txChannel) :
            I2C(i2c), errorSemaphore(), rxChannel(rxChannel), txChannel(txChannel), transfer() {
        init();
        switch (reinterpret_cast<uint32_t>(&i2c)) {
        case reinterpret_cast<uint32_t>(I2C1):
            RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN;
            NVIC_EnableIRQ (I2C1_EV_IRQn);
            NVIC_SetPriority(I2C1_EV_IRQn, 0);
            NVIC_EnableIRQ (I2C1_ER_IRQn);
            NVIC_SetPriority(I2C1_ER_IRQn, 0);
            break;
        case reinterpret_cast<uint32_t>(I2C2):
            RCC->APB1ENR1 |= RCC_APB1ENR1_I2C2EN;
            NVIC_EnableIRQ (I2C2_EV_IRQn);
            NVIC_SetPriority(I2C2_EV_IRQn, 0);
            NVIC_EnableIRQ (I2C2_ER_IRQn);
            NVIC_SetPriority(I2C2_ER_IRQn, 0);
            break;
        case reinterpret_cast<uint32_t>(I2C3):
            RCC->APB1ENR1 |= RCC_APB1ENR1_I2C3EN;
            NVIC_EnableIRQ (I2C3_EV_IRQn);
            NVIC_SetPriority(I2C3_EV_IRQn, 0);
            NVIC_EnableIRQ (I2C3_ER_IRQn);
            NVIC_SetPriority(I2C3_ER_IRQn, 0);
            break;
        }
    }

    //---------------------------------- functions ----------------------------------
    void init(void);
    static void IRQFunction(I2C_dma &obj, I2C_TypeDef *i2c);
    static void IRQErrorFunction(I2C_dma &obj, I2C_TypeDef *i2c);
    //---------------------------------- friends ------------------------------------


    friend void I2C1_EV_IRQHandler(void);
    friend void I2C2_EV_IRQHandler(void);
    friend void I2C3_EV_IRQHandler(void);

    friend void I2C1_ER_IRQHandler(void);
    friend void I2C2_ER_IRQHandler(void);
    friend void I2C3_ER_IRQHandler(void);

    friend void DMA1_Channel2_IRQHandler(void);
    friend void DMA1_Channel3_IRQHandler(void);
    friend void DMA1_Channel4_IRQHandler(void);
    friend void DMA1_Channel5_IRQHandler(void);
    friend void DMA1_Channel6_IRQHandler(void);
    friend void DMA1_Channel7_IRQHandler(void);

    friend void DMA2_Channel6_IRQHandler(void);
    friend void DMA2_Channel7_IRQHandler(void);
};

} // namespace stm32l4xx
} // namespace microhal

#endif  // _MICROHAL_I2C_DMA_STM32L4XX_H_
