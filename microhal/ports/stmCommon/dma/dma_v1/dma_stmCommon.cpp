/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      DMA driver for: STM32F102, STM32F107, STM32F103, STM32F101, STM32F100, STM32F0x2, STM32F0x8, STM32F0x1, STM32F0x0, STM32F302,
 *                             STM32F303, STM32F3x4, STM32F301, STM32F3x8, STM32F373, STM32L100, STM32L162, STM32L151
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2015-2020, Pawel Okas
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

#include "dma_stmCommon.h"
#include _MICROHAL_INCLUDE_PORT_DEVICE

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
namespace DMA {

static constexpr const IRQn_Type irq[7] = {DMA1_Channel1_IRQn, DMA1_Channel2_IRQn, DMA1_Channel3_IRQn, DMA1_Channel4_IRQn,
                                           DMA1_Channel5_IRQn, DMA1_Channel6_IRQn, DMA1_Channel7_IRQn};

void DMA::enableInterrupt(const Channel &stream, uint32_t priority) noexcept {
    const IRQn_Type interruptNumber = irq[calculateChannelNumber(stream)];
    NVIC_EnableIRQ(interruptNumber);
    NVIC_SetPriority(interruptNumber, priority);
}

void DMA::disableInterrupt(const Channel &stream) {
    const IRQn_Type interruptNumber = irq[calculateChannelNumber(stream)];
    NVIC_DisableIRQ(interruptNumber);
}

void Channel::init(MemoryDataSize memSize, PeripheralDataSize peripheralSize, MemoryIncrementMode memoryInc, PeripheralIncrementMode peripheralInc,
                   TransmisionDirection direction) {
    registers::DMA::Channel::CCRx ccr;
    ccr = 0U;
    ccr.MSIZE = static_cast<uint32_t>(memSize);
    ccr.PSIZE = static_cast<uint32_t>(peripheralSize);
    ccr.MINC = static_cast<uint32_t>(memoryInc);
    ccr.PINC = static_cast<uint32_t>(peripheralInc);
    ccr |= static_cast<uint32_t>(direction);
    channel.ccr.volatileStore(ccr);
}

}  // namespace DMA
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal
