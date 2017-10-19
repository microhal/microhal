/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 27-04-2017
 * last modification: 27-04-2017
 *
 * @copyright Copyright (c) 2017, microHAL
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
#include "microhal.h"

#if defined(MICROHAL_USE_SERIAL_PORT1_INTERRUPT) || defined(MICROHAL_USE_SERIAL_PORT1_POLLING) || defined(MICROHAL_USE_SERIAL_PORT1_DMA)
static microhal::SerialPort &serialPort1 = microhal::stm32f4xx::SerialPort::Serial1;
#endif

using namespace microhal;
using namespace std::literals::chrono_literals;

const char txt[] = "test";
char buffer[10];
int main() {
#if defined(MICROHAL_USE_SERIAL_PORT1_INTERRUPT) || defined(MICROHAL_USE_SERIAL_PORT1_POLLING) || defined(MICROHAL_USE_SERIAL_PORT1_DMA)
    serialPort1.open(SerialPort::ReadWrite);
    serialPort1.setBaudRate(SerialPort::Baud115200);
    serialPort1.setDataBits(SerialPort::Data8);
    serialPort1.setStopBits(SerialPort::OneStop);
    serialPort1.setParity(SerialPort::NoParity);

    serialPort1.write(txt);
    serialPort1.read(buffer, sizeof(buffer));
    serialPort1.getChar(buffer[0]);
    serialPort1.outputQueueSize();
    serialPort1.inputQueueSize();
    serialPort1.availableBytes();
    serialPort1.clear(microhal::SerialPort::Direction::Input);
    serialPort1.getBaudRate();
    serialPort1.isOpen();
    serialPort1.read(buffer, sizeof(buffer), 1s);
    serialPort1.waitForWriteFinish(1s);
    serialPort1.close();
#endif
    return 0;
}
