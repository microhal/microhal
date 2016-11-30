#ifndef DATABUS_STM_H_
#define DATABUS_STM_H_

#ifdef MCU_TYPE_STM32F0XX
#include "STM32F0xx/dataBus_STM32F0xx.h"
namespace microhal {
    namespace activePort = stm32f0xx;
}
#elif defined(MCU_TYPE_STM32F4XX)
#include "ports/stm32f4xx/dataBus_stm32f4xx.h"
namespace microhal {
    namespace activePort = stm32f4xx;
}
#else
#error "MCU type must be specified."
#endif

#endif // DATABUS_STM_H_
