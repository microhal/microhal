/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      NRF51 serial port driver implementation. Driver support receiving and transmitting using interrupts.
 *
 * @authors    Pawel Okas
 * created on: 05-12-2017
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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
#include "serialPort_interrupt_nrf51.h"
#include "../clockManager.h"

namespace microhal {
namespace nrf51 {
//***********************************************************************************************//
//                                   STATIC VARIABLES
//***********************************************************************************************//
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT
static char txBufferData_1[MICROHAL_SERIAL_PORT1_TX_BUFFER_SIZE];
static char rxBufferData_1[MICROHAL_SERIAL_PORT1_RX_BUFFER_SIZE];
SerialPort_interrupt SerialPort_interrupt::Serial1(*NRF_UART0, rxBufferData_1, txBufferData_1, sizeof(rxBufferData_1), sizeof(txBufferData_1));
SerialPort &SerialPort::Serial1 = SerialPort_interrupt::Serial1;
#endif

SerialPort_interrupt::SerialPort_interrupt(NRF_UART_Type &usart, char *const rxData, char *const txData, size_t rxDataSize, size_t txDataSize)
    : SerialPort_BufferedBase(usart, rxData, rxDataSize, txData, txDataSize) {
    usart.INTENCLR = 0xffffffffUL;
    enableInterrupt(serialPort1_priority);  // serialPort1_priority have to be defined in port configuration file
}

bool SerialPort_interrupt::open(OpenMode mode) noexcept {
    if (mode == OpenMode::NotOpen) {
        close();
        return false;
    }
    if (isOpen()) return false;

    uart.ENABLE |= UART_ENABLE_ENABLE_Enabled;

    uart.EVENTS_RXDRDY = 0;
    uart.EVENTS_TXDRDY = 0;
    uart.CONFIG &= ~(UART_CONFIG_HWFC_Enabled << UART_CONFIG_HWFC_Pos);
    uart.INTENSET = UART_INTENSET_TXDRDY_Set << UART_INTENSET_TXDRDY_Pos;
    if (mode != OpenMode::WriteOnly) {
        uart.TASKS_STARTRX = 1;
        uart.INTENSET = UART_INTENSET_RXDRDY_Set << UART_INTENSET_RXDRDY_Pos;
    }
    return true;
}

//***********************************************************************************************//
//                                     interrupt functions                                       //
//***********************************************************************************************//
inline void serialPort_interruptFunction(NRF_UART_Type *const uart, SerialPort_interrupt &serial) {
    if ((uart->EVENTS_RXDRDY != 0) && (uart->INTENSET & UART_INTENSET_RXDRDY_Msk)) {
        uart->EVENTS_RXDRDY = 0;
        char tmp = uart->RXD;
        serial.rxBuffer.append(tmp);
        if (serial.waitForBytes != 0 && serial.rxBuffer.getLength() == serial.waitForBytes) {
            serial.waitForBytes = 0;
            bool shouldYeld = serial.rxSemaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
            portYIELD_FROM_ISR(shouldYeld);
#else
            (void)shouldYeld;
#endif
        }
    }
    if ((uart->EVENTS_TXDRDY != 0) && (uart->INTENSET & UART_INTENSET_TXDRDY_Msk)) {
        uart->EVENTS_TXDRDY = 0;
        if (serial.txBuffer.isEmpty()) {
            uart->TASKS_STOPTX = 1;
            if (serial.txWait) {
                auto shouldYeld = serial.txFinish.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
                portYIELD_FROM_ISR(shouldYeld);
#else
                (void)shouldYeld;
#endif
            }
        } else {
            uart->TXD = serial.txBuffer.get_unsafe();
        }
    }

    if ((uart->EVENTS_ERROR != 0) && (uart->INTENSET & UART_INTENSET_ERROR_Msk)) {
        // todo add error handling
        uart->EVENTS_ERROR = 0;
    }
}
//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT
void UART0_IRQHandler(void) {
    serialPort_interruptFunction(NRF_UART0, SerialPort_interrupt::Serial1);
}
#endif

}  // namespace nrf51
}  // namespace microhal
