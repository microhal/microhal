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

microhal::SerialPort &serialPort = microhal::stm32f4xx::SerialPort::Serial3;
// ---------------------------- Sensors ----------------------------------------
microhal::SPI &at45db_spi = microhal::stm32f4xx::SPI::spi1;
microhal::GPIO::IOPin at45db_CEpin(microhal::stm32f4xx::GPIO::Port::PortE, 3);
microhal::GPIO::IOPin at45db_RESETpin(microhal::stm32f4xx::GPIO::Port::PortE,
                                      3);
microhal::GPIO::IOPin at45db_WPpin(microhal::stm32f4xx::GPIO::Port::PortE, 3);

// ----------------------------- LEDs
// -------------------------------------------
microhal::GPIO::IOPin Led3(microhal::stm32f4xx::GPIO::Port::PortD, 13);
microhal::GPIO::IOPin Led4(microhal::stm32f4xx::GPIO::Port::PortD, 12);
microhal::GPIO::IOPin Led5(microhal::stm32f4xx::GPIO::Port::PortD, 14);
microhal::GPIO::IOPin Led6(microhal::stm32f4xx::GPIO::Port::PortD, 15);
microhal::GPIO::IOPin Sw1(microhal::stm32f4xx::GPIO::Port::PortA, 0);
microhal::GPIO::IOPin GreenLed = Led4;
microhal::GPIO::IOPin RedLed = Led3;

void hardwareConfig(void) {
  Core::pll_start(8000000, 168000000);
  Core::fpu_enable();

  IOManager::routeSerial<3, Txd, stm32f4xx::GPIO::PortD, 8>();
  IOManager::routeSerial<3, Rxd, stm32f4xx::GPIO::PortD, 9>();

  IOManager::routeSPI<Spi1, SCK, stm32f4xx::GPIO::PortA, 5>();
  IOManager::routeSPI<Spi1, MISO, stm32f4xx::GPIO::PortA, 6>();
  IOManager::routeSPI<Spi1, MOSI, stm32f4xx::GPIO::PortA, 7>();

  stm32f4xx::SPI::spi1.init(stm32f4xx::SPI::Mode3,
                            stm32f4xx::SPI::PRESCALER_16);
  stm32f4xx::SPI::spi1.enable();

  SysTick_Config(168000000 / 1000);
}

uint64_t SysTick_time = 0;

extern "C" void SysTick_Handler(void) { SysTick_time++; }
