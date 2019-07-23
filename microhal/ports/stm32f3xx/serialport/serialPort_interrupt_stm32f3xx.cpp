/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    buleks
 * created on: 26-08-2016
 * last modification: 26-08-2016
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
#include "serialPort_interrupt_stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {
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

SerialPort_interrupt::SerialPort_interrupt(USART_TypeDef &usart, char *const rxData, char *const txData, size_t rxDataSize, size_t txDataSize)
    : stm32f3xx::SerialPort(usart), rxBuffer(rxData, rxDataSize), txBuffer(txData, txDataSize) {
    uint32_t rccEnableFlag;

    switch (reinterpret_cast<uint32_t>(&usart)) {
        case reinterpret_cast<uint32_t>(USART1_BASE):
            rccEnableFlag = RCC_APB2ENR_USART1EN;
            NVIC_SetPriority(USART1_IRQn, 0);
            NVIC_ClearPendingIRQ(USART1_IRQn);
            NVIC_EnableIRQ(USART1_IRQn);

            break;
        case reinterpret_cast<uint32_t>(USART2_BASE):
            rccEnableFlag = RCC_APB1ENR_USART2EN;
            NVIC_SetPriority(USART2_IRQn, 0);
            NVIC_ClearPendingIRQ(USART2_IRQn);
            NVIC_EnableIRQ(USART2_IRQn);

            break;
        case reinterpret_cast<uint32_t>(USART3_BASE):
            rccEnableFlag = RCC_APB1ENR_USART3EN;
            NVIC_SetPriority(USART3_IRQn, 0);
            NVIC_ClearPendingIRQ(USART3_IRQn);
            NVIC_EnableIRQ(USART3_IRQn);

            break;
        default:
            std::terminate();
    }
    if (reinterpret_cast<uint32_t>(&usart) == reinterpret_cast<uint32_t>(USART1)) {
        RCC->APB2ENR |= rccEnableFlag;
    } else {
        RCC->APB1ENR |= rccEnableFlag;
    }
}

bool SerialPort_interrupt::open(OpenMode mode) noexcept {
    if (isOpen() || (mode > 0x03)) return false;
    usart.CR1 |= (mode << 2) | USART_CR1_UE | USART_CR1_RXNEIE;
    return true;
}

bool SerialPort_interrupt::putChar(char c) noexcept {
    __disable_irq();
    if (txBuffer.append(c)) {
        startSending();
        __enable_irq();
        return true;
    }
    __enable_irq();
    return false;
}

bool SerialPort_interrupt::getChar(char &c) noexcept {
    if (rxBuffer.isNotEmpty()) {
        c = rxBuffer.get();
        return true;
    } else {
        return false;
    }
}

bool SerialPort_interrupt::waitForWriteFinish(std::chrono::milliseconds timeout) const noexcept {
    // todo timeout
    while (1) {
        __disable_irq();
        if (txBuffer.isEmpty()) {
            __enable_irq();
            return true;
        }
        __enable_irq();
        timeout--;
    }
    __enable_irq();
    return false;
}

size_t SerialPort_interrupt::write(const char *data, size_t length) noexcept {
    size_t writeByte = 0;

    if (length > 0) {
        __disable_irq();
        for (; writeByte < length; writeByte++) {
            if (txBuffer.append(*data++) == false) break;
        }
        if (writeByte != 0) startSending();
        __enable_irq();
    }
    return writeByte;
}

size_t SerialPort_interrupt::read(char *data, size_t length, std::chrono::milliseconds /*timeout*/) noexcept {
    size_t len = rxBuffer.getLength();

    if (len == 0) return 0;

    if (len > length) {
        len = length;
    } else {
        length = len;
    }

    while (length--) {
        *data++ = rxBuffer.get();
    }

    return len;
}

bool SerialPort_interrupt::clear(Direction dir) noexcept {
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
void SerialPort_interrupt::__SerialPort_USART_interruptFunction() {
    uint32_t sr = usart.ISR;
    if (sr & USART_ISR_ORE) {
        usart.ICR = USART_ICR_ORECF;
    }
    if (sr & USART_ISR_RXNE) {
        char tmp = usart.RDR;

        rxBuffer.append(tmp);
    }
    if ((sr & USART_ISR_TXE) && (usart.CR1 & USART_CR1_TXEIE)) {
        if (txBuffer.isEmpty()) {
            usart.CR1 &= ~USART_CR1_TXEIE;
        } else {
            usart.TDR = txBuffer.get_unsafe();
        }
        if (txBuffer.isEmpty()) {
            usart.CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT
void USART1_IRQHandler(void) {
    SerialPort_interrupt::Serial1.__SerialPort_USART_interruptFunction();
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_INTERRUPT
void USART2_IRQHandler(void) {
    SerialPort_interrupt::Serial2.__SerialPort_USART_interruptFunction();
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_INTERRUPT
void USART3_IRQHandler(void) {
    SerialPort_interrupt::Serial3.__SerialPort_USART_interruptFunction();
}
#endif

}  // namespace stm32f3xx
}  // namespace microhal
