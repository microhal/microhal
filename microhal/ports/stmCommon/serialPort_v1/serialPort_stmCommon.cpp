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

#include "serialPort_stmCommon.h"
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#if _MICROHAL_PORT_STM_SERIAL_PORT_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_SERIAL_PORT(POLLING) || ENABLED_ANY_SERIAL_PORT(INTERRUPT) || \
    ENABLED_ANY_SERIAL_PORT(DMA)  // Check if driver is enabled in microhal port config
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "../stmCommonDefines.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

bool SerialPort::setBaudRate(uint32_t baudRate) noexcept {
    usart.brr.volatileStore(ClockManager::USARTFrequency(usart::number(&usart)) / baudRate);
    return true;
}

uint32_t SerialPort::getBaudRate() const noexcept {
    return ClockManager::USARTFrequency(usart::number(&usart)) * (uint32_t)usart.brr.volatileLoad();
}

bool SerialPort::setParity(SerialPort::Parity parity) noexcept {
    auto cr1 = usart.cr1.volatileLoad();
    switch (parity) {
        case SerialPort::NoParity:
            cr1.PCE.clear();
            break;
        case SerialPort::EvenParity:
            cr1.PS.clear();
            cr1.PCE.set();
            break;
        case SerialPort::OddParity:
            cr1.PS.set();
            cr1.PCE.set();
            break;
        default:
            return false;
    }
    usart.cr1.volatileStore(cr1);
    return true;
}

bool SerialPort::setStopBits(SerialPort::StopBits stopBits) noexcept {
    auto cr2 = usart.cr2.volatileLoad();
    switch (stopBits) {
        case SerialPort::OneStop:
            cr2.STOP.clear();
            break;
        case SerialPort::OneAndHalfStop:
            cr2.STOP = 0b11;
            break;
        case SerialPort::TwoStop:
            cr2.STOP = 0b10;
            break;
        default:
            return false;
    }
    usart.cr2.volatileStore(cr2);
    return true;
}

bool SerialPort::setDataBits(SerialPort::DataBits dataBits) noexcept {
    auto cr1 = usart.cr1.volatileLoad();
    switch (dataBits) {
        case SerialPort::Data8:
            cr1.M.clear();
            break;
        case SerialPort::Data9:
            cr1.M.set();
            break;
        default:
            return false;
    }
    usart.cr1.volatileStore(cr1);
    return true;
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // ENABLED_ANY_SPI(POLLING) || ENABLED_ANY_SPI(INTERRUPT) || ENABLED_ANY_SPI(DMA)
#endif  // _MICROHAL_PORT_STM_SERIAL_PORT_DRIVER_VERSION == 1
