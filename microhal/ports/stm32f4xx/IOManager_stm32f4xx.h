/*
 * IOManager_STM32F4xx.h
 *
 *  Created on: 1 kwi 2014
 *      Author: pawel
 */

#ifndef _MICROHAL_IOMANAGER_STM32F4XX_H_
#define _MICROHAL_IOMANAGER_STM32F4XX_H_
/* ************************************************************************************************
 * INCLUDES
 */
#if defined(STM32F405xx) || defined(STM32F407xx)
#include "ioManager_mcu_specific_files/IOManager_stm32f40xxx.h"
#elif defined(STM32F411xE)
#include "ioManager_mcu_specific_files/IOManager_stm32f411xx.h"
#endif

//
//    template <USBPinType usbPinType, IOPin::Port port, IOPin::Pin pinNr>
//    static void routeUSB(stm32f4xx::GPIO::PullType pull = stm32f4xx::GPIO::PullType::NoPull,
//                         stm32f4xx::GPIO::OutputType type = stm32f4xx::GPIO::OutputType::PushPull) {
//        constexpr IOPin pin(port, pinNr);
//        static_assert((usbPinType != OTG_FS_SOF || (port == IOPin::PortA && pinNr == 8)), "USB Fast Speed SOF pin can be connected only to
//        PortA.8"); static_assert((usbPinType != OTG_FS_VBUS || (port == IOPin::PortA && pinNr == 9)),
//                      "USB Fast Speed VBus pin can be connected only to PortA.9");
//        static_assert((usbPinType != OTG_FS_ID || (port == IOPin::PortA && pinNr == 10)), "USB Fast Speed ID pin can be connected only to
//        PortA.10"); static_assert((usbPinType != OTG_FS_DM || (port == IOPin::PortA && pinNr == 11)), "USB Fast Speed DM pin can be connected only
//        to PortA.11"); static_assert((usbPinType != OTG_FS_DP || (port == IOPin::PortA && pinNr == 12)), "USB Fast Speed DP pin can be connected
//        only to PortA.12");
//
//        if (usbPinType == OTG_FS_ID) {
//            pull = stm32f4xx::GPIO::PullType::PullUp;
//            type = GPIO::OutputType::OpenDrain;
//        }
//
//        GPIO gpio(pin);
//        gpio.setAlternateFunction(stm32f4xx::GPIO::AlternateFunction::AF10, pull, type, GPIO::Speed::HighSpeed);
//    }

#endif  // IOMANAGER_STM32F4XX_H_
