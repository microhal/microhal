/*
 * bsp_windows.cpp
 *
 *  Created on: 23 cze 2015
 *      Author: Pawel
 */

#include "serialPort_windows.h"
#include "consoleIODevice.h"
#include "microhal_bsp.h"
#include "microhal.h"

using namespace microhal;
using namespace diagnostic;
// only the sequence of namespaces is proper...

//windows::SerialPort serialPortA("\\\\.\\COM13");
//windows::SerialPort serialPortB("\\\\.\\COM19");
windows::SerialPort serialPortA("\\\\.\\COM1");
windows::SerialPort serialPortB("\\\\.\\COM2");

microhal::IODevice &debugPort = windows::consoleIODev;
microhal::SerialPort &communicationPortA = serialPortA;
microhal::SerialPort &communicationPortB = serialPortB;

bool BSP_Init(void) {

	bool result = false;
	do {
		// setup diagnostic port
		if(!debugPort.open(SerialPort::ReadWrite)) {break;}
		diagChannel.setOutputDevice(debugPort);

		// Port A
		if(!communicationPortA.open(SerialPort::ReadWrite)) {break;}
		if(!communicationPortA.setDataBits(SerialPort::Data8)) {break;}
		if(!communicationPortA.setStopBits(SerialPort::OneStop)){break;}
		if(!communicationPortA.setParity(SerialPort::NoParity)) {break;}
		if(!communicationPortA.setBaudRate(SerialPort::Baud115200, SerialPort::AllDirections)) {break;}

		// Port B
		if(!communicationPortB.open(SerialPort::ReadWrite)){break;}
		if(!communicationPortB.setDataBits(SerialPort::Data8)){break;}
		if(!communicationPortB.setStopBits(SerialPort::OneStop)){break;}
		if(!communicationPortB.setParity(SerialPort::NoParity)){break;}
		if(!communicationPortB.setBaudRate(SerialPort::Baud115200, SerialPort::AllDirections)){break;}

		result = true;
	} while(0);
	return result;
}

bool BSP_Deinit(void) {

	bool result = false;
	do {

		communicationPortA.close();
		communicationPortB.close();

		result = true;
	} while(0);
	return result;
}
