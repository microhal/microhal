/*
 * freeRTOS_port.cpp
 *
 *  Created on: 8 kwi 2014
 *      Author: pawel
 */

#include <chrono>
#include <thread>
#include "FreeRTOS.h"
#include "task.h"

#include <sys/time.h>

#include "diagnostic/diagnostic.h"

using namespace microhal;
using namespace diagnostic;

time_t sec_correction;
suseconds_t usec_correction;

namespace microhal {
namespace os {

void set_system_clock_time(const std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration> &actual_time) noexcept {
    diagChannel << lock << MICROHAL_DEBUG << "Setting time to: "
                << static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(actual_time.time_since_epoch()).count()) << unlock;
    auto delta = actual_time - std::chrono::system_clock::now();

    sec_correction += std::chrono::duration_cast<std::chrono::seconds>(delta).count();
    usec_correction += std::chrono::duration_cast<std::chrono::microseconds>(delta).count() -
                       std::chrono::duration_cast<std::chrono::seconds>(delta).count() * 1000000;

    diagChannel << lock << MICROHAL_DEBUG << "Actual time: "
                << static_cast<uint64_t>(
                       std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
                << unlock;
}
}  // namespace os
}  // namespace microhal

extern "C" int _gettimeofday(struct timeval *tv, void *tzvp __attribute__((unused))) {
    TickType_t tickCount = xTaskGetTickCount();
    tv->tv_sec = sec_correction + tickCount / configTICK_RATE_HZ;  // convert to seconds
    tv->tv_usec =
        usec_correction + (static_cast<uint64_t>((tickCount % configTICK_RATE_HZ)) * 1000 * 1000 / configTICK_RATE_HZ);  // get remaining microseconds

    return 0;  // return non-zero for error
}

#ifndef USE_WRAP_MALLOCK
extern "C" void *malloc(size_t size) {
    return pvPortMalloc(size);
}

extern "C" void free(void *pv) {
    vPortFree(pv);
}
#else
volatile uint32_t mallock_failure_counter = 0;
void *freeRTOS_malloc(size_t size) {
    void *ptr = pvPortMalloc(size);

    //    diagChannel << MICROHAL_DEBUG << "allocating memory: " << (uint32_t)size << " left: " << (uint32_t)xPortGetFreeHeapSize();
    if (ptr == nullptr) {
        diagChannel << MICROHAL_ERROR << "mallock failed, out of memory";
        mallock_failure_counter++;
    }

    return ptr;
}

void freeRTOS_free(void *ptr) {
    vPortFree(ptr);
}

extern "C" void *__wrap_malloc(size_t size) {
    return freeRTOS_malloc(size);
}

extern "C" void __wrap_free(void *ptr) {
    freeRTOS_free(ptr);
}

extern "C" void *__wrap__malloc_r(struct _reent *ptr, size_t size) {
    return freeRTOS_malloc(size);
}

extern "C" void __wrap__free_r(struct _reent *r_ptr, void *ptr) {
    freeRTOS_free(ptr);
}
#endif
