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

#include <ports/stmCommon/driverConfiguration_stmCommon.h>
#if _MICROHAL_PORT_STM_DMA_DRIVER_VERSION == 1
#ifndef DMA_V1_STMCOMMON_H_
#define DMA_V1_STMCOMMON_H_

#include <cstddef>
#include <utility>
#include "../../registers/dma_registers_v1.h"
#include "../../stmCommonDefines.h"
#include "ports/stmCommon/clockManager/dmaClock.h"

#ifndef _MICROHAL_ACTIVE_PORT_NAMESPACE
#error _MICROHAL_ACTIVE_PORT_NAMESPACE have to be defined.
#endif

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {
namespace DMA {

class Channel {
 public:
    enum class Priority : uint32_t { Low = 0x00000000, Medium = 0b01, High = 0b10, VeryHigh = 0b11 };

    enum class MemoryDataSize : uint32_t { Byte = 0x00000000, HalfWord = 0b01, Word = 0b10 };

    enum class PeripheralDataSize : uint32_t { Byte = 0x00, HalfWord = 0b01, Word = 0b10 };

    enum class MemoryIncrementMode : uint32_t { PointerFixed = 0x0, PointerIncremented = 0x1 };

    enum class PeripheralIncrementMode : uint32_t { PointerFixed = 0x00000000, PointerIncremented = 0b1 };

    enum class TransmisionDirection : uint32_t { PerToMem = 0x00, MemToPer = 0b10000, MemToMem = 1 << 14 };

    enum class Interrupt : uint32_t { TransferComplete = 1 << 1, HalfTransferComplete = 1 << 2, TransferError = 1 << 3 };

    void init(MemoryDataSize memSize, PeripheralDataSize peripheralSize, MemoryIncrementMode memoryInc, PeripheralIncrementMode peripheralInc,
              TransmisionDirection direction);

    void deinit() { channel.ccr.volatileStore(0); }

    void enableCircularMode() {
        auto ccr = channel.ccr.volatileLoad();
        ccr.CIRC.set();
        channel.ccr.volatileStore(ccr);
    }
    void disableCircularMode() {
        auto ccr = channel.ccr.volatileLoad();
        ccr.CIRC.clear();
        channel.ccr.volatileStore(ccr);
    }

    void setPriority(Priority priority) {
        auto ccr = channel.ccr.volatileLoad();
        ccr.PL = static_cast<uint32_t>(priority);
        channel.ccr.volatileStore(ccr);
    }

    void setPeripheralAddress(volatile void *addr) { channel.cpar.volatileStore(reinterpret_cast<uint32_t>(addr)); }
    void setMemoryAddress(void *memoryAddr) { channel.cmar.volatileStore(reinterpret_cast<uint32_t>(memoryAddr)); }

    void *getMemoryAddress() { return reinterpret_cast<void *>(channel.cmar.volatileLoad()); }

    void setNumberOfItemsToTransfer(size_t len) { channel.cndtr.volatileStore(len); }
    size_t getNumberOfItemsInTransfer() const { return channel.cndtr.volatileLoad().operator uint32_t(); }
    void enableInterrupt(Interrupt interrupts) {
        auto ccr = channel.ccr.volatileLoad();
        ccr |= static_cast<uint32_t>(interrupts);
        channel.ccr.volatileStore(ccr);
    }
    void disableInterrupt(Interrupt interrupts) {
        auto ccr = channel.ccr.volatileLoad();
        ccr &= ~static_cast<uint32_t>(interrupts);
        channel.ccr.volatileStore(ccr);
    }
    void enable() {
        auto ccr = channel.ccr.volatileLoad();
        ccr.EN.set();
        channel.ccr.volatileStore(ccr);
    }
    void disable() {
        auto ccr = channel.ccr.volatileLoad();
        ccr.EN.clear();
        channel.ccr.volatileStore(ccr);
    }

 private:
    registers::DMA::Channel channel;
};

class DMA {
    registers::DMA dma;

 public:
    Channel channel[7];
    void clearInterruptFlag(const Channel &channel, Channel::Interrupt interrupt) {
        const auto channelNumber = calculateChannelNumber(channel);
        dma.ifcr.volatileStore((0x0F & static_cast<uint32_t>(interrupt)) << (channelNumber * 4));
    }

    void enableInterrupt(const Channel &stream, uint32_t priority) noexcept;
    void disableInterrupt(const Channel &stream);

    void clockEnable() {
#ifndef _MICROHAL_DMA2_BASE_ADDRESS
        microhal::ClockManager::enableDMA(1);
#else
        microhal::ClockManager::enableDMA(reinterpret_cast<uint32_t>(this) == _MICROHAL_DMA1_BASE_ADDRESS ? 1 : 2);
#endif
    }
    void clockDisable() {
#ifndef _MICROHAL_DMA2_BASE_ADDRESS
        microhal::ClockManager::disableDMA(1);
#else
        microhal::ClockManager::disableDMA(reinterpret_cast<uint32_t>(this) == _MICROHAL_DMA1_BASE_ADDRESS ? 1 : 2);
#endif
    }

 private:
    uint32_t calculateChannelNumber(const Channel &channel) const noexcept { return &channel - &this->channel[0]; }
};

#ifdef _MICROHAL_DMA1_BASE_ADDRESS
static DMA *dma1[[maybe_unused]] = reinterpret_cast<DMA *>(_MICROHAL_DMA1_BASE_ADDRESS);
#endif

#ifdef _MICROHAL_DMA2_BASE_ADDRESS
static DMA *dma2[[maybe_unused]] = reinterpret_cast<DMA *>(_MICROHAL_DMA2_BASE_ADDRESS);
#endif

}  // namespace DMA
}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif /* DMA_V1_STMCOMMON_H_ */
#endif
