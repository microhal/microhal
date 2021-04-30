/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 13-03-2021
 *
 * @copyright Copyright (c) 2021, Pawel Okas
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

#include <ports/stmCommon/lin/lin_v1/lin_stmCommon.h>
#include <ports/stmCommon/nvic/nvic.h>
#include <ports/stmCommon/usart/usart.h>
#include <cassert>
#include _MICROHAL_INCLUDE_PORT_INTERRUPT_CONTROLLER

#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
#include "diagnostic/diagnostic.h"

using namespace microhal::diagnostic;
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

#if MICROHAL_USE_LIN1 == 1
LIN *LIN::lin1 = nullptr;
#endif
#if MICROHAL_USE_LIN2 == 1
LIN *LIN::lin2 = nullptr;
#endif

LIN::LIN(registers::USART &usart) : usart(usart) {
    const auto usartNumber = usart::number(&usart);

    if (usartNumber == 1) {
#if MICROHAL_USE_LIN1 == 1
        if (lin1 != nullptr) {
#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
            diagChannel << lock << MICROHAL_EMERGENCY << "Unable to create LIN interface, LIN1 already created." << unlock;
#endif
            std::terminate();
        }
        lin1 = this;
#else
#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
        diagChannel << lock << MICROHAL_EMERGENCY << "Unable to create LIN interface, LIN1 not enabled in port configuration file." << unlock;
#endif
        std::terminate();
#endif
    }

    if (usartNumber == 2) {
#if MICROHAL_USE_LIN2 == 1
        if (lin2 != nullptr) {
#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
            diagChannel << lock << MICROHAL_EMERGENCY << "Unable to create LIN interface, LIN2 already created." << unlock;
#endif
            std::terminate();
        }
        lin2 = this;
#else
#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
        diagChannel << lock << MICROHAL_EMERGENCY << "Unable to create LIN interface, LIN2 not enabled in port configuration file." << unlock;
#endif
        std::terminate();
#endif
    }
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::enableUSART(usartNumber, ClockManager::PowerMode::Normal);
#else
    ClockManager::enableUSART(usartNumber);
#endif
}

LIN::~LIN() {
    const auto usartNumber = usart::number(&usart);
    disableInterrupt();
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::disableUSART(usartNumber, ClockManager::PowerMode::Normal | ClockManager::PowerMode::Sleep);
#else
    ClockManager::disableUSART(usartNumber);
#endif
#if MICROHAL_USE_LIN1 == 1
    if (usartNumber == 1) lin1 = nullptr;
#endif
#if MICROHAL_USE_LIN2 == 1
    if (usartNumber == 2) lin2 = nullptr;
#endif
}

bool LIN::init(uint32_t baudRate, uint32_t interruptPriority) {
    // set baudrate
    usart.brr.volatileStore(ClockManager::USARTFrequency(2) / baudRate);
    auto cr1 = usart.cr1.volatileLoad();
    if (!cr1.UE.get()) {
        // no parity
        cr1.PCE.clear();
        // 8 data bit
        cr1.M.clear();
        //
        cr1.RE.set();      // enable receiver
        cr1.RXNEIE.set();  // enable receiver interrupt
        //
        cr1.TE.set();
        usart.cr1.volatileStore(cr1);

        auto cr2 = usart.cr2.volatileLoad();
        // enable lin
        cr2.LINEN.set();
        cr2.LBDIE.set();  // enable lin break detection interrupt
        // one stop
        cr2.STOP.clear();
        usart.cr2.volatileStore(cr2);

        enableInterrupt(interruptPriority);
        return true;
    }
    return false;
}

void LIN::enableInterrupt(uint32_t priority) {
#ifdef MICROHAL_RTOS_FreeRTOS
    assert(priority >= configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
#endif
    enableUSARTInterrupt(usart::number(&usart), priority);
}

void LIN::disableInterrupt() {
    disableUSARTInterrupt(usart::number(&usart));
}

void LIN::enable() {
    auto cr1 = usart.cr1.volatileLoad();
    cr1.UE.set();
    usart.cr1.volatileStore(cr1);
}

void LIN::disable() {
    auto cr1 = usart.cr1.volatileLoad();
    cr1.UE.clear();
    usart.cr1.volatileStore(cr1);
}

void LIN::sendBreak() {
#if defined(_MICROHAL_USE_USART_REGISTERS_V1)
    auto cr1 = usart.cr1.volatileLoad();
    cr1.SBK.set();
    usart.cr1.volatileStore(cr1);
#elif defined(_MICROHAL_USE_USART_REGISTERS_V2)
    auto rqr = usart.rqr.volatileLoad();
    rqr.SBKRQ.set();
    usart.rqr.volatileStore(rqr);
#else
#error USART Registers version unspecified.
#endif
}

LIN::Error LIN::write(gsl::span<uint8_t> data, bool sendBreak, std::chrono::milliseconds timeout) {
    txPtr = data.data();
    txEndPtr = txPtr + data.length_bytes() - 1;
    if (sendBreak) {
        mode = TransmitterWaitForBreak;
        this->sendBreak();
    } else {
        mode = Transmitter;
#if defined(_MICROHAL_USE_USART_REGISTERS_V1)
        usart.dr.volatileStore(*txPtr);
#elif defined(_MICROHAL_USE_USART_REGISTERS_V2)
        usart.tdr.volatileStore(*txPtr);
#endif
    }

    if (!txDone.wait(timeout)) return Error::Timeout;
    return status;
}

LIN::Error LIN::request_impl(lin::Frame &frame, std::chrono::milliseconds timeout) {
    txPtr = (uint8_t *)&frame.header;
    txEndPtr = txPtr + sizeof(frame.header) - 1;
    waitForBytes = frame.size();
    mode = TransmitterWaitForBreak;
    sendBreak();

    if (!rxDone.wait(timeout)) return Error::Timeout;
    std::copy_n(rxBuffer, frame.size(), (uint8_t *)&frame.header);
    return status;
}

LIN::Error LIN::readHeader_to(lin::Header &header, std::chrono::milliseconds timeout) {
    mode = WaitForBreak;
    if (rxDone.wait(timeout) == false) return Error::Timeout;
    header.sync = rxBuffer[0];  // should be 0x55
    header.protectedIdentifier.raw = rxBuffer[1];
    return Error::None;
}

LIN::Error LIN::read_to(uint8_t *data, uint_fast8_t length, std::chrono::milliseconds timeout) {
    if (int_fast8_t(length) > rxBufferIter) {
        waitForBytes = length;
        if (rxDone.wait(timeout) == false) return Error::Timeout;
    }
    std::copy_n(rxBuffer, length, data);
    return status;
}

static void giveSemaphoreFromISR(os::Semaphore &semaphore) {
    [[maybe_unused]] bool shouldYeld = semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#endif
}

void LIN::interrupt() {
#if defined(_MICROHAL_USE_USART_REGISTERS_V1)
    auto isr = usart.sr.volatileLoad();
#elif defined(_MICROHAL_USE_USART_REGISTERS_V2)
    auto isr = usart.isr.volatileLoad();
#endif
    auto cr1 = usart.cr1.volatileLoad();

#if defined(_MICROHAL_USE_USART_REGISTERS_V1)
    if (isr.LBD) {
#elif defined(_MICROHAL_USE_USART_REGISTERS_V2)
    if (isr.LBDF) {
#endif
        // detected lin break
        if (mode == Receiver) {
            // received break during reception this is an unexpected situation,
            // stop receiving data and give semaphore to exit receiving function
            status = Error::BreakDetected;
            giveSemaphoreFromISR(rxDone);
        } else if (mode == Transmitter) {
            // received break during transition this is an unexpected situation,
            // stop transmitting data and give semaphore to exit transmitting function
            // also switch mode to receiver
            status = Error::BreakDetected;
            mode = Receiver;
            giveSemaphoreFromISR(txDone);
        } else if (mode == TransmitterWaitForBreak) {
#if defined(_MICROHAL_USE_USART_REGISTERS_V1)
            usart.dr.volatileStore(*txPtr);
#elif defined(_MICROHAL_USE_USART_REGISTERS_V2)
            usart.tdr.volatileStore(*txPtr);
#endif
            mode = Transmitter;
        } else {
            mode = Receiver;
        }
        rxBufferIter = 0;
        // clear lin break interrupt
#if defined(_MICROHAL_USE_USART_REGISTERS_V1)
        isr.LBD.clear();
        usart.sr.volatileStore(isr);
#elif defined(_MICROHAL_USE_USART_REGISTERS_V2)
        registers::USART::ICR icr = {};
        icr.LBDCF.set();
        usart.icr.volatileStore(icr);
#endif
        // enable RX not empty interrupt
        cr1.RXNEIE.set();
        usart.cr1.volatileStore(cr1);
    }
    if (isr.RXNE && cr1.RXNEIE) {
#if defined(_MICROHAL_USE_USART_REGISTERS_V1)
        const uint8_t rxData = (uint32_t)usart.dr.volatileLoad();
#elif defined(_MICROHAL_USE_USART_REGISTERS_V2)
        const uint8_t rxData = (uint32_t)usart.rdr.volatileLoad();
#endif
        rxBuffer[rxBufferIter] = rxData;
        rxBufferIter++;
        if (rxBufferIter > int_fast8_t(sizeof(rxBuffer))) {
            // disable RX not empty interrupt
            cr1.RXNEIE.clear();
            usart.cr1.volatileStore(cr1);
        }
        if (mode == Receiver) {
            if (rxBufferIter == 2 || rxBufferIter == waitForBytes) {
                if (rxBufferIter == waitForBytes) {
                    waitForBytes = -1;
                    mode = WaitForBreak;
                }
                status = Error::None;
                giveSemaphoreFromISR(rxDone);
            }
        } else if (mode == Transmitter) {
            if (txPtr < txEndPtr) {
                if (*txPtr != rxData) {
                    // detected transmission error
                    status = Error::BusConflictDetected;
                    giveSemaphoreFromISR(txDone);
                } else {
                    txPtr++;
#if defined(_MICROHAL_USE_USART_REGISTERS_V1)
                    usart.dr.volatileStore(*txPtr);
#elif defined(_MICROHAL_USE_USART_REGISTERS_V2)
                    usart.tdr.volatileStore(*txPtr);
#endif
                }
            } else {
                if (waitForBytes < 0) {
                    // all data transmitted
                    status = Error::None;
                    giveSemaphoreFromISR(txDone);
                } else if (rxBufferIter == waitForBytes) {
                    waitForBytes = -1;
                    mode = WaitForBreak;
                    status = Error::None;
                    giveSemaphoreFromISR(rxDone);
                }
            }
        }
    }
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE

#if MICROHAL_USE_LIN1 == 1
extern "C" void USART1_IRQHandler(void) {
    LIN::lin1->interrupt();
}
#endif
#if MICROHAL_USE_LIN2 == 1
void USART2_IRQHandler(void) {
    LIN::lin2->interrupt();
}
#endif

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
