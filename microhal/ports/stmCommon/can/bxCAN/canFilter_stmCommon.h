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

#ifndef _MICROHAL_PORTS_STMCOMMON_BXCAN_CANFILTER_H_
#define _MICROHAL_PORTS_STMCOMMON_BXCAN_CANFILTER_H_

#include <array>
#include <cstdint>
#include "can/canFilter.h"
#include "can/canMessage.h"
#include "ports/stmCommon/registers/canFilter_registers.h"
#include "ports/stmCommon/stmCommonDefines.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

#undef CAN1
#undef CAN2
#undef CAN

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

class CANFilter {
    enum class FilterMode { Mask16bit = 0, Mask32bit, List16bit, List32bit };

 public:
    using FilterRegister = microhal::registers::CANFilter::FilterRegister;
    using Message = can::Message;

    constexpr CANFilter(bool master) : canMaster(master) {}

    bool addFilter(const can::Filter &filter);
    bool removeFilter(const can::Filter &filter);
    void removeAllFilters();

    // Filter functions
    CANFilter &findInactiveFilter(FilterMode mode);

    bool addIdentifierMask32(FilterRegister::ID32 id, FilterRegister::ID32 mask);
    bool addIdentifierMask16(FilterRegister::ID16 id, FilterRegister::ID16 mask);
    bool addIdentifierList32(FilterRegister::ID32 id);
    bool addIdentifierList16(FilterRegister::ID16 id);
    bool removeIdentifierMask32(FilterRegister::ID32 id, FilterRegister::ID32 mask);
    bool removeIdentifierMask16(FilterRegister::ID16 id, FilterRegister::ID16 mask);
    bool removeIdentifierList32(FilterRegister::ID32 id);
    bool removeIdentifierList16(FilterRegister::ID16 id);
    FilterMode getFilterMode(uint_fast8_t filterNumber) const;
    static FilterMode getFilterMode(microhal::registers::CANFilter::FS1R fs1r, microhal::registers::CANFilter::FM1R fm1r, uint_fast8_t filterNumber);

    static void activateFilterInitMode();
    static void deactivateFilterInitMode();
    void activateFilter(uint_fast8_t filterNumber) {
        using namespace registers;
        auto fa1r = canFilter->fa1r.volatileLoad();
        fa1r |= 1 << filterNumber;
        canFilter->fa1r.volatileStore(fa1r);
    }
    void deactivateFilter(uint_fast8_t filterNumber) {
        using namespace registers;
        auto fa1r = canFilter->fa1r.volatileLoad();
        fa1r &= ~(1 << filterNumber);
        canFilter->fa1r.volatileStore(fa1r);
    }
    bool isFilterActive(uint_fast8_t filterNumber) {
        using namespace registers;
        auto fa1r = canFilter->fa1r.volatileLoad();
        return isFilterActive(fa1r, filterNumber);
    }
    static bool isFilterActive(microhal::registers::CANFilter::FA1R fa1r, uint_fast8_t filterNumber) { return fa1r.isFilterActive(filterNumber); }
    void setFilterMode(uint_fast8_t filterNumber, FilterMode filterMode);

#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    static void can2StartBank(uint_fast8_t startBank);
#endif

    void dumpFilterConfig();

 private:
#ifdef _MICROHAL_REGISTERS_STM_CAN_FMR_HAS_CAN2SB
    bool canMaster;  // true master false slave

    std::array<size_t, 2> filterRange() {
        using namespace registers;
        const auto can2sb = canFilter->fmr.volatileLoad().CAN2SB.get();
        if (canMaster) {
            return {0, can2sb};
        } else {
            return {can2sb, canFilter->filterRegister.size()};
        }
    }
#endif
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_BXCAN_CANFILTER_H_ */
