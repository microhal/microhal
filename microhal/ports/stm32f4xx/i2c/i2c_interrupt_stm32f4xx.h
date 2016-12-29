/*
 * i2c_interrupt_stm32f4xx.h
 *
 *  Created on: 17 kwi 2014
 *      Author: pawel
 */

#ifndef I2C_INTERRUPT_STM32F4XX_H_
#define I2C_INTERRUPT_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "../stm32f4xx.h"
#include "../i2c_stm32f4xx.h"
#include "../clockManager.h"

namespace microhal {
namespace stm32f4xx {

class I2C_interrupt: public stm32f4xx::I2C {
public:
    //---------------------------------------- variables ----------------------------------------//
#ifdef MICROHAL_USE_I2C1_INTERRUPT
    static I2C_interrupt i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_INTERRUPT
    static I2C_interrupt i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_INTERRUPT
    static I2C_interrupt i2c3;
#endif
private:
    enum class Mode { Receive = 0x01, Transmit = 0x02, TransmitReceive = 0x04, ReceiveDoubleBuffer = 0x11, TransmitDoubleBuffer = 0x12 };
    struct Buffer {
    	uint8_t *ptr;
    	size_t length;
    };
    typedef struct {
        uint8_t deviceAddress;
        Buffer bufferA;
        Buffer bufferB;
        Mode mode;
    } Transfer;

    Transfer transfer;

    volatile I2C::Error ErrorSemaphore;
//---------------------------------------- constructors ---------------------------------------
    I2C_interrupt(I2C_TypeDef &i2c) :
            I2C(i2c), transfer(), ErrorSemaphore() {
    	ClockManager::enable(i2c);

        switch (reinterpret_cast<uint32_t>(&i2c)) {
        case reinterpret_cast<uint32_t>(I2C1):
            NVIC_EnableIRQ (I2C1_EV_IRQn);
            NVIC_SetPriority(I2C1_EV_IRQn, 0);
            NVIC_EnableIRQ (I2C1_ER_IRQn);
            NVIC_SetPriority(I2C1_ER_IRQn, 0);
            break;
        case reinterpret_cast<uint32_t>(I2C2):
            NVIC_EnableIRQ (I2C2_EV_IRQn);
            NVIC_SetPriority(I2C2_EV_IRQn, 0);
            NVIC_EnableIRQ (I2C2_ER_IRQn);
            NVIC_SetPriority(I2C2_ER_IRQn, 0);
            break;
        case reinterpret_cast<uint32_t>(I2C3):
            NVIC_EnableIRQ (I2C3_EV_IRQn);
            NVIC_SetPriority(I2C3_EV_IRQn, 0);
            NVIC_EnableIRQ (I2C3_ER_IRQn);
            NVIC_SetPriority(I2C3_ER_IRQn, 0);
            break;
        }
    }
//---------------------------------------- functions ----------------------------------------//
    Error writeRead(DeviceAddress address, const uint8_t *write, size_t writeLength, uint8_t *read_, size_t readLength) noexcept final;

    Error write(DeviceAddress deviceAddress, const uint8_t *data, size_t length) noexcept final;
    Error write(DeviceAddress deviceAddress, const uint8_t *data, size_t dataLength, const uint8_t *dataB, size_t dataBLength) noexcept final;

    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t length) noexcept final;
    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept final;

    static void IRQFunction(I2C_interrupt &obj, I2C_TypeDef *i2c);
//------------------------------------------- friends -----------------------------------------
    friend void I2C1_ER_IRQHandler(void);
    friend void I2C1_EV_IRQHandler(void);
    friend void I2C2_ER_IRQHandler(void);
    friend void I2C2_EV_IRQHandler(void);
    friend void I2C3_ER_IRQHandler(void);
    friend void I2C3_EV_IRQHandler(void);
}
;

} // namsepace stm32f4xx
} // namespace microhal

#endif /* I2C_INTERRUPT_STM32F4XX_H_ */
