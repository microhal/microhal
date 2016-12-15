/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 11-09-2016
 * last modification: 11-09-2016
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "serialPort_interrupt_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {
//***********************************************************************************************//
//                                   STATIC VARIABLES
//***********************************************************************************************//
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT
static char txBufferData_1[MICROHAL_SERIAL_PORT1_TX_BUFFER_SIZE];
static char rxBufferData_1[MICROHAL_SERIAL_PORT1_RX_BUFFER_SIZE];
SerialPort_interrupt SerialPort_interrupt::Serial1(*USART1, rxBufferData_1, txBufferData_1, sizeof(rxBufferData_1), sizeof(txBufferData_1));
SerialPort &SerialPort::Serial1 = SerialPort_interrupt::Serial1;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_INTERRUPT
static char txBufferData_2[MICROHAL_SERIAL_PORT2_TX_BUFFER_SIZE];
static char rxBufferData_2[MICROHAL_SERIAL_PORT2_RX_BUFFER_SIZE];
SerialPort_interrupt SerialPort_interrupt::Serial2(*USART2, rxBufferData_2, txBufferData_2, sizeof(rxBufferData_2), sizeof(txBufferData_2));
SerialPort &SerialPort::Serial2 = SerialPort_interrupt::Serial2;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_INTERRUPT
static char txBufferData_3[MICROHAL_SERIAL_PORT3_TX_BUFFER_SIZE];
static char rxBufferData_3[MICROHAL_SERIAL_PORT3_RX_BUFFER_SIZE];
SerialPort_interrupt SerialPort_interrupt::Serial3(*USART3, rxBufferData_3, txBufferData_3, sizeof(rxBufferData_3), sizeof(txBufferData_3));
SerialPort &SerialPort::Serial3 = SerialPort_interrupt::Serial3;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_INTERRUPT
static char txBufferData_4[MICROHAL_SERIAL_PORT4_TX_BUFFER_SIZE];
static char rxBufferData_4[MICROHAL_SERIAL_PORT4_RX_BUFFER_SIZE];
SerialPort_interrupt SerialPort_interrupt::Serial4(*UART4, rxBufferData_4, txBufferData_4, sizeof(rxBufferData_4), sizeof(txBufferData_4));
SerialPort &SerialPort::Serial4 = SerialPort_interrupt::Serial4;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT5_INTERRUPT
static char txBufferData_5[MICROHAL_SERIAL_PORT5_TX_BUFFER_SIZE];
static char rxBufferData_5[MICROHAL_SERIAL_PORT5_RX_BUFFER_SIZE];
SerialPort_interrupt SerialPort_interrupt::Serial5(*USART5, rxBufferData_5, txBufferData_5, sizeof(rxBufferData_5), sizeof(txBufferData_5));
SerialPort &SerialPort::Serial5 = SerialPort_interrupt::Serial5;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_INTERRUPT
static char txBufferData_6[MICROHAL_SERIAL_PORT6_TX_BUFFER_SIZE];
static char rxBufferData_6[MICROHAL_SERIAL_PORT6_RX_BUFFER_SIZE];
SerialPort_interrupt SerialPort_interrupt::Serial6(*LPUART1, rxBufferData_6, txBufferData_6, sizeof(rxBufferData_6), sizeof(txBufferData_6));
SerialPort &SerialPort::Serial6 = SerialPort_interrupt::Serial6;
#endif

bool SerialPort_interrupt::open(OpenMode mode) {
    if (isOpen() || (mode > 0x03)) return false;
    usart.CR1 |= (mode << 2) | USART_CR1_UE | USART_CR1_RXNEIE;
    return true;
}


/**@brief This function clear buffer specified in dir parameter. If dir == Input function will
 *        flush rxBuffer, if dir == Output then txBuffer will be flushed.
 *        If dir == AllDirections both buffers will be cleared.
 *
 * @param[in] dir - buffer direction to be cleared
 * @retval true - if buffer was cleared successful
 * @retval false - if an error occurred
 */
bool SerialPort_interrupt::clear(Direction dir) {
    switch (dir) {
    case SerialPort::Input:
        rxBuffer.flush();
        break;
    case SerialPort::Output:
        txBuffer.flush();
        break;
    case SerialPort::AllDirections:
        rxBuffer.flush();
        txBuffer.flush();
        break;
    default:
        return false;
    }
    return true;
}

//***********************************************************************************************//
//                                     interrupt functions                                       //
//***********************************************************************************************//
inline void __SerialPort_USART_interruptFunction(USART_TypeDef * const usart,
        SerialPort_interrupt &serialObject) {
    uint16_t sr = usart->ISR;

    if (sr & USART_ISR_RXNE) {
        char tmp = usart->RDR;

        serialObject.rxBuffer.append(tmp);
        if (tmp == '\n') {

        }
    }
    if ((sr & USART_ISR_TXE) && (usart->CR1 & USART_CR1_TXEIE)) {
    	if (serialObject.txBuffer.isEmpty()) {
    		usart->CR1 &= ~USART_CR1_TXEIE;
    	} else {
    		usart->TDR = serialObject.txBuffer.get_unsafe();
    	}
       // if (serialObject.txBuffer.isEmpty()) {
        //    usart->CR1 &= ~USART_CR1_TXEIE;
            //todo
            // usart->CR1 |= USART_CR1_RXNEIE;
       // }
    }
}
//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT
void USART1_IRQHandler(void) {
    __SerialPort_USART_interruptFunction(USART1, SerialPort_interrupt::Serial1);
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_INTERRUPT
void USART2_IRQHandler(void) {
    __SerialPort_USART_interruptFunction(USART2, SerialPort_interrupt::Serial2);
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_INTERRUPT
void USART3_IRQHandler(void) {
    __SerialPort_USART_interruptFunction(USART3, SerialPort_interrupt::Serial3);
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_INTERRUPT
void UART4_IRQHandler(void) {
    __SerialPort_USART_interruptFunction(UART4, SerialPort_interrupt::Serial4);
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT5_INTERRUPT
void UART5_IRQHandler(void) {
    __SerialPort_USART_interruptFunction(UART5, SerialPort_interrupt::Serial5);
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_INTERRUPT
void LPUART1_IRQHandler(void) {
    __SerialPort_USART_interruptFunction(LPUART1, SerialPort_interrupt::Serial6);
}
#endif

} // namespace stm32l4xx
} // namespace microhal



