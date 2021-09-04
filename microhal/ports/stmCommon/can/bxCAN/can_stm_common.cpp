/*
 * can_stm32f4xx.cpp
 *
 *  Created on: Apr 2, 2019
 *      Author: pokas
 */

#include "can_stm_common.h"
#include <cassert>
#include "diagnostic/diagnostic.h"
#include _MICROHAL_INCLUDE_PORT_INTERRUPT_CONTROLLER

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

using namespace diagnostic;

CAN *CAN::objectPtr[2] = {nullptr, nullptr};

static void mailboxToMessage(CAN::RxMailbox &mailbox, can::Message &message);

CAN::CAN(microhal::registers::CAN *const canDevice, uint32_t interruptPriority) : can(*canDevice), flt(canDevice == registers::can1) {
#ifdef HAL_RTOS_FreeRTOS
    assert(interruptPriority >= configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
#endif

    const int canNumber = getNumber();
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::enableCan(canNumber, ClockManager::PowerMode::Normal);
#else
    microhal::ClockManager::enableCan(canNumber);
#endif

    assert(objectPtr[canNumber - 1] == nullptr);
    objectPtr[canNumber - 1] = this;

    auto ier = can.ier.volatileLoad();
    ier.enableInterrupt(Interrupt::TransmitMailboxEmpty);
    can.ier.volatileStore(ier);

    enableInterrupt(interruptPriority);
}

CAN::~CAN() {
    if ((objectPtr[0] != this) && (objectPtr[1] != this)) {
        return;
    }
    initializationRequest();  // This will wait until ongoing transmission will be finished
    disableInterrupt();
    auto ier = can.ier.volatileLoad();
    ier.disableInterrupt(Interrupt::TransmitMailboxEmpty);
    can.ier.volatileStore(ier);
    if (objectPtr[0] == this) objectPtr[0] = nullptr;
    if (objectPtr[1] == this) objectPtr[1] = nullptr;
#if defined(_MICROHAL_CLOCKMANAGER_HAS_POWERMODE) && _MICROHAL_CLOCKMANAGER_HAS_POWERMODE == 1
    ClockManager::disableCan(getNumber(), ClockManager::PowerMode::Normal);
#else
    ClockManager::disableCan(getNumber());
#endif
}

uint32_t CAN::setBaudrate(uint32_t baudrate) {
    initializationRequest();
    if (baudrate > bitRateMax) baudrate = bitRateMax;
    uint32_t frequency = ClockManager::CANFrequency(getNumber());
    auto btr = can.btr.volatileLoad();
    uint32_t brp = frequency / (baudrate * ((btr.TS1 + 1) + (btr.TS2 + 1)));
    if (brp > 0) {
        brp -= 1;
    }
    brp = brp > 0x1FF ? 0x1FF : brp;
    btr.BRP = brp;
    can.btr.volatileStore(btr);
    exitInitializationMode();
    return getBaudrate();
}

uint32_t CAN::getBaudrate() {
    uint32_t frequency = ClockManager::CANFrequency(getNumber());
    auto btr = can.btr.volatileLoad();
    uint32_t brp = btr.BRP + 1;
    uint32_t ts1 = btr.TS1 + 1;
    uint32_t ts2 = btr.TS2 + 1;
    return frequency / (brp * (ts1 + ts2));
}

bool CAN::transmit(const Message &message) {
    auto mailbox_no = getEmptyMailboxNumber();
    if (mailbox_no >= 0) {
        TxMailbox &mailbox = can.txMailBox[mailbox_no];
        auto tir = mailbox.tir.volatileLoad();

        tir.IDE = message.getID().isExtended();
        if (message.isExtendedID()) {
            tir.EXID = message.getID().getID();
        } else if (message.isStandardID()) {
            tir.STID = message.getID().getID();
        } else {
            return false;
        }
        auto data = message.getData();
        tir.RTR = message.isRemoteFrame() ? 1 : 0;

        auto tdtr = mailbox.tdtr.volatileLoad();

        tdtr.DLC = data.size_bytes();
        // put data into mailbox
        if (data.size()) {
            CAN::TxMailbox::TDLxR tdlr;
            tdlr = *reinterpret_cast<const uint32_t *>(data.data());
            mailbox.tdlr.volatileStore(tdlr);
        }
        if (data.size() > 4) {
            CAN::TxMailbox::TDHxR tdhr;
            tdhr = *reinterpret_cast<const uint32_t *>(data.data() + sizeof(uint32_t));
            mailbox.tdhr.volatileStore(tdhr);
        }
        tdtr.TGT = 0;
        // Request transmission
        tir.TXRQ = 1;

        mailbox.tdtr.volatileStore(tdtr);
        mailbox.tir.volatileStore(tir);
        return true;
    }
    return false;
}

bool CAN::receive(Message &message) {
    auto rf0r = can.rf0r.volatileLoad();
    auto rf1r = can.rf1r.volatileLoad();

    if (rf0r.mesagesCount()) {
        mailboxToMessage(can.fifoMailBox[0], message);
        rf0r.releaseMessage();
        can.rf0r.volatileStore(rf0r);
        return true;
    }
    if (rf1r.mesagesCount()) {
        mailboxToMessage(can.fifoMailBox[1], message);
        rf1r.releaseMessage();
        can.rf1r.volatileStore(rf1r);
        return true;
    }

    return false;
}

void CAN::setMode(Mode mode) {
    initializationRequest();
    auto btr = can.btr.volatileLoad();
    switch (mode) {
        case Mode::Normal:
            btr.SILM = 0;
            btr.LBKM = 0;
            break;
        case Mode::Loopback:
            btr.SILM = 0;
            btr.LBKM = 1;
            break;
        case Mode::Silent:
            btr.SILM = 1;
            btr.LBKM = 0;
            break;
        case Mode::LoopbackAndSilent:
            btr.SILM = 1;
            btr.LBKM = 1;
            break;
    };
    can.btr.volatileStore(btr);
    exitInitializationMode();
}

void CAN::sleepMode(Sleep sleepMode) {
    initializationRequest();
    auto mcr = can.mcr.volatileLoad();
    switch (sleepMode) {
        case Sleep::Sleep:
            mcr.AWUM = 0;
            mcr.SLEEP = 1;
            break;
        case Sleep::Wakeup:
            mcr.AWUM = 0;
            mcr.SLEEP = 0;
            break;
        case Sleep::AutoWakeup:
            mcr.AWUM = 1;
            break;
    }
    can.mcr.volatileStore(mcr);
    exitInitializationMode();
}

static void mailboxToMessage(CAN::RxMailbox &mailbox, can::Message &message) {
    auto rir = mailbox.rir.volatileLoad();
    if (rir.IDE) {
        message.setExtendedID(rir.EXID);
    } else {
        message.setStandardID(rir.STID);
    }
    uint_fast8_t size = mailbox.rdtr.volatileLoad().DLC;
    if (rir.RTR) {
        message.setRemoteRequest(size);
    } else if (size) {
        uint8_t data[8];
        *reinterpret_cast<uint32_t *>(data) = mailbox.rdlr.volatileLoad();
        *reinterpret_cast<uint32_t *>(&data[4]) = mailbox.rdhr.volatileLoad();
        message.setDataFrame({data, size});
    }
}

void CAN::enableInterrupt(uint32_t priority) {
    const uint8_t number = getNumber();
    enableCanTxInterrupt(number, priority);
    enableCanRx0Interrupt(number, priority);
    enableCanRx1Interrupt(number, priority);
}

void CAN::disableInterrupt() {
    const uint8_t number = getNumber();
    disableCanTxInterrupt(number);
    disableCanRx0Interrupt(number);
    disableCanRx1Interrupt(number);
}

//***********************************************************************************************//
//                                     interrupt functions                                       //
//***********************************************************************************************//
void CAN::interruptFunction() {
    auto tsr = can.tsr.volatileLoad();
    if (tsr.RQCP0 || tsr.RQCP1 || tsr.RQCP2) {
        tsr.RQCP0 = 1;
        tsr.RQCP1 = 1;
        tsr.RQCP2 = 1;
        can.tsr.volatileStore(tsr);
        if (txWait && emptyTxMailboxCount() == 3) {
            txWait = false;
            auto shouldYeld = txFinish.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
            portYIELD_FROM_ISR(shouldYeld);
#else
            (void)shouldYeld;
#endif
        }
    }
}

void CAN::rxInterruptFunction() {
    auto rf0r = can.rf0r.volatileLoad();
    auto rf1r = can.rf1r.volatileLoad();
    if (rf0r.mesagesCount() || rf1r.mesagesCount()) {
        auto ier = can.ier.volatileLoad();
        ier.disableInterrupt(Interrupt::FIFO0_MessagePending | Interrupt::FIFO1_MessagePending);
        can.ier.volatileStore(ier);
        auto shouldYeld = dataReady.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
        portYIELD_FROM_ISR(shouldYeld);
#else
        (void)shouldYeld;
#endif
    }
}
//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
// CAN 1
extern "C" void CAN1_TX_IRQHandler(void) {
    CAN::objectPtr[0]->interruptFunction();
}

extern "C" void CAN1_RX0_IRQHandler(void) {
    CAN::objectPtr[0]->rxInterruptFunction();
}
extern "C" void CAN1_RX1_IRQHandler(void) {
    CAN::objectPtr[0]->rxInterruptFunction();
}

extern "C" void CAN1_SCE_IRQHandler(void) {}

#ifdef _MICROHAL_CAN2_BASE_ADDRESS
extern "C" void CAN2_TX_IRQHandler(void) {
    CAN::objectPtr[1]->interruptFunction();
}

extern "C" void CAN2_RX0_IRQHandler(void) {
    CAN::objectPtr[1]->rxInterruptFunction();
}
extern "C" void CAN2_RX1_IRQHandler(void) {
    CAN::objectPtr[1]->rxInterruptFunction();
}

extern "C" void CAN2_SCE_IRQHandler(void) {}
#endif
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
