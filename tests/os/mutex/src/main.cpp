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
#include "microhal.h"

using namespace std::literals::chrono_literals;
using namespace microhal;

std::mutex mutex;

void mutexFunctionsCheck() {
    debugPort.write("checking std::mutex\n\r");
    std::mutex m;
    m.lock();
    debugPort.write("mutex locked\n\r");
    m.unlock();
    debugPort.write("mutex unlocked\n\r");

    if (m.try_lock()) {
        debugPort.write("mutex try_lock() OK\n\r");
    } else {
        debugPort.write("mutex try_lock() error.\n");
    }

    if (m.try_lock() == false) {
        debugPort.write("mutex try_lock() OK, failed to lock mutex that is already locked -> expected behavior.\n\r");
    } else {
        debugPort.write("mutex try_lock() error, locked again mutex that isn't recursive.\n\r");
    }
    m.unlock();
}

void timed_mutexFunctionsCheck() {
    debugPort.write("checking std::timed_mutex\n\r");
    std::timed_mutex m;
    m.lock();
    debugPort.write("timed_mutex locked\n\r");
    m.unlock();
    debugPort.write("timed_mutex unlocked\n\r");

    if (m.try_lock()) {
        debugPort.write("timed_mutex try_lock() OK\n\r");
    } else {
        debugPort.write("timed_mutex try_lock() error.\n");
    }

    if (m.try_lock() == false) {
        debugPort.write("timed_mutex try_lock() OK, failed to lock mutex that is already locked -> expected behavior.\n\r");
    } else {
        debugPort.write("timed_mutex try_lock() error, locked again mutex that isn't recursive.\n\r");
    }

    m.unlock();

    if (m.try_lock_for(1ms)) {
        debugPort.write("timed_mutex try_lock_for() OK.\n\r");
    } else {
        debugPort.write("timed_mutex try_lock_for() error.\n\r");
    }

    if (m.try_lock_for(1ms) == false) {
        debugPort.write("timed_mutex try_lock_for() OK, failed to lock mutex that is already locked -> expected behavior.\n\r");
    } else {
        debugPort.write("timed_mutex try_lock_for() OK.  \n");
    }

    m.unlock();

    if (m.try_lock_until(std::chrono::steady_clock::now() + 1ms)) {
        debugPort.write("timed_mutex try_lock_until() OK.\n\r");
    } else {
        debugPort.write("timed_mutex try_lock_until() error.\n\r");
    }

    if (m.try_lock_until(std::chrono::steady_clock::now() + 1ms) == false) {
        debugPort.write("timed_mutex try_lock_until() OK, failed to lock mutex that is already locked -> expected behavior.\n\r");
    } else {
        debugPort.write("timed_mutex try_lock_until() OK.  \n\r");
    }
    m.unlock();
}

void recursive_mutexFunctionsCheck() {
    debugPort.write("checking std::recursive_mutex\n\r");
    std::recursive_mutex m;
    m.lock();
    debugPort.write("recursive_mutex locked\n\r");
    m.unlock();
    debugPort.write("recursive_mutex unlocked\n\r");

    if (m.try_lock()) {
        debugPort.write("recursive_mutex try_lock() OK\n\r");
    } else {
        debugPort.write("recursive_mutex try_lock() error.\n");
    }

    if (m.try_lock()) {
        debugPort.write("recursive_mutex try_lock() OK, mutex is recursive so we should be able to lock it many times from the same thread.\n\r");
    } else {
        debugPort.write("recursive_mutex try_lock() error, unable to lock again recursive mutex.\n\r");
    }

    std::thread t([&]() {
        if (m.try_lock() == false) {
            debugPort.write("recursive_mutex try_lock() OK, unable to lock mutex from different thread.\n\r");
        } else {
            debugPort.write("recursive_mutex try_lock() error, mutex was lock from different thread.\n\r");
        }
    });

    m.unlock();

    t.join();
}

void recursive_timed_mutexFunctionsCheck() {
    debugPort.write("checking std::recursive_timed_mutex\n\r");
    std::recursive_timed_mutex m;
    m.lock();
    debugPort.write("recursive_timed_mutex locked\n\r");
    m.unlock();
    debugPort.write("recursive_timed_mutex unlocked\n\r");

    ////////////// testing try_lock()
    if (m.try_lock()) {
        debugPort.write("recursive_timed_mutex try_lock() OK\n\r");
    } else {
        debugPort.write("recursive_timed_mutex try_lock() error.\n");
    }

    if (m.try_lock()) {
        debugPort.write(
            "recursive_timed_mutex try_lock() OK, mutex is recursive so we should be able to lock it many times from the same thread.\n\r");
    } else {
        debugPort.write("recursive_timed_mutex try_lock() error, unable to lock again recursive mutex.\n\r");
    }

    std::thread t([&]() {
        if (m.try_lock() == false) {
            debugPort.write("recursive_timed_mutex try_lock() OK, unable to lock mutex from different thread.\n\r");
        } else {
            debugPort.write("recursive_timed_mutex try_lock() error, mutex was lock from different thread.\n\r");
        }
    });

    m.unlock();
    ////////////// testing try_lock_for()
    if (m.try_lock_for(1ms)) {
        debugPort.write("timed_mutex try_lock_for() OK.\n\r");
    } else {
        debugPort.write("timed_mutex try_lock_for() error.\n\r");
    }

    if (m.try_lock_for(1ms) == false) {
        debugPort.write("timed_mutex try_lock_for() OK, failed to lock mutex that is already locked -> expected behavior.\n\r");
    } else {
        debugPort.write("timed_mutex try_lock_for() OK.  \n");
    }

    std::thread t2([&]() {
        if (m.try_lock_for(1ms) == false) {
            debugPort.write("recursive_timed_mutex try_lock() OK, unable to lock mutex from different thread.\n\r");
        } else {
            debugPort.write("recursive_timed_mutex try_lock() error, mutex was lock from different thread.\n\r");
        }
    });

    m.unlock();
    ////////////// testing try_lock_until()
    if (m.try_lock_until(std::chrono::steady_clock::now() + 1ms)) {
        debugPort.write("timed_mutex try_lock_until() OK.\n\r");
    } else {
        debugPort.write("timed_mutex try_lock_until() error.\n\r");
    }

    if (m.try_lock_until(std::chrono::steady_clock::now() + 1ms) == false) {
        debugPort.write("timed_mutex try_lock_until() OK, failed to lock mutex that is already locked -> expected behavior.\n\r");
    } else {
        debugPort.write("timed_mutex try_lock_until() OK.  \n\r");
    }

    std::thread t3([&]() {
        if (m.try_lock_until(std::chrono::steady_clock::now() + 1ms) == false) {
            debugPort.write("recursive_timed_mutex try_lock() OK, unable to lock mutex from different thread.\n\r");
        } else {
            debugPort.write("recursive_timed_mutex try_lock() error, mutex was lock from different thread.\n\r");
        }
    });
    m.unlock();
}

void runTests() {
    mutexFunctionsCheck();
    timed_mutexFunctionsCheck();
    recursive_mutexFunctionsCheck();
    debugPort.write("--- End of tests ---\n\r");
}

int main(int argc, char *argv[]) {
#if defined(LINUX)
    initialize(argc, argv);
#endif

    debugPort.open(SerialPort::ReadWrite);

    std::thread thread(runTests);

#ifdef HAL_RTOS_FreeRTOS
    vTaskStartScheduler();
#else
    t.join();
#endif

    return 0;
}
