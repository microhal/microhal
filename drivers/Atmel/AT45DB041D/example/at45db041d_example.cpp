#include "microhal.h"
#include "microhal_bsp.h"
#include "at45db041d.h"

using namespace microhal;
using namespace diagnostic;

int main(void) {
	 serialPort.setDataBits(SerialPort::Data8);
	 serialPort.setStopBits(SerialPort::OneStop);
	 serialPort.setParity(SerialPort::NoParity);
	 serialPort.open(SerialPort::ReadWrite);
	 serialPort.setBaudRate(SerialPort::Baud115200);

	 serialPort.write("\n\r------------------- AT45DB41D Demo -------------------------\n\r");

	 diagChannel.setOutputDevice(serialPort);

	 GPIO led(GreenLed, GPIO::Direction::Output);

	 AT45DB041D at45(at45db_spi, at45db_CEpin, at45db_RESETpin, at45db_WPpin, AT45DB041D::PageSize::Size_256);

	 at45.init();

	 uint16_t id;
	 if (at45.manufacturerID(id) == false) {
		 diagChannel << Error << "Error in reading manufacturer ID" << endl;
	 } else {
		 diagChannel << Notice << "Manufacturer ID:" << id << endl;
	 }

	 char txt[] = "Ala ma kota";
	 char buffer[100];
	 at45.write(AT45DB041D::BufferNumber::Buffer1, 0x00, txt, sizeof(txt));

	 at45.bufferToMainMemoryWithErase(AT45DB041D::BufferNumber::Buffer1, 0x00);
	 at45.busyWait();
	 at45.continuousArrayRead(0x00, 0x00, buffer, sizeof(txt));

	 diagChannel << Notice << buffer;

	 std::chrono::milliseconds duration(1000);
	 while (1) {
		 std::this_thread::sleep_for(duration);
		 led.toggle();
	 }
}
