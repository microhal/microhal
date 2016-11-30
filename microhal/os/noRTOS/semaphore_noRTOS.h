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
        std::chrono::milliseconds time(0);
        while (sem == false) {
            if (time == timeout) {
                return false;
            }

            time++;
            std::this_thread::sleep_for(std::chrono::milliseconds{1});
        }
        sem = false;
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

#endif // _MICROHAL_SEMAPHORE_NORTOS_H_
