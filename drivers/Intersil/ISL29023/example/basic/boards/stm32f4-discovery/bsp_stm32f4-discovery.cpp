/*
 * stm32f4_discovery.cpp
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
 */
#include "microhal.h"
#include "microhal_bsp.h"
#include "SPIDevice/SPIDevice.h"
#include "i2c.h"

using namespace microhal;
using namespace stm32f4xx;

void hardwareConfig(void) {
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    IOManager::routeSerial<3, Txd, stm32f4xx::GPIO::PortD, 8>();
    IOManager::routeSerial<3, Rxd, stm32f4xx::GPIO::PortD, 9>();

    IOManager::routeI2C<2, SDA, stm32f4xx::GPIO::PortB, 11>();
    IOManager::routeI2C<2, SCL, stm32f4xx::GPIO::PortB, 10>();

    stm32f4xx::I2C::i2c2.init();
    stm32f4xx::I2C::i2c2.setMode(microhal::I2C::Mode::Fast);
    stm32f4xx::I2C::i2c2.enable();

    SysTick_Config(168000000/1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void)
{
	SysTick_time++;
}
