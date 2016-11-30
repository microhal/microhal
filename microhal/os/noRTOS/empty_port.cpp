/*
 * empty_port.cpp
 *
 *  Created on: 9 kwi 2014
 *      Author: pawel
 */

//#include "empty_port.h"
#include <thread>

#include <sys/time.h>

void std::this_thread::__sleep_for(std::chrono::duration<long long, std::ratio<1ll, 1ll> > s,
        std::chrono::duration<long long, std::ratio<1ll, 1000000000ll> > ns) {

    volatile uint32_t delay = ns.count() / 1000 ;
    delay += s.count() * 1000000;

    // diagChannel << Warning << "delay [ms]: " << (uint32_t) delay << endl;

    volatile uint32_t i;

    while (delay--) {
        i = 22;
        while (i--) {
        }
    }

}

extern uint64_t SysTick_time;
extern "C" int _gettimeofday( struct timeval *tv, void *tzvp __attribute__ ((unused)))
{
   uint64_t t = SysTick_time;// SysTick->VAL; // get uptime in nanoseconds
   tv->tv_sec = t / 1000;  // convert to seconds
   tv->tv_usec = ( t % 1000 ) * 1000;  // get remaining microseconds
  // tv->tv_sec = 0;//t / 1000000000;  // convert to seconds
  // tv->tv_usec = SysTick->VAL;
   return 0;  // return non-zero for error
}
