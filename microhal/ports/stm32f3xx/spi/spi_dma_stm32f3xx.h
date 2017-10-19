/*
 * SPI_dma_stm32f4xx.h
 *
 *  Created on: 17 kwi 2014
 *      Author: pawel
 */

#ifndef SPI_DMA_STM32F3XX_H_
#define SPI_DMA_STM32F3XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "../clockManager.h"
#include "../dma_stm32f3xx.h"
#include "../spi_stm32f3xx.h"
#include "microhal_semaphore.h"

namespace microhal {
namespace stm32f3xx {
extern "C"{
void DMA1_Channel2_IRQHandler(void);
void DMA1_Channel3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
void DMA1_Channel6_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
class SPI_dma: public stm32f3xx::SPI {
public:
    //---------------------------------------- variables ----------------------------------------//
#ifdef MICROHAL_USE_SPI1_DMA
    static SPI_dma spi1;
#endif
#ifdef MICROHAL_USE_SPI2_DMA
    static SPI_dma spi2;
#endif
#ifdef MICROHAL_USE_SPI3_DMA
    static SPI_dma spi3;
#endif
    //---------------------------------------- functions ----------------------------------------//
    SPI::Error write(const void *data, size_t len, bool last) final;
    SPI::Error read(void *data, size_t len, uint8_t write = 0x00) final {
    	return writeRead(&write, data, 1, len);
    }
    SPI::Error writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) final {
    	return writeRead(dataWrite, dataRead, readWriteLength, readWriteLength);
    }

    void setDMAStreamPriority(DMA::Channel::Priority rxPriority, DMA::Channel::Priority txPriority) {
    	rxStream.priority(rxPriority);
    	txStream.priority(txPriority);
    }
private:
    //---------------------------------------- variables ----------------------------------------//
    os::Semaphore semaphore;
    DMA::DMA &dma;
    DMA::Channel &rxStream;
    DMA::Channel &txStream;

    //--------------------------------------- constructors --------------------------------------//
    SPI_dma(SPI_TypeDef &spi, DMA::DMA &dma, DMA::Channel & rxStream, DMA::Channel & txStream, stm32f3xx::GPIO::IOPin misoPin) :
            SPI(spi, misoPin), semaphore(), dma(dma), rxStream(rxStream), txStream(txStream) {
    	ClockManager::enable(spi);
#if defined(HAL_RTOS_FreeRTOS)
    	priority(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
#else
    	priority(0);
#endif
        init();
    }
    //---------------------------------------- functions ----------------------------------------//
    SPI::Error writeRead(const void *writePtr, void *readPtr, size_t writeLen, size_t readLen);
    void init();

    static inline void IRQfunction(SPI_dma &object, SPI_TypeDef *spi);
    //----------------------------------------- friends -----------------------------------------//
    friend void DMA1_Channel2_IRQHandler(void);
    friend void DMA1_Channel3_IRQHandler(void);
    friend void DMA1_Channel4_IRQHandler(void);
    friend void DMA1_Channel5_IRQHandler(void);
    friend void DMA1_Channel5_IRQHandler(void);
    friend void DMA1_Channel6_IRQHandler(void);

    friend void SPI1_IRQHandler(void);
    friend void SPI2_IRQHandler(void);
    friend void SPI3_IRQHandler(void);
};

} // namespace stm32f3xx
} // namespace microhal

#endif /* SPI_DMA_STM32F4XX_H_ */
