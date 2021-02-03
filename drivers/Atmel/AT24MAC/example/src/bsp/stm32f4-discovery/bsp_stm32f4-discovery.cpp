#include "bsp.h"
#include "i2c.h"
#include "microhal.h"

using namespace microhal;
using namespace stm32f4xx;

namespace bsp {
bool init() {
    bsp::debugPort.clear();

    bsp::debugPort.setDataBits(microhal::SerialPort::Data8);
    bsp::debugPort.setStopBits(microhal::SerialPort::OneStop);
    bsp::debugPort.setParity(microhal::SerialPort::NoParity);
    bsp::debugPort.open(microhal::SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(microhal::SerialPort::Baud115200);
    return true;
}

void deinit() {}
}  // namespace bsp

extern "C" ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t nbyte) {
    (void)r;     // suppress warning
    (void)file;  // suppress warning

    return bsp::debugPort.write((const char *)buf, nbyte);
}

extern "C" int _isatty(int file) {
    return 1;
}

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
}
