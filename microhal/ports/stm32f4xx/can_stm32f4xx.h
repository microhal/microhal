/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-03-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#ifndef _MICROHAL_STM32F3XX_H_
#define _MICROHAL_STM32F3XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

#include "gsl/span"

#include "can/canInterface.h"
#include "can/canMessage.h"
#include "can_registers.h"
#include "clockManager.h"
#include "core_stm32f4xx.h"
#include "microhal_semaphore.h"

#include "microhalPortConfig_stm32f4xx.h"

#undef CAN1
#undef CAN2

namespace microhal {
namespace stm32f4xx {

#undef CAN

extern "C" {
void CAN1_TX_IRQHandler();
void CAN1_RX0_IRQHandler();
void CAN1_RX1_IRQHandler();
void CAN2_TX_IRQHandler();
void CAN2_RX0_IRQHandler();
void CAN2_RX1_IRQHandler();
}

class CAN final : public can::CAN_Interface {
    enum class FilterMode { Mask16bit = 0, Mask32bit, List16bit, List32bit };
    enum Fifo { Fifo0, Fifo1 };

 public:
    using Message = can::Message;
    using Interrupt = registers::CAN::IER::Interrupt;
    using TxMailbox = registers::CAN_TxMailBox_TypeDef;
    using Filter = registers::CAN_FilterRegister_TypeDef;
    using RxMailbox = registers::CAN_FIFOMailBox_TypeDef;
    using Device = registers::CAN::Device;
    enum Error { None = 0, Stuff, Form, Acknowledgment, BitRecessive, BitDominant, Crc, SoftwareSet };
    enum class Mode { Normal, Loopback, Silent, LoopbackAndSilent };
    enum class Sleep { Sleep, Wakeup, AutoWakeup };

    static constexpr const Protocol supportedProtocols[] = {Protocol::v2_0A, Protocol::V2_0B};
    static constexpr const uint32_t bitRateMax = 1000000;
#if defined(CAN1_BASE) || defined(CAN2_BASE)
    CAN(registers::CAN::Device canDevice) : can(*reinterpret_cast<registers::CAN *>(canDevice)) {
        ClockManager::enable(can, ClockManager::PowerMode::Normal);
        objectPtr[registers::CAN::getNumber(canDevice) - 1] = this;
        can.ier.enableInterrupt(Interrupt::TransmitMailboxEmpty);
#ifndef HAL_RTOS_FreeRTOS
        const uint32_t priority = 0;
#else
        const uint32_t priority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
#endif
        enableInterrupt(priority);
    };

    ~CAN() {
        initializationRequest();  // This will wait until ongoing transmission will be finished
        disableInterrupt();
        can.ier.disableInterrupt(Interrupt::TransmitMailboxEmpty);
        if (objectPtr[0] == this) objectPtr[0] = nullptr;
        if (objectPtr[1] == this) objectPtr[1] = nullptr;
        ClockManager::disable(can, ClockManager::PowerMode::Normal);
    }
#endif
    //  constexpr CAN_TypeDef *canNumToDevicePtr(uint8_t num) { return reinterpret_cast<CAN_TypeDef *>(CAN1_BASE); }
    //  registers::CAN &getCanFromNum(uint8_t num) { return registers::can1; }

    bool isProtocolSupported(Protocol protocol) final {
        if (std::find(std::begin(supportedProtocols), std::end(supportedProtocols), protocol) != std::end(supportedProtocols))
            return true;
        else
            return false;
    }

    void reset() { can.mcr.bitfield.RESET = 1; }
    void setMode(Mode mode);
    void sleepMode(Sleep sleepMode);

    void enableAutomaticRetransmission() {
        initializationRequest();
        can.mcr.bitfield.NART = 0;
        exitInitializationMode();
    }
    void disableAutomaticRetransmission() {
        initializationRequest();
        can.mcr.bitfield.NART = 1;
        exitInitializationMode();
    }

    uint32_t setBaudrate(uint32_t baudrate);
    uint32_t getBaudrate();

    bool transmit(const Message &message) final;
    bool receive(Message &message) final;

    bool waitForTransmitFinish(std::chrono::milliseconds timeout) const noexcept final {
        if (emptyTxMailboxCount() != 3) {
            txWait = true;
            if (txFinish.wait(timeout) == false) {
                txWait = false;
                return false;
            }
        }
        return true;
    }

    bool waitForMessage(std::chrono::milliseconds timeout) const noexcept final {
        if (pendingMessageCount() == 0) {
            can.ier.enableInterrupt(Interrupt::FIFO0_MessagePending | Interrupt::FIFO1_MessagePending);
            return dataReady.wait(timeout);
        }
        return true;
    }

    uint_fast8_t pendingMessageCount() const { return can.rf0r.mesagesCount() + can.rf1r.mesagesCount(); }

    Error getLastError() { return static_cast<Error>(can.esr.bitfield.LEC); }

    uint32_t receiveErrorCount() const final { return can.esr.bitfield.REC; }
    uint32_t transmitErrorCount() const final { return can.esr.bitfield.TEC; }

    bool addFilter(Message::ID id, uint32_t mask, bool isRemoteFrame) final;
    bool removeFilter(Message::ID id, uint32_t mask, bool isRemoteFrame) final;

    uint_fast8_t emptyTxMailboxCount() const {
        uint_fast8_t count = 0;
        if (can.tsr.bitfield.TME0) count++;
        if (can.tsr.bitfield.TME1) count++;
        if (can.tsr.bitfield.TME2) count++;
        return count;
    }

 private:
    registers::CAN &can;
    static CAN *objectPtr[2];
    mutable microhal::os::Semaphore txFinish;
    mutable bool txWait = false;
    mutable microhal::os::Semaphore dataReady;

    int_fast8_t getEmptyMailboxNumber() {
        if (emptyTxMailboxCount()) {
            return can.tsr.bitfield.CODE;
        }
        return -1;
    }

    void initializationRequest() {
        can.mcr.bitfield.INRQ = 1;
        while (can.msr.bitfield.INAK == 0) {
        }
    }

    void exitInitializationMode() {
        can.mcr.bitfield.INRQ = 0;
        // while (can.msr.bitfield.INAK == 1) {
        // }
    }

    // Filter functions
    void activateFilterInitMode() { can.fmr.bitfield.FINIT = 1; }
    void deactivateFilterInitMode() { can.fmr.bitfield.FINIT = 0; }
    void activateFilter(uint_fast8_t filterNumber) { can.FA1R |= 1 << filterNumber; }
    void deactivateFilter(uint_fast8_t filterNumber) { can.FA1R &= ~(1 << filterNumber); }
    bool isFilterActive(uint_fast8_t filterNumber) { return can.FA1R & (1 << filterNumber); }

    // ISR related functions
    void enableInterrupt(uint32_t priority) {
        NVIC_SetPriority(irq(), priority);
        NVIC_EnableIRQ(irq());
    }

    void disableInterrupt() { NVIC_DisableIRQ(irq()); }

    IRQn_Type irq() {
#if defined(CAN1_BASE)
        if (&can == &registers::can1) return CAN1_TX_IRQn;
#endif
#if defined(CAN2_BASE)
        if (&can == &registers::can2) return CAN2_TX_IRQn;
#endif
        std::terminate();
    }

    void interruptFunction();
    void rxInterruptFunction();

    friend void CAN1_TX_IRQHandler();
    friend void CAN1_RX0_IRQHandler();
    friend void CAN1_RX1_IRQHandler();
    friend void CAN2_TX_IRQHandler();
    friend void CAN2_RX0_IRQHandler();
    friend void CAN2_RX1_IRQHandler();
};  // namespace stm32f4xx
}  // namespace stm32f4xx
}  // namespace microhal

#endif  // _MICROHAL_STM32F3XX_H_
