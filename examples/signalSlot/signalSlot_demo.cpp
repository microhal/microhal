/* ========================================================================================================================== *//**
 @license    BSD 3-Clause
 @copyright  microHAL
 @version    $Id$
 @brief      signalSlot demo

 @authors    Pawel Okas
 created on: 26-04-2014
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

#include "microhal.h"
#include "microhal_bsp.h"

#include "signalSlot/signalSlot.h"

using namespace microhal;
using namespace microhal::diagnostic;

void freeFunction() {
    diagChannel << Debug << "Text from free function. No parameter." << endl;
}

void freeFunctionBool(bool b) {
    diagChannel << Debug << "Text from free function with bool parameter. Value: ";
    if (b) {
        diagChannel << Debug << "true" << endl;
    } else {
        diagChannel << Debug << "false" << endl;
    }
}

void freeFunctionBoolInt(bool b, int i) {
    diagChannel << Debug << "Text from free function with bool parameter. Value: ";
    if (b == true) {
        diagChannel << Debug << "true" << endl;
    } else {
        diagChannel << Debug << "false" << endl;
    }
}

class SlotContainer {
    void function() {
        diagChannel << Debug << "Text from class function. No parameter." << endl;
    }

    void function(bool b) {
        diagChannel << Debug << "Text from class function with bool parameter. Value: ";
        if (b == true) {
            diagChannel << Debug << "true" << endl;
        } else {
            diagChannel << Debug << "false" << endl;
        }
    }
public:
    //create slots
    Slot_0<SlotContainer, &SlotContainer::function> slot;
    Slot_1<SlotContainer, bool, &SlotContainer::function> slot2;
};

int main(void) {
	debugPort.open(IODevice::WriteOnly);

	debugPort.write("\n\r------------------- Signal Slot Demo -------------------------\n\r");

    diagChannel.setOutputDevice(debugPort);

    //------------------------------------ create signals -----------------------------------------
    // no parameter signal
    Signal<void> signal_void;
    // signal with one parameter -> bool
    Signal<bool> signal_bool;
    // signal with two parameters -> bool and int
    Signal<bool, int> signal_bool_int;

    //-------------------------------- connecting slots to signal ---------------------------------
    signal_void.connect(freeFunction);
    signal_bool.connect(freeFunctionBool);

    //--------------------------------------- emit signals ----------------------------------------
    signal_void.emit();
    signal_bool.emit(true);

    debugPort.write("\n\radd new slots\n\r\n\r");

    SlotContainer slotContainer;

    signal_void.connect(slotContainer.slot, slotContainer);
    signal_bool.connect(slotContainer.slot2, slotContainer);

    //--------------------------------------- emit signals ----------------------------------------
    signal_void.emit();
    signal_bool.emit(true);

    while (1) {
    }
}
