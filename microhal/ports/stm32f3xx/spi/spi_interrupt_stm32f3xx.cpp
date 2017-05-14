/*
 * spi_interrupt_STM32F4xx.cpp
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
 */
/* ************************************************************************************************
 * INCLUDES
 */
#include "spi_interrupt_stm32f3xx.h"

namespace microhal {
namespace stm32f3xx {

#ifdef MICROHAL_USE_SPI1_INTERRUPT
GPIO::IOPin spi1MisoPin(GPIO::PortB, 4);
SPI_interrupt SPI_interrupt::spi1(*SPI1, spi1MisoPin);
SPI &SPI::spi1 = SPI_interrupt::spi1;
#endif
#ifdef MICROHAL_USE_SPI2_INTERRUPT
GPIO::IOPin spi2MisoPin(GPIO::PortB, 14);
SPI_interrupt SPI_interrupt::spi2(*SPI2, spi2MisoPin);
SPI &SPI::spi2 = SPI_interrupt::spi2;
#endif
#ifdef MICROHAL_USE_SPI3_INTERRUPT
GPIO::IOPin spi3MisoPin(GPIO::PortC, 11);
SPI_interrupt SPI_interrupt::spi3(*SPI3, spi3MisoPin);
SPI &SPI::spi3 = SPI_interrupt::spi3;
#endif

SPI::Error SPI_interrupt::write(const void *data, size_t len, bool last) {
    readPtr = nullptr;
    writePtr = (uint8_t *)data;
    writeEnd = ((uint8_t *)data) + len;

    enableTransmitterEmptyInterrupt();
    semaphore.wait(std::chrono::milliseconds::max());

    // this is last transaction on SPI bus, so we need to wait until last byte will be send before we exit from function because someone could
    // change CS signal to high when we will sending last byte
    if (last) {
        while (spi.SR & SPI_SR_FTLVL_Msk) {
        }
        while (spi.SR & SPI_SR_BSY) {
        }
        // workaround, I don't know why but BSY flag is cleared in the middle of last bit. This may cause some error when other function will deassert
        // CS pin
        volatile uint32_t i = 100;
        while (i--)
            ;
    }

    return SPI::Error::None;
}

SPI::Error SPI_interrupt::read(void *data, size_t len, uint8_t write) {
    writePtr = nullptr;
    readPtr = static_cast<uint8_t *>(data);
    readEnd = static_cast<uint8_t *>(data) + len;

    writeData = write;
    while (spi.SR & SPI_SR_FTLVL_Msk) {
    }
    while (spi.SR & SPI_SR_BSY) {
    }
    while (spi.SR & SPI_SR_FRLVL_Msk) {
        volatile uint16_t tmp __attribute__((unused)) = spi.DR;
    }
    enableReceiverNotEmptyInterrupt();

    spi.DR = write;
    semaphore.wait(std::chrono::milliseconds::max());

    return SPI::Error::None;
}

SPI::Error SPI_interrupt::writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) {
    readPtr = static_cast<uint8_t *>(dataRead);
    readEnd = static_cast<uint8_t *>(dataRead) + readWriteLength;
    writePtr = static_cast<const uint8_t *>(dataWrite);
    writeEnd = static_cast<const uint8_t *>(dataWrite) + readWriteLength;

    while (spi.SR & SPI_SR_FTLVL_Msk) {
    }
    while (spi.SR & SPI_SR_BSY) {
    }
    while (spi.SR & SPI_SR_FRLVL_Msk) {
        volatile uint16_t tmp __attribute__((unused)) = spi.DR;
    }

    enableTransmitterEmptyInterrupt();
    enableReceiverNotEmptyInterrupt();

    semaphore.wait(std::chrono::milliseconds::max());
    return SPI::Error::None;
}
//***********************************************************************************************//
//                                     interrupt functions //
//***********************************************************************************************//
inline void IRQfunction(SPI_interrupt &object, SPI_TypeDef *spi) {
    uint32_t sr = spi->SR;

    if ((object.readPtr != nullptr) && (sr & SPI_SR_RXNE)) {
        *object.readPtr++ = object.readDR();
        if (object.readPtr == object.readEnd) {
            object.readPtr = nullptr;
            spi->CR2 &= ~(SPI_CR2_RXNEIE);  // fixme maybe bitband
            bool shouldYeld = object.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
            portYIELD_FROM_ISR(shouldYeld);
#else
            (void)shouldYeld;
#endif
        } else if (object.writePtr == nullptr) {
            object.writeDR(object.writeData);
        }
    }
    if ((object.writePtr != nullptr) && (sr & SPI_SR_TXE)) {
        object.writeDR(*object.writePtr++);
        if (object.writePtr == object.writeEnd) {
            object.writePtr = nullptr;
            spi->CR2 &= ~(SPI_CR2_TXEIE);  // fixme maybe bitband
            if (object.readPtr == nullptr) {
                bool shouldYeld = object.semaphore.giveFromISR();
#if defined(HAL_RTOS_FreeRTOS)
                portYIELD_FROM_ISR(shouldYeld);
#else
                (void)shouldYeld;
#endif
            }
        }
    }

    sr = ~sr;
    spi->SR = sr;
}
//***********************************************************************************************//
//                                          IRQHandlers //
//***********************************************************************************************//
#ifdef MICROHAL_USE_SPI1_INTERRUPT
void SPI1_IRQHandler(void) {
    IRQfunction(SPI_interrupt::spi1, SPI1);
}
#endif
#ifdef MICROHAL_USE_SPI2_INTERRUPT
void SPI2_IRQHandler(void) {
    IRQfunction(SPI_interrupt::spi2, SPI2);
}
#endif
#ifdef MICROHAL_USE_SPI3_INTERRUPT
void SPI3_IRQHandler(void) {
    IRQfunction(SPI_interrupt::spi3, SPI3);
}
#endif

}  // namespace stm32f3xx
}  // namespace microhal
