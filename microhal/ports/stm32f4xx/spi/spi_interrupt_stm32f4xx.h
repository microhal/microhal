/*
 * spi_interrupt_STM32F4xx.h
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
 */

#ifndef SPI_INTERRUPT_STM32F4XX_H_
#define SPI_INTERRUPT_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "../spi_stm32f4xx.h"
#include "microhal_semaphore.h"

namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 * CLASS
 */
class SPI_interrupt: public stm32f4xx::SPI {
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
#ifdef MICROHAL_USE_SPI4_INTERRUPT
    static SPI_interrupt spi4;
#endif
#ifdef MICROHAL_USE_SPI5_INTERRUPT
    static SPI_interrupt spi5;
#endif
#ifdef MICROHAL_USE_SPI6_INTERRUPT
    static SPI_interrupt spi6;
#endif
    //---------------------------------------- functions ----------------------------------------//
    SPI::Error write(uint8_t data, bool last) final;
    SPI::Error read(uint8_t &data, uint8_t write = 0x00) final;
    SPI::Error writeBuffer(const void *data, size_t len, bool last) final;
    SPI::Error readBuffer(void *data, size_t len, uint8_t write = 0x00) final;
    SPI::Error readWrite(void *dataRead, const void *dataWrite, size_t readWriteLength) final;
private:
    typedef enum {
        WAITING, TRANSMIT, RECEIVE
    } Mode;
    //---------------------------------------- variables ----------------------------------------//
    uint8_t *readPtr = nullptr;
    uint8_t *readEnd = nullptr;
    const uint8_t *writePtr = nullptr;
    const uint8_t *writeEnd = nullptr;
    uint8_t writeData = 0x00;
    Mode mode = WAITING;

    //volatile bool semaphore;
    os::Semaphore semaphore;

    //--------------------------------------- constructors --------------------------------------//
    SPI_interrupt(SPI_TypeDef &spi, stm32f4xx::GPIO::IOPin misoPin) :
            SPI(spi, misoPin), semaphore() {
#if defined(HAL_RTOS_FreeRTOS)
    	const uint32_t priority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
#else
    	const uint32_t priority = 0;
#endif
        switch (reinterpret_cast<uint32_t>(&spi)) {
        case reinterpret_cast<uint32_t>(SPI1):
            RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
            NVIC_EnableIRQ(SPI1_IRQn);
            NVIC_SetPriority(SPI1_IRQn, priority);
            break;
        case reinterpret_cast<uint32_t>(SPI2):
            RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
            NVIC_EnableIRQ(SPI2_IRQn);
            NVIC_SetPriority(SPI2_IRQn, priority);
            break;
        case reinterpret_cast<uint32_t>(SPI3):
            RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
            NVIC_EnableIRQ (SPI3_IRQn);
            NVIC_SetPriority(SPI3_IRQn, priority);
            break;
#ifdef SPI4_IRQn
            case reinterpret_cast<uint32_t>(SPI4):
            RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
            NVIC_EnableIRQ (SPI4_IRQn);
            NVIC_SetPriority(SPI4_IRQn, priority);
            break;
#endif
#ifdef SPI5_IRQn
            case reinterpret_cast<uint32_t>(SPI5):
            RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
            NVIC_EnableIRQ (SPI5_IRQn);
            NVIC_SetPriority(SPI5_IRQn, priority);
            break;
#endif
#ifdef SPI6_IRQn
            case reinterpret_cast<uint32_t>(SPI6):
            RCC->APB2ENR |= RCC_APB2ENR_SPI6EN;
            NVIC_EnableIRQ (SPI6_IRQn);
            NVIC_SetPriority(SPI6_IRQn, priority);
            break;
#endif
        }
    }

    virtual ~SPI_interrupt() { }
    SPI_interrupt& operator=(const SPI_interrupt&);
	
    SPI_interrupt(const SPI_interrupt&);
    //---------------------------------------- functions ----------------------------------------//
    void enableTransmitterEmptyInterrupt() {
        spi.CR2 |= SPI_CR2_TXEIE; //fixme maybe bitband
    }
    void enableReceiverNotEmptyInterrupt() {
        spi.CR2 |= SPI_CR2_RXNEIE; //fixme maybe bitband
    }
    //----------------------------------------- friends -----------------------------------------//
    friend inline void IRQfunction(SPI_interrupt &object, SPI_TypeDef *spi) __attribute__ ((always_inline));
    friend void SPI1_IRQHandler(void);
    friend void SPI2_IRQHandler(void);
    friend void SPI3_IRQHandler(void);
    friend void SPI4_IRQHandler(void);
    friend void SPI5_IRQHandler(void);
    friend void SPI6_IRQHandler(void);
}
;
} // namespace stm32f4xx
} // namespace microhal

#endif /* SPI_INTERRUPT_STM32F4XX_H_ */
