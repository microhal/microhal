/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief      GPIO example main file
 *
 * @authors    Pawel Okas
 * created on: 30-01-2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2014-2018, Pawel Okas
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
#include "diagnostic/ticToc.h"
#include "microhal.h"

using namespace microhal;
using namespace std::literals::chrono_literals;
using namespace diagnostic;

void setSpeedTest() {
    const uint32_t repeatCounter = 10000000;
    uint32_t counter = repeatCounter;
    TicToc executionTime;
    executionTime.callibrate();

    executionTime.tic();
    do {
        bsp::redLed.set();
    } while (counter--);
    executionTime.toc();

    diagChannel << MICROHAL_DEBUG << "execution time of " << repeatCounter << " GPIO set() took " << executionTime;
}

void nativeSetSpeedTest() {
    const uint32_t repeatCounter = 10000000;
    uint32_t counter = repeatCounter;
    TicToc executionTime;
    executionTime.callibrate();

    executionTime.tic();
    do {
        bsp::nativeRedLed.set();
    } while (counter--);
    executionTime.toc();

    diagChannel << MICROHAL_DEBUG << "execution time of " << repeatCounter << " native GPIO set() took " << executionTime;
}

void resetSpeedTest() {
    const uint32_t repeatCounter = 10000000;
    uint32_t counter = repeatCounter;
    TicToc executionTime;
    executionTime.callibrate();

    executionTime.tic();
    do {
        bsp::redLed.reset();
    } while (counter--);
    executionTime.toc();

    diagChannel << MICROHAL_DEBUG << "execution time of " << repeatCounter << " GPIO reset() took " << executionTime;
}

void nativeResetSpeedTest() {
    const uint32_t repeatCounter = 10000000;
    uint32_t counter = repeatCounter;
    TicToc executionTime;
    executionTime.callibrate();

    executionTime.tic();
    do {
        bsp::nativeRedLed.set();
    } while (counter--);
    executionTime.toc();

    diagChannel << MICROHAL_DEBUG << "execution time of " << repeatCounter << " native GPIO reset() took " << executionTime;
}

void getSpeedTest() {
    const uint32_t repeatCounter = 10000000;
    uint32_t counter = repeatCounter;
    TicToc executionTime;
    executionTime.callibrate();

    executionTime.tic();
    do {
        bool status = bsp::button.get();
    } while (counter--);
    executionTime.toc();

    diagChannel << MICROHAL_DEBUG << "execution time of " << repeatCounter << " GPIO get() took " << executionTime;
}

void nativeGetSpeedTest() {
    const uint32_t repeatCounter = 10000000;
    uint32_t counter = repeatCounter;
    TicToc executionTime;
    executionTime.callibrate();

    executionTime.tic();
    do {
        bool status = bsp::nativeButton.get();
    } while (counter--);
    executionTime.toc();

    diagChannel << MICROHAL_DEBUG << "execution time of " << repeatCounter << " native GPIO get() took " << executionTime;
}

void toggleSpeedTest() {
    const uint32_t repeatCounter = 10000000;
    uint32_t counter = repeatCounter;
    TicToc executionTime;
    executionTime.callibrate();

    executionTime.tic();
    do {
        bsp::redLed.toggle();
    } while (counter--);
    executionTime.toc();

    diagChannel << MICROHAL_DEBUG << "execution time of " << repeatCounter << " GPIO toggle() took " << executionTime;
}

void nativeToggleSpeedTest() {
    const uint32_t repeatCounter = 10000000;
    uint32_t counter = repeatCounter;
    TicToc executionTime;
    executionTime.callibrate();

    executionTime.tic();
    do {
        bsp::nativeRedLed.toggle();
    } while (counter--);
    executionTime.toc();

    diagChannel << MICROHAL_DEBUG << "execution time of " << repeatCounter << " native GPIO toggle() took " << executionTime;
}

int main(void) {
    bsp::debugPort.open(IODevice::WriteOnly);
    diagChannel.setOutputDevice(bsp::debugPort);

    diagChannel << Debug << endl << "--------------------------------------------------------" << endl << "\t\tGPIO speed benchmark" << endl;
    setSpeedTest();
    nativeSetSpeedTest();
    resetSpeedTest();
    nativeResetSpeedTest();
    getSpeedTest();
    nativeGetSpeedTest();
    toggleSpeedTest();
    nativeToggleSpeedTest();
    while (1) {
    }

    return 0;
}
