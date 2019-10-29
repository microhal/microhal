/* ========================================================================================================================== */ /**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      board support package for stm32f4Discovery board

 @authors    Pawel Okas
 created on: 16-04-2014
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 	 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 	 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 	 	in the documentation and/or other materials provided with the distribution.
 	 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 	 	from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */ /* ==========================================================================================================================
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              */

#include "microhal.h"

#include "bsp.h"

using namespace microhal;
using namespace stm32f4xx;

void hardwareConfig(void) {
    // Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    SysTick_Config(168000000 / 1000);

#if defined(MICROHAL_USE_SERIAL_PORT1_INTERRUPT) || defined(MICROHAL_USE_SERIAL_PORT1_POLLING) || defined(MICROHAL_USE_SERIAL_PORT1_DMA)
    IOManager::routeSerial<1, Txd, stm32f4xx::GPIO::PortA, 9>();
    IOManager::routeSerial<1, Rxd, stm32f4xx::GPIO::PortA, 10>();
#endif

#if defined(MICROHAL_USE_SERIAL_PORT2_INTERRUPT) || defined(MICROHAL_USE_SERIAL_PORT2_POLLING) || defined(MICROHAL_USE_SERIAL_PORT2_DMA)
    IOManager::routeSerial<2, Txd, stm32f4xx::GPIO::PortA, 2>();
    IOManager::routeSerial<2, Rxd, stm32f4xx::GPIO::PortA, 3>();
#endif

#if defined(MICROHAL_USE_SERIAL_PORT3_INTERRUPT) || defined(MICROHAL_USE_SERIAL_PORT3_POLLING) || defined(MICROHAL_USE_SERIAL_PORT3_DMA)
    IOManager::routeSerial<3, Txd, stm32f4xx::GPIO::PortB, 10>();
    IOManager::routeSerial<3, Rxd, stm32f4xx::GPIO::PortB, 11>();
#endif

#if defined(MICROHAL_USE_SERIAL_PORT4_INTERRUPT) || defined(MICROHAL_USE_SERIAL_PORT4_POLLING) || defined(MICROHAL_USE_SERIAL_PORT4_DMA)
    IOManager::routeSerial<4, Txd, stm32f4xx::GPIO::PortC, 10>();
    IOManager::routeSerial<4, Rxd, stm32f4xx::GPIO::PortC, 11>();
#endif

#if defined(MICROHAL_USE_SERIAL_PORT5_INTERRUPT) || defined(MICROHAL_USE_SERIAL_PORT5_POLLING) || defined(MICROHAL_USE_SERIAL_PORT5_DMA)
    IOManager::routeSerial<5, Txd, stm32f4xx::GPIO::PortC, 12>();
    IOManager::routeSerial<5, Rxd, stm32f4xx::GPIO::PortA, 3>();
#endif

#if defined(MICROHAL_USE_SERIAL_PORT6_INTERRUPT) || defined(MICROHAL_USE_SERIAL_PORT6_POLLING) || defined(MICROHAL_USE_SERIAL_PORT7_DMA)
    IOManager::routeSerial<6, Txd, stm32f4xx::GPIO::PortC, 6>();
    IOManager::routeSerial<6, Rxd, stm32f4xx::GPIO::PortC, 7>();
#endif

#if defined(MICROHAL_USE_SERIAL_PORT7_INTERRUPT) || defined(MICROHAL_USE_SERIAL_PORT7_POLLING) || defined(MICROHAL_USE_SERIAL_PORT7_DMA)
    IOManager::routeSerial<7, Txd, stm32f4xx::GPIO::PortE, 8>();
    IOManager::routeSerial<7, Rxd, stm32f4xx::GPIO::PortE, 7>();
#endif

#if defined(MICROHAL_USE_SERIAL_PORT8_INTERRUPT) || defined(MICROHAL_USE_SERIAL_PORT8_POLLING) || defined(MICROHAL_USE_SERIAL_PORT8_DMA)
    IOManager::routeSerial<8, Txd, stm32f4xx::GPIO::PortE, 1>();
    IOManager::routeSerial<8, Rxd, stm32f4xx::GPIO::PortE, 0>();
#endif
}

uint64_t SysTick_time = 0;
;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
