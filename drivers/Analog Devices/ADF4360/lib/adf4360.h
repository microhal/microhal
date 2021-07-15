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

#ifndef _MICROHAL_DEVICE_ADF4360_H_
#define _MICROHAL_DEVICE_ADF4360_H_

#include <cstdint>
#include "spi.h"

namespace microhal {
namespace deviceDriver {

class ADF4360 {
    using EnumUnderlyingType = uint32_t;

 public:
    using Error = SPI::Error;

    enum class CorePowerLevel : EnumUnderlyingType { Power_5mA = 0b00 << 2, Power_10mA = 0b01 << 2, Power_15mA = 0b10 << 2, Power_20mA = 0b11 << 2 };
    enum class CounterOperation : EnumUnderlyingType { Normal = 0 << 4, Reset = 1 << 4 };
    enum class MuxoutMode : EnumUnderlyingType {
        ThreeStateOuptput = 0b000 << 5,
        DigitalLockDetect = 0b001 << 5,  //! Active high
        NDividerOutput = 0b010 << 5,
        DVdd = 0b011 << 5,
        RdividerOutput = 0b100 << 5,
        NChannelOpenDrainLockDetect = 0b101 << 5,
        SerialDataOutput = 0b110 << 5,
        Dgnd = 0b111 << 5
    };
    enum class PhaseDetectorPolarity : EnumUnderlyingType { Negative = 0U << 8, Positive = 1U << 8 };
    enum class ChargePumpOutput : EnumUnderlyingType { Normal = 0, ThreeState = 1U << 9 };
    enum class ChargePumpSelect : EnumUnderlyingType { CurrentSetting1 = 0, CurrentSetting2 = 1U << 10 };
    enum class MuteTillLockDetect : EnumUnderlyingType { Enabled = 0, Disabled = 1U << 11 };
    enum class OuptutPowerLevel : EnumUnderlyingType {
        Minus14dBm = 0b00 << 12,
        Minus11dBm = 0b01 << 12,
        Minus8dBm = 0b10 << 12,
        Minus5dBm = 0b11 << 12
    };
    enum class ChargePumpCurrent : EnumUnderlyingType {
        Current_0_31mA = 0b000 << 14,
        Current_0_62mA = 0b001 << 14,
        Current_0_93mA = 0b010 << 14,
        Current_1_25mA = 0b011 << 14,
        Current_1_56mA = 0b100 << 14,
        Current_1_87mA = 0b101 << 14,
        Current_2_18mA = 0b110 << 14,
        Current_2_5mA = 0b111 << 14
    };

    enum class PowerDownMode : EnumUnderlyingType {
        NormalOperation = 0b00 << 20,
        AsynchronousPowerDown = 0b01 << 20,
        SynchronousPowerDown = 0b11 << 20
    };

    enum class Prescaler : EnumUnderlyingType { Prescaler_8_9 = 0b00 << 22, Prescaler_16_17 = 0b01 << 22, Prescaler_32_33 = 0b10 << 22 };
    enum class AntibacklashPulse : EnumUnderlyingType {
        Width_3ns = 0b00 << 16U,
        Width_1_3ns = 0b01 << 16U,  //!< 1.3ns
        Width_6ns = 0b10 << 16U
    };
    enum class LockDetectPrecision : EnumUnderlyingType {
        ThreeConsecutiveCycles = 0,        //!< Three consecutive cycles of phase delay less than 15ns must occur before lock detect is set.
        FiveConsecutiveCycles = 1U << 18U  //!< Five consecutive cycles of phase delay less than 15ns must occur before lock detect is set.
    };
    /**
     * BandSelectClockDivider is used to set band select clock divider.
     * Band select clock should be less than 1MHz.
     * Chose Band select clock divider to satisfy equation:
     * frefin / Rcounter / BandSelectClockDivider < 1MHz
     */
    enum class BandSelectClockDivider : EnumUnderlyingType {
        DivideBy1 = 0b00 << 20U,
        DivideBy2 = 0b01 << 20U,
        DivideBy4 = 0b10 << 20U,
        DivideBy8 = 0b11 << 20U,
    };

    enum class PrescalerInputDivider : EnumUnderlyingType {
        Disable = 0,       //! Disable N prescaler divider: fnprescaler = fvco
        Enable = 1 << 23U  //! Enable N prescaler division by 2: fnpresaaler = fvco/2
    };
    enum class OutputDivider : EnumUnderlyingType {
        Disable = 0,       //! Disable output divider: fout = fvco
        Enable = 1 << 22U  //! Enable output division by 2: fout = fvco/2
    };

    static constexpr const uint32_t prescalerMaxOutputFrequency_Hz = 300'000'000;  // 300MHz

    ADF4360(SPI &spi, GPIO &le, GPIO &ce, GPIO &muxout);

    void enable() { ce.set(); }
    void disable() { ce.reset(); }

    static constexpr uint32_t vcoFreq(Prescaler pPrescaler, uint32_t bPrescaler, uint32_t aPrescaler, uint32_t frefin, uint32_t rprescaler) {
        const uint64_t prescaler = value(pPrescaler);
        const uint64_t N = (prescaler * bPrescaler) + aPrescaler;
        const uint32_t fvco = N * frefin / rprescaler;
        return fvco;
    }

    /**
     *
     * @param rCounter input clock prescaler, values form 1 to 16383
     * @param bandSelectClock
     * @param ldp
     * @param antibacklash
     * @return
     */
    Error rCounterLatch(uint16_t rCounter, BandSelectClockDivider bandSelectClock, LockDetectPrecision ldp, AntibacklashPulse antibacklash);

    Error nCounterLatch(uint16_t bCounter, uint_fast8_t aCounter, ChargePumpSelect cpgain, PrescalerInputDivider prescalerInputDivider,
                        OutputDivider outputDivider);

    Error controlLatch(CorePowerLevel corePowerLevel, CounterOperation counterOperation, MuxoutMode muxMode,
                       PhaseDetectorPolarity phaseDetectorPolarity, ChargePumpOutput chargePumpOutput, ChargePumpSelect chargePumpSelect,
                       MuteTillLockDetect muteTillLockDetect, OuptutPowerLevel ouptutPowerLevel, ChargePumpCurrent currentSetting1,
                       ChargePumpCurrent currentSetting2, PowerDownMode powerDownMode, Prescaler prescaler);

 private:
    SPI &spi;
    GPIO &ce;
    GPIO &le;
    GPIO &muxout;

    Error write(uint32_t data);

    static constexpr uint32_t value(Prescaler pPrescaler) {
        switch (pPrescaler) {
            case Prescaler::Prescaler_8_9:
                return 8;
            case Prescaler::Prescaler_16_17:
                return 16;
            case Prescaler::Prescaler_32_33:
                return 32;
        }
    }
};

}  // namespace deviceDriver
}  // namespace microhal

#endif /* _MICROHAL_DEVICE_ADF4360_H_ */
