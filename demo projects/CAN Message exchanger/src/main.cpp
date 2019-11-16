/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 *
 * @copyright Copyright (c) 2019, Pawel Okas
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

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"

using namespace std::literals::chrono_literals;

using namespace microhal;
using namespace diagnostic;

void canInit() {
    can1.setBaudrate(30000);
    can1.setMode(CAN::Mode::Normal);
    can1.sleepMode(CAN::Sleep::AutoWakeup);
    can1.addFilter({can::Message::StandardID{10}, can::Filter::Match::DataFrame});
}

void receiveCANMessage() {
    can::Message recMessage;
    if (can1.receive(recMessage)) {
        char buffer[can::Message::maxDataLength];
        auto data = recMessage.getData();
        for (size_t i = 0; i < data.length(); i++) {
            buffer[i] = data[i];
        }
        std::string_view dataString(buffer, data.length_bytes());

        diagChannel << lock << MICROHAL_INFORMATIONAL << "\tMessage have extended ID: " << recMessage.isExtendedID() << endl
                    << "\tMessage ID: " << recMessage.getID() << endl
                    << "\tMessage Data: " << dataString << unlock;
    }
}

int main(int argc, char* const argv[]) {
    int result = -1;

    if (bsp::init()) {
        bsp::debugPort.write("\n\r------------------- CAN Demo -------------------------\n\r");
        diagChannel.setOutputDevice(bsp::debugPort);

        diagChannel << lock << MICROHAL_INFORMATIONAL << "Message exchange" << endl << unlock;

        canInit();
        while (1) {
            uint8_t buffer[8];
            if (auto availableData = bsp::debugPort.read(reinterpret_cast<char*>(buffer), 8, 5s); availableData > 0) {
                can::Message message;
                message.setStandardID(10);
                gsl::span<uint8_t> data(buffer, availableData);
                message.setDataFrame(data);
                diagChannel << lock << MICROHAL_INFORMATIONAL << "Sending Message." << endl << unlock;
                can1.transmit(message);
            }
            receiveCANMessage();
            //    std::this_thread::sleep_for(500ms);
        }

        bsp::deinit();
    } else {
        diagChannel << lock << MICROHAL_EMERGENCY << "Unable to open communication ports." << endl << unlock;
    }

    return result;
}
