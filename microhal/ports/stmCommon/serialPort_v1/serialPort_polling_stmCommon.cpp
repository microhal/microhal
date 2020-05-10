/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    buleks
 * created on: 25-08-2016
 * last modification: 25-08-2016
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

/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#include "serialPort_polling_stmCommon.h"
#if _MICROHAL_PORT_STM_SERIAL_PORT_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_SERIAL_PORT(POLLING)                    // Check if driver is enabled in microhal port config
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "clockManager/usartClock.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

#if MICROHAL_USE_SERIAL_PORT1_POLLING == 1
SerialPort_polling SerialPort_polling::Serial1(*registers::usart1);
SerialPort &SerialPort::Serial1 = SerialPort_polling::Serial1;
#endif
#if MICROHAL_USE_SERIAL_PORT2_POLLING == 1
SerialPort_polling SerialPort_polling::Serial2(*registers::usart2);
SerialPort &SerialPort::Serial2 = SerialPort_polling::Serial2;
#endif
#if MICROHAL_USE_SERIAL_PORT3_POLLING == 1
SerialPort_polling SerialPort_polling::Serial3(*registers::usart3);
SerialPort &SerialPort::Serial3 = SerialPort_polling::Serial3;
#endif
#if MICROHAL_USE_SERIAL_PORT4_POLLING == 1
SerialPort_polling SerialPort_polling::Serial4(*registers::uart4);
SerialPort &SerialPort::Serial4 = SerialPort_polling::Serial4;
#endif
#if MICROHAL_USE_SERIAL_PORT5_POLLING == 1
SerialPort_polling SerialPort_polling::Serial5(*registers::uart5);
SerialPort &SerialPort::Serial5 = SerialPort_polling::Serial5;
#endif
#if MICROHAL_USE_SERIAL_PORT6_POLLING == 1
SerialPort_polling SerialPort_polling::Serial6(*registers::usart6);
SerialPort &SerialPort::Serial6 = SerialPort_polling::Serial6;
#endif
#if MICROHAL_USE_SERIAL_PORT7_POLLING == 1
SerialPort_polling SerialPort_polling::Serial7(*registers::uart7);
SerialPort &SerialPort::Serial7 = SerialPort_polling::Serial7;
#endif
#if MICROHAL_USE_SERIAL_PORT8_POLLING == 1
SerialPort_polling SerialPort_polling::Serial8(*registers::uart8);
SerialPort &SerialPort::Serial8 = SerialPort_polling::Serial8;
#endif

SerialPort_polling::SerialPort_polling(registers::USART &usart) : SerialPort(usart) {
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::enableUSART(number(), ClockManager::PowerMode::Normal);
#else
    ClockManager::enableUSART(number());
#endif
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // _MICROHAL_PORT_STM_SERIAL_PORT_DRIVER_VERSION == 1
#endif  // _MICROHAL_PORTS_SERIALPORT_POLLING_STMCOMMON_H_
