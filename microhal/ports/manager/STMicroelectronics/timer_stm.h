/*
 * adc_stm.h
 *
 *  Created on: 12 cze 2014
 *      Author: Dell
 */

#ifndef TIMER_STM_H_
#define TIMER_STM_H_

#ifdef MCU_TYPE_STM32F0XX
#include "STM32F0xx/timer_stm32f0xx.h"
namespace microhal {
    namespace activePort = stm32f0xx;
}
#elif MCU_TYPE_STM32F4XX
#include "STM32F4xx/timer_stm32f4xx.h"
namespace microhal {
    namespace activePort = stm32f4xx;
}
#else
#error "MCU type must be specified."
#endif

#endif // TIMER_STM_H_
