/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      DS2786 driver example
 *
 * @authors    Pawel Okas
 * created on: 2015
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015-2017, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 	   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 * 	      documentation and/or other materials provided with the distribution.
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
#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "ds2786.h"
#include "microhal.h"

using namespace microhal;
using namespace diagnostic;

int main(void) {
    bsp::debugPort.clear();

    bsp::debugPort.setDataBits(SerialPort::Data8);
    bsp::debugPort.setStopBits(SerialPort::OneStop);
    bsp::debugPort.setParity(SerialPort::NoParity);
    bsp::debugPort.open(SerialPort::ReadWrite);
    bsp::debugPort.setBaudRate(SerialPort::Baud115200);

    bsp::debugPort.write("\n\r------------------- DS2786 Demo -------------------------\n\r");

    diagChannel.setOutputDevice(bsp::debugPort);

    DS2786 ds2786(bsp::ds2786::i2c, 500000);

    while (1) {
        if (ds2786.isCapacityLearningEnabled())
            diagChannel << Debug << "Capacity Learning enabled." << endl;
        else
            diagChannel << Debug << "Capacity Learning disabled." << endl;

        if (auto vout = ds2786.isVoutDisabled())
            diagChannel << Debug << "Vout " << (*vout ? "disabled" : "enabled") << endl;
        else
            diagChannel << Debug << "Error while reading Vout status." << endl;

        if (auto aux0 = ds2786.readAxuiliary0())
            diagChannel << Debug << "Value of Auxiliary 0: " << *aux0 << endl;
        else
            diagChannel << Debug << "Auxiliary 0 reading error." << endl;

        if (auto aux1 = ds2786.readAxuiliary1())
            diagChannel << Debug << "Value of Auxiliary 1: " << *aux1 << endl;
        else
            diagChannel << Debug << "Auxiliary 1 reading error: " << aux1.error() << endl;

        if (auto temperature = ds2786.readTemperature())
            diagChannel << Debug << "Die temperature " << *temperature << endl;
        else
            diagChannel << Debug << "Auxiliary 1 reading error: " << temperature.error() << endl;

        if (auto initVoltage = ds2786.readInitialVoltage())
            diagChannel << Debug << "Initial battery voltage: " << *initVoltage << " V" << endl;
        else
            diagChannel << Debug << "Initial battery voltage reading error" << endl;

        if (auto voltage = ds2786.readVoltage())
            diagChannel << Debug << "Battery voltage: " << *voltage << " V" << endl;
        else
            diagChannel << Debug << "Battery voltage reading error." << endl;

        if (auto current = ds2786.readCurrent())
            diagChannel << Debug << "Battery current: " << *current << " uA" << endl;
        else
            diagChannel << Debug << "Battery current reading error." << endl;

        if (auto capacity = ds2786.relativeCapacity())
            diagChannel << Debug << "Battery capacity: " << *capacity << " %" << endl;
        else
            diagChannel << Debug << "Battery capacity reading error." << endl;

        diagChannel << Debug << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds{1000});
    }
}
