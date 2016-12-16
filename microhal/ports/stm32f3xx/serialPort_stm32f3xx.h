/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    buleks
 * created on: 22-07-2016
 * last modification: 22-07-2016
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

#ifndef _MICROHAL_SERIALPORT_STM32F3XX_H_
#define _MICROHAL_SERIALPORT_STM32F3XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <microhal.h>
#include <stdint.h>
#include "interfaces/serialPort_interface.h"
#include "microhalPortConfig_stm32f3xx.h"

/* **************************************************************************************************************************************************
 * CLASS
 */
namespace microhal {
namespace stm32f3xx {

class SerialPort : public microhal::SerialPort {
 public:
#if defined(MICROHAL_USE_SERIAL_PORT1_POLLING) || (defined MICROHAL_USE_SERIAL_PORT1_INTERRUPT)
    static SerialPort &Serial1;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT2_POLLING) || (defined MICROHAL_USE_SERIAL_PORT2_INTERRUPT)
    static SerialPort &Serial2;
#endif
#if (defined MICROHAL_USE_SERIAL_PORT3_POLLING) || (defined MICROHAL_USE_SERIAL_PORT3_INTERRUPT)
    static SerialPort &Serial3;
#endif

    virtual bool open(OpenMode mode) noexcept = 0;
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
        SerialPort(USART_TypeDef &usart)
        : usart(usart) {
    }
};

}  // namespace stm32f3xx
}  // namespace microhal

#endif  // _MICROHAL_SERIALPORT_STM32F3XX_H_
