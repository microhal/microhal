/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 26-08-2016
 * last modification: 26-08-2016
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#ifndef _MICROHAL_SERIALPORT_INTERRUPT_STM32F3XX_H_
#define _MICROHAL_SERIALPORT_INTERRUPT_STM32F3XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "../serialPort_stm32f3xx.h"
#include "buffers/cyclicBuffer.h"
#include "ports/common/serialPort_bufferedBase.h"

/* **************************************************************************************************************************************************
 * CLASS
 */
namespace microhal {
namespace stm32f3xx {
/* ************************************************************************************************
 * EXTERN DECLARATION
 */
extern "C" {
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
class SerialPort_interrupt : public common::SerialPort_BufferedBase<SerialPort_interrupt> {
 public:
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT
    static SerialPort_interrupt Serial1;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_INTERRUPT
    static SerialPort_interrupt Serial2;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_INTERRUPT
    static SerialPort_interrupt Serial3;
#endif

    //--------------------------------------------- functions ---------------------------------------//
    bool open(OpenMode mode) noexcept;

 private:
    //------------------------------------------- constructors --------------------------------------//
    inline SerialPort_interrupt(USART_TypeDef &usart, char *const rxData, char *const txData, size_t rxDataSize, size_t txDataSize);

    void startTransmission_impl() { usart.CR1 |= USART_CR1_TXEIE; }

    void updateRxBuffer_impl() {}

    void configureRxWait_impl(size_t bytesToReceive) { waitForBytes = bytesToReceive; }

    //------------------------------------------- friends -------------------------------------------//
    friend SerialPort_BufferedBase<SerialPort_interrupt>;
    void __SerialPort_USART_interruptFunction();

    friend void USART1_IRQHandler(void);
    friend void USART2_IRQHandler(void);
    friend void USART3_IRQHandler(void);
};

}  // namespace stm32f3xx
}  // namespace microhal

#endif  // _MICROHAL_SERIALPORT_INTERRUPT_STM32F3XX_H_
