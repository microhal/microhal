/*
 * clockManager.h
 *
 *  Created on: Mar 3, 2020
 *      Author: pokas
 */

#ifndef _MICROHAL_PORTS_STM32F4XX_CLOCKMANAGER_H_
#define _MICROHAL_PORTS_STM32F4XX_CLOCKMANAGER_H_

#include "../stmCommon/clockManager/adcClock.h"
#include "../stmCommon/clockManager/hse.h"
#include "../stmCommon/clockManager/hsi.h"
#include "../stmCommon/clockManager/lse.h"
#include "../stmCommon/clockManager/lsi.h"

#ifdef STM32F411xE
#include "../stmCommon/clockManager/pll_v2.h"
#else
#include "../stmCommon/clockManager/pll.h"
#endif

#include "../stmCommon/clockManager/sysclk.h"
#include "../stmCommon/clockManager/usbClock.h"

#endif /* _MICROHAL_PORTS_STM32F4XX_CLOCKMANAGER_H_ */
