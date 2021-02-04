/*
 * i2c_stm.h
 *
 *  Created on: 9 kwi 2014
 *      Author: pawel
 */

#ifndef I2C_STM_H_
#define I2C_STM_H_

#ifdef MCU_TYPE_STM32F0XX
#include "ports/stm32f0xx/i2c_stm32f0xx.h"
namespace microhal {
namespace activePort = stm32f0xx;
}
#elif defined(MCU_TYPE_STM32F1XX)
#include "ports/stm32f1xx/i2c_stm32f1xx.h"
// namespace microhal {
// namespace activePort = stm32f1xx;
//}
#elif defined(MCU_TYPE_STM32F3XX)
#include "ports/stm32f3xx/i2c_stm32f3xx.h"
namespace microhal {
namespace activePort = stm32f3xx;
}
#elif defined(MCU_TYPE_STM32F4XX)
#include "ports/stm32f4xx/i2c_stm32f4xx.h"
// namespace microhal {
// namespace activePort = stm32f4xx;
//}
#elif defined(MCU_TYPE_STM32G0XX)
//#include "ports/stm32f1xx/i2c_stm32f1xx.h"
// namespace microhal {
// namespace activePort = stm32f1xx;
//}
#else
#error "MCU type must be specified."
#endif

#endif /* I2C_STM_H_ */
