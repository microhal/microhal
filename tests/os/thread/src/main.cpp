/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      serial port test
 *
 * @authors    Pawel Okas
 * created on: 21-11-2017
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2017, Pawel Okas
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

#include <thread>

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"
#include "os/os.h"

using namespace std::literals::chrono_literals;
using namespace microhal;
using namespace microhal::diagnostic;

void foo() {
    // simulate expensive operation
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void bar() {
    // simulate expensive operation
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void independentThread() {
    diagChannel << lock << MICROHAL_DEBUG << "Starting concurrent thread." << unlock;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    diagChannel << lock << MICROHAL_DEBUG << "Exiting concurrent thread." << unlock;
}

void threadCaller() {
    diagChannel << lock << MICROHAL_DEBUG << "Starting thread caller." << unlock;
    std::thread t(independentThread);
    t.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    diagChannel << lock << MICROHAL_DEBUG << "Exiting thread caller." << unlock;
}

void testDetach() {
    threadCaller();
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

void testJoin() {
    diagChannel << lock << MICROHAL_DEBUG << "starting first helper..." << unlock;
    std::thread helper1(foo);

    diagChannel << MICROHAL_DEBUG << "starting second helper..." << unlock;
    std::thread helper2(bar);

    diagChannel << lock << MICROHAL_DEBUG << "waiting for helpers to finish..." << unlock;

    helper1.join();
    helper2.join();

    diagChannel << lock << MICROHAL_DEBUG << "done!" << unlock;
}

void testJoinable() {
    std::thread t;
    diagChannel << lock << MICROHAL_DEBUG << "before starting, should be false, joinable: " << t.joinable() << unlock;

    t = std::thread(foo);
    diagChannel << lock << MICROHAL_DEBUG << "after starting, should be true, joinable: " << t.joinable() << unlock;

    t.join();
    diagChannel << lock << MICROHAL_DEBUG << "after joining, should be false, joinable: " << t.joinable() << unlock;
}

void runTest() {
    diagChannel << lock << MICROHAL_DEBUG << "std::thread test" << endl << unlock;
    testJoin();
    testJoinable();
    testDetach();
    diagChannel << lock << MICROHAL_DEBUG << "--- End of tests ---" << endl << unlock;
}

void stackThread() {}

int main(int argc, char *argv[]) {
#if defined(LINUX)
    initialize(argc, argv);
#endif

    debugPort.open(SerialPort::ReadWrite);

    diagChannel.setOutputDevice(debugPort);

    microhal::os::setDefaultStackSize(1024);
    std::thread smallStack(stackThread);
    microhal::os::setDefaultStackSize(4 * 1024);
    std::thread largeStack(stackThread);

    std::thread t(runTest);

#ifdef HAL_RTOS_FreeRTOS
    vTaskStartScheduler();
#else
    t.join();
#endif

    return 0;
}
