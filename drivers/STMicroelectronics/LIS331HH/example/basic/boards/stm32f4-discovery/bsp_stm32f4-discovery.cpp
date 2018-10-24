/*
 * stm32f4_discovery.cpp
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
 */
#include "SPIDevice/SPIDevice.h"
#include "i2c.h"
#include "microhal.h"
#include "microhal_bsp.h"

using namespace microhal;
using namespace stm32f4xx;

void hardwareConfig(void) {
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    IOManager::routeSerial<2, Txd, stm32f4xx::IOPin::PortA, 2>();
    IOManager::routeSerial<2, Rxd, stm32f4xx::IOPin::PortA, 3>();

    IOManager::routeI2C<3, SDA, stm32f4xx::IOPin::PortC, 9>(stm32f4xx::GPIO::PullUp);
    IOManager::routeI2C<3, SCL, stm32f4xx::IOPin::PortA, 8>(stm32f4xx::GPIO::PullUp);

    // configure Serial Port interfaces
    stm32f4xx::SerialPort::Serial2.clear();

    stm32f4xx::SerialPort::Serial2.setDataBits(stm32f4xx::SerialPort::Data8);
    stm32f4xx::SerialPort::Serial2.setStopBits(stm32f4xx::SerialPort::OneStop);
    stm32f4xx::SerialPort::Serial2.setParity(stm32f4xx::SerialPort::NoParity);
    stm32f4xx::SerialPort::Serial2.setBaudRate(stm32f4xx::SerialPort::Baud115200);
    stm32f4xx::SerialPort::Serial2.open(stm32f4xx::SerialPort::ReadWrite);

    stm32f4xx::I2C::i2c3.init();
    stm32f4xx::I2C::i2c3.enable();

    SysTick_Config(168000000 / 1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
