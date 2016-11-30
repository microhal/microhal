/*
 * freeRTOS_port.h
 *
 *  Created on: 8 kwi 2014
 *      Author: pawel
 */

#ifndef FREERTOS_PORT_H_
#define FREERTOS_PORT_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define __GTHREADS 1

//#define __GTHREAD_RECURSIVE_MUTEX_INIT xSemaphoreCreateRecursiveMutex()
#define __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION(x) *(x) = xSemaphoreCreateMutex()
#define __GTHREAD_ONCE_INIT 0
//#define __GTHREAD_MUTEX_INIT xSemaphoreCreateMutex()
#define __GTHREAD_MUTEX_INIT_FUNCTION(x) *(x) = xSemaphoreCreateMutex()

typedef int __gthread_key_t;
typedef int __gthread_once_t;
typedef xSemaphoreHandle __gthread_mutex_t;
typedef xSemaphoreHandle __gthread_recursive_mutex_t;

static int __gthread_active_p() {
    return 0;
}

static int __gthread_once(__gthread_once_t *once, void (*func)()) {
    volatile int i = 0;
    i++;
    return -1;
}

static int __gthread_key_create(__gthread_key_t *keyp, void (*dtor)(void *)) {
	return -1;
}
static int __gthread_key_delete(__gthread_key_t key) {
	return -1;
}

static void *__gthread_getspecific(__gthread_key_t key) {
	return -1;
}
static int __gthread_setspecific(__gthread_key_t key, const void *ptr) {
	return -1;
}

static inline int __gthread_mutex_destroy(__gthread_mutex_t *mutex) {
    vSemaphoreDelete(*mutex);
    return 0;
}
static inline int __gthread_recursive_mutex_destroy(__gthread_recursive_mutex_t *mutex) {
    vSemaphoreDelete(*mutex);
    return 0;
}

static inline int __gthread_mutex_lock(__gthread_mutex_t *mutex) {
    return (xSemaphoreTake(*mutex, portMAX_DELAY) == pdTRUE) ? 0 : 1;
}
static inline int __gthread_mutex_trylock(__gthread_mutex_t *mutex) {
	return (xSemaphoreTake(*mutex, 0) == pdTRUE) ? 0 : 1;
}
static inline int __gthread_mutex_unlock(__gthread_mutex_t *mutex) {
    return (xSemaphoreGive(*mutex) == pdTRUE) ? 0 : 1;
}

static inline int __gthread_recursive_mutex_lock(__gthread_recursive_mutex_t *mutex) {
    return xSemaphoreTakeRecursive(*mutex, portMAX_DELAY);
}
static inline int __gthread_recursive_mutex_trylock(__gthread_recursive_mutex_t *mutex) {
	return xSemaphoreTakeRecursive(*mutex, 0);
}
static inline int __gthread_recursive_mutex_unlock(__gthread_recursive_mutex_t *mutex) {
    return xSemaphoreGiveRecursive(*mutex);
}
//////////////////////

typedef int __gthread_cond_t;

//////////////////////
#define __GTHREADS_CXX0X 1

typedef xTaskHandle __gthread_t;
typedef portTickType __gthread_time_t;

static inline int __gthread_create(__gthread_t *thread, void *(*func)(void*), void *args) {
    xTaskCreate(reinterpret_cast<void (*)(void*)>(func), "NAME", 256, args, tskIDLE_PRIORITY, thread);
    return 0;
}
static inline int __gthread_join(__gthread_t thread, void **value_ptr) {
	return 0;
}
static inline int __gthread_detach(__gthread_t thread) {
    vTaskDelete(thread);
    return 0;
}
static inline int __gthread_equal(__gthread_t t1, __gthread_t t2) {
    return t1 == t2;
}
static inline __gthread_t __gthread_self (void) {
    return xTaskGetCurrentTaskHandle();
}


#define _GLIBCXX_USE_SCHED_YIELD
static inline int __gthread_yield(void) {
    taskYIELD();
    return 0;
}

static inline int __gthread_mutex_timedlock(__gthread_mutex_t *m,
        const __gthread_time_t *abs_timeout) {
    return xSemaphoreTake(*m, *abs_timeout);
}
static inline int __gthread_recursive_mutex_timedlock(__gthread_recursive_mutex_t *m,
        const __gthread_time_t *abs_time) {
    return xSemaphoreTakeRecursive(*m, *abs_time);
}

static inline int __gthread_mutex_timedlock_relative(__gthread_mutex_t *m,
        const __gthread_time_t *timeout) {
    return xSemaphoreTake(*m, *timeout);
}
static inline int __gthread_recursive_mutex_timedlock_relative(__gthread_recursive_mutex_t *m,
        const __gthread_time_t *timeout) {
    return xSemaphoreTakeRecursive(*m, *timeout);
}


static inline int __gthread_cond_signal(__gthread_cond_t *cond) {
	return 0;
}
static inline int __gthread_cond_timedwait(__gthread_cond_t *cond, __gthread_mutex_t *mutex,
        const __gthread_time_t *abs_timeout) {
	return 0;
}

#endif /* FREERTOS_PORT_H_ */
