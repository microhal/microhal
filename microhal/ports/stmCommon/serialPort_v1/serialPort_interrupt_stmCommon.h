/**
 * @file
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      STM32Fxxx serial port driver implementation. Driver support receiving and transmitting using interrupts.
 *
 * @authors    Pawel Okas
 * created on: 17-04-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014-2019, Pawel Okas
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

#ifndef _MICROHAL_PORTS_SERIALPORT_INTERRUPT_STMCOMMON_H_
#define _MICROHAL_PORTS_SERIALPORT_INTERRUPT_STMCOMMON_H_
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#include <ports/stmCommon/driverConfiguration_stmCommon.h>
#if _MICROHAL_PORT_STM_SERIAL_PORT_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU
#include "serialPort_preprocessor_macros.h"
#if ENABLED_ANY_SERIAL_PORT(INTERRUPT)  // Check if driver is enabled in microhal port config
/* ************************************************************************************************
 * INCLUDES
 */
#include <thread>
#include "../stmCommonDefines.h"
#include "buffers/cyclicBuffer.h"
#include "ports/common/serialPort_bufferedBase.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
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
void UART7_IRQHandler(void);
void UART8_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
class SerialPort_interrupt : public common::SerialPort_BufferedBase<SerialPort_interrupt> {
 public:
#if MICROHAL_USE_SERIAL_PORT1_INTERRUPT == 1
    static SerialPort_interrupt Serial1;
#endif
#if MICROHAL_USE_SERIAL_PORT2_INTERRUPT == 1
    static SerialPort_interrupt Serial2;
#endif
#if MICROHAL_USE_SERIAL_PORT3_INTERRUPT == 1
    static SerialPort_interrupt Serial3;
#endif
#if MICROHAL_USE_SERIAL_PORT4_INTERRUPT == 1
    static SerialPort_interrupt Serial4;
#endif
#if MICROHAL_USE_SERIAL_PORT5_INTERRUPT == 1
    static SerialPort_interrupt Serial5;
#endif
#if MICROHAL_USE_SERIAL_PORT6_INTERRUPT == 1
    static SerialPort_interrupt Serial6;
#endif
#if MICROHAL_USE_SERIAL_PORT7_INTERRUPT == 1
    static SerialPort_interrupt Serial7;
#endif
#if MICROHAL_USE_SERIAL_PORT8_INTERRUPT == 1
    static SerialPort_interrupt Serial8;
#endif
    //--------------------------------------------- functions ---------------------------------------//
    int open(OpenMode mode) noexcept;

 private:
    //------------------------------------------- variables -----------------------------------------//

    //------------------------------------------- constructors --------------------------------------//
    inline SerialPort_interrupt(registers::USART &usart, char *const rxData, char *const txData, size_t rxDataSize, size_t txDataSize);

    // virtual ~SerialPort_interrupt(){
    //}
    //--------------------------------------------- functions ---------------------------------------//
    void startTransmission_impl() {
        auto cr1 = usart.cr1.volatileLoad();
        cr1.TXEIE.set();
        usart.cr1.volatileStore(cr1);
    }

    void updateRxBuffer_impl() {}

    void configureRxWait_impl(size_t bytesToReceive) { waitForBytes = bytesToReceive; }

    void __SerialPort_USART_interruptFunction();
    //------------------------------------------- friends -------------------------------------------//
    friend SerialPort_BufferedBase<SerialPort_interrupt>;

    friend void USART1_IRQHandler(void);
    friend void USART2_IRQHandler(void);
    friend void USART3_IRQHandler(void);
    friend void UART4_IRQHandler(void);
    friend void UART5_IRQHandler(void);
    friend void USART6_IRQHandler(void);
    friend void UART7_IRQHandler(void);
    friend void UART8_IRQHandler(void);
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // ENABLED_ANY_SERIAL_PORT(INTERRUPT)
#endif  // _MICROHAL_PORT_STM_SERIAL_PORT_DRIVER_VERSION == 1
#endif  // _MICROHAL_PORTS_SERIALPORT_INTERRUPT_STMCOMMON_H_
