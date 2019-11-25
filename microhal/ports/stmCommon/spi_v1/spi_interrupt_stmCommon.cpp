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
    readPtr = nullptr;
    writePtr = (uint8_t *)data;
    writeEnd = ((uint8_t *)data) + len;

    enableInterrupt(Interrupt::TransmitterEmpty);
    if (semaphore.wait(std::chrono::milliseconds::max())) {
        // this is last transaction on SPI bus, so we need to wait until last byte will be send before we exit from function because someone could
        // change CS signal to high when we will sending last byte
        if (last) {
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
            while (spi.sr.volatileLoad().FTLVL) {
            }
#endif
            busyWait();
            // workaround, I don't know why but BSY flag is cleared in the middle of last bit. This may cause some error when other function will
            // deassert
            // CS pin
            volatile uint32_t i = 100;
            while (i--) {
            }
        }
        return Error::None;
    }
    return Error::Timeout;
}

SPI::Error SPI_interrupt::read(void *data, size_t len, uint8_t write) {
    writePtr = nullptr;
    readPtr = static_cast<uint8_t *>(data);
    readEnd = static_cast<uint8_t *>(data) + len;
    writeData = write;

#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
    while (spi.sr.volatileLoad().FTLVL) {
    }
#endif
    busyWait();
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
    for (size_t i = spi.sr.volatileLoad().FRLVL; i > 0; i--) {
        spi.dr.volatileLoad();
    }
#else
    spi.dr.volatileLoad();
#endif

    enableInterrupt(Interrupt::ReceiverNotEmpty);

    spi.dr.volatileStore(write);
    if (semaphore.wait(std::chrono::milliseconds::max())) return Error::None;
    return Error::Timeout;
}

SPI::Error SPI_interrupt::writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) {
    readPtr = static_cast<uint8_t *>(dataRead);
    readEnd = static_cast<uint8_t *>(dataRead) + readWriteLength;
    writePtr = static_cast<const uint8_t *>(dataWrite);
    writeEnd = static_cast<const uint8_t *>(dataWrite) + readWriteLength;

#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
    while (spi.sr.volatileLoad().FTLVL) {
    }
#endif
    busyWait();
#ifdef _MICROHAL_REGISTERS_SPI_SR_HAS_FRLVL
    for (size_t i = spi.sr.volatileLoad().FRLVL; i > 0; i--) {
        spi.dr.volatileLoad();
    }
#else
    spi.dr.volatileLoad();
#endif

    enableInterrupt(Interrupt::TransmitterEmpty | Interrupt::ReceiverNotEmpty);

    if (semaphore.wait(std::chrono::milliseconds::max())) return Error::None;
    return Error::Timeout;
}
//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
void SPI_interrupt::IRQfunction() {
    auto sr = spi.sr.volatileLoad();
    auto cr2 = spi.cr2.volatileLoad();

    if ((readPtr != nullptr) && (sr.RXNE)) {
        sr.RXNE.clear();
        *readPtr++ = (uint32_t)spi.dr.volatileLoad();
        if (readPtr == readEnd) {
            readPtr = nullptr;
            auto cr2 = spi.cr2.volatileLoad();
            cr2.RXNEIE.clear();  // fixme maybe bitband
            spi.cr2.volatileStore(cr2);
            bool shouldYeld = semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
            portYIELD_FROM_ISR(shouldYeld);
#else
            (void)shouldYeld;
#endif
        } else if (writePtr == nullptr) {
            spi.dr.volatileStore(writeData);
        }
    }
    if (cr2.TXEIE) {
        if ((writePtr != nullptr) && (sr.TXE)) {
            sr.TXE.clear();
            spi.dr.volatileStore(*writePtr++);
            if (writePtr == writeEnd) {
                writePtr = nullptr;
                auto cr2 = spi.cr2.volatileLoad();
                cr2.TXEIE.clear();  // fixme maybe bitband
                spi.cr2.volatileStore(cr2);
                if (readPtr == nullptr) {
                    bool shouldYeld = semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
                    portYIELD_FROM_ISR(shouldYeld);
#else
                    (void)shouldYeld;
#endif
                }
            }
        } else {
            auto cr2 = spi.cr2.volatileLoad();
            cr2.TXEIE.clear();  // fixme maybe bitband
            spi.cr2.volatileStore(cr2);
        }
    }

    spi.sr.volatileStore(sr);
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
