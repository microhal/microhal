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

#ifndef _MICROHAL_PORTS_CAN_STMCOMMON_H_
#define _MICROHAL_PORTS_CAN_STMCOMMON_H_

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include "can/canInterface.h"
#include "can/canMessage.h"
#include "canFilter_stmCommon.h"
#include "gsl/span"
#include "microhal_semaphore.h"
#include "ports/stmCommon/clockManager/canClock.h"
#include "ports/stmCommon/registers/can_registers.h"
#include "ports/stmCommon/stmCommonDefines.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

#undef CAN1
#undef CAN2
#undef CAN

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

extern "C" {
void CAN1_TX_IRQHandler();
void CAN1_RX0_IRQHandler();
void CAN1_RX1_IRQHandler();
void CAN2_TX_IRQHandler();
void CAN2_RX0_IRQHandler();
void CAN2_RX1_IRQHandler();
}

class CAN final : public can::CAN_Interface {
    enum Fifo { Fifo0, Fifo1 };

 public:
    using Message = can::Message;
    using Interrupt = microhal::registers::CAN::IER::Interrupt;
    using TxMailbox = microhal::registers::CAN::TxMailBox;
    using RxMailbox = microhal::registers::CAN::FIFOMailBox;
    enum Error { None = 0, Stuff, Form, Acknowledgment, BitRecessive, BitDominant, Crc, SoftwareSet };
    enum class Mode { Normal, Loopback, Silent, LoopbackAndSilent };
    enum class Sleep { Sleep, Wakeup, AutoWakeup };

    static constexpr const Protocol supportedProtocols[] = {Protocol::v2_0A, Protocol::V2_0B};
    static constexpr const uint32_t bitRateMax = 1000000;
#if defined(_MICROHAL_CAN_BASE_ADDRESS) || defined(_MICROHAL_CAN1_BASE_ADDRESS) || defined(_MICROHAL_CAN2_BASE_ADDRESS)
    CAN(microhal::registers::CAN *const canDevice, uint32_t interruptPriority);

    CAN(const CAN &) = delete;             // disable copying
    CAN &operator=(const CAN &) = delete;  // disable copying

    ~CAN();
#endif

    bool isProtocolSupported(Protocol protocol) final {
        if (std::find(std::begin(supportedProtocols), std::end(supportedProtocols), protocol) != std::end(supportedProtocols))
            return true;
        else
            return false;
    }

    void reset() {
        auto mcr = can.mcr.volatileLoad();
        mcr.RESET = 1;
        can.mcr.volatileStore(mcr);
    }
    void setMode(Mode mode);
    void sleepMode(Sleep sleepMode);

    void enableAutomaticRetransmission() {
        initializationRequest();
        auto mcr = can.mcr.volatileLoad();
        mcr.NART = 0;
        can.mcr.volatileStore(mcr);
        exitInitializationMode();
    }
    void disableAutomaticRetransmission() {
        initializationRequest();
        auto mcr = can.mcr.volatileLoad();
        mcr.NART = 1;
        can.mcr.volatileStore(mcr);
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
            auto ier = can.ier.volatileLoad();
            ier.enableInterrupt(Interrupt::FIFO0_MessagePending | Interrupt::FIFO1_MessagePending);
            can.ier.volatileStore(ier);
            return dataReady.wait(timeout);
        }
        return true;
    }

    uint_fast8_t pendingMessageCount() const {
        auto rf0r = can.rf0r.volatileLoad();
        auto rf1r = can.rf1r.volatileLoad();
        return rf0r.mesagesCount() + rf1r.mesagesCount();
    }

    Error getLastError() {
        auto esr = can.esr.volatileLoad();
        return static_cast<Error>(esr.LEC.get());
    }

    uint32_t receiveErrorCount() const final { return can.esr.volatileLoad().REC; }
    uint32_t transmitErrorCount() const final { return can.esr.volatileLoad().TEC; }

    uint_fast8_t emptyTxMailboxCount() const {
        auto tsr = can.tsr.volatileLoad();
        uint_fast8_t count = 0;
        if (tsr.TME0) count++;
        if (tsr.TME1) count++;
        if (tsr.TME2) count++;
        return count;
    }

    uint8_t getNumber() {
        if (&can == registers::can1) {
            return 1;
#ifdef _MICROHAL_CAN2_BASE_ADDRESS
        } else if (&can == registers::can2) {
            return 2;
#endif
        }
    }

    bool addFilter(const can::Filter &filter) final { return flt.addFilter(filter); }
    bool removeFilter(const can::Filter &filter) final { return flt.removeFilter(filter); }

 private:
    microhal::registers::CAN &can;
    CANFilter flt;
    static CAN *objectPtr[2];
    mutable microhal::os::Semaphore txFinish = {};
    mutable bool txWait = false;
    mutable microhal::os::Semaphore dataReady = {};

    int_fast8_t getEmptyMailboxNumber() {
        if (emptyTxMailboxCount()) {
            return can.tsr.volatileLoad().CODE;
        }
        return -1;
    }

    void initializationRequest() {
        auto mcr = can.mcr.volatileLoad();
        mcr.INRQ = 1;
        can.mcr.volatileStore(mcr);

        while (can.msr.volatileLoad().INAK == 0) {
        }
    }

    void exitInitializationMode() {
        auto mcr = can.mcr.volatileLoad();
        mcr.INRQ = 0;
        can.mcr.volatileStore(mcr);
        // while (can.msr.volatileLoad().INAK == 1) {
        // }
    }

    // ISR related functions
    void enableInterrupt(uint32_t priority);
    void disableInterrupt();

    void interruptFunction();
    void rxInterruptFunction();

    friend void CAN1_TX_IRQHandler();
    friend void CAN1_RX0_IRQHandler();
    friend void CAN1_RX1_IRQHandler();
    friend void CAN2_TX_IRQHandler();
    friend void CAN2_RX0_IRQHandler();
    friend void CAN2_RX1_IRQHandler();
};
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // _MICROHAL_PORTS_CAN_STMCOMMON_H_
