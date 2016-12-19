/*
 * i2c_pulling_stm32f4xx.h
 *
 *  Created on: 17 kwi 2014
 *      Author: pawel
 */

#ifndef _MICROHAL_I2C_POLLING_STM32F4XX_H_
#define _MICROHAL_I2C_POLLING_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include <cstdint>
#include "../clockManager.h"
#include "../i2c_stm32f4xx.h"
#include "../stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 * CLASS
 */
class I2C_polling : public stm32f4xx::I2C {
 public:
//---------------------------------------- variables ----------------------------------------//
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
    //---------------------------------------- constructors ---------------------------------------
    I2C_polling(I2C_TypeDef &i2c) : I2C(i2c) { ClockManager::enable(i2c); }
    //---------------------------------------- functions ----------------------------------------//
    Error writeRead(DeviceAddress address, const void *write, size_t write_size, void *read, size_t read_size) noexcept final;

    Error write(DeviceAddress deviceAddress, const uint8_t *write, size_t write_size) noexcept final;

    Error write(DeviceAddress deviceAddress, const void *write_data, size_t write_data_size, const void *write_dataB,
                size_t write_data_sizeB) noexcept final;

    Error read(DeviceAddress deviceAddress, uint8_t *data, size_t data_size) noexcept final;
    Error read(uint8_t deviceAddress, uint8_t *data, size_t dataLength, uint8_t *dataB, size_t dataBLength) noexcept final{};

    inline Error write(uint8_t data);
    Error write_implementation(DeviceAddress deviceAddress, const void *write_data, size_t write_data_size, const void *write_dataB,
                               size_t write_data_sizeB);
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // _MICROHAL_I2C_POLLING_STM32F4XX_H_
