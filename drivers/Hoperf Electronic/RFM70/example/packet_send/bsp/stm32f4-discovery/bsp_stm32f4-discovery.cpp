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

namespace bsp {
namespace detail {
microhal::stm32f4xx::GPIO led(bsp::Led3);
microhal::stm32f4xx::GPIO button(bsp::Sw1);
}  // namespace detail

microhal::GPIO &led = detail::led;
microhal::GPIO &button = detail::button;

namespace rfm70 {
namespace detail {
microhal::stm32f4xx::GPIO csn(con1::a::io2);
microhal::stm32f4xx::GPIO ce(con1::a::io1);
}  // namespace detail
microhal::GPIO &csn = detail::csn;
microhal::GPIO &ce = detail::ce;
}  // namespace rfm70
}  // namespace bsp

void hardwareConfig(void) {
    (void)bsp::rfm70::spi;
    (void)bsp::debugPort;
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    stm32f4xx::IOManager::routeSerial<3, Txd, stm32f4xx::IOPin::PortD, 8>();
    stm32f4xx::IOManager::routeSerial<3, Rxd, stm32f4xx::IOPin::PortD, 9>();

    stm32f4xx::IOManager::routeSPI<Spi1, SCK, stm32f4xx::IOPin::PortA, 5>();
    stm32f4xx::IOManager::routeSPI<Spi1, MISO, stm32f4xx::IOPin::PortA, 6>();
    stm32f4xx::IOManager::routeSPI<Spi1, MOSI, stm32f4xx::IOPin::PortA, 7>();

    stm32f4xx::SPI::spi1.init(stm32f4xx::SPI::Mode0, stm32f4xx::SPI::Prescaler128);
    stm32f4xx::SPI::spi1.enable();

    SysTick_Config(168000000 / 1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
