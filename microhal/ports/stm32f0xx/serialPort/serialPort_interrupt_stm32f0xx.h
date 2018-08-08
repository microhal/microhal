/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 06-08-2018
 * last modification: 06-08-2018
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

#ifndef _MICROHAL_SERIALPORT_INTERRUPT_STM32F0XX_H_
#define _MICROHAL_SERIALPORT_INTERRUPT_STM32F0XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "serialPort_bufferedBase.h"

#include <thread>

#include "buffers/cyclicBuffer.h"
#include "microhal_semaphore.h"

namespace microhal {
namespace stm32f0xx {
/* ************************************************************************************************
 * EXTERN DECLARATION
 */
extern "C" {
void USART1_IRQHandler(void);   /* USART1                       */
void USART2_IRQHandler(void);   /* USART2                       */
void USART3_4_IRQHandler(void); /* USART3, USART4 */
void USART3_6_IRQHandler(void); /* USART3, USART4, USART5, USART6 */
void USART3_8_IRQHandler(void); /* USART3, USART4, USART5, USART6, USART7, USART8 */
}

/* **************************************************************************************************************************************************
 * CLASS
 */
class SerialPort_interrupt : public SerialPort_BufferedBase<SerialPort_interrupt> {
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
#ifdef MICROHAL_USE_SERIAL_PORT4_INTERRUPT
    static SerialPort_interrupt Serial4;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT5_INTERRUPT
    static SerialPort_interrupt Serial5;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_INTERRUPT
    static SerialPort_interrupt Serial6;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_INTERRUPT
    static SerialPort_interrupt Serial7;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_INTERRUPT
    static SerialPort_interrupt Serial8;
#endif
    //--------------------------------------------- functions ---------------------------------------//
    bool open(OpenMode mode) noexcept;

 private:
    //------------------------------------------- variables -----------------------------------------//

    //------------------------------------------- constructors --------------------------------------//
    inline SerialPort_interrupt(USART_TypeDef &usart, char *const rxData, char *const txData, size_t rxDataSize, size_t txDataSize);

    // virtual ~SerialPort_interrupt(){
    //}
    //--------------------------------------------- functions ---------------------------------------//
    void startTransmission_impl() { usart.CR1 |= USART_CR1_TXEIE; }

    void updateRxBuffer_impl() {}

    void configureRxWait_impl(size_t bytesToReceive) { waitForBytes = bytesToReceive; }
    //------------------------------------------- friends -------------------------------------------//
    friend SerialPort_BufferedBase<SerialPort_interrupt>;
    friend inline void serialPort_interruptFunction(USART_TypeDef *const usart, SerialPort_interrupt &serialObject);

    friend void USART1_IRQHandler(void);   /* USART1                       */
    friend void USART2_IRQHandler(void);   /* USART2                       */
    friend void USART3_4_IRQHandler(void); /* USART3, USART4 */
    friend void USART3_6_IRQHandler(void); /* USART3, USART4, USART5, USART6 */
    friend void USART3_8_IRQHandler(void); /* USART3, USART4, USART5, USART6, USART7, USART8 */
};

}  // namespace stm32f0xx
}  // namespace microhal

#endif  // _MICROHAL_SERIALPORT_INTERRUPT_STM32F0XX_H_
