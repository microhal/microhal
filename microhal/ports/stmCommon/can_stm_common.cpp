/*
 * can_stm32f4xx.cpp
 *
 *  Created on: Apr 2, 2019
 *      Author: pokas
 */

#include "can_stm_common.h"
#include "diagnostic/diagnostic.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

using namespace diagnostic;

CAN *CAN::objectPtr[2] = {nullptr, nullptr};

static void mailboxToMessage(CAN::RxMailbox &mailbox, can::Message &message);

template <diagnostic::LogLevel level, bool B>
diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel, CAN::Filter::ID32 id) {
    if (!id.ide)
        logChannel << "CAN Standard ID: " << id.stid;
    else
        logChannel << "CAN Extended ID: " << id.exid;

    logChannel << diagnostic::endl << "\tisRemoteFrame: " << id.rtr;
    return logChannel;
}

template <diagnostic::LogLevel level, bool B>
diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel, CAN::Filter::ID16 id) {
    if (!id.ide)
        logChannel << "CAN Standard ID: " << id.stid;
    else
        logChannel << "CAN Extended ID: " << uint32_t{(id.stid << 3) | id.exid_15_17};

    logChannel << diagnostic::endl << "\tisRemoteFrame: " << id.rtr << diagnostic::endl;
    return logChannel;
}

template <diagnostic::LogLevel level, bool B>
diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel, CAN::FilterMode filterMode) {
    switch (filterMode) {
        case CAN::FilterMode::List16bit:
            logChannel << "List 16 bit";
            break;
        case CAN::FilterMode::List32bit:
            logChannel << "List 32 bit";
            break;
        case CAN::FilterMode::Mask16bit:
            logChannel << "Mask 16 bit";
            break;
        case CAN::FilterMode::Mask32bit:
            logChannel << "Mask 32 bit";
            break;
    }

    return logChannel;
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

CAN::Filter::ID32 makeID32(CAN::Message::ID id, bool isRemoteRequest) {
    CAN::Filter::ID32 id32 = {};
    if (id.isExtended()) {
        id32.ide = 1;
        id32.exid = id.getID();
    } else {
        id32.ide = 0;
        id32.stid = id.getID();
    }
    id32.rtr = isRemoteRequest;
    id32.zero = 0;
    return id32;
}

CAN::Filter::ID16 makeID16(CAN::Message::ID id, bool isRemoteRequest) {
    CAN::Filter::ID16 id16 = {};
    if (id.isExtended()) {
        id16.ide = 1;
        id16.setExtendedId(id.getID());
    } else {
        id16.ide = 0;
        id16.stid = id.getID();
    }
    id16.rtr = isRemoteRequest;
    return id16;
}

CAN::FilterMode CAN::getFilterMode(uint_fast8_t filterNumber) const {
    auto fs1r = can.fs1r.volatileLoad();
    auto fm1r = can.fm1r.volatileLoad();
    return getFilterMode(fs1r, fm1r, filterNumber);
}

CAN::FilterMode CAN::getFilterMode(registers::CAN::FS1R fs1r, registers::CAN::FM1R fm1r, uint_fast8_t filterNumber) {
    uint32_t fsr = fs1r;
    uint32_t fmr = fm1r;
    FilterMode filterMode = static_cast<FilterMode>(((fsr >> filterNumber) & 0b1) | (((fmr >> filterNumber) & 0b1) << 1));
    return filterMode;
}

/**
 * @note This function can be called only in filter initialization mode. To enter filter initialization mode call @ref activateFilterInitMode().
 * @param filterNumber depending on MCU can be form 0 to 13 or from 0 to 27
 * @param filterMode
 */
void CAN::setFilterMode(uint_fast8_t filterNumber, FilterMode filterMode) {
    auto fs1r = can.fs1r.volatileLoad();
    auto fm1r = can.fm1r.volatileLoad();
    switch (filterMode) {
        case FilterMode::List16bit:
            fm1r |= 1 << filterNumber;
            fs1r &= ~(1 << filterNumber);
            break;
        case FilterMode::List32bit:
            fm1r |= 1 << filterNumber;
            fs1r |= 1 << filterNumber;
            break;
        case FilterMode::Mask16bit:
            fm1r &= ~(1 << filterNumber);
            fs1r &= ~(1 << filterNumber);
            break;
        case FilterMode::Mask32bit:
            fm1r &= ~(1 << filterNumber);
            fs1r |= 1 << filterNumber;
            break;
    }
    can.fs1r.volatileStore(fs1r);
    can.fm1r.volatileStore(fm1r);
}

bool CAN::addFilter(const can::Filter &filter) {
    /* We have 4 possibilities to add filter into filter list. We can devide this possibilities into 2 basic group:
     * 1. Identifier List - it require exact match of incoming message, no bit masking is apply. One filter register can hold two 32 bit filter ID or
     *    four 16 bit filter ID.
     * 2. Identifier Mask - it require only specified bits of incoming message to match with ID stored in filter register. Bits that have to match are
     *    stored in mask register. One filter register can hold one 32 bit filter ID with mask or two 16 bit ID with masks.
     *
     * Filter adding policy:
     * 1. If we have to add 32 bit ID into filter and this ID is exact match without mask we can add this filter in two situations:
     *    a) Firstly we are scanning all filter register to find filter register configured as Identifier List. This register can hold two ID so we
     *       check if one ID is unused and can be assign with new ID. If both register ID are assigned witch the same ID it means that one ID is free
     *       and can be assigned.
     *    b) If step 1 fail to find register for new ID we are searching for inactive filter register. When inactive register
     *       if found both list ID are assigned with new ID.
     *
     */
    constexpr const uint32_t exid_0_14 = 0b0111'1111'1111'1111;
    using Match = can::Filter::Match;
    const auto filterMatch = filter.getMatchType();
    const bool matchDataFrames = filterMatch & Match::DataFrame;
    const bool matchRemoteFrames = filterMatch & Match::RemoteFrame;
    const bool matchStandardAndExtendedId = filterMatch & Match::StandardIDAndExtendedID;

    if (matchDataFrames || matchRemoteFrames) {
        if ((matchDataFrames && matchRemoteFrames) || matchStandardAndExtendedId ||
            (((filter.getIDMask().getID() & 0x1FFF'FFFF) != 0x1FFF'FFFF) && filter.getIDMask().isExtended()) ||
            (((filter.getIDMask().getID() & 0x7FF) != 0x7FF) && filter.getIDMask().isStandard())) {
            // Mask filter requested.
            if (filter.getID().isStandard() || ((filter.getIDMask().getID() & exid_0_14) == exid_0_14)) {
                // Because 0 to 14 bits in Extended ID are masked out we can use 16 bit filter with mask.
                auto id = makeID16(filter.getID(), matchRemoteFrames);
                auto mask = makeID16(filter.getIDMask(), !(matchDataFrames && matchRemoteFrames));
                mask.ide = matchStandardAndExtendedId ? 0 : 1;
                return addIdentifierMask16(id, mask);
            } else {
                // We have to use 32 bit Identifier Mask
                auto id = makeID32(filter.getID(), matchRemoteFrames);
                auto mask = makeID32(filter.getIDMask(), !(matchDataFrames && matchRemoteFrames));
                return addIdentifierMask32(id, mask);
            }
        } else {
            if (filter.getID().isExtended()) {
                auto id = makeID32(filter.getID(), matchRemoteFrames);
                return addIdentifierList32(id);
            } else {
                auto id = makeID16(filter.getID(), matchRemoteFrames);
                return addIdentifierList16(id);
            }
        }
    }
    return false;
}

bool CAN::removeFilter(const can::Filter &filter) {
    constexpr const uint32_t exid_0_14 = 0b0111'1111'1111'1111;
    using Match = can::Filter::Match;
    const auto filterMatch = filter.getMatchType();
    const bool matchDataFrames = filterMatch & Match::DataFrame;
    const bool matchRemoteFrames = filterMatch & Match::RemoteFrame;
    const bool matchStandardAndExtendedId = filterMatch & Match::StandardIDAndExtendedID;

    if (matchDataFrames || matchRemoteFrames) {
        if ((matchDataFrames && matchRemoteFrames) || matchStandardAndExtendedId ||
            (((filter.getIDMask().getID() & 0x1FFF'FFFF) != 0x1FFF'FFFF) && filter.getIDMask().isExtended()) ||
            (((filter.getIDMask().getID() & 0x7FF) != 0x7FF) && filter.getIDMask().isStandard())) {
            // Mask filter requested.
            if (filter.getID().isStandard() || ((filter.getIDMask().getID() & exid_0_14) == exid_0_14)) {
                // Because 0 to 14 bits in Extended ID are masked out we can use 16 bit filter with mask.
                auto id = makeID16(filter.getID(), matchRemoteFrames);
                auto mask = makeID16(filter.getIDMask(), !(matchDataFrames && matchRemoteFrames));
                mask.ide = matchStandardAndExtendedId ? 0 : 1;
                return removeIdentifierMask16(id, mask);
            } else {
                // We have to use 32 bit Identifier Mask
                auto id = makeID32(filter.getID(), matchRemoteFrames);
                auto mask = makeID32(filter.getIDMask(), !(matchDataFrames && matchRemoteFrames));
                return removeIdentifierMask32(id, mask);
            }
        } else {
            if (filter.getID().isExtended()) {
                auto id = makeID32(filter.getID(), matchRemoteFrames);
                return removeIdentifierList32(id);
            } else {
                auto id = makeID16(filter.getID(), matchRemoteFrames);
                return removeIdentifierList16(id);
            }
        }
    }
    return false;
}

bool CAN::addIdentifierMask32(CAN::Filter::ID32 id, CAN::Filter::ID32 mask) {
    auto fa1r = can.fa1r.volatileLoad();
    for (size_t filterNumber = 0; filterNumber < can.filterRegister.size(); filterNumber++) {
        if (!isFilterActive(fa1r, filterNumber)) {
            Filter &filter = can.filterRegister[filterNumber];
            filter.identifierMask.id.volatileStore(id);
            filter.identifierMask.mask.volatileStore(mask);
            activateFilterInitMode();
            setFilterMode(filterNumber, FilterMode::Mask32bit);
            fa1r.activateFilter(filterNumber);
            can.fa1r.volatileStore(fa1r);
            deactivateFilterInitMode();
            return true;
        }
    }
    return false;
}

bool CAN::addIdentifierMask16(CAN::Filter::ID16 id, CAN::Filter::ID16 mask) {
    auto fa1r = can.fa1r.volatileLoad();
    auto fs1r = can.fs1r.volatileLoad();
    auto fm1r = can.fm1r.volatileLoad();
    for (size_t i = 0; i < can.filterRegister.size(); i++) {
        if (isFilterActive(fa1r, i) && getFilterMode(fs1r, fm1r, i) == FilterMode::Mask16bit) {
            Filter &filter = can.filterRegister[i];
            auto identifierMask0 = filter.identifierMask16bit[0].volatileLoad();
            auto identifierMask1 = filter.identifierMask16bit[1].volatileLoad();
            if (identifierMask0.id == identifierMask1.id && identifierMask0.mask == identifierMask1.mask) {
                // second register is empty, we can assign our filter there
                identifierMask1.id = id;
                identifierMask1.mask = mask;
                activateFilterInitMode();
                filter.identifierMask16bit[1].volatileStore(identifierMask1);
                deactivateFilterInitMode();
                return true;
            }
        }
    }
    // At this point we was unable to find Mask16Bit register with space for filter. We have to find unused filter register
    for (size_t i = 0; i < can.filterRegister.size(); i++) {
        if (!isFilterActive(fa1r, i)) {
            Filter &filter = can.filterRegister[i];
            Filter::IdentifierMask16Bit identifierMask;
            identifierMask.id = id;
            identifierMask.mask = mask;
            filter.identifierMask16bit[0].volatileStore(identifierMask);
            filter.identifierMask16bit[1].volatileStore(identifierMask);
            activateFilterInitMode();
            setFilterMode(i, FilterMode::Mask16bit);
            fa1r.activateFilter(i);
            can.fa1r.volatileStore(fa1r);
            deactivateFilterInitMode();
            return true;
        }
    }
    return false;
}

bool CAN::addIdentifierList32(CAN::Filter::ID32 id) {
    auto fa1r = can.fa1r.volatileLoad();
    auto fs1r = can.fs1r.volatileLoad();
    auto fm1r = can.fm1r.volatileLoad();
    for (size_t i = 0; i < can.filterRegister.size(); i++) {
        if (isFilterActive(fa1r, i) && getFilterMode(fs1r, fm1r, i) == FilterMode::List32bit) {
            Filter &filter = can.filterRegister[i];
            auto id0 = filter.identifierList[0].volatileLoad();
            auto id1 = filter.identifierList[1].volatileLoad();
            if (id0 == id1) {
                // second register is empty, we can assign our filter there
                activateFilterInitMode();
                filter.identifierList[1].volatileStore(id);
                deactivateFilterInitMode();
                return true;
            }
        }
    }
    // unable to find partially empty filter register. Try to assign completly new filter register
    for (size_t i = 0; i < can.filterRegister.size(); i++) {
        if (!isFilterActive(fa1r, i)) {
            Filter &filter = can.filterRegister[i];
            filter.identifierList[0].volatileStore(id);
            filter.identifierList[1].volatileStore(id);
            activateFilterInitMode();
            setFilterMode(i, FilterMode::List32bit);
            fa1r.activateFilter(i);
            can.fa1r.volatileStore(fa1r);
            deactivateFilterInitMode();
            return true;
        }
    }
    return false;
}

bool CAN::addIdentifierList16(CAN::Filter::ID16 id) {
    auto fa1r = can.fa1r.volatileLoad();
    auto fs1r = can.fs1r.volatileLoad();
    auto fm1r = can.fm1r.volatileLoad();
    for (size_t i = 0; i < can.filterRegister.size(); i++) {
        if (isFilterActive(fa1r, i) && getFilterMode(fs1r, fm1r, i) == FilterMode::List16bit) {
            Filter &filter = can.filterRegister[i];
            Filter::IdentifierList16Bit list0 = filter.identifierList16bit[0].volatileLoad();
            if (list0.id0 == list0.id1) {
                // second register is empty, we can assign our filter there
                list0.id1 = id;
                activateFilterInitMode();
                filter.identifierList16bit[0].volatileStore(list0);
                deactivateFilterInitMode();
                return true;
            }
            Filter::IdentifierList16Bit list1 = filter.identifierList16bit[1].volatileLoad();
            if (list0.id0 == list1.id0 || list0.id0 == list1.id1) {
                if (list0.id0 == list1.id0) {
                    list1.id0 = id;
                } else {
                    list1.id1 = id;
                }
                activateFilterInitMode();
                filter.identifierList16bit[1].volatileStore(list1);
                deactivateFilterInitMode();
                return true;
            }
        }
    }
    // unable to find partially empty filter register. Try to assign completly new filter register
    for (size_t i = 0; i < can.filterRegister.size(); i++) {
        if (!isFilterActive(fa1r, i)) {
            Filter &filter = can.filterRegister[i];
            Filter::IdentifierList16Bit list;
            list.id0 = id;
            list.id1 = id;
            filter.identifierList16bit[0].volatileStore(list);
            filter.identifierList16bit[1].volatileStore(list);
            activateFilterInitMode();
            setFilterMode(i, FilterMode::List16bit);
            fa1r.activateFilter(i);
            can.fa1r.volatileStore(fa1r);
            deactivateFilterInitMode();
            return true;
        }
    }
    return false;
}

bool CAN::removeIdentifierMask32(CAN::Filter::ID32 id, CAN::Filter::ID32 mask) {
    auto fa1r = can.fa1r.volatileLoad();
    auto fs1r = can.fs1r.volatileLoad();
    auto fm1r = can.fm1r.volatileLoad();
    for (size_t filterNumber = 0; filterNumber < can.filterRegister.size(); filterNumber++) {
        if (isFilterActive(fa1r, filterNumber) && getFilterMode(fs1r, fm1r, filterNumber) == FilterMode::Mask32bit) {
            Filter &filter = can.filterRegister[filterNumber];
            if (id == filter.identifierMask.id.volatileLoad() && mask == filter.identifierMask.mask.volatileLoad()) {
                activateFilterInitMode();
                fa1r.deactivateFilter(filterNumber);
                can.fa1r.volatileStore(fa1r);
                deactivateFilterInitMode();
            }
            return true;
        }
    }
    return false;
}

bool CAN::removeIdentifierMask16(CAN::Filter::ID16 id, CAN::Filter::ID16 mask) {
    auto fa1r = can.fa1r.volatileLoad();
    auto fs1r = can.fs1r.volatileLoad();
    auto fm1r = can.fm1r.volatileLoad();
    for (size_t filterNumber = 0; filterNumber < can.filterRegister.size(); filterNumber++) {
        if (isFilterActive(fa1r, filterNumber) && getFilterMode(fs1r, fm1r, filterNumber) == FilterMode::Mask16bit) {
            Filter &filter = can.filterRegister[filterNumber];
            auto identifierMask0 = filter.identifierMask16bit[0].volatileLoad();
            auto identifierMask1 = filter.identifierMask16bit[1].volatileLoad();
            if (id == identifierMask0.id && mask == identifierMask0.mask) {
                if (identifierMask0.id == identifierMask1.id && identifierMask0.mask == identifierMask1.mask) {
                    // both filter settings are identical, we can disable this filter
                    activateFilterInitMode();
                    fa1r.deactivateFilter(filterNumber);
                    can.fa1r.volatileStore(fa1r);
                    deactivateFilterInitMode();
                    return true;
                } else {
                    // remove only first filter
                    activateFilterInitMode();
                    filter.identifierMask16bit[0].volatileStore(identifierMask1);
                    deactivateFilterInitMode();
                }
            } else if (id == identifierMask1.id && mask == identifierMask1.mask) {
                // remove only second filter
                activateFilterInitMode();
                filter.identifierMask16bit[1].volatileStore(identifierMask0);
                deactivateFilterInitMode();
            }
        }
    }
    return false;
}

bool CAN::removeIdentifierList32(CAN::Filter::ID32 id) {
    auto fa1r = can.fa1r.volatileLoad();
    auto fs1r = can.fs1r.volatileLoad();
    auto fm1r = can.fm1r.volatileLoad();
    for (size_t filterNumber = 0; filterNumber < can.filterRegister.size(); filterNumber++) {
        if (isFilterActive(fa1r, filterNumber) && getFilterMode(fs1r, fm1r, filterNumber) == FilterMode::List32bit) {
            Filter &filter = can.filterRegister[filterNumber];
            auto id0 = filter.identifierList[0].volatileLoad();
            auto id1 = filter.identifierList[1].volatileLoad();
            if (id == id0) {
                if (id0 == id1) {
                    // both filter settings are identical, we can disable this filter
                    activateFilterInitMode();
                    fa1r.deactivateFilter(filterNumber);
                    can.fa1r.volatileStore(fa1r);
                    deactivateFilterInitMode();
                    return true;
                } else {
                    // remove only first filter
                    activateFilterInitMode();
                    filter.identifierList[0].volatileStore(id1);
                    deactivateFilterInitMode();
                    return true;
                }
            } else if (id == id1) {
                // remove only second filter
                activateFilterInitMode();
                filter.identifierList[1].volatileStore(id0);
                deactivateFilterInitMode();
                return true;
            }
        }
    }
    return false;
}

bool CAN::removeIdentifierList16(CAN::Filter::ID16 id) {
    auto fa1r = can.fa1r.volatileLoad();
    auto fs1r = can.fs1r.volatileLoad();
    auto fm1r = can.fm1r.volatileLoad();
    for (size_t filterNumber = 0; filterNumber < can.filterRegister.size(); filterNumber++) {
        if (isFilterActive(fa1r, filterNumber) && getFilterMode(fs1r, fm1r, filterNumber) == FilterMode::List16bit) {
            Filter &filter = can.filterRegister[filterNumber];
            Filter::IdentifierList16Bit list0 = filter.identifierList16bit[0].volatileLoad();
            Filter::IdentifierList16Bit list1 = filter.identifierList16bit[1].volatileLoad();
            if (id == list0.id0 || id == list0.id1 || id == list1.id0 || id == list1.id1) {
                if (list0.id0 == list0.id1 && list1.id0 == list1.id1 && list1.id0 == list1.id0) {
                    // all filter settings are identical, we can disable this filter
                    activateFilterInitMode();
                    fa1r.deactivateFilter(filterNumber);
                    can.fa1r.volatileStore(fa1r);
                    deactivateFilterInitMode();
                    return true;
                }
                if (id == list0.id0) {
                    auto oldId0 = list0.id0;
                    list0.id0 = id;
                    if (list0.id1 == oldId0) {
                        list0.id1 = id;
                    }
                    if (list1.id0 == oldId0) {
                        list1.id0 = id;
                    }
                    if (list1.id1 == oldId0) {
                        list1.id1 = id;
                    }
                    activateFilterInitMode();
                    filter.identifierList16bit[0].volatileStore(list0);
                    filter.identifierList16bit[1].volatileStore(list1);
                    deactivateFilterInitMode();
                } else {
                    if (id == list0.id1) {
                        activateFilterInitMode();
                        list0.id1 = list0.id0;
                        filter.identifierList16bit[0].volatileStore(list0);
                        deactivateFilterInitMode();
                        return true;
                    }
                    if (id == list1.id0) {
                        activateFilterInitMode();
                        list1.id0 = list0.id0;
                        filter.identifierList16bit[1].volatileStore(list1);
                        deactivateFilterInitMode();
                        return true;
                    }
                    if (id == list1.id1) {
                        activateFilterInitMode();
                        list1.id1 = list0.id0;
                        filter.identifierList16bit[1].volatileStore(list1);
                        deactivateFilterInitMode();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void CAN::enableInterrupt(uint32_t priority) {
    NVIC_SetPriority(txIrq(), priority);
    NVIC_SetPriority(rx0Irq(), priority);
    NVIC_SetPriority(rx1Irq(), priority);
    NVIC_SetPriority(sceIrq(), priority);
    NVIC_EnableIRQ(txIrq());
    NVIC_EnableIRQ(rx0Irq());
    NVIC_EnableIRQ(rx1Irq());
}

void CAN::disableInterrupt() {
    NVIC_DisableIRQ(txIrq());
    NVIC_DisableIRQ(rx0Irq());
    NVIC_DisableIRQ(rx1Irq());
    NVIC_DisableIRQ(sceIrq());
}

IRQn_Type CAN::txIrq() {
#if defined(_MICROHAL_CAN_BASE)
    if ((int)&can == _MICROHAL_CAN_BASE) return CAN_TX_IRQn;
#endif
#if defined(_MICROHAL_CAN1_BASE)
    if ((int)&can == _MICROHAL_CAN1_BASE) return CAN1_TX_IRQn;
#endif
#if defined(_MICROHAL_CAN2_BASE)
    if ((int)&can == _MICROHAL_CAN2_BASE) return CAN2_TX_IRQn;
#endif
    std::terminate();
}

IRQn_Type CAN::rx0Irq() {
#if defined(_MICROHAL_CAN_BASE)
    if ((int)&can == _MICROHAL_CAN_BASE) return CAN_RX0_IRQn;
#endif
#if defined(_MICROHAL_CAN1_BASE)
    if ((int)&can == _MICROHAL_CAN1_BASE) return CAN1_RX0_IRQn;
#endif
#if defined(_MICROHAL_CAN2_BASE)
    if ((int)&can == _MICROHAL_CAN2_BASE) return CAN2_RX0_IRQn;
#endif
    std::terminate();
}

IRQn_Type CAN::rx1Irq() {
#if defined(_MICROHAL_CAN_BASE)
    if ((int)&can == _MICROHAL_CAN_BASE) return CAN_RX1_IRQn;
#endif
#if defined(_MICROHAL_CAN1_BASE)
    if ((int)&can == _MICROHAL_CAN1_BASE) return CAN1_RX1_IRQn;
#endif
#if defined(_MICROHAL_CAN2_BASE)
    if ((int)&can == _MICROHAL_CAN2_BASE) return CAN2_RX1_IRQn;
#endif
    std::terminate();
}

IRQn_Type CAN::sceIrq() {
#if defined(_MICROHAL_CAN_BASE)
    if ((int)&can == _MICROHAL_CAN_BASE) return CAN_SCE_IRQn;
#endif
#if defined(_MICROHAL_CAN1_BASE)
    if ((int)&can == _MICROHAL_CAN1_BASE) return CAN1_SCE_IRQn;
#endif
#if defined(_MICROHAL_CAN2_BASE)
    if ((int)&can == _MICROHAL_CAN2_BASE) return CAN2_SCE_IRQn;
#endif
    std::terminate();
}

void CAN::dumpFilterConfig() {
    auto fa1r = can.fa1r.volatileLoad();
    auto fs1r = can.fs1r.volatileLoad();
    auto fm1r = can.fm1r.volatileLoad();
    auto log = diagChannel << lock << MICROHAL_DEBUG;
    for (size_t filterNumber = 0; filterNumber < can.filterRegister.size(); filterNumber++) {
        auto filterMode = getFilterMode(fs1r, fm1r, filterNumber);
        Filter &filter = can.filterRegister[filterNumber];
        log << "---------- Filter number: " << filterNumber << endl
            << "\tFilter is active: " << isFilterActive(fa1r, filterNumber) << endl
            << "\tFilter mode: " << filterMode << endl;
        switch (filterMode) {
            case FilterMode::Mask16bit: {
                auto filter0 = filter.identifierMask16bit[0].volatileLoad();
                auto filter1 = filter.identifierMask16bit[1].volatileLoad();
                log << "\tID: " << filter0.id << ", Mask: " << filter0.mask << endl;
                log << "\tID: " << filter1.id << ", Mask: " << filter1.mask << endl;
            } break;
            case FilterMode::Mask32bit: {
                auto filterId = filter.identifierMask.id.volatileLoad();
                auto filterMask = filter.identifierMask.mask.volatileLoad();
                log << "\tID: " << filterId << ", Mask: " << filterMask << endl;
            } break;
            case FilterMode::List16bit: {
                auto list1 = filter.identifierList16bit[0].volatileLoad();
                auto list2 = filter.identifierList16bit[1].volatileLoad();
                log << "\tID: " << list1.id0 << endl
                    << "\tID: " << list1.id1 << endl
                    << "\tID: " << list2.id0 << endl
                    << "\tID: " << list2.id1 << endl;
            } break;
            case FilterMode::List32bit:
                auto id1 = filter.identifierList[0].volatileLoad();
                auto id2 = filter.identifierList[1].volatileLoad();
                log << "\tID: " << id1 << endl << "\tID: " << id2 << endl;
                break;
        }
    }
    log << unlock;
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
//***********************************************************************************************//
//                                          IRQHandlers                                          //
//***********************************************************************************************//
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

#ifdef _MICROHAL_CAN2_BASE
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
#endif
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
