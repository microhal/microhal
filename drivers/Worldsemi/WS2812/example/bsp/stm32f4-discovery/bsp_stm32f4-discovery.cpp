/*
 * stm32f4_discovery.cpp
 *
 *  Created on: 16 kwi 2014
 *      Author: pawel
 */
#include "SPIDevice/SPIDevice.h"
#include "bsp.h"
#include "microhal.h"

using namespace microhal;
using namespace stm32f4xx;
using namespace diagnostic;

namespace bsp {
microhal::stm32f4xx::SPI_dma &spi1 = microhal::stm32f4xx::SPI_dma::create<1, {IOPin::PortA, 6}, {IOPin::PortA, 7}, {IOPin::PortA, 5}>();
microhal::SPI &wsSpi = spi1;
}  // namespace bsp

extern "C" ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t nbyte) {
    (void)r;     // suppress warning
    (void)file;  // suppress warning

    return bsp::debugPort.write((const char *)buf, nbyte);
}

void hardwareConfig(void) {
    (void)bsp::wsSpi;
    Core::pll_start(8000000, 102400000);
    Core::fpu_enable();

    IOManager::routeSerial<3, Txd, stm32f4xx::IOPin::PortD, 8>();
    IOManager::routeSerial<3, Rxd, stm32f4xx::IOPin::PortD, 9>();

    bsp::debugPort.setDataBits(stm32f4xx::SerialPort::Data8);
    bsp::debugPort.setStopBits(stm32f4xx::SerialPort::OneStop);
    bsp::debugPort.setParity(stm32f4xx::SerialPort::NoParity);
    bsp::debugPort.open(stm32f4xx::SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(stm32f4xx::SerialPort::Baud115200);
    diagChannel.setOutputDevice(bsp::debugPort);

    //    IOManager::routeSPI<Spi1, SCK, stm32f4xx::GPIO::PortA, 5>();
    //    IOManager::routeSPI<Spi1, MISO, stm32f4xx::GPIO::PortA, 6>();
    // IOManager::routeSPI<1, MOSI, stm32f4xx::IOPin::PortA, 7>();

    bsp::spi1.init(stm32f4xx::SPI::Mode1, stm32f4xx::SPI::Prescaler8);
    bsp::spi1.enable();

    diagChannel << Notice << "SPI frequency: " << bsp::spi1.speed() << endl;

    SysTick_Config(102400000 / 1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) {
    SysTick_time++;
}
