/*
 * empty_port.cpp
 *
 *  Created on: 9 kwi 2014
 *      Author: pawel
 */

#include <thread>

#include <sys/time.h>
#include "ports/manager/hardware.h"

using nanosecond = std::chrono::duration<long long, std::ratio<1ll, 1000000000ll> >;

static void delay(std::chrono::duration<long long, std::ratio<1ll, 1000000000ll> > ns, uint32_t nsPerInstruction);

static void delay(std::chrono::duration<long long, std::ratio<1ll, 1ll> > s, uint32_t nsPerInstruction) {
    auto sec = s.count();
    while (sec--) {
        delay(nanosecond{1'000'000'000}, nsPerInstruction);
    }
}

void delay(std::chrono::duration<long long, std::ratio<1ll, 1000000000ll> > ns, uint32_t nsPerInstruction) {
    auto instructionCount = ns.count() / nsPerInstruction / 8;
    instructionCount -= 100;
    while (instructionCount-- > 0) {
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
    }
}

void std::this_thread::__sleep_for(std::chrono::duration<long long, std::ratio<1ll, 1ll> > s,
                                   std::chrono::duration<long long, std::ratio<1ll, 1000000000ll> > ns) {
    const uint32_t clock = microhal::hardware::Device::coreFrequency();
    const uint32_t nsPerClockTick = 1'000'000'000 / clock;
    const uint32_t nsPerInstruction = nsPerClockTick;
    const nanosecond offset{100 * nsPerInstruction};  // offset caused by division in nsPerClockTick calculation and by reading core frequency
    const nanosecond delay_ns = ns - offset;

    if (s.count() > 0) delay(s, clock);
    if (delay_ns > delay_ns.zero()) delay(delay_ns, nsPerInstruction);
}

extern uint64_t SysTick_time;
extern "C" int _gettimeofday(struct timeval *tv, void *tzvp __attribute__((unused))) {
    uint64_t t = SysTick_time;        // SysTick->VAL; // get uptime in nanoseconds
    tv->tv_sec = t / 1000;            // convert to seconds
    tv->tv_usec = (t % 1000) * 1000;  // get remaining microseconds
                                      // tv->tv_sec = 0;//t / 1000000000;  // convert to seconds
                                      // tv->tv_usec = SysTick->VAL;
    return 0;                         // return non-zero for error
}
