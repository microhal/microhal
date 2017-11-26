/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 23-11-2017
 * last modification: 23-11-2017
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

/* **************************************************************************************************************************************************
 * INCLUDES
 */

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "microhal.h"

using namespace microhal;
using namespace microhal::diagnostic;

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;

void worker_thread() {
    // Wait until main() sends data
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [] { return ready; });

    // after the wait, we own the lock.
    diagChannel << lock << MICROHAL_DEBUG << "Worker thread is processing data" << unlock;
    data += " after processing";

    // Send data back to main()
    processed = true;
    diagChannel << lock << MICROHAL_DEBUG << "Worker thread signals data processing completed" << unlock;

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one();
}

void example() {
    std::thread worker(worker_thread);
    std::this_thread::yield();

    data = "Example data";
    // send data to the worker thread
    {
        std::lock_guard<std::mutex> lk(m);
        ready = true;
        diagChannel << lock << MICROHAL_DEBUG << "main() signals data ready for processing" << unlock;
    }
    cv.notify_one();

    // wait for the worker
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [] { return processed; });
    }
    diagChannel << lock << MICROHAL_DEBUG << "Back in main(), data = " << data << unlock;

    worker.join();
}

/////////////////////////////////////////////
std::mutex cv_m;  // This mutex is used for three purposes:
                  // 1) to synchronize accesses to i
                  // 2) for the condition variable cv
int i = 0;

void waits() {
    std::unique_lock<std::mutex> lk(cv_m);
    diagChannel << lock << MICROHAL_DEBUG << "Waiting..." << unlock;
    cv.wait(lk, [] { return i == 1; });
    diagChannel << lock << MICROHAL_DEBUG << "...finished waiting. i == " << (int32_t)i << unlock;
}

void signals() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lk(cv_m);
        diagChannel << lock << MICROHAL_DEBUG << "Notifying..." << unlock;
    }
    cv.notify_all();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::lock_guard<std::mutex> lk(cv_m);
        i = 1;
        diagChannel << lock << MICROHAL_DEBUG << "Notifying again..." << unlock;
    }
    cv.notify_all();
}

void testNotifyAll() {
    std::thread t1(waits), t2(waits), t3(waits), t4(signals);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
/////////////////////////////

using namespace std::chrono_literals;

void waitsc(int idx) {
    std::unique_lock<std::mutex> lk(cv_m);
    if (cv.wait_for(lk, idx * 100ms, [] { return i == 1; }))
        diagChannel << lock << MICROHAL_DEBUG << "Thread " << (int32_t)idx << " finished waiting. i == " << (int32_t)i << unlock;
    else
        diagChannel << lock << MICROHAL_DEBUG << "Thread " << (int32_t)idx << " timed out. i == " << (int32_t)i << unlock;
}

void signalsc() {
    std::this_thread::sleep_for(120ms);
    diagChannel << lock << MICROHAL_DEBUG << "Notifying..." << unlock;
    cv.notify_all();
    std::this_thread::sleep_for(100ms);
    {
        std::lock_guard<std::mutex> lk(cv_m);
        i = 1;
    }
    diagChannel << lock << MICROHAL_DEBUG << "Notifying again..." << unlock;
    cv.notify_all();
}

void testWaitFor() {
    std::thread t1(waitsc, 1), t2(waitsc, 2), t3(waitsc, 3), t4(signalsc);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void runTests() {
    // diagChannel << lock << MICROHAL_DEBUG << "Test notify all" << unlock;
    // testNotifyAll();
    // diagChannel << lock << MICROHAL_DEBUG << "Test wait for" << unlock;
    // testWaitFor();

    example();
    diagChannel << lock << MICROHAL_DEBUG << "--- End of tests ---" << unlock;
}

int main(int argc, char *argv[]) {
#if defined(LINUX)
    initialize(argc, argv);
#endif

    debugPort.open(SerialPort::ReadWrite);

    diagChannel.setOutputDevice(debugPort);

    std::thread threadA(runTests);

#ifdef HAL_RTOS_FreeRTOS
    vTaskStartScheduler();
#else
    t.join();
#endif

    return 0;
}
