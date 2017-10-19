///*
// * ds2782_example.cpp
// *
// *  Created on: 16 pa� 2014
// *      Author: Dell
// */
//
#include "microhal.h"
#include "diagnostic/diagnostic.h"
#include "ds2782.h"

#include "bsp.h"
//
//
using namespace microhal;
using namespace diagnostic;
//
int main(void) {
	bsp::debugPort.clear();

	bsp::debugPort.setDataBits(SerialPort::Data8);
	bsp::debugPort.setStopBits(SerialPort::OneStop);
	bsp::debugPort.setParity(SerialPort::NoParity);
	bsp::debugPort.open(SerialPort::ReadWrite);
	bsp::debugPort.setBaudRate(SerialPort::Baud115200);

	bsp::debugPort.write("\n\r------------------- DS2782 Demo -------------------------\n\r");

	diagChannel.setOutputDevice(bsp::debugPort);

	DS2782 ds2782demo(bsp::ds2782::i2c, DS2782::DS2782_ADDR);
	uint8_t foo;
	uint8_t bar[2];
	uint8_t debug1[] = { 12, 34 };

	while (1) {
		if (ds2782demo.readStatus(&foo) == true)
			diagChannel << Debug << "Status: " << foo << endl;
		else
			diagChannel << Debug << "Error" << endl;
		if (ds2782demo.readEEPROMControl(&foo) == true)
			diagChannel << Debug << "EEPROM Control: " << foo << endl;
		else
			diagChannel << Debug << "Error" << endl;
		if (ds2782demo.writeCurrentCalibrationCalibrationRegister(debug1)
				== true)
			diagChannel << Debug << "Current calibration register (write): "
					<< bar[0] << bar[1] << endl;
		else
			diagChannel << Debug << "Error" << endl;
		if (ds2782demo.readCurrentCalibrationCalibrationRegister(bar) == true)
			diagChannel << Debug << "Current calibration register (read): "
					<< bar[0] << bar[1] << endl;
		else
			diagChannel << Debug << "Error" << endl;

		std::this_thread::sleep_for(std::chrono::milliseconds { 1000 });
	}
}
