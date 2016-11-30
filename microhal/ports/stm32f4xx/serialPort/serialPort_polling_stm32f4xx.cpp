/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      STM32F4xx serial port driver implementation. Driver support receiving and transmitting using polling mode.
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
#include "serialPort_polling_stm32f4xx.h"
#include "../clockManager.h"

namespace microhal {
namespace stm32f4xx {

#ifdef MICROHAL_USE_SERIAL_PORT1_POLLING
SerialPort_polling SerialPort_polling::Serial1(*USART1);
SerialPort &SerialPort::Serial1 = SerialPort_polling::Serial1;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_POLLING
SerialPort_polling SerialPort_polling::Serial2(*USART2);
SerialPort &SerialPort::Serial2 = SerialPort_polling::Serial2;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_POLLING
SerialPort_polling SerialPort_polling::Serial3(*USART3);
SerialPort &SerialPort::Serial3 = SerialPort_polling::Serial3;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_POLLING
SerialPort_polling SerialPort_polling::Serial4(*USART4);
SerialPort &SerialPort::Serial4 = SerialPort_polling::Serial4;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT5_POLLING
SerialPort_polling SerialPort_polling::Serial5(*USART5);
SerialPort &SerialPort::Serial5 = SerialPort_polling::Serial5;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT6_POLLING
SerialPort_polling SerialPort_polling::Serial6(*USART6);
SerialPort &SerialPort::Serial6 = SerialPort_polling::Serial6;
#endif

SerialPort_polling::SerialPort_polling(USART_TypeDef &usart) : stm32f4xx::SerialPort(usart) {
	ClockManager::enable(usart);
}


} // namespace stm32f4xx
} // namespace microhal
