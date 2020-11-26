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

#include "IODevice/virtualIODevice.h"
#include "bsp.h"
#include "microhal.h"
#include "ports/linux/General/consoleIODevice_linux.h"
#include "ports/linux/General/serialPort_linux.h"

using namespace microhal;
using namespace diagnostic;

//#define USE_VIRTUALIODEVICE

namespace bsp {
microhal::IODevice &debugPort = linux::consoleIODev;

#ifdef USE_VIRTUALIODEVICE
VirtualIODevice hostcomm1Device;
VirtualIODevice hostcomm2Device;
microhal::IODevice &communicationPortA = hostcomm1Device;
microhal::IODevice &communicationPortB = hostcomm2Device;
#else
microhal::linux::SerialPort serialPortA("/dev/ttyS83");  // socat -d -d pty,raw,echo=0,link=/dev/ttyS83 pty,raw,echo=0,link=/dev/ttyS84
microhal::linux::SerialPort serialPortB("/dev/ttyS84");  //
microhal::IODevice &communicationPortA = serialPortA;
microhal::IODevice &communicationPortB = serialPortB;
#endif

bool init(void) {
#ifdef USE_VIRTUALIODEVICE
    hostcomm1Device.connect(hostcomm2Device);
    hostcomm2Device.connect(hostcomm1Device);
    hostcomm1Device.open(IODevice::ReadWrite);
    hostcomm2Device.open(IODevice::ReadWrite);
    return true;
#else
    bool result = false;
    do {
        // setup diagnostic port
        if (!debugPort.open(SerialPort::ReadWrite)) {
            break;
        }
        diagChannel.setOutputDevice(debugPort);

        // Port A
        if (!serialPortA.open(SerialPort::ReadWrite)) {
            diagChannel << lock << MICROHAL_DEBUG << "Unable to open port A" << unlock;
            break;
        }
        if (!serialPortA.setDataBits(SerialPort::Data8)) {
            diagChannel << lock << MICROHAL_DEBUG << "unable to set data length" << unlock;
            break;
        }
        if (!serialPortA.setStopBits(SerialPort::OneStop)) {
            diagChannel << lock << MICROHAL_DEBUG << "unable to set stop bits" << unlock;
            break;
        }
        if (!serialPortA.setParity(SerialPort::NoParity)) {
            diagChannel << lock << MICROHAL_DEBUG << "unable to set parity" << unlock;
            break;
        }
        if (!serialPortA.setBaudRate(SerialPort::Baud115200)) {
            diagChannel << lock << MICROHAL_DEBUG << "Unable to set baudrate" << unlock;
            break;
        }

        diagChannel << lock << MICROHAL_DEBUG << "Configurnig port B" << unlock;
        // Port B
        if (!serialPortB.open(SerialPort::ReadWrite)) {
            break;
        }
        if (!serialPortB.setDataBits(SerialPort::Data8)) {
            break;
        }
        if (!serialPortB.setStopBits(SerialPort::OneStop)) {
            break;
        }
        if (!serialPortB.setParity(SerialPort::NoParity)) {
            break;
        }
        if (!serialPortB.setBaudRate(SerialPort::Baud115200)) {
            break;
        }

        result = true;
    } while (0);

    return result;
#endif
}

bool deinit(void) {
    bool result = false;
    do {
        communicationPortA.close();
        communicationPortB.close();

        result = true;
    } while (0);
    return result;
}

}  // namespace bsp
