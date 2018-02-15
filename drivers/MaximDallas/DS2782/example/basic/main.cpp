///*
// * ds2782_example.cpp
// *
// *  Created on: 16 pa� 2014
// *      Author: Dell
// */
//
#include "diagnostic/diagnostic.h"
#include "ds2782.h"
#include "microhal.h"

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

    while (1) {
        if (ds2782demo.readStatus(&foo) == true)
            diagChannel << Debug << "Status: " << foo << endl;
        else
            diagChannel << Debug << "Error" << endl;
        if (ds2782demo.readEEPROMControl(&foo) == true)
            diagChannel << Debug << "EEPROM Control: " << foo << endl;
        else
            diagChannel << Debug << "Error" << endl;

        uint16_t call = 12 + 34 << 8;
        if (ds2782demo.writeCurrentCalibrationCalibrationRegister(call) == true)
            diagChannel << Debug << "Current calibration register (write): " << call << endl;
        else
            diagChannel << Debug << "Error" << endl;

        uint16_t tmp;
        if (ds2782demo.readCurrentCalibrationCalibrationRegister(&tmp) == true) {
            diagChannel << Debug << "Current calibration register (read): " << tmp << endl;
        } else
            diagChannel << Debug << "Error" << endl;

        std::this_thread::sleep_for(std::chrono::milliseconds{1000});
    }
}
