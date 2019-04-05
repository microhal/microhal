/*
 * can_stm32f4xx.cpp
 *
 *  Created on: Apr 2, 2019
 *      Author: pokas
 */

#include "can_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
namespace registers {
#if defined(CAN1_BASE)
CAN &can1 = *reinterpret_cast<CAN *>(CAN1_BASE);
#endif
#if defined(CAN2_BASE)
CAN &can2 = *reinterpret_cast<CAN *>(CAN2_BASE);
#endif
}  // namespace registers

CAN *CAN::objectPtr[2] = {nullptr, nullptr};

uint32_t CAN::setBaudrate(uint32_t baudrate) {
    initializationRequest();
    if (baudrate > bitRateMax) baudrate = bitRateMax;
    uint32_t frequency = ClockManager::CANFrequency(can);
    uint32_t ts1 = can.btr.bitfield.TS1 + 1;
    uint32_t ts2 = can.btr.bitfield.TS2 + 1;
    uint32_t brp = frequency / (baudrate * (ts1 + ts2));
    if (brp > 0) {
        brp -= 1;
    }
    brp = brp > 0x1FF ? 0x1FF : brp;
    can.btr.bitfield.BRP = brp;
    exitInitializationMode();
    return getBaudrate();
}

uint32_t CAN::getBaudrate() {
    uint32_t frequency = ClockManager::CANFrequency(can);
    uint32_t brp = can.btr.bitfield.BRP + 1;
    uint32_t ts1 = can.btr.bitfield.TS1 + 1;
    uint32_t ts2 = can.btr.bitfield.TS2 + 1;
    return frequency / (brp * (1 + ts1 + ts2));
}

bool CAN::transmit(const Message &message) {
    auto mailbox_no = getEmptyMailboxNumber();
    if (mailbox_no >= 0) {
        volatile TxMailbox &mailbox = can.sTxMailBox[mailbox_no];
        mailbox.tir.bitfield.IDE = message.id.ide;
        if (message.isExtendedID()) {
            mailbox.tir.bitfield.STID_EXID = message.id.stid_exid;
        } else {
            mailbox.tir.bitfield.STID_EXID = message.id.stid_exid << 18;
        }
        auto data = message.getData();
        mailbox.tir.bitfield.RTR = (data.size_bytes() == 0);
        mailbox.tdtr.bitfield.DLC = data.size_bytes();
        // put data into mailbox
        if (data.size()) mailbox.TDLR = *reinterpret_cast<const uint32_t *>(data.data());
        if (data.size() > 4) mailbox.TDHR = *reinterpret_cast<const uint32_t *>(data.data() + sizeof(uint32_t));
        mailbox.tdtr.bitfield.TGT = 0;
        // Request transmission
        mailbox.tir.bitfield.TXRQ = 1;
        return true;
    }
    return false;
}

static void mailboxToMessage(volatile CAN::RxMailbox &mailbox, can::Message &message) {
    if (mailbox.rir.bitfield.IDE) {
        message.setExtendedID(mailbox.rir.bitfield.STID_EXID);
    } else {
        message.setStandardID(mailbox.rir.bitfield.STID_EXID >> 18);
    }
    uint_fast8_t size = mailbox.rdtr.bitfield.DLC;
    if (mailbox.rir.bitfield.RTR) {
        message.setRemoteRequest(size);
    } else if (size) {
        uint8_t data[8];
        *reinterpret_cast<uint32_t *>(data) = mailbox.RDLR;
        *reinterpret_cast<uint32_t *>(&data[4]) = mailbox.RDLR;
        message.setDataFrame({data, size});
    }
}

bool CAN::receive(Message &message) {
    if (can.rf0r.mesagesCount() || can.rf1r.mesagesCount()) {
        if (can.rf0r.mesagesCount()) {
            mailboxToMessage(can.sFIFOMailBox[0], message);
            can.rf0r.releaseMessage();
            return true;
        }
        if (can.rf1r.mesagesCount()) {
            mailboxToMessage(can.sFIFOMailBox[1], message);
            can.rf1r.releaseMessage();
            return true;
        }
    }
    return false;
}

void CAN::setMode(Mode mode) {
    initializationRequest();
    switch (mode) {
        case Mode::Normal:
            can.btr.bitfield.SILM = 0;
            can.btr.bitfield.LBKM = 0;
            break;
        case Mode::Loopback:
            can.btr.bitfield.SILM = 0;
            can.btr.bitfield.LBKM = 1;
            break;
        case Mode::Silent:
            can.btr.bitfield.SILM = 1;
            can.btr.bitfield.LBKM = 0;
            break;
        case Mode::LoopbackAndSilent:
            can.btr.bitfield.SILM = 1;
            can.btr.bitfield.LBKM = 1;
            break;
    };
    exitInitializationMode();
}

void CAN::sleepMode(Sleep sleepMode) {
    initializationRequest();
    switch (sleepMode) {
        case Sleep::Sleep:
            can.mcr.bitfield.AWUM = 0;
            can.mcr.bitfield.SLEEP = 1;
            break;
        case Sleep::Wakeup:
            can.mcr.bitfield.AWUM = 0;
            can.mcr.bitfield.SLEEP = 0;
            break;
        case Sleep::AutoWakeup:
            can.mcr.bitfield.AWUM = 1;
            break;
    }
    exitInitializationMode();
}

bool CAN::addFilter(can::Message::ID id, uint32_t mask, bool isRemoteFrame) {
    Fifo fifo = Fifo0;
    FilterMode mode = FilterMode::Mask32bit;
    for (uint_fast8_t filterNumber = 0; filterNumber < 27; filterNumber++) {
        if (!isFilterActive(filterNumber)) {
            activateFilterInitMode();
            Filter &filter = can.sFilterRegister[filterNumber];
            switch (mode) {
                case FilterMode::Mask16bit:
                    can.FM1R &= ~(1 << filterNumber);
                    can.FS1R &= ~(1 << filterNumber);
                    filter.identifierMask16bit.id1 = id.getID();
                    filter.identifierMask16bit.mask1 = mask;
                    break;
                case FilterMode::Mask32bit:
                    can.FM1R &= ~(1 << filterNumber);
                    can.FS1R |= 1 << filterNumber;
                    filter.identifierMask.id.stid_exid = id.getID();
                    filter.identifierMask.id.ide = id.isExtended();
                    filter.identifierMask.id.rtr = isRemoteFrame;
                    filter.identifierMask.mask.stid_exid = mask;
                    filter.identifierMask.mask.ide = id.isExtended();
                    filter.identifierMask.mask.rtr = isRemoteFrame;
                    break;
                case FilterMode::List16bit:
                    can.FM1R |= 1 << filterNumber;
                    can.FS1R &= ~(1 << filterNumber);
                    filter.identifierList16bit.id[0] = id.getID();
                    break;
                case FilterMode::List32bit:
                    can.FM1R |= 1 << filterNumber;
                    can.FS1R |= 1 << filterNumber;
                    filter.identifierList.id[0].stid_exid = id.getID();
                    filter.identifierList.id[0].ide = id.isExtended();
                    filter.identifierList.id[0].rtr = isRemoteFrame;
                    break;
            }
            switch (fifo) {
                case Fifo0:
                    can.FFA1R &= ~(1 << filterNumber);
                    break;
                case Fifo1:
                    can.FFA1R |= 1 << filterNumber;
                    break;
            }

            activateFilter(filterNumber);
            deactivateFilterInitMode();
            return true;
        }
    }
    return false;
}

bool CAN::removeFilter(can::Message::ID id, uint32_t mask, bool isRemoteFrame) {
    for (uint_fast8_t filterNumber = 0; filterNumber < 27; filterNumber++) {
        if (isFilterActive(filterNumber)) {
            Filter &filter = can.sFilterRegister[filterNumber];
            FilterMode mode = FilterMode::Mask32bit;
            switch (mode) {
                case FilterMode::Mask16bit:
                    filter.identifierMask16bit.id1 = id.getID();
                    filter.identifierMask16bit.mask1 = mask;
                    break;
                case FilterMode::Mask32bit:
                    if ((filter.identifierMask.id.stid_exid == id.getID()) && (filter.identifierMask.id.ide == id.isExtended()) &&
                        (filter.identifierMask.id.rtr == isRemoteFrame) && (filter.identifierMask.mask.stid_exid == mask) &&
                        (filter.identifierMask.mask.ide == id.isExtended()) && (filter.identifierMask.mask.rtr == isRemoteFrame)) {
                        activateFilterInitMode();
                        deactivateFilter(filterNumber);
                        deactivateFilterInitMode();
                        return true;
                    }
                    break;
                case FilterMode::List16bit:
                    filter.identifierList16bit.id[0] = id.getID();
                    break;
                case FilterMode::List32bit:
                    filter.identifierList.id[0].stid_exid = id.getID();
                    filter.identifierList.id[0].ide = id.isExtended();
                    filter.identifierList.id[0].rtr = isRemoteFrame;
                    break;
            }
        }
    }
    return false;
}

void CAN::interruptFunction() {
    if (can.tsr.bitfield.RQCP0 || can.tsr.bitfield.RQCP1 || can.tsr.bitfield.RQCP2) {
        can.tsr.bitfield.RQCP0 = 1;
        can.tsr.bitfield.RQCP1 = 1;
        can.tsr.bitfield.RQCP2 = 1;
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
    if (can.rf0r.bitfield.FMPx || can.rf1r.bitfield.FMPx) {
        can.ier.disableInterrupt(Interrupt::FIFO0_MessagePending | Interrupt::FIFO1_MessagePending);
        auto shouldYeld = dataReady.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
        portYIELD_FROM_ISR(shouldYeld);
#else
        (void)shouldYeld;
#endif
    }
}

// CAN 1
extern "C" void CAN1_TX_IRQHandler() {
    CAN::objectPtr[0]->interruptFunction();
}

extern "C" void CAN1_RX0_IRQHandler() {
    CAN::objectPtr[0]->rxInterruptFunction();
}
extern "C" void CAN1_RX1_IRQHandler() {
    CAN::objectPtr[0]->rxInterruptFunction();
}

extern "C" void CAN1_SCE_IRQHandler() {}

// CAN 2
extern "C" void CAN2_TX_IRQHandler() {
    CAN::objectPtr[1]->interruptFunction();
}

extern "C" void CAN2_RX0_IRQHandler() {
    CAN::objectPtr[1]->rxInterruptFunction();
}
extern "C" void CAN2_RX1_IRQHandler() {
    CAN::objectPtr[1]->rxInterruptFunction();
}

extern "C" void CAN2_SCE_IRQHandler() {}

}  // namespace stm32f4xx
}  // namespace microhal
