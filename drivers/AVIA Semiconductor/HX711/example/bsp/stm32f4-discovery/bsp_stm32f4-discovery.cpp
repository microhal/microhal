/*
 * stm32f4_discovery.cpp
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
 */
#include "SPIDevice/SPIDevice.h"
#include "bsp.h"
#include "i2c.h"
#include "microhal.h"

using namespace microhal;
using namespace stm32f4xx;

void hardwareConfig(void) {
    (void)bsp::spi;
    (void)bsp::debugPort;
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    IOManager::routeSerial<3, Txd, stm32f4xx::IOPin::PortD, 8>();
    IOManager::routeSerial<3, Rxd, stm32f4xx::IOPin::PortD, 9>();

    stm32f4xx::IOManager::routeSPI<1, SCK, stm32f4xx::IOPin::PortA, 5>();
    stm32f4xx::IOManager::routeSPI<1, MISO, stm32f4xx::IOPin::PortA, 6>();
    stm32f4xx::IOManager::routeSPI<1, MOSI, stm32f4xx::IOPin::PortA, 7>();

    stm32f4xx::SPI::spi1.init(stm32f4xx::SPI::Mode1, stm32f4xx::SPI::Prescaler256);
    stm32f4xx::SPI::spi1.enable();

    SysTick_Config(168000000 / 1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
