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

#ifndef _MICROHAL_PORTS_STMCOMMON_INTERRUPTCONTROLLER_H_
#define _MICROHAL_PORTS_STMCOMMON_INTERRUPTCONTROLLER_H_

#include <ports/stmCommon/stmCommonDefines.h>
#include <cstdint>

#define _MICROHAL_INTERRUPT_CONTROLLER_I2C_EV_ER_COMBINED 1

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

void enableCanTxInterrupt(uint8_t number, uint32_t priority);
void enableCanRx0Interrupt(uint8_t number, uint32_t priority);
void enableCanRx1Interrupt(uint8_t number, uint32_t priority);

void disableCanTxInterrupt(uint8_t number);
void disableCanRx0Interrupt(uint8_t number);
void disableCanRx1Interrupt(uint8_t number);

/**
 *
 * @param channelNumber starts from 0
 * @param priority
 */
void enableDMA1Interrupt(uint32_t channelNumber, uint32_t priority);
/**
 *
 * @param channelNumber starts from 0: 0 -> channel1
 */
void disableDMA1Interrupt(uint32_t channelNumber);

void enableRTCInterrupt(uint32_t priority);
void disableRTCInterrupt();

void enableTimerInterrupt(uint8_t timerNumber, uint32_t priority);
void disableTimerInterrupt(uint8_t timerNumber);

void enableI2CInterrupt(uint8_t i2cNumber, uint32_t priority);
void disableI2CInterrupt(uint8_t i2cNumber);

/**
 *
 * @param spiNumber numbering starts form 0, so spiNumber for SPI1 is equal 0
 * @param priority
 */
void enableSPIInterrupt(uint_fast8_t spiNumber, uint32_t priority);
/**
 *
 * @param spiNumber numbering starts form 0, so spiNumber for SPI1 is equal 0
 */
void disableSPIInterrupt(uint_fast8_t spiNumber);

void enableUSARTInterrupt(uint_fast8_t usartNumber, uint32_t priority);
void setUSARTInterruptPriority(uint_fast8_t usartNumber, uint32_t priority);
void disableUSARTInterrupt(uint_fast8_t usartNumber);

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* _MICROHAL_PORTS_STMCOMMON_INTERRUPTCONTROLLER_H_ */
