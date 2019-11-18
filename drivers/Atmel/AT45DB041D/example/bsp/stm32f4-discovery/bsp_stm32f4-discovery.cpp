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

SPI_interrupt &spi1 = SPI_interrupt::create<1, IOPin::PortA, 6, IOPin::PortA, 7, IOPin::PortA, 5>();

namespace bsp {
namespace detail {
constexpr microhal::IOPin cePin(microhal::stm32f4xx::GPIO::Port::PortE, 3);
constexpr microhal::IOPin resetPin(microhal::stm32f4xx::GPIO::Port::PortE, 3);
constexpr microhal::IOPin wpPin(microhal::stm32f4xx::GPIO::Port::PortE, 3);

stm32f4xx::GPIO ce(cePin, stm32f4xx::GPIO::Direction::Output);
stm32f4xx::GPIO reset(resetPin, stm32f4xx::GPIO::Direction::Output);
stm32f4xx::GPIO wp(wpPin, stm32f4xx::GPIO::Direction::Output);
}  // namespace detail

namespace at45db {

microhal::SPI &spi = spi1;
microhal::GPIO &ce = detail::ce;
microhal::GPIO &reset = detail::reset;
microhal::GPIO &wp = detail::wp;

}  // namespace at45db
}  // namespace bsp

void hardwareConfig(void) {
    (void)bsp::at45db::spi;
    (void)bsp::debugPort;
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    IOManager::routeSerial<3, Txd, stm32f4xx::IOPin::PortD, 8>();
    IOManager::routeSerial<3, Rxd, stm32f4xx::IOPin::PortD, 9>();

    spi1.init(stm32f4xx::SPI::Mode3, stm32f4xx::SPI::Prescaler16);
    spi1.enable();

    SysTick_Config(168000000 / 1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
