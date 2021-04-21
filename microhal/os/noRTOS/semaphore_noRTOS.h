/*
 * semaphore_noRTOS.h
 *
 *  Created on: 11 sie 2015
 *      Author: Pawel
 */

#ifndef _MICROHAL_SEMAPHORE_NORTOS_H_
#define _MICROHAL_SEMAPHORE_NORTOS_H_

#include <thread>

namespace microhal {
namespace os {

class Semaphore {
 public:
    constexpr Semaphore() : sem(false) {}
    ~Semaphore() {}

    bool wait(std::chrono::milliseconds timeout) {
        if (timeout == std::chrono::milliseconds::max()) {
            // block indefinitely
            while (sem == false) {
            }
        } else {
            std::chrono::microseconds time(0);
            const std::chrono::microseconds timeoutMicrosecond = timeout;
            while (sem == false) {
                if (time >= timeoutMicrosecond) {
                    return false;
                }

                time += std::chrono::microseconds{100};
                std::this_thread::sleep_for(std::chrono::microseconds{100});
            }
            sem = false;
        }
        return true;
    }

    void give() { sem = true; }

    bool giveFromISR() {
        sem = true;
        return true;
    }

 private:
    volatile bool sem;
};

}  // namespace os
}  // namespace microhal

#endif  // _MICROHAL_SEMAPHORE_NORTOS_H_
