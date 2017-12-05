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

#include <future>
#include <thread>

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"
#include "os/os.h"

using namespace std::literals::chrono_literals;
using namespace microhal;
using namespace microhal::diagnostic;

void example() {
    // future from a packaged_task
    std::packaged_task<int()> task([] { return 7; });  // wrap the function
    std::future<int> f1 = task.get_future();           // get a future
    std::thread t(std::move(task));                    // launch on a thread

    // future from an async()
    std::future<int> f2 = std::async(std::launch::async, [] { return 8; });

    // future from a promise
    std::promise<int> p;
    std::future<int> f3 = p.get_future();
    std::thread([&p] { p.set_value_at_thread_exit(9); }).detach();

    diagChannel << lock << MICROHAL_DEBUG << "Waiting..." << unlock;
    std::this_thread::yield();
    f1.wait();
    std::this_thread::yield();
    f2.wait();
    f3.wait();
    diagChannel << lock << MICROHAL_DEBUG << "Done!\nResults are: " << (int32_t)f1.get() << " " << (int32_t)f2.get() << " " << (int32_t)f3.get()
                << unlock;
    t.join();
}

int fib(int n) {
    if (n < 3)
        return 1;
    else
        return fib(n - 1) + fib(n - 2);
}

void testWait() {
    std::future<int> f1 = std::async(std::launch::async, []() { return fib(20); });
    std::future<int> f2 = std::async(std::launch::async, []() { return fib(25); });

    diagChannel << lock << MICROHAL_DEBUG << "waiting...\n" << unlock;
    f1.wait();
    f2.wait();

    diagChannel << lock << MICROHAL_DEBUG << "f1: " << (int32_t)f1.get() << unlock;
    diagChannel << lock << MICROHAL_DEBUG << "f2: " << (int32_t)f2.get() << unlock;
}

void testWaitFor() {
    std::future<int> future = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 8;
    });

    diagChannel << lock << MICROHAL_DEBUG << "waiting..." << unlock;
    std::future_status status;
    do {
        status = future.wait_for(std::chrono::seconds(1));
        if (status == std::future_status::deferred) {
            diagChannel << lock << MICROHAL_DEBUG << "deferred" << unlock;
        } else if (status == std::future_status::timeout) {
            diagChannel << lock << MICROHAL_DEBUG << "timeout" << unlock;
        } else if (status == std::future_status::ready) {
            diagChannel << lock << MICROHAL_DEBUG << "ready!" << unlock;
        }
    } while (status != std::future_status::ready);

    diagChannel << lock << MICROHAL_DEBUG << "result is " << (int32_t)future.get() << unlock;
}

void runTest() {
    // example();
    // testWait();
    testWaitFor();
#ifdef HAL_RTOS_FreeRTOS
    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
#endif
}

int main(int argc, char *argv[]) {
#if defined(LINUX)
    initialize(argc, argv);
#endif

    debugPort.open(SerialPort::ReadWrite);

    diagChannel.setOutputDevice(debugPort);

    microhal::os::setDefaultStackSize(4 * 1024);
    std::thread t(runTest);

#ifdef HAL_RTOS_FreeRTOS
    vTaskStartScheduler();
#else
    t.join();
#endif

    return 0;
}
