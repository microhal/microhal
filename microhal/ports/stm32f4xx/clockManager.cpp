/*
 * clockManager.cpp
 *
 *  Created on: 20.11.2016
 *      Author: pawel
 */

#include "ports/stm32f4xx/clockManager.h"

namespace microhal {
namespace stm32f4xx {


ClockManager::ClockManager() {
	// TODO Auto-generated constructor stub

}

ClockManager::~ClockManager() {
	// TODO Auto-generated destructor stub
}

//bool ClockManager::enable(USART_TypeDef &usart) {
//	uint32_t rccEnableFlag;
//
//	switch (reinterpret_cast<uint32_t>(&usart)) {
//	case reinterpret_cast<uint32_t>(USART1):
//		rccEnableFlag = RCC_APB2ENR_USART1EN;
//		break;
//	case reinterpret_cast<uint32_t>(USART2):
//		rccEnableFlag = RCC_APB1ENR_USART2EN;
//		break;
//	case reinterpret_cast<uint32_t>(USART3):
//		rccEnableFlag = RCC_APB1ENR_USART3EN;
//		break;
//	case reinterpret_cast<uint32_t>(UART4):
//		rccEnableFlag = RCC_APB1ENR_UART4EN;
//		break;
//	case reinterpret_cast<uint32_t>(UART5):
//		rccEnableFlag = RCC_APB1ENR_UART5EN;
//		break;
//	case reinterpret_cast<uint32_t>(USART6):
//		rccEnableFlag = RCC_APB2ENR_USART6EN;
//		break;
//	default:
//		return false;
//	}
//
//	if (reinterpret_cast<uint32_t>(&usart) == reinterpret_cast<uint32_t>(USART1)
//			|| reinterpret_cast<uint32_t>(&usart) == reinterpret_cast<uint32_t>(USART6)) {
//		RCC->APB2ENR |= rccEnableFlag;
//	} else {
//		RCC->APB1ENR |= rccEnableFlag;
//	}
//
//	return true;
//}

}  // namespace stm32f4xx
}  // namespace microhal
