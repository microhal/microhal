/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      i2c example file
 *
 * @authors    Pawel Okas
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2018, Pawel Okas
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

#include <cstring>

#include "IOExpander.h"
#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"
#include "slaveIOExpander.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

using steady_clock = std::chrono::steady_clock;

void interfacingWithSlaveDevice(I2C &i2c) {
    // first try to read some data using basic interface
    uint8_t buffer[10];
    i2c.read(0x00, buffer, 2);
}

void master(I2C &master) {
    IOExpander expander(master);
    expander.setGPIODirection(0b1100);
    expander.setGPIOPullup(0b0011);

    uint8_t debouncer = 0;
    auto lastLedChange = steady_clock::now();
    bool ledState = false;
    while (1) {
        uint8_t gpioState;
        if (expander.getGPIOStateInto(gpioState) == IOExpander::Error::None) {
            if (gpioState & 0b0001) {
                greenLed.set();
            } else {
                greenLed.reset();
            }

            if (gpioState & 0b010) {
                debouncer <<= 1;
            } else {
                debouncer <<= 0;
            }

            if (debouncer == 0xFF) {
                redLed.toggle();
            }
        }

        // every 1s toggle LEDs connected to 'IOExpander'
        if (steady_clock::now() - lastLedChange > 1s) {
            lastLedChange = steady_clock::now();
            if (ledState) {
                ledState = false;
                expander.setGPIOState(0b1000);
            } else {
                ledState = true;
                expander.setGPIOState(0b0100);
            }
        }
    }
}

void slave(I2C &i2c) {
    GPIO *gpio[8];
    gpio[0] = new stm32f3xx::GPIO({stm32f3xx::IOPin::PortA, 1}, GPIO::Direction::Input);
    gpio[1] = new stm32f3xx::GPIO({stm32f3xx::IOPin::PortA, 1}, GPIO::Direction::Input);
    gpio[2] = new stm32f3xx::GPIO({stm32f3xx::IOPin::PortA, 1}, GPIO::Direction::Input);
    gpio[3] = new stm32f3xx::GPIO({stm32f3xx::IOPin::PortA, 1}, GPIO::Direction::Input);
    SlaveIOExpander slaveDevice(gpio);
    i2c.addSlave(slaveDevice);
}

int main(void) {
    bsp::init();

    diagChannel.setOutputDevice(bsp::debugPort);

    diagChannel << lock << MICROHAL_DEBUG << "I2C example" << unlock;

    slave(bsp::i2c);
    //  master(bsp::i2c);
    // this example demonstrate usage of I2C API.

    while (1) {
    }

    return 0;
}
