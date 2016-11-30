// condition_variable -*- C++ -*-

// Copyright (C) 2008-2014 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

#include <condition_variable>

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)

bool __microhal_cond_timeout_wait(Microhal_cond_impl *impl, SemaphoreHandle_t *mutex, TickType_t timeout){

	impl->add_waiting_task(xTaskGetCurrentTaskHandle());
	xSemaphoreGive(*mutex);
	const uint32_t status = ulTaskNotifyTake( pdTRUE, timeout );
	if(status != pdTRUE){
		//impl->delete_waiting_task(xTaskGetCurrentTaskHandle());
	}
	xSemaphoreTake(*mutex, portMAX_DELAY);
	return status;
}

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

#ifdef __GTHREAD_COND_INIT
  condition_variable::condition_variable() noexcept = default;
#else
  condition_variable::condition_variable() noexcept
  {
    //__GTHREAD_COND_INIT_FUNCTION(&_M_cond);
	  //_M_cond = new Microhal_cond_impl;
  }
#endif

  condition_variable::~condition_variable() noexcept
  {
    // XXX no thread blocked
    /* int __e = */ //__gthread_cond_destroy(&_M_cond);
	  //delete _M_cond;
    // if __e == EBUSY then blocked
  }

  void
  condition_variable::wait(unique_lock<mutex>& __lock)
  {
    //int __e = __gthread_cond_wait(&_M_cond, __lock.mutex()->native_handle());

	//int __e = _M_cond.add_waiting_task(NULL);
	//__lock.unlock();
	//ulTaskNotifyTake( pdTRUE,          /* Clear the notification value before exiting. */
	 //                 portMAX_DELAY ); /* Block indefinitely. */

	bool __e = !__microhal_cond_timeout_wait(&_M_cond, __lock.mutex()->native_handle(),
					static_cast<TickType_t>(portMAX_DELAY));
    //__lock.lock();

    if (__e)
      __throw_system_error(__e);
  }

  void
  condition_variable::notify_one() noexcept
  {
    int __e = 1;//__gthread_cond_signal(&_M_cond);

    xTaskHandle task = _M_cond.get_waiting_task();

    if(task){
    	xTaskNotifyGive(task);
    	__e = 0;
    }

    // XXX not in spec
    // EINVAL
    if (__e)
      __throw_system_error(__e);
  }

  void
  condition_variable::notify_all() noexcept
  {
    //int __e = 1;//__gthread_cond_broadcast(&_M_cond);
	taskENTER_CRITICAL();
    uint8_t task_count = _M_cond.get_waiting_task_count();
    while(task_count--){
    	xTaskNotifyGive(_M_cond.get_waiting_task());
    	//__e = 0;
    }
    taskEXIT_CRITICAL();
    // XXX not in spec
    // EINVAL
    //if (__e)
      //__throw_system_error(__e);
  }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace

#endif // _GLIBCXX_HAS_GTHREADS && _GLIBCXX_USE_C99_STDINT_TR1
