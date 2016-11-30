/*
 * i2c_pulling_stm32f4xx.h
 *
 *  Created on: 17 kwi 2014
 *      Author: pawel
 */

#ifndef I2C_POLLING_STM32F4XX_H_
#define I2C_POLLING_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "../i2c_stm32f4xx.h"
#include "../stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 * CLASS
 */
class I2C_polling : public stm32f4xx::I2C {
 public:
//---------------------------------------- variables
//----------------------------------------//
#ifdef MICROHAL_USE_I2C1_POLLING
  static I2C_polling i2c1;
#endif
#ifdef MICROHAL_USE_I2C2_POLLING
  static I2C_polling i2c2;
#endif
#ifdef MICROHAL_USE_I2C3_POLLING
  static I2C_polling i2c3;
#endif
 private:
  //---------------------------------------- constructors
  //---------------------------------------
  I2C_polling(I2C_TypeDef &i2c) : I2C(i2c) {
    switch (reinterpret_cast<uint32_t>(&i2c)) {
      case reinterpret_cast<uint32_t>(I2C1):
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
        break;
      case reinterpret_cast<uint32_t>(I2C2):
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
        break;
      case reinterpret_cast<uint32_t>(I2C3):
        RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
        break;
    }
  }
  //---------------------------------------- functions
  //----------------------------------------//
  I2C::Error write(uint8_t deviceAddress, uint8_t data) override final;
  I2C::Error write(uint8_t deviceAddress, uint8_t registerAddress,
                   uint8_t data) override final;
  I2C::Error write(uint8_t deviceAddress, uint8_t registerAddress,
                   const void *data, size_t length) override final;
  I2C::Error read(uint8_t deviceAddress, uint8_t &data) override final;
  I2C::Error read(uint8_t deviceAddress, uint8_t registerAddress,
                  uint8_t &data) override final;
  I2C::Error read(uint8_t deviceAddress, uint8_t registerAddress, void *data,
                  size_t length) override final;

  inline I2C::Error write(uint8_t data) {
    Error error;
    uint16_t status;

    do {
      status = i2c.SR1;
      error = errorCheckAndClear(&i2c, status);
      if (error != NoError) return error;
    } while (!(status & I2C_SR1_TXE));
    i2c.DR = data;

    return NoError;
  }
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif /* I2C_POLLING_STM32F4XX_H_ */
