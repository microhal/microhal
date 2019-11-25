/*
 * dma_stm32f4xx.h
 *
 *  Created on: 4 kwi 2015
 *      Author: Pawel
 */

#ifndef DMA_STM32F3XX_H_
#define DMA_STM32F3XX_H_

#include <cstddef>
#include "device/stm32f3xx.h"
#include "ports/stmCommon/clockManager/dmaClock.h"

namespace microhal {
namespace stm32f3xx {
namespace DMA {

class Channel {
 public:
    enum class Priority : uint32_t { Low = 0x00000000, Medium = DMA_CCR_PL_0, High = DMA_CCR_PL_1, VeryHigh = DMA_CCR_PL_1 | DMA_CCR_PL_0 };

    enum class MemoryDataSize : uint32_t { Byte = 0x00000000, HalfWord = DMA_CCR_MSIZE_0, Word = DMA_CCR_MSIZE_1 };

    enum class PeripheralDataSize : uint32_t { Byte = 0x00, HalfWord = DMA_CCR_PSIZE_0, Word = DMA_CCR_PSIZE_1 };

    enum class MemoryIncrementMode : uint32_t { PointerFixed = 0x00000000, PointerIncremented = DMA_CCR_MINC };

    enum class PeripheralIncrementMode : uint32_t { PointerFixed = 0x00000000, PointerIncremented = DMA_CCR_PINC };

    enum class TransmisionDirection : uint32_t { PerToMem = 0x00, MemToPer = DMA_CCR_DIR, MemToMem = DMA_CCR_MEM2MEM };

    enum class Interrupt : uint32_t {
        TransferComplete = DMA_CCR_TCIE,
        HalfTransferComplete = DMA_CCR_HTIE,
        TransferError = DMA_CCR_TEIE,
    };

    void init(MemoryDataSize memSize, PeripheralDataSize peripheralSize, MemoryIncrementMode memoryInc, PeripheralIncrementMode peripheralInc,
              TransmisionDirection direction) {
        channel.CCR = static_cast<uint32_t>(memSize) | static_cast<uint32_t>(peripheralSize) | static_cast<uint32_t>(memoryInc) |
                      static_cast<uint32_t>(peripheralInc) | static_cast<uint32_t>(direction);
    }

    void deinit() { channel.CCR = 0; }

    void setMemoryIncrement(MemoryIncrementMode memoryInc) {
        channel.CCR = (channel.CCR & ~static_cast<uint32_t>(MemoryIncrementMode::PointerIncremented)) | static_cast<uint32_t>(memoryInc);
    }

    void enableCircularMode() { channel.CCR |= DMA_CCR_CIRC; }
    void disableCircularMode() { channel.CCR &= ~DMA_CCR_CIRC; }

    void setPriority(Priority priority) { channel.CCR = (channel.CCR & ~DMA_CCR_PL) | static_cast<uint32_t>(priority); }

    void setPeripheralAddress(volatile void *addr) { channel.CPAR = reinterpret_cast<uint32_t>(addr); }
    void setMemoryAddress(void *memoryAddr) { channel.CMAR = reinterpret_cast<uint32_t>(memoryAddr); }

    void *getMemoryAddress() { return reinterpret_cast<void *>(channel.CMAR); }

    void setNumberOfItemsToTransfer(size_t len) { channel.CNDTR = len; }
    size_t getNumberOfItemsInTransfer() const { return channel.CNDTR; }
    void enableInterrupt(Interrupt interrupts) { channel.CCR |= static_cast<uint32_t>(interrupts); }
    void disableInterrupt(Interrupt interrupts) { channel.CCR &= ~(static_cast<uint32_t>(interrupts)); }
    void enable() { channel.CCR |= DMA_CCR_EN; }
    void disable() { channel.CCR &= ~DMA_CCR_EN; }

 private:
    DMA_Channel_TypeDef channel;
    uint32_t reserved;
};

class DMA {
    DMA_TypeDef dma;

 public:
    Channel stream[7];
    void clearInterruptFlag(const Channel &stream, Channel::Interrupt interrupt) {
        const auto streamNumber = calculateStreamNumber(stream);
        dma.IFCR = (0x0F & static_cast<uint32_t>(interrupt)) << (streamNumber * 4);
    }

    void enableInterrupt(const Channel &stream, uint32_t priority) noexcept {
        const IRQn_Type irq[7] = {DMA1_Channel1_IRQn, DMA1_Channel2_IRQn, DMA1_Channel3_IRQn, DMA1_Channel4_IRQn,
                                  DMA1_Channel5_IRQn, DMA1_Channel6_IRQn, DMA1_Channel7_IRQn};
        IRQn_Type interruptNumber = irq[calculateStreamNumber(stream)];

        NVIC_EnableIRQ(interruptNumber);
        NVIC_SetPriority(interruptNumber, priority);
    }

    void disableInterrupt(const Channel &stream) {
        const IRQn_Type irq[7] = {DMA1_Channel1_IRQn, DMA1_Channel2_IRQn, DMA1_Channel3_IRQn, DMA1_Channel4_IRQn,
                                  DMA1_Channel5_IRQn, DMA1_Channel6_IRQn, DMA1_Channel7_IRQn};
        IRQn_Type interruptNumber = irq[calculateStreamNumber(stream)];

        NVIC_DisableIRQ(interruptNumber);
    }

    void clockEnable() { microhal::ClockManager::enableDMA(1); }
    void clockDisable() { microhal::ClockManager::disableDMA(1); }

 private:
    uint32_t calculateStreamNumber(const Channel &stream) const noexcept { return &stream - &this->stream[0]; }
};

static DMA *dma1[[maybe_unused]] = reinterpret_cast<DMA *>(DMA1);
}  // namespace DMA
}  // namespace stm32f3xx
}  // namespace microhal

#endif /* DMA_STM32F3XX_H_ */
