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
void init() {
    (void)bsp::sht21::i2c;
    (void)bsp::debugPort;
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    IOManager::routeSerial<3, Txd, stm32f4xx::IOPin::PortD, 8>();
    IOManager::routeSerial<3, Rxd, stm32f4xx::IOPin::PortD, 9>();

    IOManager::routeI2C<2, SDA, stm32f4xx::IOPin::PortB, 11>();
    IOManager::routeI2C<2, SCL, stm32f4xx::IOPin::PortB, 10>();

    stm32f4xx::I2C::i2c2.init();
    stm32f4xx::I2C::i2c2.speed(100000, microhal::I2C::Mode::Standard);
    stm32f4xx::I2C::i2c2.enable();
}
}  // namespace bsp

extern "C" ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t nbyte) {
    (void)r;     // suppress warning
    (void)file;  // suppress warning

    return bsp::debugPort.write((const char *)buf, nbyte);
}

void hardwareConfig(void) {
    SysTick_Config(168000000 / 1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
