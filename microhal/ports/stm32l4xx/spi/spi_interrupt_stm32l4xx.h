/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 08-10-2016
 * last modification: 08-10-2016
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

#ifndef _MICROHAL_SPI_INTERRUPT_STM32L4XX_H_
#define _MICROHAL_SPI_INTERRUPT_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "../spi_stm32l4xx.h"
#include "microhal_semaphore.h"

namespace microhal {
namespace stm32l4xx {
/* **************************************************************************************************************************************************
 * CLASS
 */
class SPI_interrupt: public stm32l4xx::SPI {
public:
    //---------------------------------------- variables ----------------------------------------//
#ifdef MICROHAL_USE_SPI1_INTERRUPT
    static SPI_interrupt spi1;
#endif
#ifdef MICROHAL_USE_SPI2_INTERRUPT
    static SPI_interrupt spi2;
#endif
#ifdef MICROHAL_USE_SPI3_INTERRUPT
    static SPI_interrupt spi3;
#endif
    //---------------------------------------- functions ----------------------------------------//
    SPI::Error write(uint8_t data, bool last) override final;
    SPI::Error read(uint8_t &data, uint8_t write = 0x00) override final;
    SPI::Error writeBuffer(const void *data, size_t len, bool last) override final;
    SPI::Error readBuffer(void *data, size_t len, uint8_t write = 0x00) override final;
private:
    typedef enum {
        WAITING, TRANSMIT, RECEIVE
    } Mode;
    //---------------------------------------- variables ----------------------------------------//
    uint8_t *bufferPtr = nullptr;
    uint8_t *bufferEnd = nullptr;
    uint8_t writeData = 0x00;
    Mode mode = WAITING;

    //volatile bool semaphore;
    os::Semaphore semaphore;

    //--------------------------------------- constructors --------------------------------------//
    SPI_interrupt(SPI_TypeDef &spi, stm32l4xx::GPIO::IOPin misoPin) :
            SPI(spi, misoPin), semaphore() {
#if defined(HAL_RTOS_FreeRTOS)
    	const uint32_t priority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
#else
    	const uint32_t priority = 0;
#endif
        switch (reinterpret_cast<uint32_t>(&spi)) {
        case reinterpret_cast<uint32_t>(SPI1):
            RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
            NVIC_EnableIRQ(SPI1_IRQn);
            NVIC_SetPriority(SPI1_IRQn, priority);
            break;
        case reinterpret_cast<uint32_t>(SPI2):
            RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
            NVIC_EnableIRQ(SPI2_IRQn);
            NVIC_SetPriority(SPI2_IRQn, priority);
            break;
        case reinterpret_cast<uint32_t>(SPI3):
            RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;
            NVIC_EnableIRQ (SPI3_IRQn);
            NVIC_SetPriority(SPI3_IRQn, priority);
            break;
        }
    }

    virtual ~SPI_interrupt() { }
    SPI_interrupt& operator=(const SPI_interrupt&);

    SPI_interrupt(const SPI_interrupt&);
    //---------------------------------------- functions ----------------------------------------//
    void enableTransmitterEmptyInterrupt() {
        spi.CR2 |= SPI_CR2_TXEIE; //fixme maybe bitband
    }
    void enableReceiverNotEmptyInterrupt() {
        spi.CR2 |= SPI_CR2_RXNEIE; //fixme maybe bitband
    }
    //----------------------------------------- friends -----------------------------------------//
    friend inline void IRQfunction(SPI_interrupt &object, SPI_TypeDef *spi) __attribute__ ((always_inline));
    friend void SPI1_IRQHandler(void);
    friend void SPI2_IRQHandler(void);
    friend void SPI3_IRQHandler(void);
}
;
} // namespace stm32l4xx
} // namespace microhal

#endif  // _MICROHAL_SPI_INTERRUPT_STM32L4XX_H_
