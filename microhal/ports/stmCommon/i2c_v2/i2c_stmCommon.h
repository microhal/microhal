/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      I2C implementation for STM32F3xx microcontroler
 *
 * @authors    Pawel Okas
 * created on: 20-02-2017
 * last modification: 24-02-2017
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

#ifndef _MICROHAL_I2C_STMCOMMON_H_
#define _MICROHAL_I2C_STMCOMMON_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "../registers/i2cRegisters_v2.h"
#include "../stmCommonDefines.h"
#include "interfaces/i2cSlave.h"
#include "interfaces/i2c_interface.h"

#include _MICROHAL_INCLUDE_PORT_CONFIG

namespace microhal {
class I2CDevice;
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
extern "C" {
void I2C1_ER_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);
void I2C2_EV_IRQHandler(void);
void I2C3_ER_IRQHandler(void);
void I2C3_EV_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
/**
 * @brief   This class implements I2C functions.
 */
class I2C : public microhal::I2C {
 public:
//---------------------------------- variables
//----------------------------------
#if (defined MICROHAL_USE_I2C1_INTERRUPT) || (defined MICROHAL_USE_I2C1_POLLING) || (defined MICROHAL_USE_I2C1_DMA)
    static I2C &i2c1;
#endif
#if (defined MICROHAL_USE_I2C2_INTERRUPT) || (defined MICROHAL_USE_I2C2_POLLING) || (defined MICROHAL_USE_I2C2_DMA)
    static I2C &i2c2;
#endif
#if (defined MICROHAL_USE_I2C3_INTERRUPT) || (defined MICROHAL_USE_I2C3_POLLING) || (defined MICROHAL_USE_I2C3_DMA)
    static I2C &i2c3;
#endif
    //---------------------------------- functions
    //----------------------------------
    bool init();

    bool speed(Speed speed, Mode mode) noexcept final {
        bool fastMode = false;
        bool dutyCycle = false;

        if (mode != Mode::Standard) {
            fastMode = true;
            dutyCycle = true;
        }

        return configure(speed, getMaxRiseTime(mode), fastMode, dutyCycle);
    }
    Speed speed() noexcept final;
    void busReset() noexcept final {
        auto cr2 = i2c.cr2.volatileLoad();
        cr2.STOP.set();
        i2c.cr2.volatileStore(cr2);
    }
    /**
     * @brief This function enable analog filter. Changing filter state is only
     * possible when I2C peripheral is inactive.
     * To disable peripheral you can call @ref disable function.
     *
     * @retval true if analog filter was on
     * @retval false if I2C peripheral is enabled and changing filter state is
     * impossible
     */
    bool analogFilterEnable() {
        if (isEnable() == false) {
            auto cr1 = i2c.cr1.volatileLoad();
            cr1.ANFOFF.clear();
            i2c.cr1.volatileStore(cr1);
            return true;
        }
        return false;
    }
    /**
     * @brief This function disable analog filter. Changing filter state is only
     * possible when I2C peripheral is inactive.
     * To disable peripheral you can call @ref disable function.
     *
     * @retval true if analog filter was off
     * @retval false if I2C peripheral is enabled and changing filter state is
     * impossible
     */
    bool analogFilterDisable() {
        if (isEnable() == false) {
            auto cr1 = i2c.cr1.volatileLoad();
            cr1.ANFOFF.set();
            i2c.cr1.volatileStore(cr1);
            return true;
        }
        return false;
    }

    bool digitalFilterEnable(uint8_t spikeLengthInMultipleTpclk) {
        if (spikeLengthInMultipleTpclk <= 15 && isEnable() == false) {
            auto cr1 = i2c.cr1.volatileLoad();
            cr1.DNF = spikeLengthInMultipleTpclk;
            i2c.cr1.volatileStore(cr1);
        }
        return false;
    }

    bool digitalFilterDisable() {
        if (isEnable() == false) {
            auto cr1 = i2c.cr1.volatileLoad();
            cr1.DNF.clear();
            i2c.cr1.volatileStore(cr1);
            return true;
        }
        return false;
    }
    /**
     * @brief This function checks if I2C peripheral is active.
     *
     * @return true if peripheral is enabled, false otherwise.
     */
    bool isEnable() { return i2c.cr1.volatileLoad().PE; }

    /**
     * @brief This function enable I2C peripheral.
     */
    void enable() {
        auto cr1 = i2c.cr1.volatileLoad();
        cr1.PE.set();
        i2c.cr1.volatileStore(cr1);
    }
    /**
     * @brief This function disable I2C peripheral.
     */
    void disable() {
        auto cr1 = i2c.cr1.volatileLoad();
        cr1.PE.clear();
        i2c.cr1.volatileStore(cr1);
    }

    void stop() {
        auto cr2 = i2c.cr2.volatileLoad();
        cr2.STOP.set();
        i2c.cr2.volatileStore(cr2);
    }

    void softwareReset() {
        auto cr1 = i2c.cr1.volatileLoad();
        cr1.PE.clear();
        i2c.cr1.volatileStore(cr1);
        while (i2c.cr1.volatileLoad().PE != 0) {
        }
    }

    bool configure(uint32_t speed, uint32_t riseTime, bool fastMode, bool duty);

    bool addSlave(I2CSlave &i2cSlave);

    bool removeSlave(I2CSlave &i2cSlave);

 protected:
    registers::I2C &i2c;
    I2CSlave *slave[2] = {nullptr, nullptr};
    I2CSlave *activeSlave = nullptr;
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        explicit I2C(registers::I2C &i2c)
        : i2c(i2c) {
    }

    uint16_t getNumber() {
#if defined(_MICROHAL_I2C1_BASE_ADDRESS)
        if ((int)&i2c == _MICROHAL_I2C1_BASE_ADDRESS) {
            return 1;
        }
#endif
#if defined(_MICROHAL_I2C2_BASE_ADDRESS)
        if ((int)&i2c == _MICROHAL_I2C2_BASE_ADDRESS) {
            return 2;
        }
#endif
#if defined(_MICROHAL_I2C3_BASE_ADDRESS)
        if ((int)&i2c == _MICROHAL_I2C3_BASE_ADDRESS) {
            return 3;
        }
#endif
        std::terminate();
    }

    I2C(I2C &i2c) = delete;
    I2C &operator=(const I2C &) = delete;

    void start() {
        auto cr2 = i2c.cr2.volatileLoad();
        cr2.START.set();
        i2c.cr2.volatileStore(cr2);
    }

    void setActiveSlave(uint8_t address) {
        for (int i = 0; i < 2; i++) {
            if (slave[i]->getAddress() == address) {
                activeSlave = slave[i];
                return;
            }
        }
        std::terminate();  // critical error
    }

 public:
    static I2C::Error errorCheckAndClear(registers::I2C *const i2c, registers::I2C::ISR isr) {
        auto errors = I2C::Error::None;

        registers::I2C::ICR icr = {};
        if (isr.TIMEOUT) {
            errors |= I2C::Error::Timeout;
            icr.TIMOUTCF.set();
        }
        if (isr.PECERR) {
            icr.PECCF.set();
        }
        if (isr.OVR) {
            errors |= I2C::Error::Overrun;
            icr.OVRCF.set();
        }
        if (isr.ARLO) {
            errors |= I2C::Error::ArbitrationLost;
            icr.ARLOCF.set();
        }
        if (isr.BERR) {
            errors |= I2C::Error::Bus;
            icr.BERRCF.set();
        }
        i2c->icr.volatileStore(icr);

        return errors;
    }
    //----------------------------------------- friends -----------------------------------------//
    friend microhal::I2CDevice;
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
#endif  // _MICROHAL_I2C_STMCOMMON_H_
