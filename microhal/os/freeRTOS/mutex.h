#ifndef MUTEX_H_
#define MUTEX_H_

#include <chrono>
#include "FreeRTOS.h"
#include "semphr.h"

namespace microhal {
namespace os {

class Mutex {
 public:
    Mutex() { ackSemaphore = xSemaphoreCreateMutex(); }
    ~Mutex() { vSemaphoreDelete(ackSemaphore); }

    bool wait(std::chrono::milliseconds timeout) {
        if (timeout < std::chrono::milliseconds(0)) {
            return false;
        }
        const TickType_t tmp = (timeout == std::chrono::milliseconds::max()) ? portMAX_DELAY : timeout.count() * configTICK_RATE_HZ / 1000;
        return (xSemaphoreTake(ackSemaphore, tmp) == pdTRUE);
    }

    void give() { xSemaphoreGive(ackSemaphore); }

    bool giveFromISR() {
        BaseType_t xHigherPriorityTaskWoken;
        xSemaphoreGiveFromISR(ackSemaphore, &xHigherPriorityTaskWoken);
        return xHigherPriorityTaskWoken == pdTRUE;
    }

 private:
    SemaphoreHandle_t ackSemaphore;
};

}  // namespace os
}  // namespace microhal

#endif /* MUTEX_H_ */
