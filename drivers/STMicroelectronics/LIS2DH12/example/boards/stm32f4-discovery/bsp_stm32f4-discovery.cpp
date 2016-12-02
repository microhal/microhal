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

    // I2C on microhal extension board CON1 B
    IOManager::routeI2C<1, SDA, stm32f4xx::GPIO::PortB, 7>(stm32f4xx::GPIO::PullUp);
    IOManager::routeI2C<1, SCL, stm32f4xx::GPIO::PortB, 8>(stm32f4xx::GPIO::PullUp);

    // configure Serial Port interfaces
    stm32f4xx::SerialPort::Serial3.clear();

    stm32f4xx::SerialPort::Serial3.setDataBits(stm32f4xx::SerialPort::Data8);
	stm32f4xx::SerialPort::Serial3.setStopBits(stm32f4xx::SerialPort::OneStop);
	stm32f4xx::SerialPort::Serial3.setParity(stm32f4xx::SerialPort::NoParity);
	stm32f4xx::SerialPort::Serial3.setBaudRate(stm32f4xx::SerialPort::Baud115200);
	stm32f4xx::SerialPort::Serial3.open(stm32f4xx::SerialPort::ReadWrite);

    stm32f4xx::I2C::i2c1.init();
    stm32f4xx::I2C::i2c1.enable();

    SysTick_Config(168000000/1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
	SysTick_time++;
}
