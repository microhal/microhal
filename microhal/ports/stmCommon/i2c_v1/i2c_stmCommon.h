/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      I2C implementation for STM32F4xx microcontroler
 *
 * @authors    Pawel Okas
 * created on: 2014
 * last modification: 27-06-2016
 *
 * @copyright Copyright (c) 2014-2016, Pawel Okas
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
#include <utility>

#include <ports/stmCommon/registers/i2cRegisters_v1.h>
#include "../stmCommonDefines.h"
#include "interfaces/i2c_interface.h"

#include _MICROHAL_INCLUDE_PORT_clockManager
#include _MICROHAL_INCLUDE_PORT_CONFIG

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

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
 * @addtogroup stm32f4xx_port
 * @{
 * @class I2C
 * @}
 * \brief   This class implements I2C functions.
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
    //---------------------------------- functions ----------------------------------
    bool init();
    /**
     * @brief This function change I2C peripheral speed and mode. Changing mode is only
     * possible when I2C peripheral is disabled.
     *
     * @retval true if mode was set.
     * @retval false if I2C is enabled and mode can't be changed.
     */
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
        auto cr1 = i2c.cr1.volatileLoad();
        cr1.STOP = 1;
        i2c.cr1.volatileStore(cr1);
    }
#ifdef _MICROHAL_REGISTERS_I2C_HAS_FLTR
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
            auto fltr = i2c.fltr.volatileLoad();
            fltr.ANOFF = 0;
            i2c.fltr.volatileStore(fltr);
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
            auto fltr = i2c.fltr.volatileLoad();
            fltr.ANOFF = 1;
            i2c.fltr.volatileStore(fltr);
            return true;
        }
        return false;
    }

    bool digitalFilterEnable(uint8_t spikeLengthInMultipleTpclk) {
        if (spikeLengthInMultipleTpclk <= 15 && isEnable() == false) {
            auto fltr = i2c.fltr.volatileLoad();
            fltr.DNF = spikeLengthInMultipleTpclk;
            i2c.fltr.volatileStore(fltr);
            return true;
        }
        return false;
    }

    bool digitalFilterDisable() {
        if (isEnable() == false) {
            auto fltr = i2c.fltr.volatileLoad();
            fltr.DNF.clear();
            i2c.fltr.volatileStore(fltr);
            return true;
        }
        return false;
    }
#endif
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
        auto cr1 = i2c.cr1.volatileLoad();
        cr1.STOP.set();
        i2c.cr1.volatileStore(cr1);
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
        explicit I2C(registers::I2C *i2c)
        : i2c(*i2c) {
    }
    I2C(const microhal::_MICROHAL_ACTIVE_PORT_NAMESPACE::I2C &) = delete;
    void operator=(const microhal::_MICROHAL_ACTIVE_PORT_NAMESPACE::I2C &) = delete;

    void start() {
        auto cr1 = i2c.cr1.volatileLoad();
        cr1.START.set();
        i2c.cr1.volatileStore(cr1);
    }

    void enableErrorInterrupt(uint32_t priority) {
        NVIC_SetPriority(errorIrq(), priority);
        NVIC_ClearPendingIRQ(eventIrq());
        NVIC_EnableIRQ(errorIrq());
    }

    void enableEventInterrupt(uint32_t priority) {
        NVIC_SetPriority(eventIrq(), priority);
        NVIC_ClearPendingIRQ(eventIrq());
        NVIC_EnableIRQ(eventIrq());
    }

    uint16_t getI2CNumber() {
        if ((int)&i2c == _MICROHAL_I2C1_BASE_ADDRESS)
            return 1;
        else if ((int)&i2c == _MICROHAL_I2C2_BASE_ADDRESS)
            return 2;
#if defined(_MICROHAL_I2C3_BASE_ADDRESS)
        else if ((int)&i2c == _MICROHAL_I2C3_BASE_ADDRESS)
            return 3;
#endif
        std::terminate();
    }

    IRQn_Type errorIrq() {
        if ((int)&i2c == _MICROHAL_I2C1_BASE_ADDRESS)
            return I2C1_ER_IRQn;
        else if ((int)&i2c == _MICROHAL_I2C2_BASE_ADDRESS)
            return I2C2_ER_IRQn;
#if defined(_MICROHAL_I2C3_BASE_ADDRESS)
        else if ((int)&i2c == _MICROHAL_I2C3_BASE_ADDRESS)
            return I2C3_ER_IRQn;
#endif
        std::terminate();
    }

    IRQn_Type eventIrq() {
        if ((int)&i2c == _MICROHAL_I2C1_BASE_ADDRESS)
            return I2C1_EV_IRQn;
        else if ((int)&i2c == _MICROHAL_I2C2_BASE_ADDRESS)
            return I2C2_EV_IRQn;
#if defined(_MICROHAL_I2C3_BASE_ADDRESS)
        else if ((int)&i2c == _MICROHAL_I2C3_BASE_ADDRESS)
            return I2C3_EV_IRQn;
#endif
        std::terminate();
    }

 public:
    static I2C::Error errorCheckAndClear(registers::I2C *i2c, registers::I2C::SR1 sr) {
        auto errors = I2C::Error::None;
        auto sr1 = i2c->sr1.volatileLoad();
        if (sr.TIMEOUT) {
            errors |= I2C::Error::Timeout;
            sr1.TIMEOUT.clear();
        }
        if (sr.PECERR) {
            sr1.PECERR.clear();
        }
        if (sr.OVR) {
            errors |= I2C::Error::Overrun;
            sr1.OVR.clear();
        }
        if (sr.AF) {
            errors |= I2C::Error::AcknowledgeFailure;
            sr1.AF.clear();
        }
        if (sr.ARLO) {
            errors |= I2C::Error::ArbitrationLost;
            sr1.ARLO.clear();
        }
        if (sr.BERR) {
            errors |= I2C::Error::Bus;
            sr1.BERR.clear();
        }
        i2c->sr1.volatileStore(sr1);
        return static_cast<I2C::Error>(errors);
    }
    //----------------------------------------- friends -----------------------------------------//
    friend microhal::I2CDevice;
};

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
#endif  // _MICROHAL_I2C_STMCOMMON_H_
