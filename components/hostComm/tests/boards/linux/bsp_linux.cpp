/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      board support package for Linux os
 *
 * @authors    Pawel Okas
 * created on: 12-07-2016
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "serialPort_linux.h"
#include "microhal_bsp.h"
#include "microhal.h"
#include "consoleIODevice_linux.h"

using namespace microhal;
using namespace diagnostic;

microhal::IODevice &debugPort = linux::consoleIODev;
microhal::linux::SerialPort serialPortA("/dev/pts/25"); //ttyUSB0");
microhal::linux::SerialPort serialPortB("/dev/pts/26");//ttyUSB1");

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
