/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 14-06-2021
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

#include "adf4360.h"
#include <cassert>
#include "utils/byteswap.h"
#include "utils/reverseBits.hpp"

namespace microhal {
namespace deviceDriver {

ADF4360::ADF4360(SPI &spi, GPIO &le, GPIO &ce, GPIO &muxout) : spi(spi), ce(ce), le(le), muxout(muxout) {
    ce.configureAsOutput(GPIO::OutputType::PushPull, GPIO::PullType::NoPull);
    ce.reset();

    le.set();
    le.configureAsOutput(GPIO::OutputType::PushPull, GPIO::PullType::NoPull);

    muxout.configureAsInput(GPIO::PullType::PullUp);
}

ADF4360::Error ADF4360::rCounterLatch(uint16_t rCounter, BandSelectClockDivider bandSelectClock, LockDetectPrecision ldp,
                                      AntibacklashPulse antibacklash) {
    assert(rCounter > 0);
    assert(rCounter <= 16383);

    const uint32_t rCounterLatch = static_cast<EnumUnderlyingType>(bandSelectClock) | static_cast<EnumUnderlyingType>(ldp) |
                                   static_cast<EnumUnderlyingType>(antibacklash) | (rCounter << 2) | 0b01U;
    return write(rCounterLatch);
}

ADF4360::Error ADF4360::nCounterLatch(uint16_t bCounter, uint_fast8_t aCounter, ChargePumpSelect cpgain, PrescalerInputDivider prescalerInputDivider,
                                      OutputDivider outputDivider) {
    assert(bCounter >= 3 && bCounter <= 8191);
    assert(aCounter <= 31);

    // cpgain enum class is shifted according to Control latch register layout. We need shift cpgain by additional 11 to match N counter latch
    // layout.
    const uint32_t nCounterLatch = static_cast<EnumUnderlyingType>(prescalerInputDivider) | static_cast<EnumUnderlyingType>(outputDivider) |
                                   (static_cast<EnumUnderlyingType>(cpgain) << 11U) | (bCounter << 8) | (aCounter << 2) | 0b10U;
    return write(nCounterLatch);
}

ADF4360::Error ADF4360::controlLatch(CorePowerLevel corePowerLevel, CounterOperation counterOperation, MuxoutMode muxMode,
                                     PhaseDetectorPolarity phaseDetectorPolarity, ChargePumpOutput chargePumpOutput,
                                     ChargePumpSelect chargePumpSelect, MuteTillLockDetect muteTillLockDetect, OuptutPowerLevel ouptutPowerLevel,
                                     ChargePumpCurrent currentSetting1, ChargePumpCurrent currentSetting2, PowerDownMode powerDownMode,
                                     Prescaler prescaler) {
    const uint32_t controlLatch = static_cast<EnumUnderlyingType>(prescaler) | static_cast<EnumUnderlyingType>(powerDownMode) |
                                  (static_cast<EnumUnderlyingType>(currentSetting2) << 3) | static_cast<EnumUnderlyingType>(currentSetting1) |
                                  static_cast<EnumUnderlyingType>(ouptutPowerLevel) | static_cast<EnumUnderlyingType>(muteTillLockDetect) |
                                  static_cast<EnumUnderlyingType>(chargePumpSelect) | static_cast<EnumUnderlyingType>(chargePumpOutput) |
                                  static_cast<EnumUnderlyingType>(phaseDetectorPolarity) | static_cast<EnumUnderlyingType>(muxMode) |
                                  static_cast<EnumUnderlyingType>(counterOperation) | static_cast<EnumUnderlyingType>(corePowerLevel) | 0b00;

    return write(controlLatch);
}

ADF4360::Error ADF4360::write(uint32_t data) {
    const auto reversed = byteswap(data);
    le.reset();
    const auto result = spi.write(reinterpret_cast<const uint8_t *>(&reversed) + 1, 3, true);
    le.set();
    return result;
}

}  // namespace deviceDriver
}  // namespace microhal
