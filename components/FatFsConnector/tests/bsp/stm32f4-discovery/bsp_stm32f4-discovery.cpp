#include "bsp.h"
#include "i2c.h"
#include "microhal.h"

using namespace microhal;
using namespace stm32f4xx;

namespace bsp {

microhal::stm32f4xx::GPIO cs(bsp::con1::a::io3, microhal::GPIO::Direction::Output);
microhal::stm32f4xx::GPIO dc(bsp::con1::a::io4, microhal::GPIO::Direction::Output);
microhal::stm32f4xx::GPIO reset(bsp::con1::a::io2, microhal::GPIO::Direction::Output);
microhal::GPIO &lcd_rst = cs;
microhal::GPIO &lcd_cs = dc;
microhal::GPIO &lcd_rs = dc;
microhal::GPIO &lcd_wr = dc;
microhal::GPIO &lcd_rd = dc;
microhal::GPIO &lcd_d0 = dc;
microhal::GPIO &lcd_d1 = dc;
microhal::GPIO &lcd_d2 = dc;
microhal::GPIO &lcd_d3 = dc;
microhal::GPIO &lcd_d4 = dc;
microhal::GPIO &lcd_d5 = dc;
microhal::GPIO &lcd_d6 = dc;
microhal::GPIO &lcd_d7 = dc;

bool init() {
    bsp::debugPort.clear();

    bsp::debugPort.setDataBits(microhal::SerialPort::Data8);
    bsp::debugPort.setStopBits(microhal::SerialPort::OneStop);
    bsp::debugPort.setParity(microhal::SerialPort::NoParity);
    bsp::debugPort.open(microhal::SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(microhal::SerialPort::Baud115200);

    stm32f4xx::IOManager::routeSPI<1, SCK, stm32f4xx::IOPin::PortA, 5>();
    stm32f4xx::IOManager::routeSPI<1, MISO, stm32f4xx::IOPin::PortA, 6>();
    stm32f4xx::IOManager::routeSPI<1, MOSI, stm32f4xx::IOPin::PortA, 7>();

    IOManager::routeI2C<1, SDA, stm32f4xx::IOPin::PortB, 9>();
    IOManager::routeI2C<1, SCL, stm32f4xx::IOPin::PortB, 8>();

    stm32f4xx::I2C::i2c1.init();
    stm32f4xx::I2C::i2c1.enable();
    return true;
}

void deinit() {}
}  // namespace bsp

extern "C" ssize_t _write_r(struct _reent *r, int file, const void *buf, size_t nbyte) {
    r = r;        // suppress warning
    file = file;  // suppress warning

    return bsp::debugPort.write((const char *)buf, nbyte);
}

extern "C" int main(int, void *);

static void run_main(void *) {
    const char *params[2];
    params[0] = "ISL";
    main(1, params);
    while (1)
        ;
}

void hardwareConfig(void) {
    Core::pll_start(8000000, 168000000);
    Core::fpu_enable();

    IOManager::routeSerial<3, Txd, stm32f4xx::IOPin::PortD, 8>();
    IOManager::routeSerial<3, Rxd, stm32f4xx::IOPin::PortD, 9>();

    TaskHandle_t xHandle = NULL;

    xTaskCreate(run_main, "main", 1500, NULL, tskIDLE_PRIORITY, &xHandle);

    vTaskStartScheduler();
}
