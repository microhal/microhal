///*
// * ds2786_example.cpp
// *
// *  Created on: 16 paü 2014
// *      Author: Dell
// */
//
#include "microhal.h"
#include "diagnostic/diagnostic.h"
#include "ds2786.h"
#include "microhal_bsp.h"
//
//
using namespace microhal;
using namespace diagnostic;
//
int main(void) {
	debugPort.clear();

	debugPort.setDataBits(SerialPort::Data8);
	debugPort.setStopBits(SerialPort::OneStop);
	debugPort.setParity(SerialPort::NoParity);
	debugPort.open(SerialPort::ReadWrite);
	debugPort.setBaudRate(SerialPort::Baud115200);

	debugPort.write("\n\r------------------- DS2786 Demo -------------------------\n\r");

    diagChannel.setOutputDevice(debugPort);


    ds2786::DS2786 ds2786demo(sensorI2C, ds2786::DS2786::DS2786_ADDR);
    uint8_t foo;
    uint8_t bar[2];

	while (1) {

		   if(ds2786demo.readConfig(&foo) == true)
		   		diagChannel << Debug << "Register: " << foo << endl;
		   else
		   		diagChannel << Debug << "Error" << endl;
		   if(ds2786demo.checkCapacityLearning() == true)
			   diagChannel << Debug << "Capacity on" << endl;
		   else
			   diagChannel << Debug << "Capacity off" << endl;
		   if(ds2786demo.readAxuiliary0(bar) == true)
			   diagChannel << Debug << "Value of Axuiliary 0 is " << bar[0] << " " << bar[1] << endl;
		   else
			   diagChannel << Debug << "Axuiliary 0 error" << endl;
		   if(ds2786demo.readVoltage(bar) == true)
			   diagChannel << Debug << "Value of Voltage is " << bar[0] << " " << bar[1] << endl;
		   else
			   diagChannel << Debug << "Voltage error" << endl;

	   std::this_thread::sleep_for(std::chrono::milliseconds {1000});
	}
}
