/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      Serial port for Windows

 @authors    Pawel
 created on: 11-08-2015
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
/* ************************************************************************************************
 * INCLUDES
 */
#include "serialPort_windows.h"

namespace microhal {
namespace windows {
/* ************************************************************************************************
 * FUNCTIONS
 */
bool SerialPort::setBaudRate(uint32_t baudRate) noexcept {
	dcbSerialParams.BaudRate=baudRate;

	if(isOpen()) {
    	return SetCommState(hSerial, &dcbSerialParams);
    }
    return true;
}

uint32_t SerialPort::getBaudRate() const noexcept {
    return dcbSerialParams.BaudRate;
}

bool SerialPort::setParity(SerialPort::Parity parity) noexcept {
    switch (parity) {
    case SerialPort::NoParity:
        dcbSerialParams.Parity = NOPARITY;
        break;
    case SerialPort::EvenParity:
        dcbSerialParams.Parity = EVENPARITY;
        break;
    case SerialPort::OddParity:
        dcbSerialParams.Parity = ODDPARITY;
        break;
    default:
        return false;
    }
    if(isOpen()) {
    	return SetCommState(hSerial, &dcbSerialParams);
    }
    return false;
}

bool SerialPort::setStopBits(SerialPort::StopBits stopBits) noexcept {
    switch (stopBits) {
    case SerialPort::OneStop:
    	dcbSerialParams.StopBits = ONESTOPBIT;
        break;
    case SerialPort::TwoStop:
        dcbSerialParams.StopBits = TWOSTOPBITS;
        break;
    default:
        return false;
    }
    if(isOpen()) {
    	return SetCommState(hSerial, &dcbSerialParams);
    }
    return false;
}

bool SerialPort::setDataBits(SerialPort::DataBits dataBits) noexcept {
    //apply new settings
    switch (dataBits) {
    case Data5:
    	dcbSerialParams.ByteSize=5;
        break;
    case Data6:
    	dcbSerialParams.ByteSize=6;
        break;
    case Data7:
    	dcbSerialParams.ByteSize=7;
        break;
    case Data8:
    	dcbSerialParams.ByteSize=8;
        break;
    default:
        return false;
    }
    if(isOpen()) {
    	return SetCommState(hSerial, &dcbSerialParams);
    }
    return false;
}

} // namespace windows
} // namespace microhal
