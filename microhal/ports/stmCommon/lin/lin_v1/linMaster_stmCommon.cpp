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

#include <ports/stmCommon/interruptController/interruptController.h>
#include <ports/stmCommon/lin/lin_v1/linMaster_stmCommon.h>
#include <ports/stmCommon/nvic/nvic.h>
#include <ports/stmCommon/usart/usart.h>
#include <algorithm>
#include <cassert>
#include "../../clockManager/usartClock.h"

#ifdef MICROHAL_LIN_USE_DIAGNOSTIC
#include "diagnostic/diagnostic.h"

using namespace microhal::diagnostic;
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

#if MICROHAL_USE_LIN1 == 2
LINMaster LINMaster::lin1(*microhal::registers::usart1);
#endif
#if MICROHAL_USE_LIN2 == 2
LINMaster *LINMaster::lin2(*microhal::registers::usart2);
#endif
#if MICROHAL_USE_LIN6 == 2
LINMaster *LINMaster::lin6(*microhal::registers::usart6);
#endif

bool LINMaster::init(uint32_t baudRate, uint32_t interruptPriority) {
    const auto usartNumber = usart::number(&usart);
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::enableUSART(usartNumber, ClockManager::PowerMode::Normal);
#else
    ClockManager::enableUSART(usartNumber);
#endif

    // set baudrate
    usart.brr.volatileStore(ClockManager::USARTFrequency(usart::number(&usart)) / baudRate);
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

void LINMaster::deinit() {
    using namespace std::chrono_literals;

    disable();
    status = Error::None;
    semaphore.wait(0ms);
    mode = Idle;

    disableInterrupt();
    const auto usartNumber = usart::number(&usart);
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::disableUSART(usartNumber, ClockManager::PowerMode::Normal | ClockManager::PowerMode::Sleep);
#else
    ClockManager::disableUSART(usartNumber);
#endif
}

void LINMaster::enableInterrupt(uint32_t priority) {
#ifdef MICROHAL_RTOS_FreeRTOS
    assert(priority >= configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
#endif
    enableUSARTInterrupt(usart::number(&usart), priority);
}

void LINMaster::disableInterrupt() {
    disableUSARTInterrupt(usart::number(&usart));
}

void LINMaster::enable() {
    auto cr1 = usart.cr1.volatileLoad();
    cr1.UE.set();
    usart.cr1.volatileStore(cr1);
}

void LINMaster::disable() {
    auto cr1 = usart.cr1.volatileLoad();
    cr1.UE.clear();
    usart.cr1.volatileStore(cr1);
}

void LINMaster::sendBreak() {
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

LINMaster::Error LINMaster::write(std::span<uint8_t> data, std::chrono::milliseconds timeout) {
    txPtr = data.data();
    txEndPtr = txPtr + data.size() - 1;
    mode = WaitForBreak;
    sendBreak();

    if (!semaphore.wait(timeout)) {
        mode = Idle;
        return Error::Timeout;
    }
    return status;
}

LINMaster::Error LINMaster::request_impl(lin::Frame &frame, std::chrono::milliseconds timeout) {
    txPtr = (uint8_t *)&frame.header;
    txEndPtr = txPtr + sizeof(frame.header) - 1;
    waitForBytes = frame.size();
    mode = WaitForBreak | Receiver;
    sendBreak();

    if (!semaphore.wait(timeout)) {
        mode = Idle;
        return Error::Timeout;
    }
    std::copy_n(rxBuffer, frame.size(), (uint8_t *)&frame.header);
    return status;
}

static void giveSemaphoreFromISR(os::Semaphore &semaphore) {
    [[maybe_unused]] bool shouldYeld = semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#endif
}

static void usartSend(registers::USART &usart, uint8_t data) {
#if defined(_MICROHAL_USE_USART_REGISTERS_V1)
    usart.dr.volatileStore(data);
#elif defined(_MICROHAL_USE_USART_REGISTERS_V2)
    usart.tdr.volatileStore(data);
#endif
}

static uint8_t usartRead(registers::USART &usart) {
#if defined(_MICROHAL_USE_USART_REGISTERS_V1)
    return (uint32_t)usart.dr.volatileLoad();
#elif defined(_MICROHAL_USE_USART_REGISTERS_V2)
    return (uint32_t)usart.rdr.volatileLoad();
#endif
}

void LINMaster::interrupt() {
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
        if (mode & WaitForBreak) {
            usartSend(usart, *txPtr);
            mode |= Transmitter;
            mode &= ~WaitForBreak;
        } else {
            // received break during transition this is an unexpected situation,
            // stop transmitting data and give semaphore to exit transmitting function
            // also switch mode to receiver
            status = Error::BreakDetected;
            mode = Idle;
            giveSemaphoreFromISR(semaphore);
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
    }
    if (isr.RXNE && cr1.RXNEIE) {
        const uint8_t rxData = usartRead(usart);
        if (mode & Transmitter) {
            if (*txPtr == rxData) {
                // data transmitted correctly
                if (txPtr < txEndPtr) {
                    // we still have some data to send
                    txPtr++;
                    usartSend(usart, *txPtr);
                } else {
                    // all data sent
                    if (mode == Transmitter) {  // transmitter only enabled
                        // all data transmitted
                        mode = Idle;
                        status = Error::None;
                        giveSemaphoreFromISR(semaphore);
                    } else {
                        mode = Receiver;
                    }
                }
            } else {
                // detected transmission error
                mode = Idle;
                status = Error::BusConflictDetected;
                giveSemaphoreFromISR(semaphore);
            }
        }
        if (mode & Receiver) {
            rxBuffer[rxBufferIter] = rxData;
            rxBufferIter++;
            if (rxBufferIter == waitForBytes) {
                mode = Idle;
                status = Error::None;
                giveSemaphoreFromISR(semaphore);
            } else if (rxBufferIter > sizeof(rxBuffer)) {
                mode = Idle;
                status = Error::BufferOverflow;
                giveSemaphoreFromISR(semaphore);
            }
        }
    }
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE

#if MICROHAL_USE_LIN1 == 2
extern "C" void USART1_IRQHandler(void) {
    LINMaster::lin1.interrupt();
}
#endif
#if MICROHAL_USE_LIN2 == 2
extern "C" void USART2_IRQHandler(void) {
    LINMaster::lin2.interrupt();
}
#endif
#if MICROHAL_USE_LIN6 == 2
extern "C" void USART6_IRQHandler(void) {
    LINMaster::lin6.interrupt();
}
#endif

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
