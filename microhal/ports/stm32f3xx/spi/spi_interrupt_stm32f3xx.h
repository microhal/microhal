/*
 * spi_interrupt_STM32F4xx.h
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
 */

#ifndef SPI_INTERRUPT_STM32F3XX_H_
#define SPI_INTERRUPT_STM32F3XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "../clockManager.h"
#include "../spi_stm32f3xx.h"
#include "microhal_semaphore.h"

namespace microhal {
namespace stm32f3xx {
/* ************************************************************************************************
 * CLASS
 */
class SPI_interrupt : public stm32f3xx::SPI {
 public:
    //---------------------------------------- variables ----------------------------------------//
#ifdef MICROHAL_USE_SPI1_INTERRUPT
    static SPI_interrupt spi1;
#endif
#ifdef MICROHAL_USE_SPI2_INTERRUPT
    static SPI_interrupt spi2;
#endif
#ifdef MICROHAL_USE_SPI3_INTERRUPT
    static SPI_interrupt spi3;
#endif
    //---------------------------------------- functions ----------------------------------------//
    SPI::Error write(const void *data, size_t len, bool last) final;
    SPI::Error read(void *data, size_t len, uint8_t write = 0x00) final;
    SPI::Error writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) final;

 private:
    //---------------------------------------- variables ----------------------------------------//
    uint8_t *readPtr = nullptr;
    uint8_t *readEnd = nullptr;
    const uint8_t *writePtr = nullptr;
    const uint8_t *writeEnd = nullptr;
    uint8_t writeData = 0x00;
    os::Semaphore semaphore;

    //--------------------------------------- constructors --------------------------------------//
    SPI_interrupt(SPI_TypeDef &spi, stm32f3xx::IOPin misoPin) : SPI(spi, misoPin), semaphore() {
        ClockManager::enable(spi);
#if defined(HAL_RTOS_FreeRTOS)
        priority(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
#else
        priority(0);
#endif
    }

    SPI_interrupt &operator=(const SPI_interrupt &);

    SPI_interrupt(const SPI_interrupt &);
    //---------------------------------------- functions ----------------------------------------//
    void enableTransmitterEmptyInterrupt() {
        spi.CR2 |= SPI_CR2_TXEIE;  // fixme maybe bitband
    }
    void enableReceiverNotEmptyInterrupt() {
        spi.CR2 |= SPI_CR2_RXNEIE;  // fixme maybe bitband
    }
    //----------------------------------------- friends -----------------------------------------//
    friend inline void IRQfunction(SPI_interrupt &object, SPI_TypeDef *spi) __attribute__((always_inline));
    friend void SPI1_IRQHandler(void);
    friend void SPI2_IRQHandler(void);
    friend void SPI3_IRQHandler(void);
};
}  // namespace stm32f3xx
}  // namespace microhal

#endif /* SPI_INTERRUPT_STM32F4XX_H_ */
