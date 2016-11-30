/*
 * clockManager.h
 *
 *  Created on: 20.11.2016
 *      Author: pawel
 */

#ifndef MICROHAL_PORTS_STM32F4XX_CLOCKMANAGER_H_
#define MICROHAL_PORTS_STM32F4XX_CLOCKMANAGER_H_

#include "stm32f4xx.h"

namespace microhal {
namespace stm32f4xx {

class ClockManager {
public:
	ClockManager();
	virtual ~ClockManager();

	//static bool enable(USART_TypeDef &usart);
	static bool enable(USART_TypeDef &usart) {
		uint32_t rccEnableFlag;

		switch (reinterpret_cast<uint32_t>(&usart)) {
		case reinterpret_cast<uint32_t>(USART1):
			rccEnableFlag = RCC_APB2ENR_USART1EN;
			break;
		case reinterpret_cast<uint32_t>(USART2):
			rccEnableFlag = RCC_APB1ENR_USART2EN;
			break;
		case reinterpret_cast<uint32_t>(USART3):
			rccEnableFlag = RCC_APB1ENR_USART3EN;
			break;
		case reinterpret_cast<uint32_t>(UART4):
			rccEnableFlag = RCC_APB1ENR_UART4EN;
			break;
		case reinterpret_cast<uint32_t>(UART5):
			rccEnableFlag = RCC_APB1ENR_UART5EN;
			break;
		case reinterpret_cast<uint32_t>(USART6):
			rccEnableFlag = RCC_APB2ENR_USART6EN;
			break;
		default:
			return false;
		}

		if (reinterpret_cast<uint32_t>(&usart) == reinterpret_cast<uint32_t>(USART1)
				|| reinterpret_cast<uint32_t>(&usart) == reinterpret_cast<uint32_t>(USART6)) {
			RCC->APB2ENR |= rccEnableFlag;
		} else {
			RCC->APB1ENR |= rccEnableFlag;
		}

		return true;
	}
};

}  // namespace stm32f4xx
}  // namespace microhal

#endif /* MICROHAL_PORTS_STM32F4XX_CLOCKMANAGER_H_ */
