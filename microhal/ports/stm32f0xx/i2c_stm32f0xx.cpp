/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 03-08-2018
 * last modification: 03-08-2018
 *
 * @copyright Copyright (c) 2018, Pawel Okas
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "i2c_stm32f0xx.h"
#include "clockManager.h"
#include "diagnostic/diagnostic.h"

using namespace microhal::diagnostic;

namespace microhal {
namespace stm32f0xx {

bool I2C::init() {
    // const uint32_t freqHz = ClockManager::I2CFrequency(i2c); // in Hz
    //	const uint8_t freqMHz = freqHz / 1000000; //frequency in MHz

    //	if(freqMHz >= 2 && freqMHz <= 42) {//
    // reset device
    i2c.CR1 = I2C_CR1_SWRST;
    i2c.CR1 = 0;
    // enable interrupts
    // i2c.CR1 = /*I2C_CR1_RXIE/* | I2C_CR2_ITBUFEN|*/ I2C_CR1_ERRIE;// | freqMHz;

    speed(100000, Mode::Standard);

    return true;
    //	}
    //	return false;
}
/**
 * @brief This function configure I2C peripheral. Peripheral configure is only possible when peripheral is disabled.
 *
 * @param speed - SCK speed in [Hz]
 * @param riseTime - maximal rise time in [ns]
 * @param fastMode - SCK duty control, if true then low level on SCK is twice longer than high level
 * @param duty - SCK duty control
 *
 * @retval true - if configuration was set
 * @retval false - if an error occurred, in example: peripheral was enabled
 */
bool I2C::configure(uint32_t speed, uint32_t /*riseTime*/, bool /*fastMode*/, bool /*duty*/) {
    if (isEnable() == true) {
        diagChannel << diagnostic::lock << MICROHAL_WARNING << "Unable to configure I2C, please first disable I2C and then retry."
                    << diagnostic::unlock;
        return false;
    }
    //
    const uint32_t clockFreqHz = ClockManager::I2CFrequency(i2c);  // in Hz

    diagChannel << diagnostic::lock << MICROHAL_NOTICE << "Setting I2C speed: " << speed << diagnostic::unlock;
    // 1/i2cFreq * 1000 000 000
    const uint32_t Ti2cclk = 1000'000'000 / clockFreqHz;  // in ns
    // calculate Thigh in ns
    // uint32_t Tpresc = (PRESC +1) * Ti2cclk;

    uint32_t Tscl = 1000000000 / speed;

    uint32_t Tsclh = Tscl / 2;
    uint32_t Tscll = Tscl - Tsclh;

    uint32_t scllDivider = Tscll / Ti2cclk;
    uint32_t sclhDivider = Tsclh / Ti2cclk;

    diagChannel << diagnostic::lock << MICROHAL_DEBUG << "SCL low prescaler: " << scllDivider << "SCL high prescaler: " << sclhDivider
                << diagnostic::unlock;

    i2c.TIMINGR |= 15 << I2C_TIMINGR_PRESC_Pos;
    // SCLL = Tscll / ((PRESC + 1) * Ti2cclk);

    // SCLL / Ti2cclk = Tscll / (PRESC + 1)

    // Tscll = (SCLL + 1)*Tpresc
    //	SCLL = (Tscll / Tpresc) - 1;
    // Tsclh = (SCLH + 1)*Tpresc
    //	SCLH = (Tsclh / Tpresc) - 1;
    return false;
}

I2C::Speed I2C::speed() noexcept {
    const uint32_t clockFreqHz = ClockManager::I2CFrequency(i2c);  // in Hz
    const uint32_t timingr = i2c.TIMINGR;
    const uint32_t SCLL = (timingr & I2C_TIMINGR_SCLL_Msk) >> I2C_TIMINGR_SCLL_Pos;
    const uint32_t SCLH = (timingr & I2C_TIMINGR_SCLH_Msk) >> I2C_TIMINGR_SCLH_Pos;
    const uint32_t PRESC = (timingr & I2C_TIMINGR_PRESC_Msk) >> I2C_TIMINGR_PRESC_Pos;
    return clockFreqHz / ((SCLL + SCLH + 2) * (PRESC + 1));
}
}  // namespace stm32f0xx
}  // namespace microhal
