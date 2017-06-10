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
#include <cstdint>
#include "../clockManager.h"
#include "../device/stm32f4xx.h"
#include "../dma_stm32f4xx.h"
#include "../i2c_stm32f4xx.h"
#include "microhal_semaphore.h"

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
    Error writeRead(DeviceAddress address, const uint8_t *write, size_t writeLength, uint8_t *read, size_t readLength) noexcept final;

    Error write(DeviceAddress address, const uint8_t *write, size_t writeLength) noexcept final;
    Error write(DeviceAddress address, const uint8_t *write, size_t writeLength, const uint8_t *writeB, size_t writeBLength) noexcept final;

    Error read(DeviceAddress address, uint8_t *data, size_t length) noexcept final;
    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept final;

 private:
    enum class Mode { Receive = 0x01, Transmit = 0x02, TransmitReceive = 0x04, ReceiveDoubleBuffer = 0x11, TransmitDoubleBuffer = 0x12 };
    //---------------------------------- variables ----------------------------------
    volatile I2C::Error error;
    DMA::Stream &rxStream;
    DMA::Stream &txStream;
    os::Semaphore semaphore;

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
    //---------------------------------- constructors -------------------------------
    I2C_dma(I2C_TypeDef &i2c, DMA::Stream &rxStream, DMA::Stream &txStream)
        : I2C(i2c), error(), rxStream(rxStream), txStream(txStream), semaphore(), transfer() {
        ClockManager::enable(i2c, ClockManager::PowerMode::Normal);
        init();
        const uint32_t priority = 0;
        enableEventInterrupt(priority);
        enableErrorInterrupt(priority);
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
