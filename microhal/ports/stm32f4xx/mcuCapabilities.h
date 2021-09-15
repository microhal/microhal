/*
 * mcuCapabilities.h
 *
 *  Created on: Apr 24, 2021
 *      Author: pokas
 */

#ifndef SRC_THIRD_PARTY_MICROHAL_PORTS_STM32F4XX_MCUCAPABILITIES_H_
#define SRC_THIRD_PARTY_MICROHAL_PORTS_STM32F4XX_MCUCAPABILITIES_H_

namespace microhal {
namespace stm32f4xx {
namespace hardwareLimits {

#define _MICROHAL_STM32F4XX_HAS_SPI1 1
#define _MICROHAL_STM32F4XX_HAS_SPI2 1
#define _MICROHAL_STM32F4XX_HAS_SPI3 1
#define _MICROHAL_STM32F4XX_HAS_SPI4 1
#define _MICROHAL_STM32F4XX_HAS_SPI5 1

#define _MICROHAL_STM32F4XX_HAS_USART1 1
#define _MICROHAL_STM32F4XX_HAS_USART2 1
#define _MICROHAL_STM32F4XX_HAS_USART3 1
#define _MICROHAL_STM32F4XX_HAS_USART4 1
#define _MICROHAL_STM32F4XX_HAS_USART6 1

constexpr const uint32_t spiMaxCLKFrequency = 32'000'000;  // in [Hz]

}  // namespace hardwareLimits
}  // namespace stm32f4xx
}  // namespace microhal

#endif /* SRC_THIRD_PARTY_MICROHAL_PORTS_STM32F4XX_MCUCAPABILITIES_H_ */
