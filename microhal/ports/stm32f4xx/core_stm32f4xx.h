#ifndef CORE_STM32F4XX_H__
#define CORE_STM32F4XX_H__
/* ************************************************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include "device/stm32f4xx.h"

#include "microhalPortConfig_stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

class Core {
    Core(void) = delete;

 public:
    using Frequency = uint32_t;

    typedef enum : uint32_t { HSI = 0x00, HSE = 0x01, PLLCLK = 0x02 } SYSCLKsource;
    static void flash_latency(uint32_t frequency);
    static bool pll_start(uint32_t crystal, uint32_t frequency);
    /*------------------------------------------------------------------------*/ /**
	 * \brief Enables FPU
	 * \details Enables FPU in Cortex-M4 for both privileged and user mode. This is
	 * done by enabling CP10 and CP11 coprocessors in CPACR register (possible only
	 * when in privileged mode).
	 */ /*-------------------------------------------------------------------------*/

    static void fpu_enable(void) {
        //#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
        SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // set CP10 and CP11 Full Access
        //#warning "fpu dziala"
        //#endif
    }
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif  // CORE_STM32F4XX_H__
