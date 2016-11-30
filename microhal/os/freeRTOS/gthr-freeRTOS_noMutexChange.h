/*
 * freeRTOS_port.h
 *
 *  Created on: 8 kwi 2014
 *      Author: pawel
 */

#ifndef GTHR_FREERTOS_H_
#define GTHR_FREERTOS__H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include <sys/time.h>


#define _GLIBCXX_HAS_GTHREADS
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
	if(*once == false){
		*once = true;
		func();
	}
	return 0;
}

static int __gthread_key_create(__gthread_key_t *keyp, void (*dtor)(void *)) {

}
static int __gthread_key_delete(__gthread_key_t key) {

}

static void *__gthread_getspecific(__gthread_key_t key) {

}
static int __gthread_setspecific(__gthread_key_t key, const void *ptr) {

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

/// --- timed mutex implementation ---
typedef struct{
	long sec;
	long nsec;
} __gthread_time_t;

//this function isn't a part of standard library code
static inline int __gthread_mutex_timedlock_relative(__gthread_mutex_t *m,
        const __gthread_time_t *timeout) {
	TickType_t t = timeout->sec * 1000;
	t += timeout->nsec / 1000000;
    return xSemaphoreTake(*m, t);
}
//this function isn't a part of standard library code
static inline int __gthread_recursive_mutex_timedlock_relative(__gthread_recursive_mutex_t *m,
        const __gthread_time_t *timeout) {
	TickType_t t = timeout->sec * 1000;
	t += timeout->nsec / 1000000;
    return xSemaphoreTakeRecursive(*m, t);
}

static inline int __gthread_mutex_timedlock(__gthread_mutex_t *m,
        const __gthread_time_t *abs_timeout) {
	timeval now;
	__gthread_time_t tmp_time;

	gettimeofday(&now, NULL);
	tmp_time.sec = abs_timeout->sec - now.tv_sec;
	tmp_time.nsec = abs_timeout->nsec - now.tv_usec * 1000;
	return __gthread_mutex_timedlock_relative(m, &tmp_time);
}

static inline int __gthread_recursive_mutex_timedlock(__gthread_recursive_mutex_t *m,
        const __gthread_time_t *abs_time) {
	timeval now;
	__gthread_time_t tmp_time;

	gettimeofday(&now, NULL);
	tmp_time.sec = abs_time->sec - now.tv_sec;
	tmp_time.nsec = abs_time->nsec - now.tv_usec * 1000;
    return __gthread_recursive_mutex_timedlock_relative(m, &tmp_time);
}
//////////////////////

typedef int __gthread_cond_t;

//////////////////////
#define __GTHREADS_CXX0X 1

typedef xTaskHandle __gthread_t;


static inline int __gthread_create(__gthread_t *thread, void *(*func)(void*), void *args) {
    xTaskCreate(reinterpret_cast<void (*)(void*)>(func), "NAME", 256, args, tskIDLE_PRIORITY, thread);
    return 0;
}
static inline int __gthread_join(__gthread_t thread, void **value_ptr) {

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

//static inline int __gthread_cond_signal(__gthread_cond_t *cond) {
//
//}
//static inline int __gthread_cond_timedwait(__gthread_cond_t *cond, __gthread_mutex_t *mutex,
//        const __gthread_time_t *abs_timeout) {
//
//}

#endif /* GTHR_FREERTOS__H_ */
