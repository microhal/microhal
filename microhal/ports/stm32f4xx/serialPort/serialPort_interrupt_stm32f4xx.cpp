/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      STM32F4xx serial port driver implementation. Driver support receiving and transmitting using interrupts.
 *
 * @authors    Pawel Okas
 * created on: 17-04-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014-2016, Pawel Okas
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

/* ************************************************************************************************
 * INCLUDES
 */
#include "serialPort_interrupt_stm32f4xx.h"
#include "../clockManager.h"

namespace microhal {
namespace stm32f4xx {
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
SerialPort_interrupt SerialPort_interrupt::Serial6(*USART6, rxBufferData_6, txBufferData_6, sizeof(rxBufferData_6), sizeof(txBufferData_6));
SerialPort &SerialPort::Serial6 = SerialPort_interrupt::Serial6;
#endif

SerialPort_interrupt::SerialPort_interrupt(USART_TypeDef &usart, char * const rxData, char * const txData, size_t rxDataSize, size_t txDataSize) :
		SerialPort_BufferedBase(usart, rxData, rxDataSize, txData, txDataSize) {
	ClockManager::enable(usart);
	enableInterrupt(0);
}

bool SerialPort_interrupt::open(OpenMode mode) noexcept {
    if (isOpen() || (mode > 0x03)) return false;
    usart.CR1 |= (mode << 2) | USART_CR1_UE | USART_CR1_RXNEIE;
    return true;
}

//***********************************************************************************************//
//                                     interrupt functions                                       //
//***********************************************************************************************//
inline void serialPort_interruptFunction(USART_TypeDef * const usart, SerialPort_interrupt &serial) {
    uint16_t sr = usart->SR;

    if (sr & USART_SR_RXNE) {
        char tmp = usart->DR;
        serial.rxBuffer.append(tmp);
        if (serial.waitForBytes != 0 && serial.rxBuffer.getLength() == serial.waitForBytes) {
        	serial.waitForBytes = 0;
        	bool shouldYeld = serial.rxSemaphore.giveFromISR();
#if defined (HAL_RTOS_FreeRTOS)
        	portYIELD_FROM_ISR( shouldYeld );
#else
        	(void)shouldYeld;
#endif
        }
    }
    if ((sr & USART_SR_TXE) && (usart->CR1 & USART_CR1_TXEIE)) {
    	if (serial.txBuffer.isEmpty()) {
    		usart->CR1 &= ~USART_CR1_TXEIE;
    		if (serial.txWait) {
    			auto shouldYeld = serial.txFinish.giveFromISR();
#if defined (HAL_RTOS_FreeRTOS)
    			portYIELD_FROM_ISR(shouldYeld);
#else
    			(void)shouldYeld;
#endif
    		}
    	} else {
    		usart->DR = serial.txBuffer.get_unsafe();
    	}
    }
}
//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT
void USART1_IRQHandler(void) {
    serialPort_interruptFunction(USART1, SerialPort_interrupt::Serial1);
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_INTERRUPT
void USART2_IRQHandler(void) {
    serialPort_interruptFunction(USART2, SerialPort_interrupt::Serial2);
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_INTERRUPT
void USART3_IRQHandler(void) {
    serialPort_interruptFunction(USART3, SerialPort_interrupt::Serial3);
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_INTERRUPT
void UART4_IRQHandler(void) {
    serialPort_interruptFunction(UART4, SerialPort_interrupt::Serial4);
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT5_INTERRUPT
void UART5_IRQHandler(void) {
    serialPort_interruptFunction(UART5, SerialPort_interrupt::Serial5);
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_INTERRUPT
void USART6_IRQHandler(void) {
    serialPort_interruptFunction(USART6, SerialPort_interrupt::Serial6);
}
#endif

}  // namespace stm32f4xx
}  // namespace microhal
