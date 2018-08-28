/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 02-08-2018
 * last modification: 02-08-2018
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

#ifndef _MICROHAL_I2C_STM32F0XX_H_
#define _MICROHAL_I2C_STM32F0XX_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "core_stm32f0xx.h"
#include "device/stm32f0xx.h"
#include "interfaces/i2cSlave.h"
#include "interfaces/i2c_interface.h"
#include "microhalPortConfig_stm32f0xx.h"

namespace microhal {
namespace stm32f0xx {
/* **************************************************************************************************************************************************
 * CLASS
 */
class I2C : public microhal::I2C {
 public:
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
    void busReset() noexcept final { i2c.CR2 |= I2C_CR2_STOP; }
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
     * impossible
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
            i2c.CR1 = (i2c.CR1 & ~I2C_CR1_DNF_Msk) | (I2C_CR1_DNF_Msk & (spikeLengthInMultipleTpclk << I2C_CR1_DNF_Pos));
            return true;
        }
        return false;
    }

    bool digitalFilterDisable() {
        if (isEnable() == false) {
            i2c.CR1 &= ~I2C_CR1_DNF_Msk;
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

    void stop() { i2c.CR2 |= I2C_CR2_STOP; }

    bool configure(uint32_t speed, uint32_t riseTime, bool fastMode, bool duty);

    bool addSlave(I2CSlave &i2cSlave) {
        if (slave[0] == nullptr) {
            slave[0] = &i2cSlave;
            i2c.CR1 |= I2C_CR1_ADDRIE | I2C_CR1_SBC;
            i2c.OAR1 = I2C_OAR1_OA1EN | i2cSlave.getAddress();
            return true;
        } else if (slave[1] == nullptr) {
            slave[1] = &i2cSlave;
            return true;
        } else {
            return false;
        }
    }

    bool removeSlave(I2CSlave &i2cSlave) {
        if (slave[0] == &i2cSlave) {
            i2c.OAR1 = 0;
            slave[0] = nullptr;
            return true;
        } else if (slave[1] == &i2cSlave) {
            slave[1] = nullptr;
            return true;
        } else {
            return false;
        }
    }

 protected:
    I2C_TypeDef &i2c;
    I2CSlave *slave[2] = {nullptr, nullptr};
    I2CSlave *activeSlave = nullptr;
#if defined(__MICROHAL_MUTEX_CONSTEXPR_CTOR)
    constexpr
#endif
        explicit I2C(I2C_TypeDef &i2c)
        : i2c(i2c) {
    }

    void start() { i2c.CR2 |= I2C_CR2_START; }

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
    static I2C::Error errorCheckAndClear(I2C_TypeDef *i2c, uint16_t isr) {
        auto errors = I2C::Error::None;

        if (isr & I2C_ISR_TIMEOUT) {
            errors |= I2C::Error::Timeout;
            i2c->ICR |= I2C_ICR_TIMOUTCF;
        }
        if (isr & I2C_ISR_PECERR) {
            i2c->ICR |= I2C_ICR_PECCF;
        }
        if (isr & I2C_ISR_OVR) {
            errors |= I2C::Error::Overrun;
            i2c->ICR |= I2C_ICR_OVRCF;
        }
        if (isr & I2C_ISR_ARLO) {
            errors |= I2C::Error::ArbitrationLost;
            i2c->ICR |= I2C_ICR_ARLOCF;
        }
        if (isr & I2C_ISR_BERR) {
            errors |= I2C::Error::Bus;
            i2c->ICR |= I2C_ICR_BERRCF;
        }

        return errors;
    }
    //----------------------------------------- friends -----------------------------------------//
    // friend microhal::I2CDevice;
};

}  // namespace stm32f0xx
}  // namespace microhal

#endif  // _MICROHAL_I2C_STM32F0XX_H_
