/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      externalInterrupt example main file
 *
 * @authors    Pawel
 * created on: 30-01-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014, microHAL
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

#include "microhal.h"
#include "microhal_bsp.h"

using namespace microhal;
using namespace std::literals::chrono_literals;

// ---------------------- create 3 functions that will be on and off the LEDs
void redLedToggleFunction() {
    static GPIO redLed(redLed_pin, GPIO::Direction::Output);

    redLed.toggle();
}

void greenLedToggleFunction() {
    static GPIO greenLed(greenLed_pin, GPIO::Direction::Output);

    greenLed.toggle();
}

void blueLedToggleFunction() {
    static GPIO blueLed(blueLed_pin, GPIO::Direction::Output);

    blueLed.toggle();
}

// ---------------------- create class with slot that will be on and off the LEDs
class LEDcontroller {
    void orangeLedToggleFunction() {
        static GPIO orangeLed(orangeLed_pin, GPIO::Direction::Output);

        orangeLed.toggle();
    }

 public:
    Slot_0<LEDcontroller, &LEDcontroller::orangeLedToggleFunction> orangeLedToogleSlot;
};

int main(void) {
    // initialize external interrupt module
    ExternalInterrupt::init();

    // NOTICE: We are using button as an interrupt source. To see to which pin the button is connected open boards folder
    //         and find microhal_bsp.h for your development board.

    // ------------------ using external interrupts without object -------------------------------------
    // connect function to interrupt
    ExternalInterrupt::connect(redLedToggleFunction, ExternalInterrupt::Trigger::OnFallingEdge, button_pin);
    // enable interrupt
    ExternalInterrupt::enable(button_pin);

    // ------------------ using external interrupts with object ----------------------------------------
    ExternalInterrupt switchInterrupt(button_pin);

    switchInterrupt.connect(greenLedToggleFunction, ExternalInterrupt::Trigger::OnFallingEdge);

    switchInterrupt.connect(blueLedToggleFunction, ExternalInterrupt::Trigger::OnFallingEdge);

    // create object with slot and connect this slot to interrupt signal.
    LEDcontroller object;

    switchInterrupt.connect(object.orangeLedToogleSlot, object, ExternalInterrupt::Trigger::OnFallingEdge);

    while (1) {
    }

    return 0;
}

