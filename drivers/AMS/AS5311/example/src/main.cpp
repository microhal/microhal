#include "as5311.h"
#include "bsp.h"
#include "microhal.h"

using namespace microhal;
using namespace diagnostic;
using namespace std::chrono_literals;

int main(void) {
    bsp::debugPort.setDataBits(SerialPort::Data8);
    bsp::debugPort.setStopBits(SerialPort::OneStop);
    bsp::debugPort.setParity(SerialPort::NoParity);
    bsp::debugPort.open(SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(SerialPort::Baud115200);

    bsp::debugPort.write("\n\r------------------- AS5311 Demo -------------------------\n\r");

    diagChannel.setOutputDevice(bsp::debugPort);

    AS5311 as5311(bsp::as5311::spi, bsp::as5311::ce);

    while (1) {
        bool status = as5311.update();
        diagChannel << Debug << "Position: " << as5311.getPosition() << ",\tPosition Valid: " << as5311.isPositionValid()
                    << ", Update successes: " << status << endl;

        std::this_thread::sleep_for(50ms);
    }
}
