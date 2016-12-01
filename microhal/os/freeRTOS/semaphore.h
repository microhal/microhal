/*
 * semaphore.h
 *
 *  Created on: 16 cze 2015
 *      Author: Pawel
 */

#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <chrono>
#include "FreeRTOS.h"
#include "semphr.h"

namespace microhal {
namespace os {

class Semaphore {
 public:
    Semaphore() { ackSemaphore = xSemaphoreCreateBinary(); }
    ~Semaphore() { vSemaphoreDelete(ackSemaphore); }

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

#endif /* SEMAPHORE_H_ */
