/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      Serial Port implementation for STM32F4xx
 *
 * @authors    Pawel
 * created on: 2013
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2013-2016, microHAL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 	   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * 	      documentation and/or other materials provided with the distribution.
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

#ifndef _MICROHAL_SERIALPORT_STM32F4XX_H_
#define _MICROHAL_SERIALPORT_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "stm32f4xx.h"
#include "interfaces/serialPort_interface.h"
#include "microhalPortConfig_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 * CLASS
 */
/**
 * @addtogroup stm32f4xx_port
 * @{
 * @class SerialPort
 * @}
 * \brief   This class implements SerialPort functions.
 */
class SerialPort: public microhal::SerialPort {
public:
#if (defined MICROHAL_USE_SERIAL_PORT1_POLLING) || (defined MICROHAL_USE_SERIAL_PORT1_INTERRUPT) || (defined MICROHAL_USE_SERIAL_PORT1_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT1_DMA)
    static SerialPort &Serial1;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT2_POLLING) || (defined MICROHAL_USE_SERIAL_PORT2_INTERRUPT) || (defined MICROHAL_USE_SERIAL_PORT2_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT2_DMA)
    static SerialPort &Serial2;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT3_POLLING) || (defined MICROHAL_USE_SERIAL_PORT3_INTERRUPT) || (defined MICROHAL_USE_SERIAL_PORT3_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT3_DMA)
    static SerialPort &Serial3;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT4_POLLING) || (defined MICROHAL_USE_SERIAL_PORT4_INTERRUPT) || (defined MICROHAL_USE_SERIAL_PORT4_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT4_DMA)
    static SerialPort &Serial4;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT5_POLLING) || (defined MICROHAL_USE_SERIAL_PORT5_INTERRUPT) || (defined MICROHAL_USE_SERIAL_PORT5_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT5_DMA)
    static SerialPort &Serial5;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT6_POLLING) || (defined MICROHAL_USE_SERIAL_PORT6_INTERRUPT) || (defined MICROHAL_USE_SERIAL_PORT6_INTERRUPT_DMA) || (defined MICROHAL_USE_SERIAL_PORT6_DMA)
    static SerialPort &Serial6;
#endif

    bool open(OpenMode mode) noexcept override = 0;
    bool isOpen(void) const noexcept final { return ((usart.CR1 & USART_CR1_UE) == USART_CR1_UE); }
    void close() noexcept final { usart.CR1 = 0; }

    bool setBaudRate(uint32_t baudRate) noexcept final;
    uint32_t getBaudRate() const noexcept final;
    bool setParity(SerialPort::Parity parity) noexcept final;
    bool setStopBits(SerialPort::StopBits stopBits) noexcept final;
    bool setDataBits(SerialPort::DataBits dataBits) noexcept final;
protected:
//------------------------------------------- variables -----------------------------------------//
    USART_TypeDef &usart;
//------------------------------------------- constructors --------------------------------------//
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
    SerialPort(USART_TypeDef &usart) noexcept : usart(usart) {}

    void enableInterrupt(uint32_t priority) {
        switch (reinterpret_cast<uint32_t>(&usart)) {
        case reinterpret_cast<uint32_t>(USART1):
            NVIC_EnableIRQ(USART1_IRQn);
            NVIC_SetPriority(USART1_IRQn, priority);
            break;
        case reinterpret_cast<uint32_t>(USART2):
            NVIC_EnableIRQ(USART2_IRQn);
            NVIC_SetPriority(USART2_IRQn, priority);
            break;
        case reinterpret_cast<uint32_t>(USART3):
            NVIC_EnableIRQ (USART3_IRQn);
            NVIC_SetPriority(USART3_IRQn, priority);
            break;
        case reinterpret_cast<uint32_t>(UART4):
            NVIC_EnableIRQ (UART4_IRQn);
            NVIC_SetPriority(UART4_IRQn, priority);
            break;
        case reinterpret_cast<uint32_t>(UART5):
            NVIC_EnableIRQ (UART5_IRQn);
            NVIC_SetPriority(UART5_IRQn, priority);
            break;
        case reinterpret_cast<uint32_t>(USART6):
            NVIC_EnableIRQ (USART6_IRQn);
            NVIC_SetPriority(USART6_IRQn, priority);
            break;
        }
    }

   // virtual ~SerialPort(){}
};

} // namespace stm32f4xx
} // namespace microhal

#endif // _MICROHAL_SERIALPORT_STM32F4XX_H_
