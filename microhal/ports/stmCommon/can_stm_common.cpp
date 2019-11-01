/*
 * can_stm32f4xx.cpp
 *
 *  Created on: Apr 2, 2019
 *      Author: pokas
 */

#include "can_stm_common.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

CAN *CAN::objectPtr[2] = {nullptr, nullptr};

uint32_t CAN::setBaudrate(uint32_t baudrate) {
    initializationRequest();
    if (baudrate > bitRateMax) baudrate = bitRateMax;
    uint32_t frequency = ClockManager::CANFrequency(can);
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
    uint32_t frequency = ClockManager::CANFrequency(can);
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

bool CAN::addFilter(can::Message::ID id, uint32_t mask, bool isRemoteFrame) {
    Fifo fifo = Fifo0;
    FilterMode mode = FilterMode::Mask32bit;
    for (uint_fast8_t filterNumber = 0; filterNumber < 27; filterNumber++) {
        if (!isFilterActive(filterNumber)) {
            activateFilterInitMode();
            Filter &filter = can.filterRegister[filterNumber];
            switch (mode) {
                case FilterMode::Mask16bit: {
                    auto fm1r = can.fm1r.volatileLoad();
                    auto fs1r = can.fs1r.volatileLoad();
                    fm1r &= ~(1 << filterNumber);
                    fs1r &= ~(1 << filterNumber);
                    can.fm1r.volatileStore(fm1r);
                    can.fs1r.volatileStore(fs1r);
                    Filter::ID16 id16, mask16;
                    if (id.isExtended()) {
                        id16.ide = 1;
                        id16.stid = id.getID() >> 18;
                        id16.exid_15_17 = id.getID() >> 15;
                    }
                    id16.rtr = isRemoteFrame;
                    mask16.stid = mask >> 21;
                    mask16.exid_15_17 = mask >> 18;

                    filter.identifierMask16bit.id1.volatileStore(id16);
                    filter.identifierMask16bit.mask1.volatileStore(mask16);
                } break;
                case FilterMode::Mask32bit: {
                    auto fm1r = can.fm1r.volatileLoad();
                    auto fs1r = can.fs1r.volatileLoad();
                    fm1r &= ~(1 << filterNumber);
                    fs1r |= 1 << filterNumber;
                    can.fm1r.volatileStore(fm1r);
                    can.fs1r.volatileStore(fs1r);
                    Filter::ID32 id32 = {};
                    Filter::ID32 mask32;
                    id32.ide = id.isExtended();
                    id32.stid_exid = id.getID();
                    id32.rtr = isRemoteFrame;
                    id32.zero = 0;
                    mask32 = mask;
                    filter.identifierMask.id.volatileStore(id32);
                    filter.identifierMask.mask.volatileStore(mask32);
                } break;
                case FilterMode::List16bit: {
                    auto fm1r = can.fm1r.volatileLoad();
                    auto fs1r = can.fs1r.volatileLoad();
                    fm1r |= 1 << filterNumber;
                    fs1r &= ~(1 << filterNumber);
                    can.fm1r.volatileStore(fm1r);
                    can.fs1r.volatileStore(fs1r);
                    Filter::ID16 id16;
                    if (id.isExtended()) {
                        id16.ide = 1;
                        id16.stid = id.getID() >> 18;
                        id16.exid_15_17 = id.getID() >> 15;
                    }
                    id16.rtr = isRemoteFrame;
                    filter.identifierList16bit.id[0].volatileStore(id16);
                } break;
                case FilterMode::List32bit: {
                    auto fm1r = can.fm1r.volatileLoad();
                    auto fs1r = can.fs1r.volatileLoad();
                    fm1r |= 1 << filterNumber;
                    fs1r |= 1 << filterNumber;
                    can.fm1r.volatileStore(fm1r);
                    can.fs1r.volatileStore(fs1r);
                    Filter::ID32 id32;
                    id32.ide = id.isExtended();
                    id32.stid_exid = id.getID();
                    id32.rtr = isRemoteFrame;
                    id32.zero = 0;
                    filter.identifierList.id[0].volatileStore(id32);
                } break;
            }
            auto ffa1r = can.ffa1r.volatileLoad();
            switch (fifo) {
                case Fifo0:
                    ffa1r &= ~(1 << filterNumber);
                    break;
                case Fifo1:
                    ffa1r |= 1 << filterNumber;
                    break;
            }
            can.ffa1r.volatileStore(ffa1r);

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
            Filter &filter = can.filterRegister[filterNumber];
            FilterMode mode = FilterMode::Mask32bit;
            switch (mode) {
                case FilterMode::Mask16bit:
                    std::terminate();
                    break;
                case FilterMode::Mask32bit: {
                    auto filterId = filter.identifierMask.id.volatileLoad();
                    auto filterMask = filter.identifierMask.mask.volatileLoad();
                    if ((filterId.stid_exid == id.getID()) && (filterId.ide == id.isExtended()) && (filterId.rtr == isRemoteFrame) &&
                        (filterMask.stid_exid == mask) && (filterMask.ide == id.isExtended()) && (filterMask.rtr == isRemoteFrame)) {
                        activateFilterInitMode();
                        deactivateFilter(filterNumber);
                        deactivateFilterInitMode();
                        return true;
                    }
                } break;
                case FilterMode::List16bit:
                    std::terminate();
                    break;
                case FilterMode::List32bit:
                    std::terminate();
                    break;
            }
        }
    }
    return false;
}

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
    if (rf0r.FMPx || rf1r.FMPx) {
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
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
