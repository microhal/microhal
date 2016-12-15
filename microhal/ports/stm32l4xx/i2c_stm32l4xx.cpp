/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      
 *
 * @authors    Gawor
 * created on: 17-09-2016
 * last modification: 17-09-2016
 *
 * @copyright Copyright (c) 2016, microHAL
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

#include "i2c_stm32l4xx.h"

namespace microhal {
namespace stm32l4xx {

/**
 * @brief This function reset and initialize I2C peripheral. Notice: This function doesn't enable peripheral. After calling
 * this function you should call @ref enable function.
 *
 * @return true if peripheral was successfully initialized, false otherwise.
 */
bool I2C::init(uint32_t timingr) {
	const uint32_t freqHz = Core::getI2Cclk(&i2c); // in Hz
	const uint8_t freqMHz = freqHz / 1000000; //frequency in MHz

	if(freqMHz >= 2 && freqMHz <= 42) {
		//reset device
		i2c.CR1 = I2C_CR1_SWRST;
		i2c.CR1 = 0;
		// enable interrupts
		i2c.CR1 = I2C_CR1_ERRIE | I2C_CR1_NACKIE/* | I2C_CR2_ITBUFEN*/;//| I2C_CR2_ITERREN | freqMHz;

		i2c.TIMINGR = timingr;//TODO-> constexpr function

		setMode(Mode::Standard); // TODO stm32l4 can work in 3 modes

		return true;
	}
	return false;


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
bool I2C::configure(uint32_t speed, uint32_t riseTime, bool fastMode, bool duty) {
	if(isEnable() == true) return false;

//	const uint32_t clockFreqHz = Core::getI2Cclk(&i2c); // in Hz
//	const uint8_t clockFreqMHz = clockFreqHz / 1000000; //frequency in MHz
//
//	if(clockFreqMHz >= 2 && clockFreqMHz <= 42) {
//		uint32_t ccrFlags = 0;
//		//calculate clock period in ns
//		const uint32_t Tpclk = 1000000000 / clockFreqHz;
//
//		//calculate ccr register value
//		uint32_t trTime; //< transmission time
//
//		if(fastMode == false){
//			//in this mode Thigh = Tlow
//			//calculate Thigh in ns
//			trTime = (1000000000 / 2) / speed;
//		} else {
//			if(duty == false){
//				//in this mode Thigh = Tlow / 2
//				//calculate transmission time
//				trTime = (1000000000 / 3) / speed;
//				ccrFlags = I2C_CCR_FS;
//			} else {
//				//in this mode 1/9 Thigh = 1/16 Tlow
//				trTime = (1000000000 / 25) / speed;
//				ccrFlags = I2C_CCR_FS | I2C_CCR_DUTY;
//			}
//		}
//
//		uint8_t ccr = trTime / Tpclk;
//		if(fastMode && duty){
//			if(ccr < 0x01) ccr = 0x01;
//		} else {
//			if(ccr < 0x04) ccr = 0x04;
//		}
//
//
//		//calculate rise time
//		uint8_t trise = (riseTime / Tpclk) + 1;
//		if(trise > 31) trise = 31;
//
//		//set I2C peripheral clock frequency
//		i2c.CR2 = (i2c.CR2 & ~I2C_CR2_FREQ) | clockFreqMHz;
//		i2c.CCR = ccr | ccrFlags;
//		//set max rise time
//		i2c.TRISE = trise;
//
//		return true;
//	}
	return false;
}

} // namespace stm32l4xx
} // namespace microhal



