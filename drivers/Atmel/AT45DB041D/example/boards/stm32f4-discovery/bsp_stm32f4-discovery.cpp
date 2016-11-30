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

    IOManager::routeSPI<Spi1, SCK, stm32f4xx::GPIO::PortA, 5>();
    IOManager::routeSPI<Spi1, MISO, stm32f4xx::GPIO::PortA, 6>();
    IOManager::routeSPI<Spi1, MOSI, stm32f4xx::GPIO::PortA, 7>();

    stm32f4xx::SPI::spi1.init(stm32f4xx::SPI::Mode3, stm32f4xx::SPI::PRESCALER_16);
    stm32f4xx::SPI::spi1.enable();

    SysTick_Config(168000000/1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void)
{
	SysTick_time++;
}
