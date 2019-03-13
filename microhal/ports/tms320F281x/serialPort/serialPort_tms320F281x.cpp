/*
 * serialPort_tms320F281x.cpp
 *
 *  Created on: Feb 17, 2019
 *      Author: pokas
 */

#ifndef SRC_MICROHAL_PORTS_TMS320F281X_SERIALPORT_TMS320F281X_CPP_
#define SRC_MICROHAL_PORTS_TMS320F281X_SERIALPORT_TMS320F281X_CPP_

#include "serialPort_tms320f281x.h"
#include "DSP281x_Device.h"

namespace microhal {
namespace tms320f281x {


uint32_t getCrystalClockFrequency() {
    const uint32_t clkinMin_Hz = 20000000;
    const uint32_t clkinMax_Hz = 35000000;
    return 28000000;
}

uint32_t getSysClkOutFrequency() {
    uint32_t xclkin = getCrystalClockFrequency();
    uint32_t n = SysCtrlRegs.PLLCR.bit.DIV;
    return (xclkin * n) / 2;
}

uint32_t getLSCPLKFrequency() {
    return getSysClkOutFrequency() / 4;
}

void setBrr(uint16_t brr) {
    uint16_t brrH = brr >> 8;
    uint16_t brrL = brr;
    SciaRegs.SCIHBAUD = brrH;
    SciaRegs.SCILBAUD = brrL;
}

uint32_t getBaudRateFromBrr(uint32_t brr) {
    if (brr == 0) {
        return getLSCPLKFrequency() / 16;
    } else {
        return getLSCPLKFrequency() / ((brr + 1) * 8);
    }
}

uint32_t SerialPort::getBaudRate() const {
    uint_fast16_t brr = ((uint_fast16_t)SciaRegs.SCIHBAUD << 8)  | SciaRegs.SCILBAUD;
    return getBaudRateFromBrr(brr);
}

bool SerialPort::setBaudRate(uint32_t baudrate) {
    const uint32_t lscplkFrequency = getLSCPLKFrequency();
    const uint32_t baudErrorForZeroBrr = abs((lscplkFrequency / 16) - baudrate);
    const uint32_t brr = lscplkFrequency / ((baudrate * 8) - 1);
    const uint32_t baudErrorForBrr = abs(getBaudRateFromBrr(brr) - baudrate);

    if (baudErrorForZeroBrr <= baudErrorForBrr) {
        setBrr(0);
    } else {
        setBrr(brr);
    }

    return getBaudRate();
}

bool SerialPort::open(microhal::IODevice::OpenMode){}

bool SerialPort::setParity(SerialPort::Parity parity) {
    switch(parity) {
    case NoParity:
        SciaRegs.SCICCR.bit.PARITYENA = 0;
        break;
    case EvenParity:
        SciaRegs.SCICCR.bit.PARITY = 1;
        SciaRegs.SCICCR.bit.PARITYENA = 1;
        break;
    case OddParity:
        SciaRegs.SCICCR.bit.PARITY = 0;
        SciaRegs.SCICCR.bit.PARITYENA = 1;
        break;
    default:
        return false;
    }
    return true;
}

bool SerialPort::setStopBits(SerialPort::StopBits stopBits) {
   switch(stopBits){
   case OneStop:
       SciaRegs.SCICCR.bit.STOPBITS = 0;
       break;
   case TwoStop:
       SciaRegs.SCICCR.bit.STOPBITS = 1;
       break;
   default:
       return false;
   }
   return true;
}

bool SerialPort::setDataBits(SerialPort::DataBits dataBits) {
    switch(dataBits) {
    case Data5:
        SciaRegs.SCICCR.bit.SCICHAR = 4;
        break;
    case Data6:
        SciaRegs.SCICCR.bit.SCICHAR = 5;
        break;
    case Data7:
        SciaRegs.SCICCR.bit.SCICHAR = 6;
        break;
    case Data8:
        SciaRegs.SCICCR.bit.SCICHAR = 7;
        break;
    default:
        return false;
    }
    return true;
}

}
}

#endif /* MICROHAL_PORTS_TMS320F281X_SERIALPORT_TMS320F281X_CPP_ */
