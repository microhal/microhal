/*
 * i2c_stm32f4xx.cpp
 *
 *  Created on: 30 mar 2015
 *      Author: Pawel
 */

#include "i2c_stmCommon.h"
#include "diagnostic/diagnostic.h"

#include _MICROHAL_INCLUDE_PORT_clockManager

using namespace microhal::diagnostic;

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

/**
 * @brief This function reset and initialize I2C peripheral. Notice: This function doesn't enable peripheral. After calling
 * this function you should call @ref enable function.
 *
 * @return true if peripheral was successfully initialized, false otherwise.
 */
bool I2C::init() {
    // const uint32_t freqHz = ClockManager::I2CFrequency(i2c); // in Hz
    //	const uint8_t freqMHz = freqHz / 1000000; //frequency in MHz

    //	if(freqMHz >= 2 && freqMHz <= 42) {//
    // reset device
    registers::I2C::CR1 cr1 = {};
    cr1.SWRST.set();
    i2c.cr1.volatileStore(cr1);
    cr1 = 0;
    i2c.cr1.volatileStore(cr1);
    // i2c.CR1 = I2C_CR1_SWRST;
    // i2c.CR1 = 0;
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
    const uint32_t clockFreqHz = ClockManager::I2CFrequency(getNumber());  // in Hz
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

    auto timingr = i2c.timingr.volatileLoad();
    timingr.PRESC = 15;
    i2c.timingr.volatileStore(timingr);

    // SCLL = Tscll / ((PRESC + 1) * Ti2cclk);

    // SCLL / Ti2cclk = Tscll / (PRESC + 1)

    // Tscll = (SCLL + 1)*Tpresc
    //	SCLL = (Tscll / Tpresc) - 1;
    // Tsclh = (SCLH + 1)*Tpresc
    //	SCLH = (Tsclh / Tpresc) - 1;
    return false;
}

I2C::Speed I2C::speed() noexcept {
    const uint32_t clockFreqHz = ClockManager::I2CFrequency(getNumber());  // in Hz
    auto timingr = i2c.timingr.volatileLoad();
    const uint32_t SCLL = timingr.SCLL;
    const uint32_t SCLH = timingr.SCLH;
    const uint32_t PRESC = timingr.PRESC;
    return clockFreqHz / ((SCLL + SCLH + 2) * (PRESC + 1));
}

bool I2C::addSlave(I2CSlave &i2cSlave) {
    if (slave[0] == nullptr) {
        slave[0] = &i2cSlave;
        auto cr1 = i2c.cr1.volatileLoad();
        cr1.ADDRIE.set();
        cr1.SBC.set();
        i2c.cr1.volatileStore(cr1);

        registers::I2C::OAR1 oar1;
        oar1.OA1EN.set();
        oar1.OA1 = i2cSlave.getAddress();
        i2c.oar1.volatileStore(oar1);
        return true;
    } else if (slave[1] == nullptr) {
        slave[1] = &i2cSlave;
        return true;
    } else {
        return false;
    }
}

bool I2C::removeSlave(I2CSlave &i2cSlave) {
    if (slave[0] == &i2cSlave) {
        i2c.oar1.volatileStore(0);
        // i2c.OAR1 = 0;
        slave[0] = nullptr;
        return true;
    } else if (slave[1] == &i2cSlave) {
        slave[1] = nullptr;
        return true;
    } else {
        return false;
    }
}

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
