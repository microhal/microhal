/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 31-12-2016
 * last modification: 31-12-2016
 *
 * @copyright Copyright (c) 2016 - 2017, Pawel Okas
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include "bsp.h"
#include "ds18b20.h"

using namespace microhal;
using namespace diagnostic;

int main() {
    debugPort.clear();

    debugPort.setDataBits(SerialPort::Data8);
    debugPort.setStopBits(SerialPort::OneStop);
    debugPort.setParity(SerialPort::NoParity);
    debugPort.open(SerialPort::ReadWrite);
    debugPort.setBaudRate(SerialPort::Baud115200);

    oneWirePort.setDataBits(SerialPort::Data8);
    oneWirePort.setStopBits(SerialPort::OneStop);
    oneWirePort.setParity(SerialPort::NoParity);
    oneWirePort.open(SerialPort::ReadWrite);
    oneWirePort.setBaudRate(SerialPort::Baud9600);

    debugPort.write("\n\r------------------- DS18B20 Demo -------------------------\n\r");

    diagChannel.setOutputDevice(debugPort);

    OneWire oneWire(oneWirePort);

    diagChannel << Debug << "Detected 1-Wire device: " << oneWire.sendResetPulse();

    OneWire::Rom rom;
    diagChannel << Debug << "Reading ROM of 1-Wire device: " << oneWire.readRom(&rom) << endl;

    OneWire::Rom searchRom;
    diagChannel << Debug << "Reading ROM of 1-Wire device: " << oneWire.searchRom(&searchRom) << endl;

    diagChannel << Debug << "Device ROM from read: " << rom << endl;
    diagChannel << Debug << "Device ROM from search: " << searchRom << endl;

    DS18B20 ds(oneWire, OneWire::Rom(rom));

    ds.resolution(DS18B20::Resolution::Bits_12);

    while (1) {
        float temperature;
        if (ds.temperature(&temperature))
            diagChannel << Debug << "DS18B20 Temperature: " << temperature << endl;
        else
            diagChannel << Error << "Unable to read DS18B20 temperature." << endl;

        ds.startConversion(false);
        std::this_thread::sleep_for(std::chrono::milliseconds{1000});
    }

    return 0;
}
