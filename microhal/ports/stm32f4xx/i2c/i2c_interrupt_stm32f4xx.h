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
#include <stdint.h>
#include "../stm32f4xx.h"
#include "../i2c_stm32f4xx.h"

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
    typedef enum {
        RECEIVE_FROM_REGISTER = 0x02, RECEIVE = 0x01, TRANSMIT = 0x04

    } Mode;
    typedef struct {
        uint8_t deviceAddress;
        uint8_t registerAddress;
        size_t length;
        uint8_t *buffer_ptr;
        Mode mode;
    } Transfer;

    Transfer transfer;

    volatile I2C::Error ErrorSemaphore;
//---------------------------------------- constructors ---------------------------------------
    I2C_interrupt(I2C_TypeDef &i2c) :
            I2C(i2c), transfer(), ErrorSemaphore() {
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
//---------------------------------------- functions ----------------------------------------//
    I2C::Error write(uint8_t deviceAddress, uint8_t data) override final;
    I2C::Error write(uint8_t deviceAddress, uint8_t registerAddress, uint8_t data) override final;
    I2C::Error write(uint8_t deviceAddress, uint8_t registerAddress, const void *data, size_t length) override final;
    I2C::Error read(uint8_t deviceAddress, uint8_t &data) override final;
    I2C::Error read(uint8_t deviceAddress, uint8_t registerAddress, uint8_t &data) override final;
    I2C::Error read(uint8_t deviceAddress, uint8_t registerAddress, void *data, size_t length) override final;

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
