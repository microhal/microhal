/*
 * adc_stm.h
 *
 *  Created on: 12 cze 2014
 *      Author: Dell
 */

#ifndef ADC_STM_H_
#define ADC_STM_H_

#ifdef MCU_TYPE_STM32F0XX
#include "ports/stm32f0xx/adc_stm32f0xx.h"
namespace microhal {
namespace activePort = stm32f0xx;
}
#elif defined(MCU_TYPE_STM32F1XX)
//#include "ports/stm32f1xx/_stm32f1xx.h"
// namespace microhal {
//    namespace activePort = stm32f1xx;
//}
#elif defined(MCU_TYPE_STM32F3XX)
//#include "ports/stm32f3xx/_stm32f3xx.h"
// namespace microhal {
//    namespace activePort = stm32f3xx;
//}
#elif defined(MCU_TYPE_STM32F4XX)
#include "ports/stm32f4xx/adc_stm32f4xx.h"
namespace microhal {
namespace activePort = stm32f4xx;
}
#else
#error "MCU type must be specified."
#endif

#endif /* ADC_STM_H_ */
