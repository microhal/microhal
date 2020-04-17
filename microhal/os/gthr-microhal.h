/*
 * gthr-microhal.h
 *
 *  Created on: 2 sty 2015
 *      Author: Dell
 */

#ifndef GTHR_MICROHAL_H_
#define GTHR_MICROHAL_H_

#include "microhalConfig.h"

#if defined(MICROHAL_RTOS_FreeRTOS)
#define HAL_RTOS_FreeRTOS
#include "freeRTOS/gthr-freeRTOS.h"
#elif defined(MICROHAL_RTOS_NoRTOS)
#include "noRTOS/empty_port.h"
#else
#error
#endif

#endif /* GTHR_MICROHAL_H_ */
