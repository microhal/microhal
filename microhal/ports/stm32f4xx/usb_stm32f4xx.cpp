/*
 * usb_stm32f4xx.cpp
 *
 *  Created on: 30 maj 2014
 *      Author: Dell
 */

#include "usb_stm32f4xx.h"
#include <thread>

#include "usbd_cdc_core.h"

extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
extern "C" uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);

namespace microhal {

//USB::USB() {
//	// TODO Auto-generated constructor stub
//
//}
//
//USB::~USB() {
//	// TODO Auto-generated destructor stub
//}

} /* namespace microhal */


/**
* @brief  USB_OTG_BSP_mDelay
*          This function provides delay time in milli sec
* @param  msec : Value of delay required in milli sec
* @retval None
*/
extern "C" void USB_OTG_BSP_mDelay (const uint32_t msec)
{
  std::this_thread::sleep_for(std::chrono::milliseconds{msec});
}


extern "C" void OTG_FS_IRQHandler(void)
{
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}
