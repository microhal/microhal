/*
 * clockManager.h
 *
 *  Created on: Mar 3, 2020
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STM32F1XX_CLOCKMANAGER_H_
#define SRC_MICROHAL_PORTS_STM32F1XX_CLOCKMANAGER_H_

#include "mcuCapabilities.h"

#include "../stmCommon/clockManager/adcClock.h"
#include "../stmCommon/clockManager/afioClock.h"
#include "../stmCommon/clockManager/backupRegistersClock.h"
#if _MICROHAL_STM32F1XX_HAS_ETH == 1
#include "../stmCommon/clockManager/ethernetClock.h"
#endif
#include "../stmCommon/clockManager/hse.h"
#include "../stmCommon/clockManager/hsi.h"
#include "../stmCommon/clockManager/lse.h"
#include "../stmCommon/clockManager/lsi.h"
#include "../stmCommon/clockManager/mco.h"
#include "../stmCommon/clockManager/pll.h"
#include "../stmCommon/clockManager/sysclk.h"
#include "../stmCommon/clockManager/usbClock.h"

#endif /* SRC_MICROHAL_PORTS_STM32F1XX_CLOCKMANAGER_H_ */
