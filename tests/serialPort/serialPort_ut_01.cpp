/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      serial port test
 *
 * @authors    Pawel Okas
 * created on: 5-01-2016
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

#include "microhal.h"

#include "bsp.h"

using namespace microhal;

void configureSerialPort(SerialPort &serial) {
	serial.open(SerialPort::ReadWrite);
	serial.setBaudRate(SerialPort::Baud115200);
	serial.setDataBits(SerialPort::Data8);
	serial.setStopBits(SerialPort::OneStop);
	serial.setParity(SerialPort::NoParity);
}

int main(void) {
	for (auto serial : serialPorts) {
		configureSerialPort(*serial);
	}


    while (1) {
    	for (auto serial : serialPorts) {
    		serial->write("----------------------------- SerialPort Test -----------------------------\n");
    	}
    	std::this_thread::sleep_for(std::chrono::milliseconds {500});
    }

    return 0;
}
