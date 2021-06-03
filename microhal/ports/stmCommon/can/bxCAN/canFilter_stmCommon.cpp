/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
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

#include <cassert>
#include "canFilter_stmCommon.h"
#include "diagnostic/diagnostic.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

using namespace diagnostic;
using namespace registers;

template <diagnostic::LogLevel level, bool B>
diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel, CANFilter::FilterRegister::ID32 id) {
    if (!id.ide)
        logChannel << "CAN Standard ID: " << id.stid;
    else
        logChannel << "CAN Extended ID: " << id.exid;

    logChannel << diagnostic::endl << "\tisRemoteFrame: " << id.rtr;
    return logChannel;
}

template <diagnostic::LogLevel level, bool B>
diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel, CANFilter::FilterRegister::ID16 id) {
    if (!id.ide)
        logChannel << "CAN Standard ID: " << id.stid;
    else
        logChannel << "CAN Extended ID: " << id.getExtendedId();

    logChannel << diagnostic::endl << "\tisRemoteFrame: " << id.rtr << diagnostic::endl;
    return logChannel;
}

template <diagnostic::LogLevel level, bool B>
diagnostic::LogLevelChannel<level, B> operator<<(microhal::diagnostic::LogLevelChannel<level, B> logChannel, CANFilter::FilterMode filterMode) {
    switch (filterMode) {
        case CANFilter::FilterMode::List16bit:
            logChannel << "List 16 bit";
            break;
        case CANFilter::FilterMode::List32bit:
            logChannel << "List 32 bit";
            break;
        case CANFilter::FilterMode::Mask16bit:
            logChannel << "Mask 16 bit";
            break;
        case CANFilter::FilterMode::Mask32bit:
            logChannel << "Mask 32 bit";
            break;
    }

    return logChannel;
}

CANFilter::FilterRegister::ID32 makeID32(CANFilter::Message::ID id, bool isRemoteRequest) {
    CANFilter::FilterRegister::ID32 id32 = {};
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

CANFilter::FilterRegister::ID16 makeID16(CANFilter::Message::ID id, bool isRemoteRequest) {
    CANFilter::FilterRegister::ID16 id16 = {};
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

#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
void CANFilter::can2StartBank(uint_fast8_t startBank) {
    auto fmr = canFilter->fmr.volatileLoad();
    fmr.CAN2SB = startBank;
    canFilter->fmr.volatileStore(fmr);
}
#endif

void CANFilter::activateFilterInitMode() {
    using namespace registers;
    auto fmr = canFilter->fmr.volatileLoad();
    fmr.FINIT = 1;
    canFilter->fmr.volatileStore(fmr);
}
void CANFilter::deactivateFilterInitMode() {
    using namespace registers;
    auto fmr = canFilter->fmr.volatileLoad();
    fmr.FINIT = 0;
    canFilter->fmr.volatileStore(fmr);
}

CANFilter::FilterMode CANFilter::getFilterMode(uint_fast8_t filterNumber) const {
    auto fs1r = canFilter->fs1r.volatileLoad();
    auto fm1r = canFilter->fm1r.volatileLoad();
    return getFilterMode(fs1r, fm1r, filterNumber);
}

CANFilter::FilterMode CANFilter::getFilterMode(registers::CANFilter::FS1R fs1r, registers::CANFilter::FM1R fm1r, uint_fast8_t filterNumber) {
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
void CANFilter::setFilterMode(uint_fast8_t filterNumber, FilterMode filterMode) {
    auto fs1r = canFilter->fs1r.volatileLoad();
    auto fm1r = canFilter->fm1r.volatileLoad();
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
    canFilter->fs1r.volatileStore(fs1r);
    canFilter->fm1r.volatileStore(fm1r);
}

bool CANFilter::addFilter(const can::Filter &filter) {
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

bool CANFilter::removeFilter(const can::Filter &filter) {
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

void CANFilter::removeAllFilters() {
    activateFilterInitMode();
    microhal::registers::CANFilter::FA1R fa1r;
    fa1r = 0;
    canFilter->fa1r.volatileStore(fa1r);
    deactivateFilterInitMode();
}

bool CANFilter::addIdentifierMask32(FilterRegister::ID32 id, FilterRegister::ID32 mask) {
    auto fa1r = canFilter->fa1r.volatileLoad();
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    auto [begin, end] = filterRange();
    for (size_t filterNumber = begin; filterNumber < end; filterNumber++) {
#else
    for (size_t filterNumber = 0; filterNumber < canFilter->filterRegister.size(); filterNumber++) {
#endif
        if (!isFilterActive(fa1r, filterNumber)) {
            FilterRegister &filter = canFilter->filterRegister[filterNumber];
            filter.identifierMask.id.volatileStore(id);
            filter.identifierMask.mask.volatileStore(mask);
            activateFilterInitMode();
            setFilterMode(filterNumber, FilterMode::Mask32bit);
            fa1r.activateFilter(filterNumber);
            canFilter->fa1r.volatileStore(fa1r);
            deactivateFilterInitMode();
            return true;
        }
    }
    return false;
}

bool CANFilter::addIdentifierMask16(FilterRegister::ID16 id, FilterRegister::ID16 mask) {
    auto fa1r = canFilter->fa1r.volatileLoad();
    auto fs1r = canFilter->fs1r.volatileLoad();
    auto fm1r = canFilter->fm1r.volatileLoad();
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    auto [begin, end] = filterRange();
    for (size_t i = begin; i < end; i++) {
#else
    for (size_t i = 0; i < canFilter->filterRegister.size(); i++) {
#endif
        if (isFilterActive(fa1r, i) && getFilterMode(fs1r, fm1r, i) == FilterMode::Mask16bit) {
            FilterRegister &filter = canFilter->filterRegister[i];
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
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    for (size_t i = begin; i < end; i++) {
#else
    for (size_t i = 0; i < canFilter->filterRegister.size(); i++) {
#endif
        if (!isFilterActive(fa1r, i)) {
            FilterRegister &filter = canFilter->filterRegister[i];
            FilterRegister::IdentifierMask16Bit identifierMask;
            identifierMask.id = id;
            identifierMask.mask = mask;
            filter.identifierMask16bit[0].volatileStore(identifierMask);
            filter.identifierMask16bit[1].volatileStore(identifierMask);
            activateFilterInitMode();
            setFilterMode(i, FilterMode::Mask16bit);
            fa1r.activateFilter(i);
            canFilter->fa1r.volatileStore(fa1r);
            deactivateFilterInitMode();
            return true;
        }
    }
    return false;
}

bool CANFilter::addIdentifierList32(FilterRegister::ID32 id) {
    auto fa1r = canFilter->fa1r.volatileLoad();
    auto fs1r = canFilter->fs1r.volatileLoad();
    auto fm1r = canFilter->fm1r.volatileLoad();
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    auto [begin, end] = filterRange();
    for (size_t i = begin; i < end; i++) {
#else
    for (size_t i = 0; i < canFilter->filterRegister.size(); i++) {
#endif
        if (isFilterActive(fa1r, i) && getFilterMode(fs1r, fm1r, i) == FilterMode::List32bit) {
            FilterRegister &filter = canFilter->filterRegister[i];
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
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    for (size_t i = begin; i < end; i++) {
#else
    for (size_t i = 0; i < canFilter->filterRegister.size(); i++) {
#endif
        if (!isFilterActive(fa1r, i)) {
            FilterRegister &filter = canFilter->filterRegister[i];
            filter.identifierList[0].volatileStore(id);
            filter.identifierList[1].volatileStore(id);
            activateFilterInitMode();
            setFilterMode(i, FilterMode::List32bit);
            fa1r.activateFilter(i);
            canFilter->fa1r.volatileStore(fa1r);
            deactivateFilterInitMode();
            return true;
        }
    }
    return false;
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE

bool CANFilter::addIdentifierList16(FilterRegister::ID16 id) {
    auto fa1r = canFilter->fa1r.volatileLoad();
    auto fs1r = canFilter->fs1r.volatileLoad();
    auto fm1r = canFilter->fm1r.volatileLoad();
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    auto [begin, end] = filterRange();
    for (size_t i = begin; i < end; i++) {
#else
    for (size_t i = 0; i < canFilter->filterRegister.size(); i++) {
#endif
        if (isFilterActive(fa1r, i) && getFilterMode(fs1r, fm1r, i) == FilterMode::List16bit) {
            FilterRegister &filter = canFilter->filterRegister[i];
            FilterRegister::IdentifierList16Bit list0 = filter.identifierList16bit[0].volatileLoad();
            if (list0.id0 == list0.id1) {
                // second register is empty, we can assign our filter there
                list0.id1 = id;
                activateFilterInitMode();
                filter.identifierList16bit[0].volatileStore(list0);
                deactivateFilterInitMode();
                return true;
            }
            FilterRegister::IdentifierList16Bit list1 = filter.identifierList16bit[1].volatileLoad();
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
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    for (size_t i = begin; i < end; i++) {
#else
    for (size_t i = 0; i < canFilter->filterRegister.size(); i++) {
#endif
        if (!isFilterActive(fa1r, i)) {
            FilterRegister &filter = canFilter->filterRegister[i];
            FilterRegister::IdentifierList16Bit list;
            list.id0 = id;
            list.id1 = id;
            filter.identifierList16bit[0].volatileStore(list);
            filter.identifierList16bit[1].volatileStore(list);
            activateFilterInitMode();
            setFilterMode(i, FilterMode::List16bit);
            fa1r.activateFilter(i);
            canFilter->fa1r.volatileStore(fa1r);
            deactivateFilterInitMode();
            return true;
        }
    }
    return false;
}

bool CANFilter::removeIdentifierMask32(FilterRegister::ID32 id, FilterRegister::ID32 mask) {
    auto fa1r = canFilter->fa1r.volatileLoad();
    auto fs1r = canFilter->fs1r.volatileLoad();
    auto fm1r = canFilter->fm1r.volatileLoad();
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    auto [begin, end] = filterRange();
    for (size_t filterNumber = begin; filterNumber < end; filterNumber++) {
#else
    for (size_t filterNumber = 0; filterNumber < canFilter->filterRegister.size(); filterNumber++) {
#endif
        if (isFilterActive(fa1r, filterNumber) && getFilterMode(fs1r, fm1r, filterNumber) == FilterMode::Mask32bit) {
            FilterRegister &filter = canFilter->filterRegister[filterNumber];
            if (id == filter.identifierMask.id.volatileLoad() && mask == filter.identifierMask.mask.volatileLoad()) {
                activateFilterInitMode();
                fa1r.deactivateFilter(filterNumber);
                canFilter->fa1r.volatileStore(fa1r);
                deactivateFilterInitMode();
            }
            return true;
        }
    }
    return false;
}

bool CANFilter::removeIdentifierMask16(FilterRegister::ID16 id, FilterRegister::ID16 mask) {
    auto fa1r = canFilter->fa1r.volatileLoad();
    auto fs1r = canFilter->fs1r.volatileLoad();
    auto fm1r = canFilter->fm1r.volatileLoad();
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    auto [begin, end] = filterRange();
    for (size_t filterNumber = begin; filterNumber < end; filterNumber++) {
#else
    for (size_t filterNumber = 0; filterNumber < canFilter->filterRegister.size(); filterNumber++) {
#endif
        if (isFilterActive(fa1r, filterNumber) && getFilterMode(fs1r, fm1r, filterNumber) == FilterMode::Mask16bit) {
            FilterRegister &filter = canFilter->filterRegister[filterNumber];
            auto identifierMask0 = filter.identifierMask16bit[0].volatileLoad();
            auto identifierMask1 = filter.identifierMask16bit[1].volatileLoad();
            if (id == identifierMask0.id && mask == identifierMask0.mask) {
                if (identifierMask0.id == identifierMask1.id && identifierMask0.mask == identifierMask1.mask) {
                    // both filter settings are identical, we can disable this filter
                    activateFilterInitMode();
                    fa1r.deactivateFilter(filterNumber);
                    canFilter->fa1r.volatileStore(fa1r);
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

bool CANFilter::removeIdentifierList32(FilterRegister::ID32 id) {
    auto fa1r = canFilter->fa1r.volatileLoad();
    auto fs1r = canFilter->fs1r.volatileLoad();
    auto fm1r = canFilter->fm1r.volatileLoad();
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    auto [begin, end] = filterRange();
    for (size_t filterNumber = begin; filterNumber < end; filterNumber++) {
#else
    for (size_t filterNumber = 0; filterNumber < canFilter->filterRegister.size(); filterNumber++) {
#endif

        if (isFilterActive(fa1r, filterNumber) && getFilterMode(fs1r, fm1r, filterNumber) == FilterMode::List32bit) {
            FilterRegister &filter = canFilter->filterRegister[filterNumber];
            auto id0 = filter.identifierList[0].volatileLoad();
            auto id1 = filter.identifierList[1].volatileLoad();
            if (id == id0) {
                if (id0 == id1) {
                    // both filter settings are identical, we can disable this filter
                    activateFilterInitMode();
                    fa1r.deactivateFilter(filterNumber);
                    canFilter->fa1r.volatileStore(fa1r);
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

bool CANFilter::removeIdentifierList16(FilterRegister::ID16 id) {
    auto fa1r = canFilter->fa1r.volatileLoad();
    auto fs1r = canFilter->fs1r.volatileLoad();
    auto fm1r = canFilter->fm1r.volatileLoad();
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    auto [begin, end] = filterRange();
    for (size_t filterNumber = begin; filterNumber < end; filterNumber++) {
#else
    for (size_t filterNumber = 0; filterNumber < canFilter->filterRegister.size(); filterNumber++) {
#endif
        if (isFilterActive(fa1r, filterNumber) && getFilterMode(fs1r, fm1r, filterNumber) == FilterMode::List16bit) {
            FilterRegister &filter = canFilter->filterRegister[filterNumber];
            FilterRegister::IdentifierList16Bit list0 = filter.identifierList16bit[0].volatileLoad();
            FilterRegister::IdentifierList16Bit list1 = filter.identifierList16bit[1].volatileLoad();
            if (id == list0.id0 || id == list0.id1 || id == list1.id0 || id == list1.id1) {
                if (list0.id0 == list0.id1 && list1.id0 == list1.id1 && list1.id0 == list1.id0) {
                    // all filter settings are identical, we can disable this filter
                    activateFilterInitMode();
                    fa1r.deactivateFilter(filterNumber);
                    canFilter->fa1r.volatileStore(fa1r);
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
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE

void CANFilter::dumpFilterConfig() {
    auto fa1r = canFilter->fa1r.volatileLoad();
    auto fs1r = canFilter->fs1r.volatileLoad();
    auto fm1r = canFilter->fm1r.volatileLoad();
    auto log = diagChannel << lock << MICROHAL_DEBUG;
    for (size_t filterNumber = 0; filterNumber < canFilter->filterRegister.size(); filterNumber++) {
        auto filterMode = getFilterMode(fs1r, fm1r, filterNumber);
        FilterRegister &filter = canFilter->filterRegister[filterNumber];
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

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
