/*
 * semaphore.h
 *
 *  Created on: 16 cze 2015
 *      Author: Pawel
 */

#ifndef MICROHAL_SEMAPHORE_H_
#define MICROHAL_SEMAPHORE_H_

#if defined(HAL_RTOS_FreeRTOS)
#include "freeRTOS/semaphore.h"
#elif defined(HAL_RTOS_None)
#include "noRTOS/semaphore_noRTOS.h"
#elif defined(LINUX_PORT)
#include "linux/semaphore_linux.h"
#elif defined(WINDOWS_PORT)
#include "windows/semaphore_windows.h"
#else
#error Operating system have to be defined
#endif

#endif  // MICROHAL_SEMAPHORE_H_
