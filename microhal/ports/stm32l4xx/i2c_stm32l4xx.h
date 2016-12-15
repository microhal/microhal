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

#ifndef _MICROHAL_I2C_STM32L4XX_H_
#define _MICROHAL_I2C_STM32L4XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>

#include "core_stm32l4xx.h"
#include "interfaces/i2c_interface.h"
#include "microhalPortConfig_stm32l4xx.h"
#include "device/stm32l4xx.h"

namespace microhal {
class I2CDevice;
namespace stm32l4xx {
extern "C" {
void I2C1_ER_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);
void I2C2_EV_IRQHandler(void);
void I2C3_ER_IRQHandler(void);
void I2C3_EV_IRQHandler(void);
}
/* **************************************************************************************************************************************************
 * CLASS
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
    bool init(uint32_t timingr);

    /**
     * @brief This function change I2C peripheral mode. Changing mode is only
     * possible when I2C peripheral is disabled.
     *
     * @retval true if mode was set.
     * @retval false if I2C is enabled and mode can't be changed.
     */
    bool setMode(Mode mode) final {
        bool fastMode = false;
        bool dutyCycle = false;

        if (mode != Mode::Standard) {
            fastMode = true;
            dutyCycle = true;
        }

        return configure(getSCLfreq(mode), getMaxRiseTime(mode), fastMode, dutyCycle);
    }
    /**
     * @brief This function enable analog filter. Changing filter state is only
     * possible when I2C peripheral is inactive.
     * To disable peripheral you can call @ref disable function.
     *
     * @retval true if analog filter was on
     * @retval false if I2C peripheral is enabled and changing filter state is
     * unpossible
     */
    bool analogFilterEnable() {
        if (isEnable() == false) {
            i2c.CR1 &= ~I2C_CR1_ANFOFF;
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
     * unpossible
     */
    bool analogFilterDisable() {
        if (isEnable() == false) {
            i2c.CR1 |= I2C_CR1_ANFOFF;
            return true;
        }
        return false;
    }

    bool digitalFilterEnable(uint8_t spikeLengthInMultipleTpclk) {
        if (spikeLengthInMultipleTpclk <= 15 && isEnable() == false) {
            i2c.CR1 |= (spikeLengthInMultipleTpclk << I2C_CR1_DNF); //TODO bitbang
            return true;
        }
        return false;
    }

    bool digitalFilterDisable() {
        if (isEnable() == false) {
            i2c.CR1 &= ~ (15 << I2C_CR1_DNF); //TODO bitbang
            return true;
        }
        return false;
    }
    /**
     * @brief This function checks if I2C peripheral is active.
     *
     * @return true if peripheral is enabled, false otherwise.
     */
    bool isEnable() { return i2c.CR1 & I2C_CR1_PE; }

    /**
     * @brief This function enable I2C peripheral.
     */
    void enable() { i2c.CR1 |= I2C_CR1_PE; }
    /**
     * @brief This function disable I2C peripheral.
     */
    void disable() { i2c.CR1 &= ~I2C_CR1_PE; }

    bool configure(uint32_t speed, uint32_t riseTime, bool fastMode, bool duty);

 protected:
    I2C_TypeDef &i2c;
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        explicit I2C(I2C_TypeDef &i2c)
        : i2c(i2c) {
    }
    //    virtual ~I2C() {
    //    }

    static I2C::Error errorCheckAndClear(I2C_TypeDef *i2c, uint16_t isr) {
        uint32_t errors = I2C::NoError;

        if (isr & I2C_ISR_TIMEOUT) {
            errors |= I2C::Timeout;
            i2c->ICR |= I2C_ICR_TIMOUTCF;// TODO is it working properly?
//            disable();
        }
        if (isr & I2C_ISR_PECERR) {
            i2c->ICR |= I2C_ICR_PECCF;
//            disable();
        }
        if (isr & I2C_ISR_OVR) {
            errors |= I2C::OverrunError;
            i2c->ICR |= I2C_ICR_OVRCF;
 //           disable();
        }
        if (isr & I2C_ISR_NACKF) {
            errors |= I2C::AcknowledgeFailure;
            i2c->ISR |= I2C_ICR_NACKCF;
 //           disable();
        }
        if (isr & I2C_ISR_ARLO) {
            errors |= I2C::ArbitrationLost;
            i2c->ICR |= I2C_ICR_ARLOCF;
//            disable();
        }
        if (isr & I2C_ISR_BERR) {
            errors |= I2C::BusError;
            i2c->ICR |= I2C_ICR_BERRCF;
//           disable();
        }

        return static_cast<I2C::Error>(errors);
    }
    //----------------------------------------- friends
    //-----------------------------------------//
    friend microhal::I2CDevice;
};

}  // namespace stm32l4xx
}  // namespace microhal


#endif  // _MICROHAL_I2C_STM32L4XX_H_
