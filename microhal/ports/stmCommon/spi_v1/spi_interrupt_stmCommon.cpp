/*
 * spi_interrupt_STM32F4xx.cpp
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
 */
/* ************************************************************************************************
 * INCLUDES
 */
#include "spi_interrupt_stmCommon.h"
/* ************************************************************************************************
 * 1.) Check if this driver should be used on selected MCU.
 * 2.) Check if this driver is enabled in microhal port configuration file
 */
#if _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1  // Check if driver is compatible with selected MCU
#if ENABLED_ANY_SPI(INTERRUPT)                  // Check if driver is enabled in microhal port config

namespace microhal {
namespace _MICROHAL_ACTIVE_PORT_NAMESPACE {

#if defined(MICROHAL_USE_SPI1_INTERRUPT) && MICROHAL_USE_SPI1_INTERRUPT == 1
SPI_interrupt *SPI_interrupt::spi1;
#endif
#if defined(MICROHAL_USE_SPI2_INTERRUPT) && MICROHAL_USE_SPI2_INTERRUPT == 1
SPI_interrupt *SPI_interrupt::spi2;
#endif
#if defined(MICROHAL_USE_SPI3_INTERRUPT) && MICROHAL_USE_SPI3_INTERRUPT == 1
SPI_interrupt *SPI_interrupt::spi3;
#endif
#if defined(MICROHAL_USE_SPI4_INTERRUPT) && MICROHAL_USE_SPI4_INTERRUPT == 1
SPI_interrupt *SPI_interrupt::spi4;
#endif
#if defined(MICROHAL_USE_SPI5_INTERRUPT) && MICROHAL_USE_SPI5_INTERRUPT == 1
SPI_interrupt *SPI_interrupt::spi5;
#endif
#if defined(MICROHAL_USE_SPI6_INTERRUPT) && MICROHAL_USE_SPI6_INTERRUPT == 1
SPI_interrupt *SPI_interrupt::spi6;
#endif

SPI::Error SPI_interrupt::write(const void *data, size_t len, bool last) {
    if (len == 0) return Error::None;

    readPtr = nullptr;
    writePtr = (uint8_t *)data;
    writeEnd = ((uint8_t *)data) + len;

#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
    // we have transmit fifo, lets fill fifo before enabling interrupt
    do {
        if (txFifoLevel() != FIFOLevel::Full) {
            spi.dr.volatileStore_8bit(*writePtr);
            writePtr++;
        } else {
            break;
        }
        if (last == false && writePtr == writeEnd) {
            return Error::None;  // all data fit into fifo, return success
        }
    } while (writePtr != writeEnd);
#endif
    enableInterrupt(Interrupt::TransmitterEmpty);
    if (semaphore.wait(std::chrono::milliseconds::max())) {
        // this is last transaction on SPI bus, so we need to wait until last byte will be send before we exit from function because someone could
        // change CS signal to high when we will sending last byte
        if (last) {
            busyWait();
        }
        return Error::None;
    }
    return Error::Timeout;
}

SPI::Error SPI_interrupt::read(void *data, size_t len, uint8_t write) {
    if (len == 0) return Error::None;

    writePtr = &write;
    writeEnd = &write;
    readPtr = static_cast<uint8_t *>(data);
    readEnd = static_cast<uint8_t *>(data) + len;

    clearRxFifo();

    enableInterrupt(Interrupt::ReceiverNotEmpty);

    spi.dr.volatileStore_8bit(write);
    if (semaphore.wait(std::chrono::milliseconds::max())) return Error::None;
    return Error::Timeout;
}

SPI::Error SPI_interrupt::writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) {
    if (readWriteLength == 0) return Error::None;

    readPtr = static_cast<uint8_t *>(dataRead);
    readEnd = static_cast<uint8_t *>(dataRead) + readWriteLength;
    writePtr = static_cast<const uint8_t *>(dataWrite);
    writeEnd = static_cast<const uint8_t *>(dataWrite) + readWriteLength;

    clearRxFifo();

    enableInterrupt(Interrupt::TransmitterEmpty | Interrupt::ReceiverNotEmpty);

    if (semaphore.wait(std::chrono::milliseconds::max())) return Error::None;
    return Error::Timeout;
}

void SPI_interrupt::clearRxFifo() {
    busyWait();
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
    while (rxFifoLevel() != FIFOLevel::Empty) {
        spi.dr.volatileLoad_8bit();
    }
#else
    spi.dr.volatileLoad();
#endif
}

static inline void giveSemaphoreFromIsr(os::Semaphore &semaphore) __attribute__((always_inline));
void giveSemaphoreFromIsr(os::Semaphore &semaphore) {
    [[maybe_unused]] bool shouldYeld = semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
    portYIELD_FROM_ISR(shouldYeld);
#endif
}

//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void SPI_interrupt::IRQfunction() {
    auto sr = spi.sr.volatileLoad();
    auto cr2 = spi.cr2.volatileLoad();

    if ((readPtr != nullptr) && (sr.RXNE)) {
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
        *readPtr = (uint32_t)spi.dr.volatileLoad_8bit();
#else
        *readPtr = (uint32_t)spi.dr.volatileLoad();
#endif
        readPtr++;
        if (readPtr == readEnd) {
            readPtr = nullptr;
            disableInterrupt(Interrupt::ReceiverNotEmpty);
            giveSemaphoreFromIsr(semaphore);
        } else if (writePtr == writeEnd && writePtr != nullptr) {
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
            spi.dr.volatileStore_8bit(*writePtr);
#else
            spi.dr.volatileStore(*writePtr);
#endif
        }
    }
    if (cr2.TXEIE && sr.TXE) {  // transmitter empty interrupt enabled and active
        if (writePtr != writeEnd) {
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
            spi.dr.volatileStore_8bit(*writePtr);
#else
            spi.dr.volatileStore(*writePtr);
#endif
            writePtr++;
        }
        if (writePtr == writeEnd) {
            writePtr = nullptr;
            writeEnd = nullptr;
            disableInterrupt(Interrupt::TransmitterEmpty);
            if (readPtr == nullptr) {
                giveSemaphoreFromIsr(semaphore);
            }
        }
    }
}
//***********************************************************************************************//
//                                          IRQHandlers //
//***********************************************************************************************//
#if defined(MICROHAL_USE_SPI1_INTERRUPT) && MICROHAL_USE_SPI1_INTERRUPT == 1
extern "C" void SPI1_IRQHandler(void) {
    SPI_interrupt::spi1->IRQfunction();
}
#endif
#if defined(MICROHAL_USE_SPI2_INTERRUPT) && MICROHAL_USE_SPI2_INTERRUPT == 1
extern "C" void SPI2_IRQHandler(void) {
    SPI_interrupt::spi2->IRQfunction();
}
#endif
#if defined(MICROHAL_USE_SPI3_INTERRUPT) && MICROHAL_USE_SPI3_INTERRUPT == 1
extern "C" void SPI3_IRQHandler(void) {
    SPI_interrupt::spi3->IRQfunction();
}
#endif
#if defined(MICROHAL_USE_SPI4_INTERRUPT) && MICROHAL_USE_SPI4_INTERRUPT == 1
extern "C" void SPI4_IRQHandler(void) {
    SPI_interrupt::spi4->IRQfunction();
}
#endif
#if defined(MICROHAL_USE_SPI5_INTERRUPT) && MICROHAL_USE_SPI5_INTERRUPT == 1
extern "C" void SPI5_IRQHandler(void) {
    SPI_interrupt::spi5->IRQfunction();
}
#endif
#if defined(MICROHAL_USE_SPI6_INTERRUPT) && MICROHAL_USE_SPI6_INTERRUPT == 1
extern "C" void SPI6_IRQHandler(void) {
    SPI_interrupt::spi6->IRQfunction();
}
#endif

}  // namespace _MICROHAL_ACTIVE_PORT_NAMESPACE
}  // namespace microhal

#endif  // ENABLED_ANY_SPI(INTERRUPT)
#endif  // _MICROHAL_PORT_STM_SPI_DRIVER_VERSION == 1
