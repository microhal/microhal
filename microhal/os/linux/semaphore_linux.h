/*
 * semaphore.h
 *
 *  Created on: 16 cze 2015
 *      Author: Pawel
 */

#ifndef SEMAPHORE_LINUX_H_
#define SEMAPHORE_LINUX_H_

#include <semaphore.h>

namespace microhal {
namespace os {

class Semaphore {
public:
	Semaphore(){
		sem_init(&sem, 0, 0);
	}
	~Semaphore(){
		sem_destroy(&sem);
	}

	bool wait(std::chrono::milliseconds timeout) {
		auto abs_timeout = std::chrono::system_clock::now() + timeout;
		timespec time;
		std::chrono::seconds const sec = std::chrono::duration_cast<std::chrono::seconds>(abs_timeout.time_since_epoch());
		std::chrono::nanoseconds const nano = std::chrono::duration_cast<std::chrono::nanoseconds>((abs_timeout - sec).time_since_epoch());
		time.tv_sec = sec.count();
		time.tv_nsec = nano.count();
		return sem_timedwait(&sem, &time) != -1;
	}

	void give() {
		sem_post(&sem);
	}
private:
	sem_t sem;
};

}  // namespace os
}  // namespace microhal

#endif // SEMAPHORE_LINUX_H_
