/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      STM32F4xx serial port driver implementation. Driver support receiving via interrupt and transmitting using DMA.
 *
 * @authors    Pawel Okas
 * created on: 7-07-2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, Pawel Okas
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

#ifndef _MICROHAL_SERIALPORT_RXINTERRUPTTXDMA_STM32F4XX_H_
#define _MICROHAL_SERIALPORT_RXINTERRUPTTXDMA_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "serialPort_bufferedBase.h"

#include <thread>

#include "buffers/cyclicBuffer.h"
#include "../dma_stm32f4xx.h"
#include "microhal_semaphore.h"


namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 * EXTERN DECLARATION
 */
extern "C" {
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void UART4_IRQHandler(void);
void UART5_IRQHandler(void);
void USART6_IRQHandler(void);

void DMA1_Stream6_IRQHandler(void);
void DMA1_Stream3_IRQHandler(void);
void DMA1_Stream4_IRQHandler(void);
void DMA1_Stream7_IRQHandler(void);

void DMA2_Stream6_IRQHandler(void);
void DMA2_Stream7_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
class SerialPort_RxInterruptTxDma: public SerialPort_BufferedBase<SerialPort_RxInterruptTxDma> {
 public:
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT_DMA
    static SerialPort_RxInterruptTxDma Serial1;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_INTERRUPT_DMA
    static SerialPort_RxInterruptTxDma Serial2;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_INTERRUPT_DMA
    static SerialPort_RxInterruptTxDma Serial3;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_INTERRUPT_DMA
    static SerialPort_RxInterruptTxDma Serial4;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT5_INTERRUPT_DMA
    static SerialPort_RxInterruptTxDma Serial5;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_INTERRUPT_DMA
    static SerialPort_RxInterruptTxDma Serial6;
#endif
//--------------------------------------------- functions ---------------------------------------//
    bool open(OpenMode mode) noexcept;

 private:
//------------------------------------------- variables -----------------------------------------//
    size_t transferInProgress = 0;
    DMA::DMA &dma;
    DMA::Stream &txStream;
//------------------------------------------- constructors --------------------------------------//
    inline SerialPort_RxInterruptTxDma(USART_TypeDef &usart, char * const rxData, char * const txData, size_t rxDataSize, size_t txDataSize, DMA::DMA &dma, DMA::Stream & txStream);

    //virtual ~SerialPort_interrupt(){
    //}
//--------------------------------------------- functions ---------------------------------------//
    size_t prepareDmaTransfer();
    void onDMATxFinish();

    void startTransmission_impl() {
    	if(transferInProgress == 0) {
    		prepareDmaTransfer();
    	}
    }

    void updateRxBuffer_impl() {

    }

    void configureRxWait_impl(size_t bytesToReceive) {
    	waitForBytes = bytesToReceive;
    }
//------------------------------------------- friends -------------------------------------------//
    friend SerialPort_BufferedBase<SerialPort_RxInterruptTxDma>;
    friend void serialPort_interruptFunction(USART_TypeDef * const usart, SerialPort_RxInterruptTxDma &serial);

    friend void USART1_IRQHandler(void);
    friend void USART2_IRQHandler(void);
    friend void USART3_IRQHandler(void);
    friend void USART4_IRQHandler(void);
    friend void USART5_IRQHandler(void);
    friend void USART6_IRQHandler(void);

    friend void DMA1_Stream6_IRQHandler(void);
    friend void DMA1_Stream3_IRQHandler(void);
    friend void DMA1_Stream4_IRQHandler(void);
    friend void DMA2_Stream7_IRQHandler(void);
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // _MICROHAL_SERIALPORT_RXINTERRUPTTXDMA_STM32F4XX_H_
