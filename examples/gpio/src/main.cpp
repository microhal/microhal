/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief      GPIO example main file
 *
 * @authors    Pawel Okas
 * created on: 30-01-2014
 *
 * @copyright Copyright (c) 2014-2021, Pawel Okas
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

// This demo require at least two LED and one button.
// If you want run this example with your's board. You have to create own BSP file.

#include "bsp.h"
#include "microhal.h"

using namespace microhal;
using namespace std::literals::chrono_literals;

int main(void) {
    // Lets configure our LEDs pins as output. We call 'configureAsOutput' method to apply configuration
    // We need to specify output type, it can be:
    // 		- PushPull - present in all MCU
    //		- OpenDrain - present in all MCU
    //		- OpenCollector - it is only available on specific MCUs (rarely available)
    //
    // We need to provide pin pull specification. Pull specification is ignored when GPIO is configured as PushPull.
    // Pull options:
    //		- NoPull
    //		- PullUp
    //		- PullDown
    bsp::greenLed.configureAsOutput(GPIO::OutputType::PushPull, GPIO::PullType::NoPull);
    bsp::redLed.configureAsOutput(GPIO::OutputType::PushPull, GPIO::PullType::NoPull);

    // Lets configure button GPIO as input. We need to call 'configureAsInput' method to apply configuration.
    // We need to provide pin pull specification, possible values
    //		- NoPull - also known as floating pin
    //		- PullUp - internally pulled to positive supply
    //		- PullDown - internally pulled to ground
    bsp::button.configureAsInput(GPIO::PullType::NoPull);

    while (1) {
        std::this_thread::sleep_for(500ms);

        // to change pin state to opposite call toggle() method.
        bsp::greenLed.toggle();

        // to check pin state you can call:
        //		- get()
        if (const auto state = bsp::button.get()) {
            if (state.value() == GPIO::State::High) {
                // you can change pin state by calling:
                //		- set() - change pin state to High, usually VCC
                //		- reset() - change pin state to Low, usually GND
                bsp::redLed.set();
            } else {
                bsp::redLed.reset();
            }
        } else {
            // error while checking pin state, do some stuff to handle this
        }
    }

    return 0;
}
