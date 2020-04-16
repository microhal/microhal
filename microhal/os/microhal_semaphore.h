/*
 * semaphore.h
 *
 *  Created on: 16 cze 2015
 *      Author: Pawel
 */

#ifndef MICROHAL_SEMAPHORE_H_
#define MICROHAL_SEMAPHORE_H_

#include "microhalConfig.h"

#if defined(MICROHAL_RTOS_FreeRTOS)
#include "freeRTOS/semaphore.h"
#elif defined(MICROHAL_RTOS_NoRTOS)
#include "noRTOS/semaphore_noRTOS.h"
#elif defined(MICROHAL_RTOS_Linux)
#include "linux/semaphore_linux.h"
#elif defined(MICROHAL_RTOS_Windows)
#include "windows/semaphore_windows.h"
#else
#error Operating system have to be defined
#endif

#endif  // MICROHAL_SEMAPHORE_H_
