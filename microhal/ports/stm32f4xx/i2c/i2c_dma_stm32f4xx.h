/* =========================================================================================== */ /**
 @file        i2cSTM32F4xx.cpp
 @authors     Pawel Okas
 @version     $Id$
 @package
 @brief       I2C implementation for STM32F4xx microcontroler
 @hardware    STM32F4xx
 @copyright   $Copyright$
 @details
 */ /* ============================================================================================
                                                                                                                                                                                                                                                                                                                */

#ifndef I2C_DMA_STM32F4XX_H_
#define I2C_DMA_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "../clockManager.h"
#include "../dma_stm32f4xx.h"
#include "../i2c_stm32f4xx.h"
#include "../stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

extern "C" {
void I2C1_EV_IRQHandler(void);
void I2C2_EV_IRQHandler(void);
void I2C3_EV_IRQHandler(void);

void I2C1_ER_IRQHandler(void);
void I2C2_ER_IRQHandler(void);
void I2C3_ER_IRQHandler(void);

void DMA1_Stream0_IRQHandler(void);
void DMA1_Stream2_IRQHandler(void);
void DMA1_Stream3_IRQHandler(void);
void DMA1_Stream4_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);
void DMA1_Stream7_IRQHandler(void);
}
/* ************************************************************************************************
 * CLASS
 */
class I2C_dma : public stm32f4xx::I2C {
 public:
//---------------------------------------- variables
//----------------------------------------//
#ifdef MICROHAL_USE_I2C1_DMA
    static I2C_dma i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_DMA
    static I2C_dma i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_DMA
    static I2C_dma i2c3;
#endif
 protected:
    I2C::Error writeRead(DeviceAddress deviceAddress, const void *write_data, size_t write_data_size, void *read_data,
                         size_t read_data_size) noexcept final;

    I2C::Error write(DeviceAddress deviceAddress, const uint8_t *write_data, size_t write_data_size) noexcept final;
    Error write(DeviceAddress deviceAddress, const void *write_data, size_t write_data_size, const void *write_dataB,
                size_t write_data_sizeB) noexcept final;

    I2C::Error read(DeviceAddress deviceAddress, uint8_t *read_data, size_t read_data_size) noexcept final;
    Error read(uint8_t deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept final{};

//    microhal::I2C::Error write(microhal::I2C::DeviceAddress, uint8_t);
 //   microhal::I2C::Error read(uint8_t, uint8_t &);

 private:
    enum class Mode { Receive = 0x01, Transmit = 0x02, TransmitReceive = 0x04 };
    //---------------------------------- variables
    //----------------------------------
    volatile I2C::Error errorSemaphore;
    DMA::Stream &rxStream;
    DMA::Stream &txStream;

    struct Buffer {
    	void *ptr;
    	size_t length;
    };
    struct {
        Mode mode;
        DeviceAddress deviceAddress;
        size_t txLength;
        size_t rxLength;
        Buffer bufferB;
    } transfer;
    //---------------------------------- constructors
    //-------------------------------
    I2C_dma(I2C_TypeDef &i2c, DMA::Stream &rxStream, DMA::Stream &txStream)
        : I2C(i2c), errorSemaphore(), rxStream(rxStream), txStream(txStream), transfer() {
        init();
        ClockManager::enable(i2c);
        switch (reinterpret_cast<uint32_t>(&i2c)) {
            case reinterpret_cast<uint32_t>(I2C1):
                // RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
                NVIC_EnableIRQ(I2C1_EV_IRQn);
                NVIC_SetPriority(I2C1_EV_IRQn, 0);
                NVIC_EnableIRQ(I2C1_ER_IRQn);
                NVIC_SetPriority(I2C1_ER_IRQn, 0);
                break;
            case reinterpret_cast<uint32_t>(I2C2):
                // RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
                NVIC_EnableIRQ(I2C2_EV_IRQn);
                NVIC_SetPriority(I2C2_EV_IRQn, 0);
                NVIC_EnableIRQ(I2C2_ER_IRQn);
                NVIC_SetPriority(I2C2_ER_IRQn, 0);
                break;
            case reinterpret_cast<uint32_t>(I2C3):
                // RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
                NVIC_EnableIRQ(I2C3_EV_IRQn);
                NVIC_SetPriority(I2C3_EV_IRQn, 0);
                NVIC_EnableIRQ(I2C3_ER_IRQn);
                NVIC_SetPriority(I2C3_ER_IRQn, 0);
                break;
        }
    }

    //---------------------------------- functions ----------------------------------
    void init(void);
    static void IRQFunction(I2C_dma &obj, I2C_TypeDef *i2c);
    static void IRQErrorFunction(I2C_dma &obj, I2C_TypeDef *i2c);
    //---------------------------------- friends ------------------------------------

    friend void I2C1_EV_IRQHandler(void);
    friend void I2C2_EV_IRQHandler(void);
    friend void I2C3_EV_IRQHandler(void);

    friend void I2C1_ER_IRQHandler(void);
    friend void I2C2_ER_IRQHandler(void);
    friend void I2C3_ER_IRQHandler(void);

    friend void DMA1_Stream0_IRQHandler(void);
    friend void DMA1_Stream2_IRQHandler(void);
    friend void DMA1_Stream3_IRQHandler(void);
    friend void DMA1_Stream4_IRQHandler(void);
    friend void DMA1_Stream5_IRQHandler(void);
    friend void DMA1_Stream6_IRQHandler(void);
    friend void DMA1_Stream7_IRQHandler(void);
};

}  // namespace stm32f4xx
}  // namespace microhal
#endif  // I2C_DMA_STM32F4XX_H_
