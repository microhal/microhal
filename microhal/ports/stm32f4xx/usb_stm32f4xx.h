/*
 * usb_stm32f4xx.h
 *
 *  Created on: 30 maj 2014
 *      Author: Dell
 */

#ifndef USB_STM32F4XX_H_
#define USB_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#include "stm32f4xx.h"
#include "microhalPortConfig_stm32f4xx.h"


namespace microhal {
namespace stm32f4xx {
/* ************************************************************************************************
 * CLASS
 */
/**
 * @addtogroup stm32f4xx_port
 * @{
 * @class USB
 * @}
 * \brief   This class implements USB functions.
 */
class USB {
public:
	USB();
	virtual ~USB();

	static void enable() {
		RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;
		NVIC_SetPriority(OTG_FS_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 3));
		NVIC_EnableIRQ(OTG_FS_IRQn);
	}
};

} // namespace stm32f4xx
} // namespace microhal

#endif /* USB_STM32F4XX_H_ */
