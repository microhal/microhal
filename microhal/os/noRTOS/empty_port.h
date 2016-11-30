/*
 * freeRTOS_port.h
 *
 *  Created on: 8 kwi 2014
 *      Author: pawel
 */

#ifndef EMPTY_PORT_H_
#define EMPTY_PORT_H_

#define _GLIBCXX_HAS_GTHREADS
#define __GTHREADS 1

#define __MICROHAL_MUTEX_CONSTEXPR_CTOR

#define __GTHREAD_RECURSIVE_MUTEX_INIT 0
#define __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION 0
#define __GTHREAD_ONCE_INIT 0
#define __GTHREAD_MUTEX_INIT 0
#define __GTHREAD_MUTEX_INIT_FUNCTION 0

typedef int __gthread_key_t;
typedef int __gthread_once_t;
typedef int __gthread_mutex_t;
typedef int __gthread_recursive_mutex_t;

static int __gthread_active_p() {
    return 0;
}

static int __gthread_once(__gthread_once_t *once __attribute__ ((unused)), void (*func)() __attribute__ ((unused))) {
	return 0;
}

static int __gthread_key_create(__gthread_key_t *keyp __attribute__ ((unused)), void (*dtor)(void *) __attribute__ ((unused))) {
	return 0;
}
static int __gthread_key_delete(__gthread_key_t key __attribute__ ((unused))) {
	return 0;
}

static void *__gthread_getspecific(__gthread_key_t key __attribute__ ((unused))) {
	return nullptr;
}
static int __gthread_setspecific(__gthread_key_t key __attribute__ ((unused)), const void *ptr __attribute__ ((unused))) {
	return 0;
}

static int __gthread_mutex_destroy(__gthread_mutex_t *mutex __attribute__ ((unused))) {
	return 0;
}
static int __gthread_recursive_mutex_destroy(__gthread_recursive_mutex_t *mutex __attribute__ ((unused))) {
	return 0;
}

static int __gthread_mutex_lock(__gthread_mutex_t *mutex __attribute__ ((unused))) {
	return 0;
}
static int __gthread_mutex_trylock(__gthread_mutex_t *mutex __attribute__ ((unused))) {
	return 0;
}
static int __gthread_mutex_unlock(__gthread_mutex_t *mutex __attribute__ ((unused))) {
	return 0;
}

static int __gthread_recursive_mutex_lock(__gthread_recursive_mutex_t *mutex __attribute__ ((unused))) {
	return 0;
}
static int __gthread_recursive_mutex_trylock(__gthread_recursive_mutex_t *mutex __attribute__ ((unused))) {
	return 0;
}
static int __gthread_recursive_mutex_unlock(__gthread_recursive_mutex_t *mutex __attribute__ ((unused))) {
	return 0;
}
//////////////////////

typedef int __gthread_cond_t;

//////////////////////
#define __GTHREADS_CXX0X 1

struct microHAL_timespec {
  long  tv_sec;   /* Seconds */
  long    tv_nsec;  /* Nanoseconds */
};

typedef int __gthread_t;
typedef microHAL_timespec __gthread_time_t;
//typedef unsigned long __gthread_time_t;

static inline int __gthread_create(__gthread_t *thread __attribute__ ((unused)),
		void *(*func)(void*) __attribute__ ((unused)), void *args __attribute__ ((unused))) {
	return 0;
}
static inline int __gthread_join(__gthread_t thread __attribute__ ((unused)), void **value_ptr __attribute__ ((unused))) {
	return 0;
}
static inline int __gthread_detach(__gthread_t thread __attribute__ ((unused))) {
	return 0;
}
static inline int __gthread_equal(__gthread_t t1, __gthread_t t2) {
    return t1 == t2;
}
static inline __gthread_t                                     __gthread_self (void) {
	return 0;
}
static inline int __gthread_yield(void) {
	return 0;
}

static inline int __gthread_mutex_timedlock(__gthread_mutex_t *m __attribute__ ((unused)),
        const __gthread_time_t *abs_timeout __attribute__ ((unused))) {
	return 0;
}
static inline int __gthread_recursive_mutex_timedlock(__gthread_recursive_mutex_t *m __attribute__ ((unused)),
        const __gthread_time_t *abs_time __attribute__ ((unused))) {
	return 0;
}

static inline int __gthread_cond_signal(__gthread_cond_t *cond __attribute__ ((unused))) {
	return 0;
}
static inline int __gthread_cond_timedwait(__gthread_cond_t *cond __attribute__ ((unused)),
		__gthread_mutex_t *mutex __attribute__ ((unused)), const __gthread_time_t *abs_timeout __attribute__ ((unused))) {
	return 0;
}

#endif /* FREERTOS_PORT_H_ */
