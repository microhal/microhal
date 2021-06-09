/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 26-03-2021
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

#ifndef _MICROHAL_PORTS_STM32G0XX_INTERRUPTCONTROLLER_H_
#define _MICROHAL_PORTS_STM32G0XX_INTERRUPTCONTROLLER_H_

#include <cstdint>

#define _MICROHAL_INTERRUPT_CONTROLLER_I2C_EV_ER_COMBINED 1

namespace microhal {
namespace stm32g0xx {

void enableRTCInterrupt(uint32_t priority);
void disableRTCInterrupt();

void enableTimerInterrupt(uint8_t timerNumber, uint32_t priority);
void disableTimerInterrupt(uint8_t timerNumber);

void enableI2CInterrupt(uint8_t i2cNumber, uint32_t priority);
void disableI2CInterrupt(uint8_t i2cNumber);

void enableSPIInterrupt(uint8_t spiNumber);
void disableSPIInterrupt(uint8_t spiNumber);

void enableUSARTInterrupt(uint8_t usartNumber, uint32_t priority);
void setUSARTInterruptPriority(uint8_t usartNumber, uint32_t priority);
void disableUSARTInterrupt(uint8_t usartNumber);

}  // namespace stm32g0xx
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STM32G0XX_INTERRUPTCONTROLLER_H_ */
