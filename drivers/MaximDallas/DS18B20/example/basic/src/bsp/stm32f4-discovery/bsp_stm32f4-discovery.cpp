#include "bsp.h"
#include "microhal.h"

using namespace microhal;
using namespace stm32f4xx;

stm32f4xx::GPIO oneWireDataPullup({IOPin::PortE, 5}, stm32f4xx::GPIO::Direction::Output, stm32f4xx::GPIO::NoPull, stm32f4xx::GPIO::PushPull);

void hardwareConfig(void) {
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    oneWireDataPullup.set();

    IOManager::routeSerial<3, Txd, stm32f4xx::IOPin::PortD, 8>();
    IOManager::routeSerial<3, Rxd, stm32f4xx::IOPin::PortD, 9>();

    // STM32F4-Discovery extension board CON1-A
    IOManager::routeSerial<2, Txd, stm32f4xx::IOPin::PortA, 2>();
    IOManager::routeSerial<2, Rxd, stm32f4xx::IOPin::PortA, 3>();

    SysTick_Config(168000000 / 1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
