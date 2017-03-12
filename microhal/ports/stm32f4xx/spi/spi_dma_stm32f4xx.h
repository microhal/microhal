/*
 * SPI_dma_stm32f4xx.h
 *
 *  Created on: 17 kwi 2014
 *      Author: pawel
 */

#ifndef SPI_DMA_STM32F4XX_H_
#define SPI_DMA_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "../clockManager.h"
#include "../dma_stm32f4xx.h"
#include "../spi_stm32f4xx.h"
#include "microhal_semaphore.h"

namespace microhal {
namespace stm32f4xx {
extern "C"{
void DMA1_Stream0_IRQHandler(void);
void DMA1_Stream2_IRQHandler(void);
void DMA1_Stream3_IRQHandler(void);
void DMA1_Stream4_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);
void DMA1_Stream7_IRQHandler(void);

void DMA2_Stream2_IRQHandler(void);
void DMA2_Stream3_IRQHandler(void);
void DMA2_Stream5_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
class SPI_dma: public stm32f4xx::SPI {
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
#ifdef MICROHAL_USE_SPI4_DMA
    static SPI_dma spi4;
#endif
#ifdef MICROHAL_USE_SPI5_DMA
    static SPI_dma spi5;
#endif
#ifdef MICROHAL_USE_SPI6_DMA
    static SPI_dma spi6;
#endif
    //---------------------------------------- functions ----------------------------------------//
    SPI::Error write(const void *data, size_t len, bool last) final;
    SPI::Error read(void *data, size_t len, uint8_t write = 0x00) final {
    	  return writeRead(&write, data, 1, len);
    }
    SPI::Error writeRead(void *dataRead, const void *dataWrite, size_t readWriteLength) final {
    	return writeRead(dataWrite, dataRead, readWriteLength, readWriteLength);
    }

    void setDMAStreamPriority(DMA::Stream::Priority rxPriority, DMA::Stream::Priority txPriority){
    	rxStream.setPriority(rxPriority);
    	txStream.setPriority(txPriority);
    }
private:
    //---------------------------------------- variables ----------------------------------------//
    os::Semaphore semaphore;
    DMA::DMA &dma;
    DMA::Stream &rxStream;
    DMA::Stream &txStream;

    //--------------------------------------- constructors --------------------------------------//
    SPI_dma(SPI_TypeDef &spi, DMA::DMA &dma, DMA::Stream & rxStream, DMA::Stream & txStream, stm32f4xx::GPIO::IOPin misoPin) :
            SPI(spi, misoPin), semaphore(), dma(dma), rxStream(rxStream), txStream(txStream) {
    	ClockManager::enable(spi);
#if defined(HAL_RTOS_FreeRTOS)
    	const uint32_t priority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
#else
    	const uint32_t priority = 0;
#endif
        switch (reinterpret_cast<uint32_t>(&spi)) {
        case reinterpret_cast<uint32_t>(SPI1):
            NVIC_EnableIRQ(SPI1_IRQn);
            NVIC_SetPriority(SPI1_IRQn, priority);
            break;
        case reinterpret_cast<uint32_t>(SPI2):
            NVIC_EnableIRQ(SPI2_IRQn);
            NVIC_SetPriority(SPI2_IRQn, priority);
            break;
        case reinterpret_cast<uint32_t>(SPI3):
            NVIC_EnableIRQ (SPI3_IRQn);
            NVIC_SetPriority(SPI3_IRQn, priority);
            break;
#ifdef SPI4_IRQn
            case reinterpret_cast<uint32_t>(SPI4):
            NVIC_EnableIRQ (SPI4_IRQn);
            NVIC_SetPriority(SPI4_IRQn, priority);
            break;
#endif
#ifdef SPI5_IRQn
            case reinterpret_cast<uint32_t>(SPI5):
            NVIC_EnableIRQ (SPI5_IRQn);
            NVIC_SetPriority(SPI5_IRQn, priority);
            break;
#endif
#ifdef SPI6_IRQn
            case reinterpret_cast<uint32_t>(SPI6):
            NVIC_EnableIRQ (SPI6_IRQn);
            NVIC_SetPriority(SPI6_IRQn, priority);
            break;
#endif
        }

        init();
    }

    SPI::Error writeRead(const void *writePtr, void *readPtr, size_t writeLen, size_t readLen);

    //---------------------------------------- functions ----------------------------------------//
    void init();

    static inline void IRQfunction(SPI_dma &object, SPI_TypeDef *spi);
    //----------------------------------------- friends -----------------------------------------//
    friend void DMA1_Stream0_IRQHandler(void);
    friend void DMA1_Stream2_IRQHandler(void);
    friend void DMA1_Stream3_IRQHandler(void);
    friend void DMA1_Stream4_IRQHandler(void);
    friend void DMA1_Stream5_IRQHandler(void);
    friend void DMA1_Stream7_IRQHandler(void);

    friend void DMA2_Stream2_IRQHandler(void);
    friend void DMA2_Stream3_IRQHandler(void);
    friend void DMA2_Stream5_IRQHandler(void);


    friend void SPI1_IRQHandler(void);
    friend void SPI2_IRQHandler(void);
    friend void SPI3_IRQHandler(void);
    friend void SPI4_IRQHandler(void);
    friend void SPI5_IRQHandler(void);
    friend void SPI6_IRQHandler(void);
};

} // namespace stm32f4xx
} // namespace microhal

#endif /* SPI_DMA_STM32F4XX_H_ */
