/*
 * empty_port.cpp
 *
 *  Created on: 9 kwi 2014
 *      Author: pawel
 */

#include <thread>

#include <sys/time.h>
#include "ports/manager/hardware.h"


static void delay(std::chrono::duration<long long, std::ratio<1ll, 1ll> > s, uint32_t clock) {
	auto sec = s.count();
	while (sec--) {
		volatile uint32_t i = clock/4;
		while(i--);
	}
}

static void delay(std::chrono::duration<long long, std::ratio<1ll, 1000000000ll> > ns, uint32_t clock) {
	auto usec = ns.count() / 1000;
	while (usec--) {
		volatile uint32_t i = clock/7500000;
		while(i--);
	}
}

void std::this_thread::__sleep_for(std::chrono::duration<long long, std::ratio<1ll, 1ll> > s,
        std::chrono::duration<long long, std::ratio<1ll, 1000000000ll> > ns) {

	const uint32_t clock = microhal::hardware::Device::coreFrequency();
	delay(s, clock);
	delay(ns, clock);
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
