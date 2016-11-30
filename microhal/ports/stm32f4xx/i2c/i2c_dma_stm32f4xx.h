/* =========================================================================================== *//**
 @file        i2cSTM32F4xx.cpp
 @authors     Pawel Okas
 @version     $Id$
 @package
 @brief       I2C implementation for STM32F4xx microcontroler
 @hardware    STM32F4xx
 @copyright   $Copyright$
 @details
 *//* ============================================================================================ */

#ifndef I2C_DMA_STM32F4XX_H_
#define I2C_DMA_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "../stm32f4xx.h"
#include "../i2c_stm32f4xx.h"
#include "../dma_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

extern "C" {
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
class I2C_dma: public stm32f4xx::I2C {
public:
    //---------------------------------------- variables ----------------------------------------//
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
    I2C::Error write(uint8_t deviceAddress, uint8_t data) override final;
    I2C::Error write(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data) override final;
    I2C::Error write(uint8_t deviceAddress, uint8_t registerAddress, const void *data, size_t length) override final;

    I2C::Error read(uint8_t deviceAddress, uint8_t &data) override final;
    I2C::Error read(uint8_t deviceAddress, uint8_t registerAddress, uint8_t &data) override final;
    I2C::Error read(uint8_t deviceAddress, uint8_t registerAddress, void *data, size_t length) override final;
private:
    typedef enum {
        Receive = 0x01,
        ReceiveFromRegister = 0x02,
        Transmit = 0x04
    } Mode;
    //---------------------------------- variables ----------------------------------
    volatile I2C::Error errorSemaphore;
    DMA::Stream &rxStream;
    DMA::Stream &txStream;

    struct {
		Mode mode;
		uint8_t deviceAddress;
		uint8_t registerAddress;
		size_t length;
    } transfer;
    //---------------------------------- constructors -------------------------------
    I2C_dma(I2C_TypeDef &i2c, DMA::Stream & rxStream, DMA::Stream & txStream) :
            I2C(i2c), errorSemaphore(), rxStream(rxStream), txStream(txStream), transfer() {
        init();
        switch (reinterpret_cast<uint32_t>(&i2c)) {
        case reinterpret_cast<uint32_t>(I2C1):
            RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
            NVIC_EnableIRQ (I2C1_EV_IRQn);
            NVIC_SetPriority(I2C1_EV_IRQn, 0);
            NVIC_EnableIRQ (I2C1_ER_IRQn);
            NVIC_SetPriority(I2C1_ER_IRQn, 0);
            break;
        case reinterpret_cast<uint32_t>(I2C2):
            RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
            NVIC_EnableIRQ (I2C2_EV_IRQn);
            NVIC_SetPriority(I2C2_EV_IRQn, 0);
            NVIC_EnableIRQ (I2C2_ER_IRQn);
            NVIC_SetPriority(I2C2_ER_IRQn, 0);
            break;
        case reinterpret_cast<uint32_t>(I2C3):
            RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
            NVIC_EnableIRQ (I2C3_EV_IRQn);
            NVIC_SetPriority(I2C3_EV_IRQn, 0);
            NVIC_EnableIRQ (I2C3_ER_IRQn);
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

} // namespace stm32f4xx
} // namespace microhal
#endif // I2C_DMA_STM32F4XX_H_
