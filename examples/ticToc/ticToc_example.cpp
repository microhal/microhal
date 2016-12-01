/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      ticToc example main file
 *
 * @authors    Pawel Okas
 * created on: 26-10-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2015, microHAL
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
#include "diagnostic/diagnostic.h"
#include "diagnostic/ticToc.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

void function_with_log() {
    diagChannel << MICROHAL_DEBUG << "Text from function. Starting 5ms delay.";
    std::this_thread::sleep_for(5ms);
    diagChannel << MICROHAL_DEBUG << "End of delay. Returning from function.";
}

void function_with_delay() {
    std::this_thread::sleep_for(5ms);
}

int main(void) {
    // To print some diagnostic logs we need debug IODevice, to see declaration of this device go to 'boards' folder and find your active
    // configuration (i.e. Windows). In this example IODevice instance is called 'debugPort', on Windows it may be connected to console,
    // on STM32 to UART.
    // Now open the diagnostic device.
    debugPort.open(IODevice::WriteOnly);
    // show hello text
    debugPort.write("\n\r------------------- TicToc Demo -------------------------\n\r");

    // To make 'diagChannel' useful you have to set output diagnostic IODevice for it. Below we set diagnostic IODevice to our debugPort.
    // For more information about diagnostic module see diagnostic example.
    diagChannel.setOutputDevice(debugPort);

    // TicToc is used to measure execution time.
    TicToc functionTime;

    // Lets measure execution time of nothing, it should be 0.
    functionTime.tic();
    functionTime.toc();

    diagChannel << MICROHAL_DEBUG << "Only tic and toc function was called so execution time should be equal 0, execution time = " << functionTime << endl;

    // Now lets measure execution time of function with log.
    functionTime.tic();
    function_with_log();
    functionTime.toc();
    // show execution time
    diagChannel << MICROHAL_DEBUG << "Function execution take: " << functionTime << endl;

    diagChannel << MICROHAL_DEBUG << "Calling function with 5ms delay.";
    // call function without log
    functionTime.tic();
    function_with_delay();
    functionTime.toc();
    // show execution time
    diagChannel << MICROHAL_DEBUG << "Function execution take: " << functionTime;

    return 0;
}
