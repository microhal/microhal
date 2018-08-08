/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 02-08-2018
 * last modification: 02-08-2018
 *
 * @copyright Copyright (c) 2018, Pawel Okas
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

#ifndef _MICROHAL_SERIALPORT_STM32F0XX_H_
#define _MICROHAL_SERIALPORT_STM32F0XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "device/stm32f0xx.h"
#include "interfaces/serialPort_interface.h"
#include "microhalPortConfig_stm32f0xx.h"

namespace microhal {
namespace stm32f0xx {
/* **************************************************************************************************************************************************
 * CLASS
 */
class SerialPort : public microhal::SerialPort {
 public:
#if (defined MICROHAL_USE_SERIAL_PORT1_POLLING) || (defined MICROHAL_USE_SERIAL_PORT1_INTERRUPT) || \
    (defined MICROHAL_USE_SERIAL_PORT1_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT1_DMA)
    static SerialPort &Serial1;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT2_POLLING) || (defined MICROHAL_USE_SERIAL_PORT2_INTERRUPT) || \
    (defined MICROHAL_USE_SERIAL_PORT2_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT2_DMA)
    static SerialPort &Serial2;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT3_POLLING) || (defined MICROHAL_USE_SERIAL_PORT3_INTERRUPT) || \
    (defined MICROHAL_USE_SERIAL_PORT3_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT3_DMA)
    static SerialPort &Serial3;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT4_POLLING) || (defined MICROHAL_USE_SERIAL_PORT4_INTERRUPT) || \
    (defined MICROHAL_USE_SERIAL_PORT4_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT4_DMA)
    static SerialPort &Serial4;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT5_POLLING) || (defined MICROHAL_USE_SERIAL_PORT5_INTERRUPT) || \
    (defined MICROHAL_USE_SERIAL_PORT5_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT5_DMA)
    static SerialPort &Serial5;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT6_POLLING) || (defined MICROHAL_USE_SERIAL_PORT6_INTERRUPT) || \
    (defined MICROHAL_USE_SERIAL_PORT6_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT6_DMA)
    static SerialPort &Serial6;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT7_POLLING) || (defined MICROHAL_USE_SERIAL_PORT7_INTERRUPT) || \
    (defined MICROHAL_USE_SERIAL_PORT7_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT7_DMA)
    static SerialPort &Serial7;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT8_POLLING) || (defined MICROHAL_USE_SERIAL_PORT8_INTERRUPT) || \
    (defined MICROHAL_USE_SERIAL_PORT8_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT8_DMA)
    static SerialPort &Serial8;
#endif

    bool open(OpenMode mode) noexcept override = 0;
    bool isOpen(void) const noexcept final { return ((usart.CR1 & USART_CR1_UE) == USART_CR1_UE); }
    void close() noexcept final { usart.CR1 = 0; }

    bool setBaudRate(uint32_t baudRate) noexcept final;
    uint32_t getBaudRate() const noexcept final;
    bool setParity(SerialPort::Parity parity) noexcept final;
    bool setStopBits(SerialPort::StopBits stopBits) noexcept final;
    bool setDataBits(SerialPort::DataBits dataBits) noexcept final;

    void priority(uint32_t priority) { NVIC_SetPriority(irq(), priority); }

 protected:
    //------------------------------------------- variables -----------------------------------------//
    USART_TypeDef &usart;
//------------------------------------------- constructors --------------------------------------//
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        SerialPort(USART_TypeDef &usart) noexcept
        : usart(usart) {
    }

    void enableInterrupt(uint32_t priority) {
        NVIC_EnableIRQ(irq());
        NVIC_SetPriority(irq(), priority);
    }

    uint32_t number() {
        if (&usart == USART1) return 1;
        if (&usart == USART2) return 2;
#if defined(USART3)
        if (&usart == USART3) return 3;
#endif
#if defined(UART4)
        if (&usart == UART4) return 4;
#endif
#if defined(UART5)
        if (&usart == UART5) return 5;
#endif
#if defined(USART6)
        if (&usart == USART6) return 6;
#endif
#if defined(UART7)
        if (&usart == UART7) return 7;
#endif
#if defined(UART8)
        if (&usart == UART8) return 8;
#endif
        std::terminate();
    }

    IRQn_Type irq() {
        if (&usart == USART1) return USART1_IRQn;
        if (&usart == USART2) return USART2_IRQn;
#if defined(USART3)
        if (&usart == USART3) return USART3_IRQn;
#endif
#if defined(UART4)
        if (&usart == UART4) return UART4_IRQn;
#endif
#if defined(UART5)
        if (&usart == UART5) return UART5_IRQn;
#endif
#if defined(USART6) && defined(RCC_APB2ENR_USART6EN)
        if (&usart == USART6) return USART6_IRQn;
#endif
#if defined(UART7)
        if (&usart == UART7) return UART7_IRQn;
#endif
#if defined(UART8)
        if (&usart == UART8) return UART8_IRQn;
#endif
        std::terminate();
    }
};

}  // namespace stm32f0xx
}  // namespace microhal

#endif  // _MICROHAL_SERIALPORT_STM32F0XX_H_