/*
 * i2c_stm32f4xx.cpp
 *
 *  Created on: 30 mar 2015
 *      Author: Pawel
 */

#include "i2c_stmCommon.h"
#include "../clockManager/i2cClock.h"
#include "interfaces/i2cSlave.h"

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

/**
 * @brief This function reset and initialize I2C peripheral. Notice: This function doesn't enable peripheral. After calling
 * this function you should call @ref enable function.
 *
 * @return true if peripheral was successfully initialized, false otherwise.
 */
bool I2C::init() {
    const uint32_t freqHz = ClockManager::I2CFrequency(getI2CNumber());  // in Hz
    const uint8_t freqMHz = freqHz / 1000000;                            // frequency in MHz

    if (freqMHz >= 2 && freqMHz <= 42) {
        // reset device
        registers::I2C::CR1 cr1;
        cr1 = 0;
        cr1.SWRST.set();
        i2c.cr1.volatileStore(cr1);
        cr1 = 0;
        i2c.cr1.volatileStore(cr1);
        // enable interrupts
        registers::I2C::CR2 cr2;
        cr2 = 0;
        cr2.ITEVTEN.set();
        cr2.ITERREN.set();
        cr2.FREQ = freqMHz;
        i2c.cr2.volatileStore(cr2);

        return speed(100000, Mode::Standard);
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
    if (isEnable() == true) return false;

    const uint32_t clockFreqHz = ClockManager::I2CFrequency(getI2CNumber());  // in Hz
    const uint8_t clockFreqMHz = clockFreqHz / 1000000;                       // frequency in MHz

    if (clockFreqMHz >= 2 && clockFreqMHz <= 42) {
        uint32_t ccrFlags = 0;
        // calculate clock period in ns
        const uint32_t Tpclk = 1000000000 / clockFreqHz;

        // calculate ccr register value
        uint32_t trTime;  //< transmission time
        registers::I2C::CCR ccr = {};
        if (fastMode == false) {
            // in this mode Thigh = Tlow
            // calculate Thigh in ns
            trTime = (1000000000 / 2) / speed;
        } else {
            if (duty == false) {
                // in this mode Thigh = Tlow / 2
                // calculate transmission time
                trTime = (1000000000 / 3) / speed;
                ccr.F_S.set();
            } else {
                // in this mode 1/9 Thigh = 1/16 Tlow
                trTime = (1000000000 / 25) / speed;
                ccr.F_S.set();
                ccr.DUTY.set();
            }
        }

        ccr.ccr = trTime / Tpclk;
        if (fastMode && duty) {
            if (ccr.ccr < 0x01) ccr.ccr = 0x01;
        } else {
            if (ccr.ccr < 0x04) ccr.ccr = 0x04;
        }

        // calculate rise time
        registers::I2C::TRISE trise = {};
        trise.trise = ((riseTime / Tpclk) + 1) < 31 ? (riseTime / Tpclk) + 1 : 31;
        // set I2C peripheral clock frequency
        auto cr2 = i2c.cr2.volatileLoad();
        cr2.FREQ = clockFreqMHz;
        i2c.cr2.volatileStore(cr2);
        i2c.ccr.volatileStore(ccr);
        // set max rise time
        i2c.trise.volatileStore(trise);
        return true;
    }
    return false;
}

I2C::Speed I2C::speed() noexcept {
    const uint32_t clockFreqHz = ClockManager::I2CFrequency(getI2CNumber());  // in Hz

    // get Thigh to Tlow
    uint16_t multiply = 2;  // in standard mode Tlow = Thigh => period = 2 * Thigh
    auto ccr = i2c.ccr.volatileLoad();
    if (ccr.F_S && ccr.DUTY) {
        multiply = 25;  // in HighSpeed mode Thigh = 9/16 Tlow => periond = 25 * Thigh
    } else if (ccr.F_S) {
        multiply = 3;  // in FastSpeed mode Thigh = Tlow / 2 => period = 3 * Thigh
    }
    return clockFreqHz / (multiply * ccr.ccr);
}

bool I2C::addSlave(I2CSlave &i2cSlave) {
    if (slave[0] == nullptr) {
        slave[0] = &i2cSlave;
        i2c.oar1.volatileStore(1 << 14 | i2cSlave.getAddress());
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
        i2c.oar1.volatileStore(1 << 14);
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
