/*
 * gthr-microhal.h
 *
 *  Created on: 2 sty 2015
 *      Author: Dell
 */

#ifndef GTHR_MICROHAL_H_
#define GTHR_MICROHAL_H_

#if defined(HAL_RTOS_FreeRTOS)
#include "freeRTOS/gthr-freeRTOS.h"
#else
#include "noRTOS/empty_port.h"
#endif

#endif /* GTHR_MICROHAL_H_ */
