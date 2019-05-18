#include "bsp.h"
#include "i2c.h"
#include "microhal.h"

using namespace microhal;
using namespace stm32f4xx;

void hardwareConfig(void) {
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    IOManager::routeSerial<3, Txd, stm32f4xx::IOPin::PortD, 8>();
    IOManager::routeSerial<3, Rxd, stm32f4xx::IOPin::PortD, 9>();

    // STM32F4-Discovery extension board CON1-A
    IOManager::routeSerial<2, Txd, stm32f4xx::IOPin::PortA, 2>();
    IOManager::routeSerial<2, Rxd, stm32f4xx::IOPin::PortA, 3>();

    stm32f4xx::IOManager::routeI2C<2, SDA, stm32f4xx::IOPin::PortB, 11>();
    stm32f4xx::IOManager::routeI2C<2, SCL, stm32f4xx::IOPin::PortB, 10>();

    stm32f4xx::I2C::i2c2.init();
    stm32f4xx::I2C::i2c2.speed(400000, microhal::I2C::Mode::Fast);
    stm32f4xx::I2C::i2c2.enable();

    SysTick_Config(168000000 / 1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
