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
#include "core_stm32l4xx.h"
#include "serialPort_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {

bool SerialPort::setBaudRate(uint32_t baudRate) {
    usart.BRR = Core::getUSARTclk(&usart) / baudRate;
    return true;
}

uint32_t SerialPort::getBaudRate() const {
    return Core::getUSARTclk(&usart) * usart.BRR;
}

bool SerialPort::setParity(SerialPort::Parity parity) {
    switch (parity) {
    case SerialPort::NoParity:
        usart.CR1 &= ~USART_CR1_PCE;
        break;
    case SerialPort::EvenParity:
        usart.CR1 = (usart.CR1 & ~USART_CR1_PS) | USART_CR1_PCE;
        break;
    case SerialPort::OddParity:
        usart.CR1 |= USART_CR1_PS | USART_CR1_PCE;
        break;
    default:
        return false;
    }
    return true;
}
bool SerialPort::setStopBits(SerialPort::StopBits stopBits) {
    switch (stopBits) {
    case SerialPort::OneStop:
        usart.CR2 &= ~USART_CR2_STOP;
        break;
    case SerialPort::OneAndHalfStop:
        usart.CR2 |= USART_CR2_STOP_0 | USART_CR2_STOP_1;
        break;
    case SerialPort::TwoStop:
        usart.CR2 = (usart.CR2 & ~USART_CR2_STOP) | USART_CR2_STOP_1;
        break;
    default:
        return false;
    }
    return true;
}

bool SerialPort::setDataBits(SerialPort::DataBits dataBits) {
    switch (dataBits) {
    case SerialPort::Data8:
        usart.CR1 &= ~USART_CR1_M;

        break;
    case SerialPort::Data9:
        usart.CR1 |= USART_CR1_M;
        break;
    default:
        return false;
    }
    return true;
}

} // namespace stm32l4xx
} // namespace microhal



