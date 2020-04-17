/*
 * driver_version_stmCommon.h
 *
 *  Created on: Apr 15, 2020
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_STMCOMMON_DRIVERCONFIGURATION_STMCOMMON_H_
#define SRC_MICROHAL_PORTS_STMCOMMON_DRIVERCONFIGURATION_STMCOMMON_H_

#ifdef MCU_TYPE_STM32F1XX
#include <ports/stm32f1xx/driver_configuration_stm32f1xx.h>
#endif

#ifdef MCU_TYPE_STM32F3XX
#include <ports/stm32f3xx/driver_configuration_stm32f3xx.h>
#endif

#endif /* SRC_MICROHAL_PORTS_STMCOMMON_DRIVERCONFIGURATION_STMCOMMON_H_ */
