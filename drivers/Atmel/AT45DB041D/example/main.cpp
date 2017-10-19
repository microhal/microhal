#include "at45db041d.h"
#include "bsp.h"
#include "microhal.h"

using namespace microhal;
using namespace diagnostic;

int main(void) {
    bsp::debugPort.setDataBits(SerialPort::Data8);
    bsp::debugPort.setStopBits(SerialPort::OneStop);
    bsp::debugPort.setParity(SerialPort::NoParity);
    bsp::debugPort.open(SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(SerialPort::Baud115200);

    bsp::debugPort.write("\n\r------------------- AT45DB41D Demo -------------------------\n\r");

    diagChannel.setOutputDevice(bsp::debugPort);

    AT45DB041D at45(bsp::at45db::spi, bsp::at45db::ce, bsp::at45db::reset, bsp::at45db::wp, AT45DB041D::PageSize::Size_256);

    at45.init();

    uint16_t id;
    if (at45.manufacturerID(id) == false) {
        diagChannel << Error << "Error in reading manufacturer ID" << endl;
    } else {
        diagChannel << Notice << "Manufacturer ID:" << id << endl;
    }

    char txt[] = "Ala ma kota i psa";
    char buffer[100] = {};
    at45.bufferWrite(AT45DB041D::BufferNumber::Buffer1, 0x00, txt, sizeof(txt));

    at45.bufferToMainMemoryWithErase(AT45DB041D::BufferNumber::Buffer1, 0x00);
    at45.busyWait();
    at45.continuousArrayRead(0x00, 0x00, buffer, sizeof(txt));

    diagChannel << Notice << buffer;

    std::chrono::milliseconds duration(1000);
    while (1) {
        std::this_thread::sleep_for(duration);
    }
}
