/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      driver for MRF89XA radio transceiver

 @authors    Pawel
 created on: 12-03-2014
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

#include "bsp.h"
#include "microhal.h"
#include "rfm70.h"

using namespace microhal;
using namespace diagnostic;

volatile bool pendingInterrupt = false;
void rfmInt() {
    pendingInterrupt = true;
}

// interrupt function
void rfmOnInterrupt(RFM70 &rfm) {
    RFM70::Interrupts interrupt;

    diagChannel << Debug << "RFM70: interrupt" << endl;

    rfm.getInterruptSource(interrupt);

    if (interrupt & RFM70::RX_NEW) {
        char buffer[32] = {0};
        if (rfm.getPacket((uint8_t *)buffer)) {
            diagChannel << Notice << buffer << endl;
        } else {
            diagChannel << Warning << "Received packet but unable to read it." << endl;
        }
    }

    if (interrupt & RFM70::TX_DATA_SENT) {
        rfm.switchToRX();
    }

    rfm.clearInterruptFlag(interrupt);
}

int main(void) {
    SerialPort &port = bsp::debugPort;
    port.clear();

    port.setDataBits(SerialPort::Data8);
    port.setStopBits(SerialPort::OneStop);
    port.setParity(SerialPort::NoParity);
    port.open(SerialPort::ReadWrite);
    port.setBaudRate(SerialPort::Baud115200);

    port.write("\n\r------------------- RMF70 Demo -------------------------\n\r");

    diagChannel.setOutputDevice(port);

    // lis302dl ce pin only discovery f4 board
#if defined(BSP_STM32F4DISCOVERY)
    GPIO lisCE(bsp::lis_ce, GPIO::Direction::Output);
    lisCE.set();
#endif

    bsp::led.setDirectionOutput(GPIO::OutputType::PushPull, GPIO::PullType::NoPull);
    bsp::button.setDirectionInput(GPIO::PullType::PullUp);

    RFM70 rfm(bsp::rfm70::spi, bsp::rfm70::csn, bsp::rfm70::ce);

    // initialize rfm device
    if (rfm.init() == false) {
        // call fatal error function
        while (1) {
        }
    }

    ExternalInterrupt::init();

    //    rfm.connectIRQ(rfmInt, GPIO::Port::PortA, (uint8_t) 1);
    microhal::ExternalInterrupt interrupt(bsp::rfm70::irq);
    interrupt.connect(rfmInt, microhal::ExternalInterrupt::Trigger::OnFallingEdge);
    interrupt.enable();

    rfm.enableIRQ();

    int i = 1000;
    while (1) {
        if (i-- == 0) {
            i = 1000;

            bsp::led.toggle();

            // if button pressed
            if (bsp::button.isReset()) {
                diagChannel << Notice << "Sending packet" << endl;
                // switch to tx mode
                rfm.switchToTX();
                // send data
                const uint8_t data[] = "Ala ma kota";
                gsl::span<const uint8_t> packet(data);
                rfm.sendPacket(packet, RFM70::ACK_PIPE_0);
            }
        }
        if (pendingInterrupt) {
            pendingInterrupt = false;
            rfmOnInterrupt(rfm);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }
}
