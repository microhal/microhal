/*
 * externalInterrupt_stm.h
 *
 *  Created on: 9 kwi 2014
 *      Author: pawel
 */

#ifndef EXTERNALINTERRUPT_STM_H_
#define EXTERNALINTERRUPT_STM_H_

#ifdef MCU_TYPE_STM32F0XX
#define _MICROHAL_PORT_HAS_EXTERNAL_INTERRUPT
#include "ports/stmCommon/externalInterrupt_stmCommon.h"
namespace microhal {
namespace activePort = stm32f0xx;
}
#elif defined(MCU_TYPE_STM32F1XX)
#define _MICROHAL_PORT_HAS_EXTERNAL_INTERRUPT
#include "ports/stmCommon/externalInterrupt_stmCommon.h"
namespace microhal {
namespace activePort = stm32f1xx;
}
#elif defined(MCU_TYPE_STM32F3XX)
#define _MICROHAL_PORT_HAS_EXTERNAL_INTERRUPT
#include "ports/stmCommon/externalInterrupt_stmCommon.h"
namespace microhal {
namespace activePort = stm32f3xx;
}
#elif defined(MCU_TYPE_STM32F4XX)
#define _MICROHAL_PORT_HAS_EXTERNAL_INTERRUPT
#include "ports/stmCommon/externalInterrupt_stmCommon.h"
namespace microhal {
namespace activePort = stm32f4xx;
}
#else
#error "MCU type must be specified."
#endif

#endif /* EXTERNALINTERRUPT_STM_H_ */
