/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 18-12-2014
 last modification: <DD-MM-YYYY>

 @copyright Copyright (c) 2014, microHAL
 All rights reserved.
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following
 conditions are met:
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 in the documentation and/or other materials provided with the distribution.
 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
 from this software without specific prior written permission.
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 *//* ========================================================================================================================== */

#include "microhal.h"
#include "diagnostic/diagnostic.h"
#include "microhal_bsp.h"
#include "mpl3115.h"

using namespace microhal;
using namespace diagnostic;

int main(){
	debugPort.clear();

	debugPort.setDataBits(SerialPort::Data8);
	debugPort.setStopBits(SerialPort::OneStop);
	debugPort.setParity(SerialPort::NoParity);
	debugPort.open(SerialPort::ReadWrite);
	debugPort.setBaudRate(SerialPort::Baud115200);

	debugPort.write("\n\r------------------- MPL3115 Demo -------------------------\n\r");

	diagChannel.setOutputDevice(debugPort);

	MPL3115 mpl(sensorI2C);

	mpl.registerDump(diagChannel);

//	for(uint8_t i=0; i<2; i++)
//	{
//		diagChannel << DEBUG << "Initializing MPL3115...";
//		if(mpl.init() == true){
//			diagChannel << Debug << "OK";
//		} else{
//			diagChannel << ERROR << "Cannot initialize MAG3110. Maybe unconnected?";
//		}
//	}

	while(1){

	}

	return 0;
}
