/*
 * semaphore.h
 *
 *  Created on: 12 07 2016
 *      Author: Pawel
 */

#ifndef SEMAPHORE_LINUX_H_
#define SEMAPHORE_LINUX_H_

#include <windows.h>
#include <stdio.h>

namespace microhal {

class semaphore {
public:
	semaphore(){
		sem = CreateSemaphore(
		        NULL,           // default security attributes
		        0,  // initial count
		        1,  // maximum count
		        NULL);
	}
	~semaphore(){
	}

	bool wait(std::chrono::milliseconds timeout){
		return WaitForSingleObject(sem, timeout.count())	== WAIT_OBJECT_0;
	}

	void give(){
		ReleaseSemaphore(sem,  // handle to semaphore
						1,     // increase count by one
						NULL); // not interested in previous count
	}
private:
	HANDLE sem;
};

} // namespace microhal

#endif // SEMAPHORE_LINUX_H_
