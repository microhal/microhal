/*
 * pll.h
 *
 *  Created on: Feb 6, 2021
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_PLL_H_
#define _MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_PLL_H_

#include "../driverConfiguration_stmCommon.h"

#if _MICROHAL_PORT_STM_CLOCK_MANAGER_PLL_DRIVER_VERSION == 1
#include "pll_v1.h"
#elif _MICROHAL_PORT_STM_CLOCK_MANAGER_PLL_DRIVER_VERSION == 2
#include "pll_v2.h"
#else
#error _MICROHAL_PORT_STM_CLOCK_MANAGER_PLL_DRIVER_VERSION Has to be defined inside port specific driver configuration file
#endif

#endif /* _MICROHAL_PORTS_STMCOMMON_CLOCKMANAGER_PLL_H_ */
