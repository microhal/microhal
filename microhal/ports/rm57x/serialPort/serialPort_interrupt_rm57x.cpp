/**
 * @file
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      RM57Lx serial port driver implementation. Driver support receiving and transmitting using interrupts.
 *
 * @authors    Pawel Okas
 * created on: 26-12-2017
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
#include "serialPort_interrupt_rm57x.h"
#include "../clockManager.h"

namespace microhal {
namespace rm57x {
//***********************************************************************************************//
//                                   STATIC VARIABLES
//***********************************************************************************************//
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT
static char txBufferData_1[rm57xConfig::serialPort1_txBufferSize];
static char rxBufferData_1[rm57xConfig::serialPort1_rxBufferSize];
SerialPort_interrupt SerialPort_interrupt::Serial1(*sciREG1, rxBufferData_1, txBufferData_1, sizeof(rxBufferData_1), sizeof(txBufferData_1));
SerialPort &SerialPort::Serial1 = SerialPort_interrupt::Serial1;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_INTERRUPT
static char txBufferData_2[rm57xConfig::serialPort2_txBufferSize];
static char rxBufferData_2[rm57xConfig::serialPort2_rxBufferSize];
SerialPort_interrupt SerialPort_interrupt::Serial2(*sciREG2, rxBufferData_2, txBufferData_2, sizeof(rxBufferData_2), sizeof(txBufferData_2));
SerialPort &SerialPort::Serial2 = SerialPort_interrupt::Serial2;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_INTERRUPT
static char txBufferData_3[rm57xConfig::serialPort3_txBufferSize];
static char rxBufferData_3[rm57xConfig::serialPort3_rxBufferSize];
SerialPort_interrupt SerialPort_interrupt::Serial3(*USART3, rxBufferData_3, txBufferData_3, sizeof(rxBufferData_3), sizeof(txBufferData_3));
SerialPort &SerialPort::Serial3 = SerialPort_interrupt::Serial3;
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_INTERRUPT
static char txBufferData_4[rm57xConfig::serialPort4_txBufferSize];
static char rxBufferData_4[rm57xConfig::serialPort4_rxBufferSize];
SerialPort_interrupt SerialPort_interrupt::Serial4(*UART4, rxBufferData_4, txBufferData_4, sizeof(rxBufferData_4), sizeof(txBufferData_4));
SerialPort &SerialPort::Serial4 = SerialPort_interrupt::Serial4;
#endif

SerialPort_interrupt::SerialPort_interrupt(sciBASE_t &usart, char *const rxData, char *const txData, size_t rxDataSize, size_t txDataSize)
    : SerialPort_BufferedBase(usart, rxData, rxDataSize, txData, txDataSize) {
    // ClockManager::enable(usart, ClockManager::PowerMode::Normal);
    // enableInterrupt(0);
}

bool SerialPort_interrupt::open(OpenMode mode) noexcept {
    if (isOpen() || (mode > 0x03)) return false;
    uart.GCR1 |= SCI::GCR1::RXENA | SCI::GCR1::TXENA | SCI::GCR1::TIMMING_MODE | SCI::GCR1::CLOCK | SCI::GCR1::SWnRST;
    enable(Interrupt::ReceiverReady);

    uart.PIO0 = 0b110;
    return true;
}

//***********************************************************************************************//
//                                     interrupt functions                                       //
//***********************************************************************************************//
void SerialPort_interrupt::interruptFunction() {
    uint32_t status = uart.FLR;

    if ((status & SerialPort::ReceiverReady) && isEnabled(SerialPort::Interrupt::ReceiverReady)) {
        char tmp = uart.RD;
        rxBuffer.append(tmp);
        if (waitForBytes != 0 && rxBuffer.getLength() == waitForBytes) {
            waitForBytes = 0;
            bool shouldYeld = rxSemaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
            portYIELD_FROM_ISR(shouldYeld);
#else
            (void)shouldYeld;
#endif
        }
    }
    if ((status & SerialPort::TransmitterReady) && isEnabled(SerialPort::Interrupt::TransmitterReady)) {
        if (txBuffer.isEmpty()) {
            disable(SerialPort::Interrupt::TransmitterReady);
            if (txWait) {
                // serial.enable(SerialPort::Interrupt::) usart->CR1 |= USART_CR1_TCIE;
                auto shouldYeld = txFinish.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
                portYIELD_FROM_ISR(shouldYeld);
#else
                (void)shouldYeld;
#endif
            }
        } else {
            uart.TD = txBuffer.get_unsafe();
        }
    }
    //    else if ((status & SerialPort::TransmitterEmpty) && (usart->CR1 & USART_CR1_TCIE)) {
    //        usart->CR1 &= ~USART_CR1_TCIE;
    //        if (serial.txWait) {
    //            serial.txWait = false;
    //            auto shouldYeld = serial.txFinish.giveFromISR();
    //#if defined(HAL_RTOS_FreeRTOS)
    //            portYIELD_FROM_ISR(shouldYeld);
    //#else
    //            (void)shouldYeld;
    //#endif
    //        }
    //    }
}

void SerialPort_interrupt::errorInterruptFunction() {}
//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SERIAL_PORT1_INTERRUPT
extern "C" void LIN1_Level0_IRQHandler(void) {
    volatile uint32_t intvect0 = sciREG1->INTVECT0;  // read int vector to clear interrupt
    SerialPort_interrupt::Serial1.interruptFunction();
}
extern "C" void LIN1_Level1_IRQHandler(void) {
    volatile uint32_t intvect1 = sciREG1->INTVECT1;  // read int vector to clear interrupt
    SerialPort_interrupt::Serial1.errorInterruptFunction();
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT2_INTERRUPT
extern "C" void LIN2_Level0_IRQHandler(void) {
    volatile uint32_t intvect0 = sciREG2->INTVECT0;  // read int vector to clear interrupt
    SerialPort_interrupt::Serial2.interruptFunction();
}
extern "C" void LIN2_Level1_IRQHandler(void) {
    volatile uint32_t intvect1 = sciREG2->INTVECT1;  // read int vector to clear interrupt
    SerialPort_interrupt::Serial2.errorInterruptFunction();
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT3_INTERRUPT
extern "C" void LIN3_Level0_IRQHandler(void) {
    volatile uint32_t intvect0 = sciREG3->INTVECT0;  // read int vector to clear interrupt
    SerialPort_interrupt::Serial3.interruptFunction();
}
extern "C" void LIN3_Level1_IRQHandler(void) {
    volatile uint32_t intvect1 = sciREG3 > INTVECT1;  // read int vector to clear interrupt
    SerialPort_interrupt::Serial3.errorInterruptFunction();
}
#endif
#ifdef MICROHAL_USE_SERIAL_PORT4_INTERRUPT
extern "C" void LIN4_Level0_IRQHandler(void) {
    volatile uint32_t intvect0 = sciREG4->INTVECT0;  // read int vector to clear interrupt
    SerialPort_interrupt::Serial4.interruptFunction();
}
extern "C" void LIN4_Level1_IRQHandler(void) {
    volatile uint32_t intvect1 = sciREG4->INTVECT1;  // read int vector to clear interrupt
    SerialPort_interrupt::Serial4.errorInterruptFunction();
}
#endif

}  // namespace rm57x
}  // namespace microhal
