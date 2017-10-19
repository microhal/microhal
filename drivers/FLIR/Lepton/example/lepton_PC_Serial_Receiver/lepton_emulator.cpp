/*
 * lepton_emulator.cpp
 *
 *  Created on: 21 lip 2016
 *      Author: pawel
 */


#include <cstdint>

#include "microhal.h"
#include "hostComm.h"
#include "emulator_bsp.h"

#include "picturePacket.h"


using namespace microhal;
using namespace diagnostic;

using namespace std::literals::chrono_literals;

HostComm hostComm(cameraPort, debugPort);


int main() {
	debugPort.write("\n\r------------------- Lepton IR camera emulator -------------------------\n\r");
	diagChannel.setOutputDevice(debugPort);

	if(!cameraPort.open(SerialPort::ReadWrite)) {
		diagChannel << lock << Debug << "Unable to open camera communication port." << unlock;
		return -1;
	}
	cameraPort.setDataBits(SerialPort::Data8);
	cameraPort.setStopBits(SerialPort::OneStop);
	cameraPort.setParity(SerialPort::NoParity);
	cameraPort.setBaudRate(SerialPort::Baud115200);

	// create and run hostComm proc task
	std::thread hostCommThread([](){
		while(1){
			std::this_thread::sleep_for(1ms);
			hostComm.timeProc();
		}
	});

	PicturePacket picture;
	memset(picture.payload().data, 138, sizeof(picture.payload().data));

	uint8_t color = 0;
	while(1) {
		color += 10;
		memset(picture.payload().data, color, sizeof(picture.payload().data));
		hostComm.send(picture);
		std::this_thread::sleep_for(5s);
	}
}
