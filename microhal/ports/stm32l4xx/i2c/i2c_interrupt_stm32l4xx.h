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

#ifndef _MICROHAL_I2C_INTERRUPT_STM32L4XX_H_
#define _MICROHAL_I2C_INTERRUPT_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "../i2c_stm32l4xx.h"
#include <stdint.h>
#include "../device/stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {
/* **************************************************************************************************************************************************
 * CLASS
 */
class I2C_interrupt: public stm32l4xx::I2C {
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
    typedef enum {
        RECEIVE_FROM_REGISTER = 0x02, RECEIVE = 0x01, TRANSMIT = 0x04

    } Mode;
    typedef struct {
        uint8_t deviceAddress;
        uint8_t registerAddress;
        size_t length;
        uint8_t *buffer_ptr;
        Mode mode;
    } Transfer;

    Transfer transfer;

    volatile I2C::Error ErrorSemaphore;
//---------------------------------------- constructors ---------------------------------------
    I2C_interrupt(I2C_TypeDef &i2c) :
            I2C(i2c), transfer(), ErrorSemaphore() {
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
//---------------------------------------- functions ----------------------------------------//
    I2C::Error write(uint8_t deviceAddress, uint8_t data) override final;
    I2C::Error write(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data) override final;
    I2C::Error write(uint8_t deviceAddress, uint8_t registerAddress, const void *data, size_t length) override final;
    I2C::Error read(uint8_t deviceAddress, uint8_t &data) override final;
    I2C::Error read(uint8_t deviceAddress, uint8_t registerAddress, uint8_t &data) override final;
    I2C::Error read(uint8_t deviceAddress, uint8_t registerAddress, void *data, size_t length) override final;

    static void IRQFunction(I2C_interrupt &obj, I2C_TypeDef *i2c);
//------------------------------------------- friends -----------------------------------------
    friend void I2C1_ER_IRQHandler(void);
    friend void I2C1_EV_IRQHandler(void);
    friend void I2C2_ER_IRQHandler(void);
    friend void I2C2_EV_IRQHandler(void);
    friend void I2C3_ER_IRQHandler(void);
    friend void I2C3_EV_IRQHandler(void);
}
;

} // namsepace stm32l4xx
} // namespace microhal

#endif  // _MICROHAL_I2C_INTERRUPT_STM32L4XX_H_
